//
// Created by Jazer on 6/11/2021.
//

#ifndef AUTOTESTER_TEAM07_CODE07_SRC_SPA_SRC_PQL_QUERYEVALUATOR_SUCHEVALUATOR_H_
#define AUTOTESTER_TEAM07_CODE07_SRC_SPA_SRC_PQL_QUERYEVALUATOR_SUCHEVALUATOR_H_

#include <PKB/PKB.h>
#include <Query.h>
#include "ValueStore.h"
#include "ClauseResult.h"

class SuchEvaluator {

 public:
  SuchEvaluator(PKB *pkb, ValueStore *valueStore);
  ClauseResult* evaluateClause(SuchThatClause *clause, Query *query);

 private:
  PKB *pkb;
  ValueStore *valueStore;

  // Right is a wildcard
  ClauseResult* leftEval(SuchThatClause *clause, Query *query);
  // Left is a wildcard
  ClauseResult* rightEval(SuchThatClause *clause, Query *query);
  // Neither are wildcards
  ClauseResult* bothEval(SuchThatClause *clause, Query *query);
  ClauseResult* bothEvalLeftSyn(SuchThatClause *clause, Query *query);
  ClauseResult* bothEvalLeftConst(SuchThatClause *clause, Query *query);
  ClauseResult* bothEvalBothSyn(SuchThatClause *clause, Query *query);
  ClauseResult* bothEvalLeftConstRightSyn(SuchThatClause *clause, Query *query);
  ClauseResult* bothEvalLeftSynRightConst(SuchThatClause *clause, Query *query);

  bool isLeftInvalid(SuchThatClause *clause, PKB *pkb, const std::string &r);
  bool isRightInvalid(SuchThatClause *clause, PKB *pkb, const std::string &r);

  bool testRelation(SuchThatClause *clause, const std::string &x, const std::string &y);
  bool testLeftWildcardRelation(SuchThatClause *clause, const std::string &y);
  bool testRightWildcardRelation(SuchThatClause *clause, const std::string &x);
  bool testBothWildcardRelation(SuchThatClause *clause);

  bool isSameProc(int x, int y);

  std::unordered_set<std::string> getValuesFor(Argument *arg, Query *query);
  std::unordered_set<std::string> getValuesFor(QuerySynonym *synonym);

  static void iterateStringSetAndAddRowsToResult(ClauseResult *result,
                                                 QuerySynonym *syn,
                                                 const std::unordered_set<std::string> &set,
                                                 const std::pair<QuerySynonym *, std::string> &synPair);
  void iterateIntSetAndAddRowsToResult(ClauseResult *result,
                                       QuerySynonym *syn,
                                       const std::unordered_set<int> &set,
                                       const std::pair<QuerySynonym *, std::string> &synPair);
  void addIntRowToResult(ClauseResult *result,
                         QuerySynonym *syn,
                         int num,
                         const std::pair<QuerySynonym *, std::string> &synPair);
  void naiveIterationWithExistingVals(SuchThatClause *clause,
                                      Query *query,
                                      const std::pair<QuerySynonym *, std::string> &leftSynPair,
                                      ClauseResult *result,
                                      const std::string &rightVal);
  void addLeftArgSynonymRows(SuchThatClause *clause,
                             Query *query,
                             const std::pair<QuerySynonym *, std::string> &rightSynPair,
                             ClauseResult *result,
                             const std::string &rightVal);
  void addRightArgSynonymRows(SuchThatClause *clause,
                              Query *query,
                              const std::pair<QuerySynonym *, std::string> &leftSynPair,
                              ClauseResult *result,
                              const std::string &leftVal);

  bool isMatchingStmtType(QuerySynonym *p_synonym, stmtType type);
};

#endif //AUTOTESTER_TEAM07_CODE07_SRC_SPA_SRC_PQL_QUERYEVALUATOR_SUCHEVALUATOR_H_
