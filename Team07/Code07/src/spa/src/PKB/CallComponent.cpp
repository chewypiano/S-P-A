//
// Created by Christopher Mervyn on 28/9/21.
//

#include "CallComponent.h"

CallComponent::CallComponent() {}

bool CallComponent::addCall(int stmtNum, std::string callerProc, std::string calledProc) {

  if (!callList.emplace(stmtNum, std::pair<std::string, std::string>(callerProc, calledProc)).second) {
    return false;
  }

  if (!callerTable.emplace(callerProc, std::unordered_set<std::string>{calledProc}).second) {
    callerTable.at(callerProc).emplace(calledProc);
  }

  if (!calledTable.emplace(calledProc, std::unordered_set<std::string>{callerProc}).second) {
    calledTable.at(calledProc).emplace(callerProc);
  }

  callStmtTable.emplace(stmtNum, calledProc);

  return true;
}

bool CallComponent::addCallStar(std::string callerProc, std::string calledProc) {

  if (!callStarList.emplace(std::pair<std::string, std::string>(callerProc, calledProc)).second) {
    return false;
  }

  if (!callerStarTable.emplace(callerProc, std::unordered_set<std::string>{calledProc}).second) {
    callerStarTable.at(callerProc).emplace(calledProc);
  }

  if (!calledStarTable.emplace(calledProc, std::unordered_set<std::string>{callerProc}).second) {
    calledStarTable.at(calledProc).emplace(callerProc);
  }
  return true;
}

std::string CallComponent::getCallerProc(int stmtNum) {
  if (stmtNum > 0 && callList.find(stmtNum) != callList.end()) {
    return callList.at(stmtNum).first;
  }
  return {};
}

std::string CallComponent::getCalledProc(int stmtNum) {
  if (stmtNum > 0 && callList.find(stmtNum) != callList.end()) {
    return callList.at(stmtNum).second;
  }
  return {};
}

std::unordered_set<std::string> CallComponent::getProcsCalledFromProc(std::string procName) {
  if (calledTable.find(procName) != calledTable.end()) {
    return calledTable.at(procName);
  }
  return {};
}

std::unordered_set<std::string> CallComponent::getProcsCalledStarFromProc(std::string procName) {
  if (calledStarTable.find(procName) != calledStarTable.end()) {
    return calledStarTable.at(procName);
  }
  return {};
}

std::unordered_set<std::string> CallComponent::getProcsCalledByProc(std::string procName) {
  if (callerTable.find(procName) != callerTable.end()) {
    return callerTable.at(procName);
  }
  return {};
}

std::unordered_set<std::string> CallComponent::getProcsCalledStarByProc(std::string procName) {
  if (callerStarTable.find(procName) != callerStarTable.end()) {
    return callerStarTable.at(procName);
  }
  return {};
}

std::unordered_map<int, std::string> CallComponent::getAllCallStmts() {
  return callStmtTable;
}

bool CallComponent::isCallRelation(std::string callerProc, std::string calledProc) {
  if (callerTable.find(callerProc) == callerTable.end()) {
    return false;
  }
  if (callerTable.at(callerProc).find(calledProc) != callerTable.at(callerProc).end()) {
    return true;
  }
  return false;
}

bool CallComponent::isCallStarRelation(std::string callerProc, std::string calledProc) {
  if (callerStarTable.find(callerProc) == callerStarTable.end()) {
    return false;
  }
  if (callerStarTable.at(callerProc).find(calledProc) != callerStarTable.at(callerProc).end()) {
    return true;
  }
  return false;
}





