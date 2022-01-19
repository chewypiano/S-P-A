//
// Created by Junhua on 4/11/2021.
//

#ifndef AUTOTESTER_SRC_SPA_SRC_PQL_QUERYEVALUATOR_WITHEVALUATOR_H_
#define AUTOTESTER_SRC_SPA_SRC_PQL_QUERYEVALUATOR_WITHEVALUATOR_H_

#include "PKB/PKB.h"
#include "ClauseResult.h"
#include "Query.h"

class WithEvaluator {
 private:
  PKB *pkb;
  std::vector<std::tuple<std::string,
                         std::string,
                         QuerySynonym *>> makeComparableResult(WithArgument *withArg, Query *query);
  ClauseResult *evaluateWithSynSynClause(WithClause *clause, Query *query);
  ClauseResult *evaluateWithSynClause(WithClause *clause, Query *query, WithArgument *leftArg, WithArgument *rightArg);
  ClauseResult *evaluateWithConst(Query *query, const std::string &leftValue, WithArgument *rightArg);
 public:
  explicit WithEvaluator(PKB *pkb);
  ClauseResult *evaluateWithClause(WithClause *clause, Query *query);
};

#endif //AUTOTESTER_SRC_SPA_SRC_PQL_QUERYEVALUATOR_WITHEVALUATOR_H_
