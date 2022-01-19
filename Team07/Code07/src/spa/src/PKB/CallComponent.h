//
// Created by Christopher Mervyn on 28/9/21.
//

#ifndef AUTOTESTER_CALLCOMPONENT_H
#define AUTOTESTER_CALLCOMPONENT_H

#include <string>
#include <unordered_set>
#include <unordered_map>
#include <PKB/PairHasher.h>

class CallComponent {
 public:
  CallComponent();

  bool addCall(int stmtNum, std::string callerProc, std::string calledProc);
  bool addCallStar(std::string callerProc, std::string calledProc);

  bool isCallRelation(std::string callerProc, std::string calledProc);
  bool isCallStarRelation(std::string callerProc, std::string calledProc);

  std::string getCallerProc(int stmtNum);
  std::string getCalledProc(int stmtNum);
  std::unordered_set<std::string> getProcsCalledFromProc(std::string procName);
  std::unordered_set<std::string> getProcsCalledStarFromProc(std::string procName);
  std::unordered_set<std::string> getProcsCalledByProc(std::string procName);
  std::unordered_set<std::string> getProcsCalledStarByProc(std::string procName);
  std::unordered_map<int, std::string> getAllCallStmts();


 private:
  std::unordered_map<int, std::pair<std::string, std::string>> callList;
  std::unordered_map<int, std::string> callStmtTable;
  std::unordered_map<std::string, std::unordered_set<std::string>> callerTable;
  std::unordered_map<std::string, std::unordered_set<std::string>> calledTable;

  std::unordered_set<std::pair<std::string, std::string>, std::hash_pair> callStarList;
  std::unordered_map<std::string, std::unordered_set<std::string>> callerStarTable;
  std::unordered_map<std::string, std::unordered_set<std::string>> calledStarTable;

};

#endif //AUTOTESTER_CALLCOMPONENT_H
