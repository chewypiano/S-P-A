//
// Created by vaish on 1/10/2021.
//

#include "WithClause.h"
#include <sstream>

WithClause::WithClause(WithArgument* leftArg, WithArgument* rightArg) : Clause(ClauseType::With) {
  this->left = leftArg;
  this->right = rightArg;
}

WithArgument *WithClause::getLeft() {
  return this->left;
}

WithArgument *WithClause::getRight() {
  return this->right;
}

std::string WithClause::toString() {
  std::stringstream stringStream;
  stringStream << "With(" << this->left->toString() << ", " << this->right->toString() << ")";
  return stringStream.str();
}
