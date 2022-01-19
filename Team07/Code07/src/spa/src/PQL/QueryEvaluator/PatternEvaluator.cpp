//
// Created by Junhua on 13/10/2021.
//

#include "PatternEvaluator.h"
#include <regex>
#include <utility>

PatternEvaluator::PatternEvaluator(PKB *pkb,
                                   ValueStore *valueStore) {
  this->pkb = pkb;
  this->ifPatternCheck = [this](std::string var) {
    return !this->pkb->getIfCtrlVar(std::move(var)).empty();
  };
  this->whilePatternCheck = [this](std::string var) {
    return !this->pkb->getWhileCtrlVar(std::move(var)).empty();
  };
  this->valueStore = valueStore;
}

std::unordered_set<std::string> PatternEvaluator::getValuesFor(QuerySynonym *synonym) {
  return this->valueStore->valuesInType(synonym->getType());
}

std::unordered_set<std::string> PatternEvaluator::getValuesFor(PQLPattern *arg, Query *query) {
  PatternType type = arg->getType();
  if (type == PatternType::Synonym) {
    return getValuesFor(query->findSynonymByName(arg->getValue()));
  }
  if (type == PatternType::DoubleWildcard || type == PatternType::String) {
    return {arg->getInner()};
  }
  return {};
}

ClauseResult *PatternEvaluator::evaluatePatternClause(PatternClause *clause, Query *query) {
  switch (query->findSynonymByName(clause->getSynonymName())->getType()) {
    case DesignEntityType::Assign: {
      return evaluateAssignPattern(clause, query);
    }
    case DesignEntityType::If: {
      return evaluateIfPattern(clause, query);
    }
    case DesignEntityType::While: {
      return evaluateWhilePattern(clause, query);
    }
    default:
      return new ClauseResult();
  }
}
ClauseResult *PatternEvaluator::evaluateIfPattern(PatternClause *clause, Query *query) {
  auto clauseResult = new ClauseResult();

  // Center and right must be wildcards
  if (clause->getCentre()->getType() != PatternType::Wildcard
      || clause->getRight()->getType() != PatternType::Wildcard) {
    throw std::invalid_argument("Invalid If pattern");
  }

  auto leftPattern = clause->getLeft();
  switch (leftPattern->getType()) {
    case PatternType::Wildcard: {
      auto allVars = pkb->getVarList();
      return new ClauseResult(std::any_of(allVars.begin(), allVars.end(), this->ifPatternCheck));
    }
    case PatternType::String: {
      for (auto stmt: pkb->getIfCtrlVar(leftPattern->getInner())) {
        Row r = Row();
        r.addSynonym(*query->findSynonymByName(clause->getSynonymName()), std::to_string(stmt));
        clauseResult->addRow(r);
      }
      return clauseResult;
    }
    case PatternType::Synonym: {
      auto syn = query->findSynonymByName(leftPattern->getInner());
      for (const auto &synValue: getValuesFor(syn)) {
        for (auto stmt: pkb->getIfCtrlVar(synValue)) {
          Row r = Row();
          r.addSynonym(*query->findSynonymByName(clause->getSynonymName()), std::to_string(stmt));
          r.addSynonym(*syn, synValue);
          clauseResult->addRow(r);
        }
      }
      return clauseResult;
    }
    default:
      throw std::invalid_argument("Invalid argument type to if pattern");
  }
}
ClauseResult *PatternEvaluator::evaluateWhilePattern(PatternClause *clause, Query *query) {
  auto clauseResult = new ClauseResult();

  if (clause->getRight()->getType() != PatternType::Wildcard) {
    throw std::invalid_argument("Invalid Right wildcard");
  }
  auto leftPattern = clause->getLeft();
  switch (leftPattern->getType()) {
    case PatternType::Wildcard: {
      auto allVars = pkb->getVarList();
      return new ClauseResult(std::any_of(allVars.begin(), allVars.end(), this->whilePatternCheck));
    }
    case PatternType::String: {
      for (auto stmt: pkb->getWhileCtrlVar(leftPattern->getInner())) {
        Row r = Row();
        r.addSynonym(*query->findSynonymByName(clause->getSynonymName()), std::to_string(stmt));
        clauseResult->addRow(r);
      }
      return clauseResult;
    }
    case PatternType::Synonym: {
      auto syn = query->findSynonymByName(leftPattern->getInner());
      for (const auto &synValue: getValuesFor(syn)) {
        for (auto stmt: pkb->getWhileCtrlVar(synValue)) {
          Row r = Row();
          r.addSynonym(*query->findSynonymByName(clause->getSynonymName()), std::to_string(stmt));
          r.addSynonym(*syn, synValue);
          clauseResult->addRow(r);
        }
      }
      return clauseResult;
    }
    default:
      throw std::invalid_argument("Invalid argument type to if pattern");
  }
}
ClauseResult *PatternEvaluator::evaluateAssignPattern(PatternClause *clause, Query *query) {
  auto clauseResult = new ClauseResult();

  std::unordered_set<int> stmts = pkb->getAssignStmts();

  auto leftPattern = clause->getLeft();
  auto rightPattern = clause->getRight();
  std::unordered_set<std::string> leftValues = getValuesFor(leftPattern, query);
  std::unordered_set<std::string> rightValues = getValuesFor(rightPattern, query);

  if (leftPattern->getType() == PatternType::Wildcard && rightPattern->getType() == PatternType::Wildcard) {
    // Any statement in the set will fulfill this clause.
    return new ClauseResult(!pkb->getAssignStmts().empty());
  }

  if (leftPattern->getType() == PatternType::Wildcard) {
    for (auto stmt: stmts) {
      for (const auto &rightVal: rightValues) {
        // Any statement matching right side will fulfill this clause
        if (pkb->subStringsAt(stmt).empty()) continue;

        if (matchPattern(*pkb->subStringsAt(stmt).begin(),
                         rightVal,
                         rightPattern->getType() == PatternType::DoubleWildcard)) {
          Row r = Row();
          r.addSynonym(*query->findSynonymByName(clause->getSynonymName()), std::to_string(stmt));
          if (rightPattern->getType() == PatternType::Synonym)
            r.addSynonym(*query->findSynonymByName(rightPattern->getInner()), rightVal);
          clauseResult->addRow(r);
        }
      }
    }
    return clauseResult;
  }

  if (rightPattern->getType() == PatternType::Wildcard) {
    for (auto stmt: stmts) {
      for (const auto &leftVal: leftValues) {
        // Any statement matching left side will fulfill this clause
        if (matchPattern(pkb->getLHSAssign(stmt), leftVal, leftPattern->getType() == PatternType::DoubleWildcard)) {
          Row r = Row();
          r.addSynonym(*query->findSynonymByName(clause->getSynonymName()), std::to_string(stmt));
          if (leftPattern->getType() == PatternType::Synonym)
            r.addSynonym(*query->findSynonymByName(leftPattern->getInner()), leftVal);

          clauseResult->addRow(r);
        }
      }
    }
    return clauseResult;
  }

  // Neither are wildcards, need to match both sides
  for (auto stmt: stmts) {
    for (const auto &leftVal: leftValues) {
      for (const auto &rightVal: rightValues) {
        if (pkb->subStringsAt(stmt).empty()) continue;
        if (
            matchPattern(pkb->getLHSAssign(stmt), leftVal, leftPattern->getType() == PatternType::DoubleWildcard)
                && matchPattern(*pkb->subStringsAt(stmt).begin(),
                                rightVal,
                                rightPattern->getType() == PatternType::DoubleWildcard)
            ) {

          Row r = Row();
          r.addSynonym(*query->findSynonymByName(clause->getSynonymName()), std::to_string(stmt));
          if (leftPattern->getType() == PatternType::Synonym)
            r.addSynonym(*query->findSynonymByName(leftPattern->getInner()), leftVal);
          if (rightPattern->getType() == PatternType::Synonym)
            r.addSynonym(*query->findSynonymByName(rightPattern->getInner()), rightVal);

          clauseResult->addRow(r);
        }
      }
    }
  }
  return clauseResult;

}
bool PatternEvaluator::matchPattern(const std::string &text, const std::string &pattern, bool isDoubleWildcard) {
  if (!isDoubleWildcard) {
    return text == pattern;
  }

  return text.find(pattern) != std::string::npos;
}
