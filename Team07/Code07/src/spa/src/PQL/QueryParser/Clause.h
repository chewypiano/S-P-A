//
// Created by Junhua on 29/9/2021.
//

#ifndef AUTOTESTER_SRC_SPA_SRC_PQL_QUERYPARSER_CLAUSE_H_
#define AUTOTESTER_SRC_SPA_SRC_PQL_QUERYPARSER_CLAUSE_H_

#include <string>
#include "ClauseType.h"

class Clause {
 protected:
  ClauseType type;
 public:
  explicit Clause(ClauseType type);
  ClauseType getType();
  virtual std::string toString() = 0;
};

#endif //AUTOTESTER_SRC_SPA_SRC_PQL_QUERYPARSER_CLAUSE_H_
