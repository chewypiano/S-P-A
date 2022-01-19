//
// Created by Junhua on 4/10/2021.
//

#ifndef AUTOTESTER_SRC_SPA_SRC_PQL_QUERYPROJECTOR_QUERYPROJECTOR_H_
#define AUTOTESTER_SRC_SPA_SRC_PQL_QUERYPROJECTOR_QUERYPROJECTOR_H_

#include "ClauseResult.h"
#include <list>
#include <PKB/PKB.h>

class QueryProjector {
  static std::unordered_set<DesignEntityType> readPrintType;
  static std::string falseResultString;
  static std::string trueResultString;
  PKB *pkb;

  std::string getLineFromRow(const RowMap &mp, const std::vector<QueryOutput *> &outputList);
  std::string addStringBasedOnType(QueryOutput *singleOut, RowMap mp);
 public:
  explicit QueryProjector(PKB *pkb);
  void projectAnswer(ClauseResult *clauseResult,
                     const std::vector<QueryOutput *> &outputList,
                     std::list<std::string> &results);
};

#endif //AUTOTESTER_SRC_SPA_SRC_PQL_QUERYPROJECTOR_QUERYPROJECTOR_H_
