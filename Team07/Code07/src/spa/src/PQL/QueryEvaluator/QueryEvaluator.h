//
// Created by Junhua on 30/9/2021.
//

#ifndef AUTOTESTER_SRC_SPA_SRC_NEWQE_QUERYEVALUATORV2_H_
#define AUTOTESTER_SRC_SPA_SRC_NEWQE_QUERYEVALUATORV2_H_

#include <QueryParser.h>
#include <PKB/PKB.h>
#include <sstream>
#include <string>
#include <functional>

#include "ClauseResult.h"
#include "Query.h"
#include "QueryParser.h"
#include "PatternEvaluator.h"
#include "WithEvaluator.h"
#include "ValueStore.h"
#include "SuchEvaluator.h"

class QueryEvaluator {
 public:
  ClauseResult *evaluate();
  explicit QueryEvaluator(QueryParser *parser, PKB *pkb);

  ClauseResult *evaluateQuery(Query &);
  ClauseResult *collector;

 private:
  QueryParser *parser;
  std::function<size_t(Clause *)> typeSort;
  std::function<size_t(ClauseResult *)> sizeSort;
  WithEvaluator *withEvaluator;
  PatternEvaluator *patternEvaluator;
  SuchEvaluator *suchEvaluator;
  ValueStore *valueStore;
  PKB *pkb;

  std::unordered_set<std::string> getValuesFor(QuerySynonym *synonym);

  ClauseResult *evaluateClause(Clause *clause, Query *query);


};

#endif //AUTOTESTER_SRC_SPA_SRC_NEWQE_QUERYEVALUATORV2_H_
