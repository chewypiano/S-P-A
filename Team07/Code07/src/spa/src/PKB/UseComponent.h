//
// Created by Christopher Mervyn on 7/9/21.
//

#ifndef AUTOTESTER_USECOMPONENT_H
#define AUTOTESTER_USECOMPONENT_H

#include <string>
#include <unordered_set>
#include <unordered_map>
#include <PKB/PairHasher.h>

/*
 * Sub-Component of PKB to store Use Relations. Store Use(stmt, var) and Use(proc, var) relations.
 */
class UseComponent {
 public:
  UseComponent();

  //Adds the Use(stmt, var) relation into the table
  bool addUseStmt(int stmtNum, std::string var);

  //Adds the Use(proc, var) relation into the table
  bool addUseProc(std::string procName, std::string var);

  //Checks if the Use(stmt, var) relation exists in the table
  bool isStmtVarPair(std::pair<int, std::string> tuple);

  //Checks if the Use(proc, var) relation exists in the table
  bool isProcVarPair(std::pair<std::string, std::string> tuple);

  //Returns all the Use(stmt, var) relations that exists in the table as pairs
  std::unordered_set<std::pair<int, std::string>, std::hash_pair> getStmtVarPair();

  //Returns all the Use(proc, var) relations that exists in the table as pairs
  std::unordered_set<std::pair<std::string, std::string>, std::hash_pair> getProcVarPair();

  //Returns all the variables that are used by the specified statement
  std::unordered_set<std::string> getVarUsedByStmt(int stmtNum);

  //Returns all the variables that are used by the specified procedure
  std::unordered_set<std::string> getVarUsedByProc(std::string procName);

  //Returns all the statements that modifies the specified variable
  std::unordered_set<int> getStmtUsingVar(std::string var);

  //Returns all the procedures that modifies the specified variable
  std::unordered_set<std::string> getProcUsingVar(std::string var);

  //Returns all statements using at least one var
  std::unordered_set<int> getAllUseStmt();

 private:
  std::unordered_set<std::pair<int, std::string>, std::hash_pair> stmtVarPairs;
  std::unordered_set<std::pair<std::string, std::string>, std::hash_pair> procVarPairs;
  std::unordered_map<int, std::unordered_set<std::string>> stmtListofVar;
  std::unordered_map<std::string, std::unordered_set<int>> varListofStmt;
  std::unordered_map<std::string, std::unordered_set<std::string>> procListofVar;
  std::unordered_map<std::string, std::unordered_set<std::string>> varListofProc;

};

#endif //AUTOTESTER_USECOMPONENT_H
