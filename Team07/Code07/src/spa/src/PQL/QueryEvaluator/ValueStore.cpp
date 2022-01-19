//
// Created by Jazer on 6/11/2021.
//

#include "ValueStore.h"

ValueStore::ValueStore(PKB *pkb) {
  this->storage = {
      {DesignEntityType::Stmt, intSetToStringSet(pkb->getAllStmt())},
      {DesignEntityType::Read, intSetToStringSet(pkb->getReadStmts())},
      {DesignEntityType::Print, intSetToStringSet(pkb->getPrintStmts())},
      {DesignEntityType::While, intSetToStringSet(pkb->getAllWhile())},
      {DesignEntityType::If, intSetToStringSet(pkb->getAllIf())},
      {DesignEntityType::Assign, intSetToStringSet(pkb->getAssignStmts())},
      {DesignEntityType::Variable, pkb->getVarList()},
      {DesignEntityType::Constant, pkb->getConstList()},
      {DesignEntityType::Procedure, pkb->getProcList()},
      {DesignEntityType::Call, intSetToStringSet(pkb->getCallStmts())},
      {DesignEntityType::ProgLine, intSetToStringSet(pkb->getAllStmt())},
  };
  this->results = new ClauseResult();

}
std::unordered_set<std::string> ValueStore::intSetToStringSet(const std::unordered_set<int> &set) {
  std::unordered_set<std::string> result;
  for (auto i: set) {
    result.insert(std::to_string(i));
  }
  return result;
}
std::unordered_set<std::string> ValueStore::valuesInType(DesignEntityType type) {
  if (storage.count(type) == 0) {
    return {};
  }
  return storage.at(type);
}

void ValueStore::clearResults() {
  this->results = new ClauseResult();
}

void ValueStore::setStoredResults(ClauseResult *collected) {
  this->results = collected;
}

bool ValueStore::resultHasSynonym(QuerySynonym *synonym) {
  return this->results->getSynonyms().count(*synonym) > 0;
}
RowList ValueStore::getResultRows() {
  return this->results->getRows();
}

