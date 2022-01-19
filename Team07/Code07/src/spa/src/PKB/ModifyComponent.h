//
// Created by Christopher Mervyn on 3/9/21.
//

#ifndef AUTOTESTER_MODIFYCOMPONENT_H
#define AUTOTESTER_MODIFYCOMPONENT_H

#include <string>
#include <unordered_set>
#include <unordered_map>
#include <PKB/PairHasher.h>

/*
 * Sub-Component of PKB to store Modify Relations. Store Modifies(stmt, var) and Modifies(proc, var) relations.
 */
class ModifyComponent {

 public:
  ModifyComponent();

  //Adds the Modifies(stmt, var) relation into the table
  bool addModifyStmt(int stmtNum, std::string var);

  //Adds the Modifies(proc, var) relation into the table
  bool addModifyProc(std::string procName, std::string var);

  //Checks if the Modifies(stmt, var) relation exists in the table
  bool isStmtVarPair(std::pair<int, std::string> tuple);

  //Checks if the Modifies(proc, var) relation exists in the table
  bool isProcVarPair(std::pair<std::string, std::string> tuple);

  //Returns all the Modifies(stmt, var) relations that exists in the table as pairs
  std::unordered_set<std::pair<int, std::string>, std::hash_pair> getStmtVarPair();

  //Returns all the Modifies(proc, var) relations that exists in the table as pairs
  std::unordered_set<std::pair<std::string, std::string>, std::hash_pair> getProcVarPair();

  //Returns all the variables that are modified by the specified statement
  std::unordered_set<std::string> getVarModifiedByStmt(int stmtNum);

  //Returns all the variables that are modified by the specified procedure
  std::unordered_set<std::string> getVarModifiedByProc(std::string procName);

  //Returns all the statements that modifies the specified variable
  std::unordered_set<int> getStmtModifyingVar(std::string var);

  //Returns all the procedures that modifies the specified variable
  std::unordered_set<std::string> getProcModifyingVar(std::string var);

  //Returns all statements that modifies at least one variable
  std::unordered_set<int> getAllModifies();

  //Returns all the procedures that modifies at least one variable
  std::unordered_set<std::string> getAllModifiesProc();

  //Returns the stmtListofVar table (used for affects relation)
  std::unordered_map<int, std::unordered_set<std::string>> getStmtListofVar();

 private:
  std::unordered_set<std::pair<int, std::string>, std::hash_pair> stmtVarPairs;
  std::unordered_set<std::pair<std::string, std::string>, std::hash_pair> procVarPairs;
  std::unordered_map<int, std::unordered_set<std::string>> stmtListofVar;
  std::unordered_map<std::string, std::unordered_set<int>> varListofStmt;
  std::unordered_map<std::string, std::unordered_set<std::string>> procListofVar;
  std::unordered_map<std::string, std::unordered_set<std::string>> varListofProc;

};

#endif //AUTOTESTER_MODIFYCOMPONENT_H


