//
// Created by vaish on 1/10/2021.
//

#include "WithArgument.h"
#include <sstream>

WithArgument::WithArgument(WithType type, std::string value, AttributeType attribute) {
  this->type = type;
  this->value = value;
  this->attribute = attribute;
}

WithType WithArgument::getType() {
  return this->type;
}

std::string WithArgument::getValue() {
  return this->value;
}

AttributeType WithArgument::getAttribute() {
  return this->attribute;
}
std::string WithArgument::toString() {
  std::stringstream stringStream;
  stringStream << this->value;
  if (this->type == WithType::Attribute) {
    switch (this->attribute) {
      case AttributeType::ProcName:
        stringStream << ".procName";
        break;
      case AttributeType::VarName:
        stringStream << ".varName";
        break;
      case AttributeType::Value:
        stringStream << ".value";
        break;
      case AttributeType::StmtNo:
        stringStream << ".stmt#";
        break;
      default:
        throw std::runtime_error("Invalid Attribute Type in WithArgument");
    }
  }
  return stringStream.str();
}
std::string WithArgument::getInner() {
  switch (this->type) {
    case WithType::String:
      return this->getValue().substr(1, this->getValue().size() - 2);
    default:
      return this->getValue();
  }
}
