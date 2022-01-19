//
// Created by vaish on 6/11/2021.
//

#include "IrrelevantClauseProcessor.h"

static std::unordered_set<ClauseType> suchThatClauseTypeSet({
                                                                ClauseType::Parent, ClauseType::Follows,
                                                                ClauseType::Calls, ClauseType::Next,
                                                                ClauseType::ModifiesS, ClauseType::ModifiesP,
                                                                ClauseType::UsesS, ClauseType::UsesP,
                                                                ClauseType::CallsT, ClauseType::ParentT,
                                                                ClauseType::FollowsT, ClauseType::NextT,
                                                                ClauseType::Affects, ClauseType::AffectsT,
                                                                ClauseType::NextBip, ClauseType::NextBipT,
                                                                ClauseType::AffectsBip
                                                            });
static std::unordered_set<DesignEntityType> generalStmtTypeSet({
                                                                   DesignEntityType::Stmt, DesignEntityType::ProgLine
                                                               });
static std::unordered_set<ClauseType> noLeftWildcardClauseTypeSet({
                                                                      ClauseType::ModifiesS, ClauseType::ModifiesP,
                                                                      ClauseType::UsesS, ClauseType::UsesP,
                                                                  });
static std::unordered_set<WithType> synonymWithTypes({
                                                         WithType::Synonym, WithType::Attribute
                                                     });

IrrelevantClauseProcessor::IrrelevantClauseProcessor(Query *query) {
  this->query = query;
}

void IrrelevantClauseProcessor::processIrrelevantClauses() {
  if (query->getOutputList().at(0)->getType() != QueryOutputType::Boolean) {
    for (auto &output: query->getOutputList()) {
      relevanceMap.emplace(output->getSynonym()->getName(), true);
    }
  }
  checkRelevanceOfAllClauses();
  convertIrrelevantClauses();
}

void IrrelevantClauseProcessor::checkRelevanceOfAllClauses() {
  for (auto &clause: query->getClauses()) {
    switch (clause->getType()) {
      case ClauseType::With: {
        auto *currClause = dynamic_cast<WithClause *>(clause);
        this->checkWithClauseSynRelevance(currClause);
        break;
      }
      case ClauseType::Pattern: {
        auto *currClause = dynamic_cast<PatternClause *>(clause);
        this->checkPatternClauseSynRelevance(currClause);
        break;
      }
      default:
        auto *currClause = dynamic_cast<SuchThatClause *>(clause);
        this->checkSuchThatClauseSynRelevance(currClause);
    }
  }
}

void IrrelevantClauseProcessor::makePairedSynRelevant(const std::string &syn) {
  if (irrelevantPairMap.count(syn) == 0) {
    return;
  }
  std::string other = irrelevantPairMap.at(syn);
  irrelevantPairMap.erase(syn);
  relevanceMap.insert_or_assign(other, true);
  makePairedSynRelevant(other);
}

void IrrelevantClauseProcessor::checkSuchThatClauseSynRelevance(SuchThatClause *clause) {
  if (suchThatClauseTypeSet.count(clause->getType()) == 0) {
    throw std::runtime_error("Received invalid clause type in IrrelevantClauseProcessor");
  }

  int numSynonyms = 0;
  std::string leftSyn;
  std::string rightSyn;
  bool isRelevant = false;

  if (clause->getLeft()->getType() == ArgumentType::Synonym) {
    numSynonyms++;
    leftSyn = clause->getLeft()->getValue();
    isRelevant = relevanceMap.count(leftSyn) > 0; // If syn is encountered before -> relevant
  }
  if (clause->getRight()->getType() == ArgumentType::Synonym) {
    numSynonyms++;
    rightSyn = clause->getRight()->getValue();
    isRelevant = isRelevant || relevanceMap.count(rightSyn) > 0; // If syn is encountered before -> relevant
    if (isRelevant) {
      makePairedSynRelevant(rightSyn);
    }
    relevanceMap.insert_or_assign(rightSyn, isRelevant);
  }

  modifyDataStructuresAccordingToRelevance(isRelevant, {leftSyn, rightSyn}, numSynonyms);
}

void IrrelevantClauseProcessor::checkWithClauseSynRelevance(WithClause *clause) {
  int numSynonyms = 0;
  std::string leftSyn;
  std::string rightSyn;
  bool isRelevant = false;

  if (synonymWithTypes.count(clause->getLeft()->getType()) > 0) {
    numSynonyms++;
    leftSyn = clause->getLeft()->getValue();
    isRelevant = relevanceMap.count(leftSyn) > 0; // If syn is encountered before -> relevant
  }
  if (synonymWithTypes.count(clause->getRight()->getType()) > 0) {
    numSynonyms++;
    rightSyn = clause->getRight()->getValue();
    isRelevant = isRelevant || relevanceMap.count(rightSyn) > 0; // If syn is encountered before -> relevant
    if (isRelevant) {
      makePairedSynRelevant(rightSyn);
    }
    relevanceMap.insert_or_assign(rightSyn, isRelevant);
  }

  modifyDataStructuresAccordingToRelevance(isRelevant, {leftSyn, rightSyn}, numSynonyms);
}

void IrrelevantClauseProcessor::checkPatternClauseSynRelevance(PatternClause *clause) {
  int numSynonyms = 0;
  // pattern leftSyn (rightSyn, ...)
  std::string leftSyn;
  std::string rightSyn;
  bool isRelevant = false;

  // Handle left synonym, which is outside brackets
  numSynonyms++;
  leftSyn = clause->getSynonymName();
  isRelevant = relevanceMap.count(leftSyn) > 0; // If syn is encountered before -> relevant

  if (clause->getLeft()->getType() == PatternType::Synonym) {
    numSynonyms++;
    rightSyn = clause->getLeft()->getValue();
    isRelevant = isRelevant || relevanceMap.count(rightSyn) > 0; // If syn is encountered before -> relevant
    if (isRelevant) {
      makePairedSynRelevant(rightSyn);
    }
    relevanceMap.insert_or_assign(rightSyn, isRelevant);
  }

  modifyDataStructuresAccordingToRelevance(isRelevant, {leftSyn, rightSyn}, numSynonyms);
}

void IrrelevantClauseProcessor::modifyDataStructuresAccordingToRelevance(bool isRelevant,
                                                                         const std::pair<std::string,
                                                                                         std::string> &pair,
                                                                         int numSynonyms) {

  if (isRelevant && !pair.first.empty()) {
    makePairedSynRelevant(pair.first);
  }
  if (!pair.first.empty()) {
    relevanceMap.insert_or_assign(pair.first, isRelevant);
  }

  if (numSynonyms == 2 && !isRelevant && pair.first != pair.second) {
    irrelevantPairMap.emplace(pair.first, pair.second);
    irrelevantPairMap.emplace(pair.second, pair.first);
  }
}

void IrrelevantClauseProcessor::convertIrrelevantClauses() {
  for (auto &clause: query->getClauses()) {
    switch (clause->getType()) {
      case ClauseType::With: {
        auto *currClause = dynamic_cast<WithClause *>(clause);
        this->makeWithClauseIrrelevantIfNeeded(currClause);
        break;
      }
      case ClauseType::Pattern: {
        auto *currClause = dynamic_cast<PatternClause *>(clause);
        this->makePatternClauseIrrelevantIfNeeded(currClause);
        break;
      }
      default:
        auto *currClause = dynamic_cast<SuchThatClause *>(clause);
        this->makeSuchThatClauseIrrelevantIfNeeded(currClause);
    }
  }
  query->setClauses(relevantClauses);
}

void IrrelevantClauseProcessor::makeSuchThatClauseIrrelevantIfNeeded(SuchThatClause *clause) {
  bool leftIsSyn = clause->getLeft()->getType() == ArgumentType::Synonym;
  bool rightIsSyn = clause->getRight()->getType() == ArgumentType::Synonym;
  bool leftIsRelevantSyn = leftIsSyn && relevanceMap.at(clause->getLeft()->getValue());
  bool rightIsRelevantSyn = rightIsSyn && relevanceMap.at(clause->getRight()->getValue());
  if (leftIsRelevantSyn || rightIsRelevantSyn) {
    relevantClauses.emplace_back(clause);
    return;
  }
  std::vector<Clause *> lst = query->getClauses();
  query->insertIrrelevantClause(clause);

  if (leftIsSyn && rightIsSyn && clause->getLeft()->getValue() == clause->getRight()->getValue()) {
    // e.g. in Follows (s, s) -> Same arguments mean that context will be lost if converted to wildcard
    return;
  }

  std::string leftVal = clause->getLeft()->getValue();
  ClauseType type = clause->getType();
  if (leftIsSyn && generalStmtTypeSet.count(query->findSynonymByName(leftVal)->getType()) > 0
      && noLeftWildcardClauseTypeSet.count(type) == 0) {
    clause->setLeft("_");
  }

  std::string rightVal = clause->getRight()->getValue();
  if (rightIsSyn && generalStmtTypeSet.count(query->findSynonymByName(rightVal)->getType()) > 0) {
    clause->setRight("_");
  }
}

void IrrelevantClauseProcessor::makeWithClauseIrrelevantIfNeeded(WithClause *clause) {
  bool leftIsSyn = synonymWithTypes.count(clause->getLeft()->getType()) > 0;
  bool rightIsSyn = synonymWithTypes.count(clause->getRight()->getType()) > 0;
  bool leftIsRelevantSyn = leftIsSyn && relevanceMap.at(clause->getLeft()->getValue());
  bool rightIsRelevantSyn = rightIsSyn && relevanceMap.at(clause->getRight()->getValue());
  if (leftIsRelevantSyn || rightIsRelevantSyn) {
    relevantClauses.emplace_back(clause);
    return;
  }
  query->insertIrrelevantClause(clause);
}

void IrrelevantClauseProcessor::makePatternClauseIrrelevantIfNeeded(PatternClause *clause) {
  bool rightIsSyn = clause->getLeft()->getType() == PatternType::Synonym;
  bool leftIsRelevantSyn = relevanceMap.at(clause->getSynonymName());
  bool rightIsRelevantSyn = rightIsSyn && relevanceMap.at(clause->getLeft()->getValue());
  if (leftIsRelevantSyn || rightIsRelevantSyn) {
    relevantClauses.emplace_back(clause);
    return;
  }
  query->insertIrrelevantClause(clause);
}

