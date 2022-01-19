//
// Created by Junhua on 4/11/2021.
//

#include "WithEvaluator.h"

WithEvaluator::WithEvaluator(PKB *pkb) {
  this->pkb = pkb;
}

std::vector<std::tuple<std::string,
                       std::string,
                       QuerySynonym *>> WithEvaluator::makeComparableResult(WithArgument *withArg, Query *query) {
  std::vector<std::tuple<std::string, std::string, QuerySynonym *>>
      result = std::vector<std::tuple<std::string, std::string, QuerySynonym *>>();
  auto syn = query->findSynonymByName(withArg->getValue());
  switch (withArg->getAttribute()) {
    case AttributeType::ProcName: {
      switch (syn->getType()) {
        case DesignEntityType::Call: {
          for (auto stmt: pkb->getCallStmts()) {
            std::string name = pkb->getCalledProc(stmt);
            result.emplace_back(std::make_tuple(name, std::to_string(stmt), syn));
          }
          return result;
        }
        case DesignEntityType::Procedure: {
          for (const auto &proc: pkb->getProcList()) {
            result.emplace_back(std::make_tuple(proc, proc, syn));
          }
          return result;
        }
        default: {
          throw std::invalid_argument("Invalid call type for comparable result");
        }
      }
    }
    case AttributeType::VarName: {
      switch (syn->getType()) {
        case DesignEntityType::Variable: {
          for (const auto &varName: pkb->getVarList()) {
            result.emplace_back(std::make_tuple(varName, varName, syn));
          }
          return result;
        }
        case DesignEntityType::Read: {
          for (const auto &p: pkb->getReadList()) {
            result.emplace_back(std::make_tuple(p.second, std::to_string(p.first), syn));
          }
          return result;
        }
        case DesignEntityType::Print: {
          for (const auto &p: pkb->getPrintList()) {
            result.emplace_back(std::make_tuple(p.second, std::to_string(p.first), syn));
          }
          return result;
        }
        default: {
          throw std::invalid_argument("Invalid call type for comparable result");
        }
      }

    }
    case AttributeType::Value: {
      if (syn->getType() != DesignEntityType::Constant) {
        throw std::invalid_argument("Invalid call type");
      }
      for (const auto &p: pkb->getConstList()) {
        result.emplace_back(std::make_tuple(p, p, syn));
      }
      return result;
    }
    case AttributeType::StmtNo: {
      std::unordered_set<DesignEntityType> acceptable =
          {DesignEntityType::Stmt, DesignEntityType::Read, DesignEntityType::Print, DesignEntityType::Call,
           DesignEntityType::While, DesignEntityType::If, DesignEntityType::Assign};
      if (acceptable.find(syn->getType()) == acceptable.end()) {
        throw std::invalid_argument("Invalid argument call");
      }
      std::unordered_set<int> lines;
      switch (syn->getType()) {
        case DesignEntityType::Stmt: {
          lines = pkb->getAllStmt();
          break;
        }
        case DesignEntityType::Assign: {
          lines = pkb->getAssignStmts();
          break;
        }
        case DesignEntityType::Print: {
          lines = pkb->getPrintStmts();
          break;
        }
        case DesignEntityType::Read: {
          lines = pkb->getReadStmts();
          break;
        }
        case DesignEntityType::Call: {
          lines = pkb->getCallStmts();
          break;
        }
        case DesignEntityType::While: {
          lines = pkb->getAllWhile();
          break;
        }
        case DesignEntityType::If: {
          lines = pkb->getAllIf();
          break;
        }
        default:
          throw std::invalid_argument("Invalid argument call");
      }
      for (const auto &p: lines) {
        result.emplace_back(make_tuple(std::to_string(p), std::to_string(p), syn));
      }
      return result;
    }
    case AttributeType::NotApplicable: {
      if (syn->getType() != DesignEntityType::ProgLine)
        throw std::invalid_argument("Invalid WithSyn type");

      std::unordered_set<int> lines = pkb->getAllStmt();

      for (const auto &p: lines) {
        result.emplace_back(make_tuple(std::to_string(p), std::to_string(p), syn));
      }
      return result;

    }
    default:
      throw std::invalid_argument("Wrong attribute call");
  }
}

ClauseResult *WithEvaluator::evaluateWithSynSynClause(WithClause *clause, Query *query) {
  auto result = new ClauseResult();
  auto leftArg = clause->getLeft();
  auto rightArg = clause->getRight();

  auto leftArgsRepr = makeComparableResult(leftArg, query);
  auto rightArgsRepr = makeComparableResult(rightArg, query);

  for (auto leftRes: leftArgsRepr) {
    for (auto rightRes: rightArgsRepr) {
      if (std::get<0>(leftRes) == std::get<0>(rightRes)) {
        Row r = Row();
        r.addSynonym(*std::get<2>(leftRes), std::get<1>(leftRes));
        r.addSynonym(*std::get<2>(rightRes), std::get<1>(rightRes));
        result->addRow(r);
      }
    }
  }
  return result;
}
ClauseResult *WithEvaluator::evaluateWithSynClause(
    WithClause *clause,
    Query *query,
    WithArgument *leftArg,
    WithArgument *rightArg
) {
  auto result = new ClauseResult();
  switch (rightArg->getType()) {
    case WithType::Attribute:
    case WithType::Synonym:
      return evaluateWithSynSynClause(clause, query);
    case WithType::Integer:
    case WithType::String:
      for (auto lRes: makeComparableResult(leftArg, query)) {
        if (std::get<0>(lRes) == rightArg->getInner()) {
          Row r = Row();
          r.addSynonym(*std::get<2>(lRes), std::get<1>(lRes));
          result->addRow(r);
        }
      }
      return result;

    default:
      throw std::invalid_argument("Invalid WithSyn rightArg type");
  }
}

ClauseResult *WithEvaluator::evaluateWithConst(Query *query, const std::string &leftValue, WithArgument *rightArg) {
  auto result = new ClauseResult();
  switch (rightArg->getType()) {
    case WithType::String:
    case WithType::Integer:
      return new ClauseResult(rightArg->getInner() == leftValue);
    case WithType::Attribute: {
      auto rightVals = makeComparableResult(rightArg, query);
      for (auto p: rightVals) {
        if (std::get<0>(p) == leftValue) {
          Row r = Row();
          r.addSynonym(*std::get<2>(p), std::get<1>(p));
          result->addRow(r);
        }
      }
      return result;
    }
    default:
      throw std::invalid_argument("Invalid obj");
  }
}

ClauseResult *WithEvaluator::evaluateWithClause(WithClause *clause, Query *query) {
  auto leftArg = clause->getLeft();
  switch (leftArg->getType()) {
    case WithType::String:
    case WithType::Integer:
      return evaluateWithConst(query, leftArg->getValue(), clause->getRight());
    case WithType::Attribute:
    case WithType::Synonym:
      return evaluateWithSynClause(clause, query, leftArg, clause->getRight());
    default:
      return new ClauseResult();
  }
}