//
// Created by vaish on 4/9/2021.
//

#include <iostream>
#include <sstream>
#include "Query.h"

Query::Query() {
  this->isInvalid = false;
  this->synonymMap = std::unordered_map<std::string, QuerySynonym>();
}

void Query::insertSynonym(QuerySynonym synonym) {
  this->synonymList.push_back(synonym);
  this->synonymMap.insert({synonym.getName(), synonym});
}

void Query::insertClause(Clause *clause) {
  this->clauses.push_back(clause);
}

void Query::insertIrrelevantClause(Clause *clause) {
  this->irrelevantClauses.push_back(clause);
}

QuerySynonym *Query::findSynonymByName(std::string name) {
  try {
    return &this->synonymMap.at(name);
  } catch (std::out_of_range &error) {
    // no such synonym - throw exception
    throw std::runtime_error("Cannot find QuerySynonym with input name of " + name);
  }
}

bool Query::checkIfSynonymExists(const std::string &name) {
  return this->synonymMap.count(name) > 0;
}

void Query::insertOutput(QueryOutput *queryOutput) {
  this->outputList.emplace_back(queryOutput);
}

void Query::insertOutputSynonym(QuerySynonym *synonym) {
  auto *queryOutput = new QueryOutput(QueryOutputType::Synonym, synonym);
  this->outputList.emplace_back(queryOutput);
}

std::vector<QueryOutput *> Query::getOutputList() {
  return this->outputList;
}

std::vector<QuerySynonym> Query::getSynonymList() {
  return this->synonymList;
}

ClauseList Query::getClauses() {
  return this->clauses;
}

ClauseList Query::getIrrelevantClauses() {
  return this->irrelevantClauses;
}

void Query::setClauses(ClauseList newClauses) {
  this->clauses = newClauses;
}

void Query::setInvalidQuery() {
  this->isInvalid = true;
}

bool Query::isInvalidQuery() {
  return this->isInvalid;
}

std::string Query::toString() {
  std::stringstream stringStream;

  if (this->isInvalid) {
    stringStream << "Query(\n  INVALID QUERY\n)";
    return stringStream.str();
  }

  if (this->outputList.empty()) {
    // Should not typically reach here except for possibly tests on empty Query
    stringStream << "Query(\n";
  } else {
    stringStream << "Query(\n  Output: ";
    for (int i = 0; i < this->outputList.size(); i++) {
      if (i == this->outputList.size() - 1) {
        stringStream << this->outputList.at(i)->toString() << "\n";
      } else {
        stringStream << this->outputList.at(i)->toString() << ", ";
      }
    }
  }

  for (auto &synonym: this->synonymList) {
    stringStream << "  " << synonym.toString() << "\n";
  }

  for (auto &clause: this->clauses) {
    stringStream << "  " << clause->toString() << "\n";
  }

  if (!this->irrelevantClauses.empty()) {
    stringStream << "  -----\n";
    for (auto &clause: this->irrelevantClauses) {
      stringStream << "  " << clause->toString() << "\n";
    }
  }

  stringStream << ")";

  return stringStream.str();
}

void Query::print() {
  std::cout << this->toString() << std::endl;
}
