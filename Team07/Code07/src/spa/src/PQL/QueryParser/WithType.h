//
// Created by vaish on 1/10/2021.
//

#ifndef AUTOTESTER_SPA_TEAM07_CODE07_SRC_SPA_SRC_PQL_QUERYPARSER_WITHTYPE_H_
#define AUTOTESTER_SPA_TEAM07_CODE07_SRC_SPA_SRC_PQL_QUERYPARSER_WITHTYPE_H_

enum class WithType {
  String,
  Integer,
  Attribute,
  Synonym
};

enum class AttributeType {
  ProcName,
  VarName,
  Value,
  StmtNo,
  NotApplicable
};

#endif //AUTOTESTER_SPA_TEAM07_CODE07_SRC_SPA_SRC_PQL_QUERYPARSER_WITHTYPE_H_
