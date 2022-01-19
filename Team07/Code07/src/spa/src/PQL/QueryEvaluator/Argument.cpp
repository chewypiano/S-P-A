//
// Created by Jazer on 15/9/2021.
//

#include "Argument.h"
#include <algorithm>

bool isNumber(const std::string &str) {
  return std::all_of(str.begin(), str.end(), [](char c) {
    return std::isdigit(c);
  });
}

Argument::Argument(std::string argumentValue) {
  this->value = argumentValue;

  if (argumentValue == "_") {
    this->type = ArgumentType::Wildcard;
  } else if (isNumber(argumentValue)) {
    this->type = ArgumentType::Statement;
  } else if (argumentValue[0] == '"') {
    this->type = ArgumentType::String;
  } else {
    this->type = ArgumentType::Synonym;
  }

}

ArgumentType Argument::getType() {
  return this->type;
}

std::string Argument::getValue() {
  switch (this->type) {
    case ArgumentType::String:
      return this->value.substr(1, this->value.size() - 2);
    default:
      return this->value;
  }
}

std::string Argument::toString() {
  if (this->type == ArgumentType::String) {
    return "\"" + this->getValue() + "\"";
  }
  return this->getValue();
}