//
// Created by Christopher Mervyn on 7/9/21.
//

#include "UseComponent.h"


UseComponent::UseComponent() {}

/*
 * Adders for the source processor to input into the PKB
 */
bool UseComponent::addUseStmt(int stmtNum, std::string var) {
  //Checks if the pair<stmtNum,var> already exists in the list, returns false if exists.
  //Else adds the pair<stmtNum, var> to the list.
  if (!stmtVarPairs.emplace(std::pair<int, std::string>(stmtNum, var)).second) {
    return false;
  }
  //Hashmap with stmtNum as key and value as a list of the var used by the statement
  // Adds the used variable to the list
  if (!stmtListofVar.emplace(stmtNum, std::unordered_set<std::string>{var}).second) {
    stmtListofVar.at(stmtNum).emplace(var);
  }
  //Hashmap with var as key and value as a list of the statements using the var
  // Adds the stmtNum to the list
  if (!varListofStmt.emplace(var, std::unordered_set<int>{stmtNum}).second) {
    varListofStmt.at(var).emplace(stmtNum);
  }
  return true;
}

bool UseComponent::addUseProc(std::string procName, std::string var) {
  //Checks if the pair<procName,var> already exists in the list, returns false if exists.
  //Else adds the pair<procName, var> to the list.
  if (!procVarPairs.emplace(std::pair<std::string, std::string>(procName, var)).second) {
    return false;
  }
  //Hashmap with procName as key and value as a list of the var used by the procedure
  // Adds the modified variable to the list
  if (!procListofVar.emplace(procName, std::unordered_set<std::string>{var}).second) {
    procListofVar.at(procName).emplace(var);
  }
  //Hashmap with var as key and value as a list of the procedures using the var
  // Adds the procName to the list
  if (!varListofProc.emplace(var, std::unordered_set<std::string>{procName}).second) {
    varListofProc.at(var).emplace(procName);
  }
  return true;
}

/*
 * Getters for the PQL queries to fetch the required relations
 */
bool UseComponent::isStmtVarPair(std::pair<int, std::string> tuple) {
  return stmtVarPairs.find(tuple) != stmtVarPairs.end();
}

bool UseComponent::isProcVarPair(std::pair<std::string, std::string> tuple) {
  return procVarPairs.find(tuple) != procVarPairs.end();
}

std::unordered_set<std::pair<int, std::string>, std::hash_pair> UseComponent::getStmtVarPair() {
  return stmtVarPairs;
}

std::unordered_set<std::pair<std::string, std::string>, std::hash_pair> UseComponent::getProcVarPair() {
  return procVarPairs;
}

std::unordered_set<std::string> UseComponent::getVarUsedByStmt(int stmtNum) {
  if (stmtListofVar.find(stmtNum) != stmtListofVar.end()) {
    return stmtListofVar.at(stmtNum);
  }
  return {};
}

std::unordered_set<std::string> UseComponent::getVarUsedByProc(std::string procName) {
  if (procListofVar.find(procName) != procListofVar.end()) {
    return procListofVar.at(procName);
  }
  return {};
}

std::unordered_set<int> UseComponent::getStmtUsingVar(std::string var) {
  if (varListofStmt.find(var) != varListofStmt.end()) {
    return varListofStmt.at(var);
  }
  return {};
}

std::unordered_set<std::string> UseComponent::getProcUsingVar(std::string var) {
  if (varListofProc.find(var) != varListofProc.end()) {
    return varListofProc.at(var);
  }
  return {};
}

std::unordered_set<int> UseComponent::getAllUseStmt() {
  std::unordered_set<int> allUseStmt;
  for (auto kv: stmtListofVar) {
    allUseStmt.emplace(kv.first);
  }
  return allUseStmt;
}


