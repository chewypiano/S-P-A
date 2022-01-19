//
// Created by vaish on 3/9/2021.
//

#include <sstream>
#include "PatternClause.h"

PatternClause::PatternClause(PatternSynonymType synonymType,
                             std::string left,
                             std::string right,
                             std::string synonymName)
    : Clause(ClauseType::Pattern) {
  this->synonymType = synonymType;
  this->synonymName = std::move(synonymName);
  this->left = new PQLPattern(std::move(left), Location::Left);
  this->right = new PQLPattern(std::move(right), Location::Right);
  this->centre = nullptr;
}

PatternClause::PatternClause(PatternSynonymType synonymType,
                             std::string left,
                             std::string centre,
                             std::string right,
                             std::string synonymName)
    : Clause(ClauseType::Pattern) {
  this->synonymType = synonymType;
  this->synonymName = std::move(synonymName);
  this->left = new PQLPattern(std::move(left), Location::Left);
  this->right = new PQLPattern(std::move(right), Location::Right);
  this->centre = new PQLPattern(std::move(centre), Location::Centre);
}

std::string PatternClause::getSynonymName() {
  return this->synonymName;
}

PQLPattern *PatternClause::getLeft() {
  return this->left;
}

PQLPattern *PatternClause::getRight() {
  return this->right;
}

PQLPattern *PatternClause::getCentre() {
  if (this->synonymType != PatternSynonymType::If) {
    throw std::runtime_error("Invalid Pattern synonym type to be getting centre value");
  }

  return this->centre;
}

std::string PatternClause::toString() {
  std::stringstream stringStream;
  stringStream << "Pattern " << this->synonymName << "(" << this->left->toString() << ", ";
  if (this->synonymType == PatternSynonymType::If) {
    stringStream << this->centre->toString() << ", ";
  }
  stringStream << this->right->toString() << ")";
  return stringStream.str();
}
