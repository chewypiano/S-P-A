//
// Created by vaish on 1/10/2021.
//

#ifndef AUTOTESTER_SPA_TEAM07_CODE07_SRC_SPA_SRC_PQL_QUERYPARSER_WITHCLAUSE_H_
#define AUTOTESTER_SPA_TEAM07_CODE07_SRC_SPA_SRC_PQL_QUERYPARSER_WITHCLAUSE_H_

#include "Clause.h"
#include "WithArgument.h"

class WithClause : public Clause {
  WithArgument *left;
  WithArgument *right;
 public:
  WithClause(WithArgument *leftArg, WithArgument *rightArg);
  WithArgument *getLeft();
  WithArgument *getRight();
  std::string toString() override;
};

#endif //AUTOTESTER_SPA_TEAM07_CODE07_SRC_SPA_SRC_PQL_QUERYPARSER_WITHCLAUSE_H_
