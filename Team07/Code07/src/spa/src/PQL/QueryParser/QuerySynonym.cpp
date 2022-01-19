//
// Created by vaish on 3/9/2021.
//

#include <iostream>
#include <sstream>
#include <utility>
#include "QuerySynonym.h"

QuerySynonym::QuerySynonym(DesignEntityType type, std::string name) {
  this->type = type;
  this->name = std::move(name);
}

DesignEntityType QuerySynonym::getType() {
  return this->type;
}

std::string QuerySynonym::getName() {
  return this->name;
}

std::string QuerySynonym::toString() {
  std::stringstream stringStream;
  stringStream << "QuerySynonym(" << DesignEntityToName::designEntityToName(this->type) << ", " << this->name << ")";

  return stringStream.str();
}

void QuerySynonym::print() {
  std::cout << this->toString() << std::endl;
}