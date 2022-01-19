//
// Created by Junhua on 13/10/2021.
//

#ifndef AUTOTESTER_SRC_SPA_SRC_PQL_QUERYEVALUATOR_QUERYEVALUATOR_CPP_PATTERNEVALUATOR_H_
#define AUTOTESTER_SRC_SPA_SRC_PQL_QUERYEVALUATOR_QUERYEVALUATOR_CPP_PATTERNEVALUATOR_H_

#include "ClauseResult.h"
#include "Query.h"
#include "PKB/PKB.h"
#include "ValueStore.h"

#include <string>
#include <functional>
#include <unordered_set>

class PatternEvaluator {
  ClauseResult *evaluateIfPattern(PatternClause *clause, Query *query);
  ClauseResult *evaluateWhilePattern(PatternClause *clause, Query *query);
  ClauseResult *evaluateAssignPattern(PatternClause *clause, Query *query);
  static bool matchPattern(const std::string &text, const std::string &pattern, bool isDoubleWildcard);
  std::unordered_set<std::string> getValuesFor(QuerySynonym *);
  std::unordered_set<std::string> getValuesFor(PQLPattern *arg, Query *query);

  PKB *pkb;
  ValueStore *valueStore;
  std::function<bool(std::string)> ifPatternCheck;
  std::function<bool(std::string)> whilePatternCheck;
 public:
  explicit PatternEvaluator(PKB *pkb,
                            ValueStore *valueStore);
  ClauseResult *evaluatePatternClause(PatternClause *clause, Query *query);
};

#endif //AUTOTESTER_SRC_SPA_SRC_PQL_QUERYEVALUATOR_QUERYEVALUATOR_CPP_PATTERNEVALUATOR_H_
