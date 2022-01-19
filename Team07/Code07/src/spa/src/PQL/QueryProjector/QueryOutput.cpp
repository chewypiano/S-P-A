//
// Created by vaish on 8/10/2021.
//

#include "QueryOutput.h"
#include <sstream>

QueryOutput::QueryOutput(QueryOutputType type) {
  this->type = type;
  this->synonym = nullptr;
}

QueryOutput::QueryOutput(QueryOutputType type, QuerySynonym *synonym) {
  this->type = type;
  this->synonym = synonym;
}

QueryOutputType QueryOutput::getType() {
  return this->type;
}

QuerySynonym *QueryOutput::getSynonym() {
  if (this->type == QueryOutputType::Boolean) {
    throw std::runtime_error("Invalid Output type for getSynonym.");
  }
  return this->synonym;
}

std::string QueryOutput::toString() {
  std::stringstream stringStream;

  switch (this->type) {
    case QueryOutputType::Synonym:
      stringStream << this->synonym->getName();
      break;
    case QueryOutputType::StmtNoAttribute:
      stringStream << this->synonym->getName() << ".stmt#";
      break;
    case QueryOutputType::ProcNameAttribute:
      stringStream << this->synonym->getName() << ".procName";
      break;
    case QueryOutputType::VarNameAttribute:
      stringStream << this->synonym->getName() << ".varName";
      break;
    case QueryOutputType::ValueAttribute:
      stringStream << this->synonym->getName() << ".value";
      break;
    case QueryOutputType::Boolean:
      stringStream << "BOOLEAN T/F";
      break;
    default:
      throw std::runtime_error("Invalid QueryOutputType im QueryOutput");
  }

  return stringStream.str();
}
