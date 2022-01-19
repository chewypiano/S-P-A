//
// Created by Jazer on 6/11/2021.
//

#include "SuchEvaluator.h"

static std::unordered_map<DesignEntityType, stmtType> stmtTypeMap = {
    {DesignEntityType::Print, stmtType::print},
    {DesignEntityType::Read, stmtType::read},
    {DesignEntityType::Assign, stmtType::assign},
    {DesignEntityType::Call, stmtType::call},
    {DesignEntityType::If, stmtType::ifStmt},
    {DesignEntityType::While, stmtType::whileStmt}
};

std::unordered_set<std::string> SuchEvaluator::getValuesFor(QuerySynonym *synonym) {
  if (!valueStore->resultHasSynonym(synonym)) {
    return valueStore->valuesInType(synonym->getType());
  }

  std::unordered_set<std::string> result;
  for (auto row: valueStore->getResultRows()) {
    result.insert(row.getMap().at(*synonym));
  }
  return result;
}

std::unordered_set<std::string> SuchEvaluator::getValuesFor(Argument *arg, Query *query) {
  ArgumentType type = arg->getType();
  if (type == ArgumentType::Synonym) {
    return getValuesFor(query->findSynonymByName(arg->getValue()));
  }
  if (type == ArgumentType::String || type == ArgumentType::Statement) {
    return {arg->getValue()};
  }
  return {};
}

bool SuchEvaluator::isSameProc(int x, int y) {
  return pkb->getStmtProc(x) == pkb->getStmtProc(y);
}

bool SuchEvaluator::testRelation(SuchThatClause *clause, const std::string &x, const std::string &y) {
  switch (clause->getType()) {
    case ClauseType::ModifiesS:
      return pkb->isStmtModifyingVar(std::stoi(x), y);
    case ClauseType::ModifiesP:
      return pkb->isProcModifyingVar(x, y);
    case ClauseType::UsesS:
      return pkb->isStmtUsingVar(std::stoi(x), y);
    case ClauseType::UsesP:
      return pkb->isProcUsingVar(x, y);
    case ClauseType::Parent:
      return pkb->isParentPair(std::stoi(x), std::stoi(y));
    case ClauseType::ParentT:
      return pkb->isParentStarPair(std::stoi(x), std::stoi(y));
    case ClauseType::Follows:
      return pkb->isFollow(std::stoi(x), std::stoi(y));
    case ClauseType::FollowsT:
      return pkb->isFollowStar(std::stoi(x), std::stoi(y));
    case ClauseType::Calls:
      return pkb->isCallRelation(x, y);
    case ClauseType::CallsT:
      return pkb->isCallStarRelation(x, y);
    case ClauseType::Next: {
      int left = std::stoi(x), right = std::stoi(y);
      auto proc = pkb->getStmtProc(left);
      if (!isSameProc(left, right) || proc.empty()) {
        return false;
      }
      auto stmts = pkb->getNextStatementsOf(left);
      return stmts.count(std::stoi(y)) > 0;
    }
    case ClauseType::NextT: {
      int left = std::stoi(x), right = std::stoi(y);
      auto proc = pkb->getStmtProc(left);
      if (!isSameProc(left, right) || proc.empty()) {
        return false;
      }
      auto cfg = pkb->getCFG(proc);
      return cfg->isNextStar(left, right);
    }
    case ClauseType::Affects: {
      int left = std::stoi(x), right = std::stoi(y);
      std::string proc = pkb->getStmtProc(left);
      if (proc != pkb->getStmtProc(right) || proc.empty()) {
        return false;
      }
      auto cfg = pkb->getCFG(proc);
      return cfg->isAffects(left, right);
    }
    case ClauseType::AffectsT: {
      int left = std::stoi(x), right = std::stoi(y);
      std::string proc = pkb->getStmtProc(left);
      if (!isSameProc(left, right) || proc.empty()) {
        return false;
      }
      auto cfg = pkb->getCFG(proc);
      return cfg->isAffectsStar(left, right);
    }
    case ClauseType::NextBip: {
      int left = std::stoi(x), right = std::stoi(y);
      return pkb->getCFGBip()->isNextBip(left, right);
    }
    case ClauseType::NextBipT: {
      int left = std::stoi(x), right = std::stoi(y);
      auto cfgBip = pkb->getCFGBip();
      return cfgBip->isNextStar(left, right);
    }
    case ClauseType::AffectsBip: {
      int left = std::stoi(x), right = std::stoi(y);
      auto cfgBip = pkb->getCFGBip();
      return cfgBip->isAffects(left, right);
    }
    default:
      return false;
  }
}

bool SuchEvaluator::testLeftWildcardRelation(SuchThatClause *clause, const std::string &y) {
  switch (clause->getType()) {
    // Left wildcards are not allowed for Uses and Modifies

    case ClauseType::Parent:
    case ClauseType::ParentT:
      return pkb->isChild(std::stoi(y));

    case ClauseType::Follows:
    case ClauseType::FollowsT:
      return pkb->isFollowing(std::stoi(y));

    case ClauseType::Calls:
    case ClauseType::CallsT:
      return !pkb->getProcsCalledFromProc(y).empty();

    case ClauseType::Next:
    case ClauseType::NextT:
      return !pkb->getPrevStatementsOf(std::stoi(y)).empty();

    case ClauseType::Affects:
    case ClauseType::AffectsT: {
      int right = std::stoi(y);
      auto left = pkb->getAllStmt();
      return std::any_of(left.begin(), left.end(), [this, right](int left) {
        std::string lProc = this->pkb->getStmtProc(left);
        if (lProc != this->pkb->getStmtProc(right) || lProc.empty()) return false;
        return pkb->getCFG(pkb->getStmtProc(left))->getAffectsList(left).count(right) > 0;
      });
    }

    case ClauseType::NextBip:
    case ClauseType::NextBipT: {
      int right = std::stoi(y);

      // Fast check
      if (!pkb->getPrevStatementsOf(right).empty()) return true;

      auto left = pkb->getAllStmt();
      return std::any_of(left.begin(), left.end(), [this, right](int left) {
        return this->pkb->getCFGBip()->getNextBipList(left).count(right) > 0;
      });
    }

    case ClauseType::AffectsBip: {
      int right = std::stoi(y);
      auto left = pkb->getAllStmt();
      return std::any_of(left.begin(), left.end(), [this, right](int left) {
        return pkb->getCFGBip()->getAffectsList(left).count(right) > 0;
      });
    }

    default:
      return false;
  }
}
bool SuchEvaluator::testRightWildcardRelation(SuchThatClause *clause, const std::string &x) {
  switch (clause->getType()) {
    case ClauseType::ModifiesS:
      return !pkb->getVarModifiedByStmt(std::stoi(x)).empty();
    case ClauseType::ModifiesP:
      return !pkb->getVarModifiedByProc(x).empty();

    case ClauseType::UsesS:
      return !pkb->getVarUsedByStmt(std::stoi(x)).empty();
    case ClauseType::UsesP:
      return !pkb->getVarUsedByProc(x).empty();

    case ClauseType::Parent:
    case ClauseType::ParentT:
      return pkb->isParent(std::stoi(x));

    case ClauseType::Follows:
    case ClauseType::FollowsT:
      return pkb->isBeingFollowed(std::stoi(x));

    case ClauseType::Calls:
    case ClauseType::CallsT:
      return !pkb->getProcsCalledByProc(x).empty();

    case ClauseType::Next:
    case ClauseType::NextT:
      return !pkb->getNextStatementsOf(std::stoi(x)).empty();

    case ClauseType::Affects:
    case ClauseType::AffectsT: {
      int left = std::stoi(x);
      std::string proc = pkb->getStmtProc(left);
      if (proc.empty()) {
        return false;
      }
      return pkb->getCFG(proc)->hasAffects(left);
    }
    case ClauseType::NextBip:
    case ClauseType::NextBipT: {
      int left = std::stoi(x);
      auto cfg = pkb->getCFGBip();
      return cfg->hasNextBip(left);
    }

    case ClauseType::AffectsBip: {
      int left = std::stoi(x);
      std::string proc = pkb->getStmtProc(left);
      return pkb->getCFGBip()->hasAffects(left);
    }

    default:
      return false;
  }
}
bool SuchEvaluator::testBothWildcardRelation(SuchThatClause *clause) {

  switch (clause->getType()) {
    // Left wildcards are not allowed for Uses and Modifies

    case ClauseType::Parent:
    case ClauseType::ParentT:
      return !pkb->getParentList().empty();

    case ClauseType::Follows:
    case ClauseType::FollowsT:
      return !pkb->getAllFollowedBy().empty();

    case ClauseType::Calls:
    case ClauseType::CallsT:
      return !pkb->getCallStmts().empty();

    case ClauseType::Next:
    case ClauseType::NextT:
      return !pkb->getAllNextPairs().empty();

    case ClauseType::Affects:
    case ClauseType::AffectsT: {
      auto cfgList = pkb->getCFGList();
      return std::any_of(cfgList.begin(), cfgList.end(), [this](CFG *c) {
        for (auto i: this->pkb->getAssignStmts()) {
          if (c->hasAffects(i)) return true;
        }
        return false;
      });
    }

    case ClauseType::NextBip:
    case ClauseType::NextBipT:
      return !pkb->getCFGBip()->getNextList().empty();

    case ClauseType::AffectsBip: {
      auto cfgBip = pkb->getCFGBip();
      auto items = pkb->getAllStmt();
      return std::any_of(items.begin(), items.end(), [cfgBip](int stmt) {
        return cfgBip->hasAffects(stmt);
      });
    }

    default:
      return false;
  }
}
bool SuchEvaluator::isLeftInvalid(SuchThatClause *clause, PKB *pkb, const std::string &r) {

  switch (clause->getType()) {
    case ClauseType::Follows:
    case ClauseType::FollowsT:
      return pkb->getFollower(std::stoi(r)) == 0;

    case ClauseType::Parent:
    case ClauseType::ParentT:
      return pkb->getChild(std::stoi(r)).empty();

    case ClauseType::ModifiesP:
      return pkb->getVarModifiedByProc(r).empty();
    case ClauseType::ModifiesS:
      return pkb->getVarModifiedByStmt(std::stoi(r)).empty();

    case ClauseType::UsesP:
      return pkb->getVarUsedByProc(r).empty();
    case ClauseType::UsesS:
      return pkb->getVarUsedByStmt(std::stoi(r)).empty();

    case ClauseType::Calls:
    case ClauseType::CallsT:
      return pkb->getProcsCalledByProc(r).empty();

    case ClauseType::Next:
    case ClauseType::NextT:
      return pkb->getNextStatementsOf(std::stoi(r)).empty();

    case ClauseType::Affects:
    case ClauseType::AffectsT: {
      std::string proc = pkb->getStmtProc(std::stoi(r));
      if (proc.empty()) {
        return false;
      }
      return !pkb->getCFG(proc)->hasAffects(std::stoi(r));
    }
    case ClauseType::NextBip:
    case ClauseType::NextBipT:
      return !pkb->getCFGBip()->hasNextBip(std::stoi(r));

    case ClauseType::AffectsBip:
      return !pkb->getCFGBip()->hasAffects(std::stoi(r));

    default:
      return false;
  }
}
bool SuchEvaluator::isRightInvalid(SuchThatClause *clause, PKB *pkb, const std::string &r) {

  switch (clause->getType()) {
    case ClauseType::Follows:
    case ClauseType::FollowsT:
      return pkb->getFollowedBy(std::stoi(r)) == 0;

    case ClauseType::Parent:
    case ClauseType::ParentT:
      return pkb->getParent(std::stoi(r)) == 0;

    case ClauseType::ModifiesP:
      return pkb->getProcModifyingVar(r).empty();
    case ClauseType::ModifiesS:
      return pkb->getStmtModifyingVar(r).empty();

    case ClauseType::UsesP:
      return pkb->getProcUsingVar(r).empty();
    case ClauseType::UsesS:
      return pkb->getStmtUsingVar(r).empty();

    case ClauseType::Calls:
    case ClauseType::CallsT:
      return pkb->getProcsCalledFromProc(r).empty();

    case ClauseType::Next:
    case ClauseType::NextT:
      return pkb->getPrevStatementsOf(std::stoi(r)).empty();

    case ClauseType::Affects:
    case ClauseType::AffectsT: {
      int right = std::stoi(r);
      std::string proc = pkb->getStmtProc(right);
      if (proc.empty()) {
        return false;
      }
      auto cfg = pkb->getCFG(proc);
      for (auto &s: pkb->getStmtList(proc)) {
        if (cfg->isAffects(s, right)) return false;
      }
      return true;
    }

    case ClauseType::NextBip:
    case ClauseType::NextBipT: {
      int right = std::stoi(r);
      auto cfg = pkb->getCFGBip();
      auto stmts = pkb->getAllStmt();
      return std::none_of(stmts.begin(), stmts.end(), [cfg, right](int s) {
        return cfg->isNextBip(s, right);
      });
    }

    case ClauseType::AffectsBip: {
      int right = std::stoi(r);
      auto cfg = pkb->getCFGBip();
      auto all = pkb->getAllStmt();
      return std::none_of(all.begin(), all.end(), [cfg, right](int s) {
        return cfg->getAffectsList(s).count(right) > 0;
      });
    }

    default:
      return false;
  }
}
ClauseResult *SuchEvaluator::leftEval(SuchThatClause *clause, Query *query) {
  switch (clause->getLeft()->getType()) {
    case ArgumentType::String:
    case ArgumentType::Statement:
      return new ClauseResult(testRightWildcardRelation(clause, clause->getLeft()->getValue()));
    case ArgumentType::Synonym: {
      auto *result = new ClauseResult();
      for (const auto &r: getValuesFor(clause->getLeft(), query)) {
        if (isLeftInvalid(clause, pkb, r)) {
          continue;
        }
        Row row = Row();
        row.addSynonym(*query->findSynonymByName(clause->getLeft()->getValue()), r);
        result->addRow(row);
      }

      return result;
    }
    default:
      throw std::invalid_argument("Argument type not supported in left eval");
  }
}
ClauseResult *SuchEvaluator::rightEval(SuchThatClause *clause, Query *query) {
  switch (clause->getRight()->getType()) {
    case ArgumentType::String:
    case ArgumentType::Statement:
      return new ClauseResult(testLeftWildcardRelation(clause, clause->getRight()->getValue()));
    case ArgumentType::Synonym: {
      auto *result = new ClauseResult();
      for (const auto &r: getValuesFor(clause->getRight(), query)) {
        if (isRightInvalid(clause, pkb, r)) {
          continue;
        }
        Row row = Row();
        row.addSynonym(*query->findSynonymByName(clause->getRight()->getValue()), r);
        result->addRow(row);
      }
      return result;
    }
    default:
      throw std::invalid_argument("Argument type not supported in left eval");
  }
}

ClauseResult *SuchEvaluator::evaluateClause(SuchThatClause *clause, Query *query) {

  ArgumentType leftType = clause->getLeft()->getType(), rightType = clause->getRight()->getType();

  // Populating left and right value sets
  std::unordered_set<std::string> leftValues = getValuesFor(clause->getLeft(), query),
      rightValues = getValuesFor(clause->getRight(), query);

  // Wildcard handling
  if (leftType == ArgumentType::Wildcard && rightType == ArgumentType::Wildcard) {
    return new ClauseResult(testBothWildcardRelation(clause));
  }

  if (leftType == ArgumentType::Wildcard) {
    // Right -> Syn / String
    return rightEval(clause, query);
  }

  if (rightType == ArgumentType::Wildcard) {
    // Left -> Syn / String
    return leftEval(clause, query);
  }

  // Left & right -> syn / string
  return bothEval(clause, query);
}
ClauseResult *SuchEvaluator::bothEval(SuchThatClause *clause, Query *query) {
  Argument *left = clause->getLeft();
  switch (left->getType()) {
    case ArgumentType::String:
    case ArgumentType::Statement:
      return bothEvalLeftConst(clause, query);
    case ArgumentType::Synonym:
      return bothEvalLeftSyn(clause, query);
    default:
      throw std::invalid_argument("Argument type not supported in both eval");
  }
}
ClauseResult *SuchEvaluator::bothEvalLeftSyn(SuchThatClause *clause, Query *query) {
  Argument *left = clause->getLeft();
  Argument *right = clause->getRight();
  switch (right->getType()) {
    case ArgumentType::String:
    case ArgumentType::Statement:
      return bothEvalLeftSynRightConst(clause, query);
    case ArgumentType::Synonym:
      return bothEvalBothSyn(clause, query);
    default:
      throw std::invalid_argument("Argument type not supported in both eval");
  }
}
ClauseResult *SuchEvaluator::bothEvalBothSyn(SuchThatClause *clause, Query *query) {
  auto result = new ClauseResult();
  Argument *left = clause->getLeft();
  Argument *right = clause->getRight();
  if (left->getValue() == right->getValue()) {
    std::unordered_set<std::string> values = getValuesFor(left, query);
    QuerySynonym *syn = query->findSynonymByName(left->getValue());
    for (const auto &i: values) {
      if (!testRelation(clause, i, i)) continue;
      Row r = Row();
      r.addSynonym(*syn, i);
      result->addRow(r);
    }
    return result;
  }
  std::unordered_set<std::string> leftVals = getValuesFor(left, query);
  std::unordered_set<std::string> rightVals = getValuesFor(right, query);
  if (leftVals.size() <= rightVals.size()) {
    QuerySynonym *syn = query->findSynonymByName(left->getValue());
    for (const auto &x: leftVals) {
      std::pair<QuerySynonym *, SynonymValue> curr = {syn, x};
      addRightArgSynonymRows(clause, query, curr, result, x);
    }
  } else {
    QuerySynonym *syn = query->findSynonymByName(right->getValue());
    for (const auto &y: rightVals) {
      std::pair<QuerySynonym *, SynonymValue> curr = {syn, y};
      addLeftArgSynonymRows(clause, query, curr, result, y);
    }
  }
  return result;
}

ClauseResult *SuchEvaluator::bothEvalLeftConst(SuchThatClause *clause, Query *query) {
  Argument *left = clause->getLeft();
  Argument *right = clause->getRight();
  switch (right->getType()) {
    case ArgumentType::String:
    case ArgumentType::Statement: {
      std::string leftVal = left->getValue();
      std::string rightVal = right->getValue();
      return new ClauseResult(testRelation(clause, leftVal, rightVal));
    }
    case ArgumentType::Synonym:
      return bothEvalLeftConstRightSyn(clause, query);
    default:
      throw std::invalid_argument("Argument type not supported in both eval");
  }
}

ClauseResult *SuchEvaluator::bothEvalLeftConstRightSyn(SuchThatClause *clause, Query *query) {
  auto *result = new ClauseResult();
  Argument *left = clause->getLeft();
  addRightArgSynonymRows(clause, query, {nullptr, ""}, result, left->getValue());
  return result;
}

ClauseResult *SuchEvaluator::bothEvalLeftSynRightConst(SuchThatClause *clause, Query *query) {
  auto *result = new ClauseResult();
  Argument *right = clause->getRight();
  addLeftArgSynonymRows(clause, query, {nullptr, ""}, result, right->getValue());
  return result;
}

void SuchEvaluator::iterateStringSetAndAddRowsToResult(ClauseResult *result,
                                                       QuerySynonym *syn,
                                                       const std::unordered_set<std::string> &set,
                                                       const std::pair<QuerySynonym *, std::string> &synPair) {
  for (const auto &a: set) {
    Row row = Row();
    row.addSynonym(*syn, a);
    if (synPair.first != nullptr) {
      row.addSynonym(*synPair.first, synPair.second);
    }
    result->addRow(row);
  }
}
void SuchEvaluator::iterateIntSetAndAddRowsToResult(ClauseResult *result,
                                                    QuerySynonym *syn,
                                                    const std::unordered_set<int> &set,
                                                    const std::pair<QuerySynonym *, std::string> &synPair) {
  for (const auto &a: set) {
    addIntRowToResult(result, syn, a, synPair);
  }
}

void SuchEvaluator::addIntRowToResult(ClauseResult *result,
                                      QuerySynonym *syn,
                                      int num,
                                      const std::pair<QuerySynonym *, std::string> &synPair) {
  if (!isMatchingStmtType(syn, pkb->getStmtType(num)) || num == 0) {
    return;
  }
  Row row = Row();
  row.addSynonym(*syn, std::to_string(num));
  if (synPair.first != nullptr) {
    row.addSynonym(*synPair.first, synPair.second);
  }
  result->addRow(row);
}

void SuchEvaluator::naiveIterationWithExistingVals(SuchThatClause *clause,
                                                   Query *query,
                                                   const std::pair<QuerySynonym *, std::string> &rightSynPair,
                                                   ClauseResult *result,
                                                   const std::string &rightVal) {
  QuerySynonym *syn = query->findSynonymByName(clause->getLeft()->getValue());
  for (const auto &left: getValuesFor(clause->getLeft(), query)) {
    if (!testRelation(clause, left, rightVal)) continue;
    addIntRowToResult(result, syn, std::stoi(left), rightSynPair);
  }
}

void SuchEvaluator::addLeftArgSynonymRows(SuchThatClause *clause,
                                          Query *query,
                                          const std::pair<QuerySynonym *, std::string> &rightSynPair,
                                          ClauseResult *result,
                                          const std::string &rightVal) {
  QuerySynonym *syn = query->findSynonymByName(clause->getLeft()->getValue());

  switch (clause->getType()) {
    case ClauseType::ModifiesS: {
      std::unordered_set<int> set = pkb->getStmtModifyingVar(rightVal);
      iterateIntSetAndAddRowsToResult(result, syn, set, rightSynPair);
      break;
    }
    case ClauseType::ModifiesP: {
      std::unordered_set<std::string> set = pkb->getProcModifyingVar(rightVal);
      iterateStringSetAndAddRowsToResult(result, syn, set, rightSynPair);
      break;
    }
    case ClauseType::UsesS: {
      std::unordered_set<int> set = pkb->getStmtUsingVar(rightVal);
      iterateIntSetAndAddRowsToResult(result, syn, set, rightSynPair);
      break;
    }
    case ClauseType::UsesP: {
      std::unordered_set<std::string> set = pkb->getProcUsingVar(rightVal);
      iterateStringSetAndAddRowsToResult(result, syn, set, rightSynPair);
      break;
    }
    case ClauseType::Parent: {
      int parent = pkb->getParent(std::stoi(rightVal));
      addIntRowToResult(result, syn, parent, rightSynPair);
      break;
    }
    case ClauseType::ParentT: {
      std::unordered_set<int> set = pkb->getParentStar(std::stoi(rightVal));
      iterateIntSetAndAddRowsToResult(result, syn, set, rightSynPair);
      break;
    }
    case ClauseType::Follows: {
      int stmtNo = pkb->getFollowedBy(std::stoi(rightVal));
      addIntRowToResult(result, syn, stmtNo, rightSynPair);
      break;
    }
    case ClauseType::FollowsT: {
      std::unordered_set<int> set = pkb->getFollowedByStar(std::stoi(rightVal));
      iterateIntSetAndAddRowsToResult(result, syn, set, rightSynPair);
      break;
    }

    case ClauseType::Calls: {
      std::unordered_set<std::string> set = pkb->getProcsCalledFromProc(rightVal);
      iterateStringSetAndAddRowsToResult(result, syn, set, rightSynPair);
      break;
    }
    case ClauseType::CallsT: {
      std::unordered_set<std::string> set = pkb->getProcsCalledStarFromProc(rightVal);
      iterateStringSetAndAddRowsToResult(result, syn, set, rightSynPair);
      break;
    }
    case ClauseType::Next: {
      std::unordered_set<int> set = pkb->getPrevStatementsOf(std::stoi(rightVal));
      iterateIntSetAndAddRowsToResult(result, syn, set, rightSynPair);
      break;
    }
    case ClauseType::NextT:
    case ClauseType::Affects:
    case ClauseType::AffectsT:
    case ClauseType::NextBip:
    case ClauseType::NextBipT:
    case ClauseType::AffectsBip: {
      naiveIterationWithExistingVals(clause, query, rightSynPair, result, rightVal);
      break;
    }
    default:
      throw std::invalid_argument("Invalid clause type in addRightArgSynonymRows");
  }
}

void SuchEvaluator::addRightArgSynonymRows(SuchThatClause *clause,
                                           Query *query,
                                           const std::pair<QuerySynonym *, std::string> &leftSynPair,
                                           ClauseResult *result,
                                           const std::string &leftVal) {
  QuerySynonym *syn = query->findSynonymByName(clause->getRight()->getValue());

  switch (clause->getType()) {
    case ClauseType::ModifiesS: {
      std::unordered_set<std::string> set = pkb->getVarModifiedByStmt(std::stoi(leftVal));
      iterateStringSetAndAddRowsToResult(result, syn, set, leftSynPair);
      break;
    }
    case ClauseType::ModifiesP: {
      std::unordered_set<std::string> set = pkb->getVarModifiedByProc(leftVal);
      iterateStringSetAndAddRowsToResult(result, syn, set, leftSynPair);
      break;
    }
    case ClauseType::UsesS: {
      std::unordered_set<std::string> set = pkb->getVarUsedByStmt(std::stoi(leftVal));
      iterateStringSetAndAddRowsToResult(result, syn, set, leftSynPair);
      break;
    }
    case ClauseType::UsesP: {
      std::unordered_set<std::string> set = pkb->getVarUsedByProc(leftVal);
      iterateStringSetAndAddRowsToResult(result, syn, set, leftSynPair);
      break;
    }
    case ClauseType::Parent: {
      std::unordered_set<int> set = pkb->getChild(std::stoi(leftVal));
      iterateIntSetAndAddRowsToResult(result, syn, set, leftSynPair);
      break;
    }
    case ClauseType::ParentT: {
      std::unordered_set<int> set = pkb->getChildStar(std::stoi(leftVal));
      iterateIntSetAndAddRowsToResult(result, syn, set, leftSynPair);
      break;
    }
    case ClauseType::Follows: {
      int stmtNo = pkb->getFollower(std::stoi(leftVal));
      addIntRowToResult(result, syn, stmtNo, leftSynPair);
      break;
    }
    case ClauseType::FollowsT: {
      std::unordered_set<int> set = pkb->getFollowerStar(std::stoi(leftVal));
      iterateIntSetAndAddRowsToResult(result, syn, set, leftSynPair);
      break;
    }
    case ClauseType::Calls: {
      std::unordered_set<std::string> set = pkb->getProcsCalledByProc(leftVal);
      iterateStringSetAndAddRowsToResult(result, syn, set, leftSynPair);
      break;
    }
    case ClauseType::CallsT: {
      std::unordered_set<std::string> set = pkb->getProcsCalledStarByProc(leftVal);
      iterateStringSetAndAddRowsToResult(result, syn, set, leftSynPair);
      break;
    }
    case ClauseType::Next: {
      std::unordered_set<int> set = pkb->getNextStatementsOf(std::stoi(leftVal));
      iterateIntSetAndAddRowsToResult(result, syn, set, leftSynPair);
      break;
    }
    case ClauseType::NextT: {
      int stmtNo = std::stoi(leftVal);
      auto proc = pkb->getStmtProc(stmtNo);
      auto cfg = pkb->getCFG(proc);
      std::unordered_set<int> set = cfg->getNextStarList(stmtNo);
      iterateIntSetAndAddRowsToResult(result, syn, set, leftSynPair);
      break;
    }
    case ClauseType::Affects: {
      int stmtNo = std::stoi(leftVal);
      auto proc = pkb->getStmtProc(stmtNo);
      if (proc.empty()) {
        return;
      }
      auto cfg = pkb->getCFG(proc);
      std::unordered_set<int> set = cfg->getAffectsList(stmtNo);
      iterateIntSetAndAddRowsToResult(result, syn, set, leftSynPair);
      break;
    }
    case ClauseType::AffectsT: {
      int stmtNo = std::stoi(leftVal);
      auto proc = pkb->getStmtProc(stmtNo);
      if (proc.empty()) {
        return;
      }
      auto cfg = pkb->getCFG(proc);
      std::unordered_set<int> set = cfg->getAffectsStarList(stmtNo);
      iterateIntSetAndAddRowsToResult(result, syn, set, leftSynPair);
      break;
    }
    case ClauseType::NextBip: {
      int left = std::stoi(leftVal);
      auto cfgBip = pkb->getCFGBip();
      auto set = cfgBip->getNextBipList(left);
      iterateIntSetAndAddRowsToResult(result, syn, set, leftSynPair);
      break;
    }
    case ClauseType::NextBipT: {
      int left = std::stoi(leftVal);
      auto cfgBip = pkb->getCFGBip();
      std::unordered_set<int> set = cfgBip->getNextStarList(left);
      iterateIntSetAndAddRowsToResult(result, syn, set, leftSynPair);
      break;
    }
    case ClauseType::AffectsBip: {
      int left = std::stoi(leftVal);
      auto cfgBip = pkb->getCFGBip();
      std::unordered_set<int> set = cfgBip->getAffectsList(left);
      iterateIntSetAndAddRowsToResult(result, syn, set, leftSynPair);
      break;
    }
    default:
      throw std::invalid_argument("Invalid clause type in addRightArgSynonymRows");
  }
}

bool SuchEvaluator::isMatchingStmtType(QuerySynonym *syn, stmtType receivedType) {
  DesignEntityType requiredType = syn->getType();
  return requiredType == DesignEntityType::ProgLine
      || requiredType == DesignEntityType::Stmt
      || stmtTypeMap[requiredType] == receivedType;
}

SuchEvaluator::SuchEvaluator(PKB *pkb, ValueStore *valueStore) {
  this->pkb = pkb;
  this->valueStore = valueStore;

}


