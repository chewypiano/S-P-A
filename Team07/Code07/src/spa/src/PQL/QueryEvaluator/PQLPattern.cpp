//
// Created by Junhua on 14/9/2021.
//

#include "PQLPattern.h"

PQLPattern::PQLPattern(std::string patternValue, Location loc) {
  this->value = patternValue;

  switch (patternValue[0]) {
    case '_':
      if (patternValue.size() == 1) {
        this->type = PatternType::Wildcard;
      } else {
        this->type = PatternType::DoubleWildcard;
      }
      break;
    case '"':
      this->type = PatternType::String;
      break;
    default:
      this->type = PatternType::Synonym;
  }
}

PatternType PQLPattern::getType() {
  return this->type;
}

std::string PQLPattern::getValue() {
  return this->value;
}

std::string PQLPattern::toString() {
  return getValue();
}

std::string PQLPattern::getInner() {
  switch (this->type) {
    case PatternType::DoubleWildcard:
      return this->getValue().substr(2, this->getValue().size() - 4);
    case PatternType::String:
      return this->getValue().substr(1, this->getValue().size() - 2);
    default:
      return this->getValue();
  }
}