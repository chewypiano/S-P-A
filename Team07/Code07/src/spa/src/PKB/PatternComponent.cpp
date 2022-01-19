//
// Created by Christopher Mervyn on 4/10/21.
//

#include "PatternComponent.h"

PatternComponent::PatternComponent() {}

bool PatternComponent::addAssignPattern(int stmtNum, std::string LHS, std::string RHS) {
  if (assignPatternList.find(stmtNum) == assignPatternList.end()) {
    assignPatternList.emplace(stmtNum, std::pair<std::string, std::string>(LHS, RHS));
    return true;
  }
  return false;
}

void PatternComponent::addSubString(int stmtNum, std::string subString) {
  if (!subStringList.emplace(stmtNum, std::unordered_set<std::string>{subString}).second) {
    subStringList.at(stmtNum).emplace(subString);
  }
}

std::unordered_set<std::string> PatternComponent::subStringsAt(int stmtNum) {
  if (subStringList.find(stmtNum) != subStringList.end()) {
    return subStringList.at(stmtNum);
  }
  return {};
}

std::string PatternComponent::getLHSAssign(int stmtNum) {
  if (assignPatternList.find(stmtNum) != assignPatternList.end()) {
    return assignPatternList.at(stmtNum).first;
  }
  return {};
}

std::string PatternComponent::getRHSAssign(int stmtNum) {
  if (assignPatternList.find(stmtNum) != assignPatternList.end()) {
    return assignPatternList.at(stmtNum).second;
  }
  return {};
}

std::unordered_map<int, std::pair<std::string, std::string>> PatternComponent::getAllPatternsAssignStmts() {
  return assignPatternList;
}

std::unordered_map<int, std::unordered_set<std::string>> PatternComponent::getAllIfWhilePatterns() {
  return subStringList;
}


