//
// Created by vaish on 3/9/2021.
//

#ifndef AUTOTESTER_SUCHTHATCLAUSE_H
#define AUTOTESTER_SUCHTHATCLAUSE_H

#include <string>
#include "Clause.h"
#include "ClauseType.h"
#include "PQL/QueryEvaluator/Argument.h"

class SuchThatClause : public Clause {
  Argument *left;
  Argument *right;
 public:
  explicit SuchThatClause(ClauseType type, const std::string &left, const std::string &right);
  Argument *getLeft();
  Argument *getRight();
  void setLeft(const std::string &val);
  void setRight(const std::string &val);
  std::string toString() override;
  void print();
};

#endif //AUTOTESTER_SUCHTHATCLAUSE_H
