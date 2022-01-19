//
// Created by vaish on 1/10/2021.
//

#ifndef AUTOTESTER_SPA_TEAM07_CODE07_SRC_SPA_SRC_PQL_QUERYPARSER_WITHARGUMENT_H_
#define AUTOTESTER_SPA_TEAM07_CODE07_SRC_SPA_SRC_PQL_QUERYPARSER_WITHARGUMENT_H_

#include "WithType.h"
#include <string>

class WithArgument {
  WithType type;
  std::string value;
  AttributeType attribute;

 public:
  WithArgument(WithType type, std::string value, AttributeType attribute);
  WithType getType();
  std::string getValue();
  std::string getInner();
  AttributeType getAttribute();
  std::string toString();
};

#endif //AUTOTESTER_SPA_TEAM07_CODE07_SRC_SPA_SRC_PQL_QUERYPARSER_WITHARGUMENT_H_
