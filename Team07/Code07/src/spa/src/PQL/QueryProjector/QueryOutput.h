//
// Created by vaish on 8/10/2021.
//

#ifndef AUTOTESTER_SPA_TEAM07_CODE07_SRC_SPA_SRC_PQL_QUERYPROJECTOR_QUERYOUTPUT_H_
#define AUTOTESTER_SPA_TEAM07_CODE07_SRC_SPA_SRC_PQL_QUERYPROJECTOR_QUERYOUTPUT_H_

#include "QuerySynonym.h"
#include "QueryOutputType.h"

class QueryOutput {
  QueryOutputType type;
  QuerySynonym *synonym;

 public:
  explicit QueryOutput(QueryOutputType type);

  QueryOutput(QueryOutputType type, QuerySynonym *synonym);

  QueryOutputType getType();

  QuerySynonym *getSynonym();

  std::string toString();
};

#endif //AUTOTESTER_SPA_TEAM07_CODE07_SRC_SPA_SRC_PQL_QUERYPROJECTOR_QUERYOUTPUT_H_
