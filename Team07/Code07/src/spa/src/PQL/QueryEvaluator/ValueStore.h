//
// Created by Jazer on 6/11/2021.
//

#ifndef AUTOTESTER_TEAM07_CODE07_SRC_SPA_SRC_PQL_QUERYEVALUATOR_VALUESTORE_H_
#define AUTOTESTER_TEAM07_CODE07_SRC_SPA_SRC_PQL_QUERYEVALUATOR_VALUESTORE_H_

#include <PKB/PKB.h>
#include <DesignEntityType.h>
#include "ClauseResult.h"

class ValueStore {
 private:
  std::unordered_map<DesignEntityType, std::unordered_set<std::string>> storage;
  static std::unordered_set<std::string> intSetToStringSet(const std::unordered_set<int> &set);
  ClauseResult* results;


 public:
  ValueStore(PKB* pkb);
  std::unordered_set<std::string> valuesInType(DesignEntityType type);
  RowList getResultRows();

  bool resultHasSynonym(QuerySynonym *synonym);
  void setStoredResults(ClauseResult *collected);
  void clearResults();
};

#endif //AUTOTESTER_TEAM07_CODE07_SRC_SPA_SRC_PQL_QUERYEVALUATOR_VALUESTORE_H_
