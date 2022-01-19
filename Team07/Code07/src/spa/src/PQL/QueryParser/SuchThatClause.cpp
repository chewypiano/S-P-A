//
// Created by vaish on 3/9/2021.
//

#include <iostream>
#include <sstream>
#include <unordered_map>
#include "SuchThatClause.h"

SuchThatClause::SuchThatClause(ClauseType type, const std::string &left, const std::string &right) : Clause(type) {
  this->left = new Argument(left);
  this->right = new Argument(right);
}

Argument *SuchThatClause::getLeft() {
  return this->left;
}

Argument *SuchThatClause::getRight() {
  return this->right;
}

void SuchThatClause::setLeft(const std::string &val) {
  this->left = new Argument(val);
}

void SuchThatClause::setRight(const std::string &val) {
  this->right = new Argument(val);
}

std::string SuchThatClause::toString() {
  std::string relType;
  std::stringstream stringStream;

  static std::unordered_map<ClauseType, std::string> typeToStringMap = {
      {ClauseType::ModifiesS, "ModifiesS"},
      {ClauseType::ModifiesP, "ModifiesP"},
      {ClauseType::UsesP, "UsesP"},
      {ClauseType::UsesS, "UsesS"},
      {ClauseType::Parent, "Parent"},
      {ClauseType::ParentT, "Parent*"},
      {ClauseType::Follows, "Follows"},
      {ClauseType::FollowsT, "Follows*"},
      {ClauseType::Calls, "Calls"},
      {ClauseType::CallsT, "Calls*"},
      {ClauseType::Next, "Next"},
      {ClauseType::NextT, "Next*"},
      {ClauseType::Affects, "Affects"},
      {ClauseType::AffectsT, "Affects*"},
      {ClauseType::NextBip, "NextBip"},
      {ClauseType::NextBipT, "NextBip*"},
      {ClauseType::AffectsBip, "AffectsBip"},
  };

  if (typeToStringMap.count(this->type) == 0) {
    throw std::runtime_error("Unexpected type in SuchThatClause.");
  }
  relType = typeToStringMap[this->type];
  stringStream << "SuchThat " << relType << "(" << this->left->toString() << ", " << this->right->toString() << ")";
  return stringStream.str();
}

void SuchThatClause::print() {
  std::cout << this->toString() << std::endl;
}
