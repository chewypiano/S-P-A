//
// Created by Christopher Mervyn on 3/9/21.
//



#include "ModifyComponent.h"

ModifyComponent::ModifyComponent() {
}

/*
 * Adders for the source processor to input into the PKB
 */
bool ModifyComponent::addModifyStmt(int stmtNum, std::string var) {
  //Checks if the pair<stmtNum,var> already exists in the list, returns false if exists.
  //Else adds the pair<stmtNum, var> to the list.
  if (!stmtVarPairs.emplace(std::pair<int, std::string>(stmtNum, var)).second) {
    return false;
  }
  //Hashmap with stmtNum as key and value as a list of the var modified by the statement
  // Adds the modified variable to the list
  if (!stmtListofVar.emplace(stmtNum, std::unordered_set<std::string>{var}).second) {
    stmtListofVar.at(stmtNum).emplace(var);
  }
  //Hashmap with var as key and value as a list of the statements modifying the var
  // Adds the stmtNum to the list
  if (!varListofStmt.emplace(var, std::unordered_set<int>{stmtNum}).second) {
    varListofStmt.at(var).emplace(stmtNum);
  }
  return true;
}

bool ModifyComponent::addModifyProc(std::string procName, std::string var) {
  //Checks if the pair<procName,var> already exists in the list, returns false if exists.
  //Else adds the pair<procName, var> to the list.
  if (!procVarPairs.emplace(std::pair<std::string, std::string>(procName, var)).second) {
    return false;
  }
  //Hashmap with procName as key and value as a list of the var modified by the procedure
  // Adds the modified variable to the list
  if (!procListofVar.emplace(procName, std::unordered_set<std::string>{var}).second) {
    procListofVar.at(procName).emplace(var);
  }
  //Hashmap with var as key and value as a list of the procedures modifying the var
  // Adds the procName to the list
  if (!varListofProc.emplace(var, std::unordered_set<std::string>{procName}).second) {
    varListofProc.at(var).emplace(procName);
  }
  return true;
}

/*
 * Getters for the PQL queries to fetch the required relations
 */
bool ModifyComponent::isStmtVarPair(std::pair<int, std::string> tuple) {
  return stmtVarPairs.find(tuple) != stmtVarPairs.end();
}

bool ModifyComponent::isProcVarPair(std::pair<std::string, std::string> tuple) {
  return procVarPairs.find(tuple) != procVarPairs.end();
}

std::unordered_set<std::pair<int, std::string>, std::hash_pair> ModifyComponent::getStmtVarPair() {
  return stmtVarPairs;
}

std::unordered_set<std::pair<std::string, std::string>, std::hash_pair> ModifyComponent::getProcVarPair() {
  return procVarPairs;
}

std::unordered_set<std::string> ModifyComponent::getVarModifiedByStmt(int stmtNum) {
  if (stmtListofVar.find(stmtNum) != stmtListofVar.end()) {
    return stmtListofVar.at(stmtNum);
  }
  return {};
}

std::unordered_set<std::string> ModifyComponent::getVarModifiedByProc(std::string procName) {
  if (procListofVar.find(procName) != procListofVar.end()) {
    return procListofVar.at(procName);
  }
  return {};
}

std::unordered_set<int> ModifyComponent::getStmtModifyingVar(std::string var) {
  if (varListofStmt.find(var) != varListofStmt.end()) {
    return varListofStmt.at(var);
  }
  return {};
}

std::unordered_set<std::string> ModifyComponent::getProcModifyingVar(std::string var) {
  if (varListofProc.find(var) != varListofProc.end()) {
    return varListofProc.at(var);
  }
  return {};
}

std::unordered_set<int> ModifyComponent::getAllModifies() {
  std::unordered_set<int> allModifiesStmt;
  for (auto kv: stmtListofVar) {
    allModifiesStmt.emplace(kv.first);
  }
  return allModifiesStmt;
}

std::unordered_set<std::string> ModifyComponent::getAllModifiesProc() {
  std::unordered_set<std::string> allModifiesProc;
  for(auto kv: procListofVar) {
    allModifiesProc.emplace(kv.first);
  }
  return allModifiesProc;
}

std::unordered_map<int, std::unordered_set<std::string>> ModifyComponent::getStmtListofVar() {
  return stmtListofVar;
}



