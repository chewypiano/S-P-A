//
// Created by Christopher Mervyn on 4/10/21.
//

#ifndef AUTOTESTER_PATTERNCOMPONENT_H
#define AUTOTESTER_PATTERNCOMPONENT_H

#include <string>
#include <unordered_set>
#include <unordered_map>
#include <PKB/PairHasher.h>

class PatternComponent {
 public:
  PatternComponent();

  bool addAssignPattern(int stmtNum, std::string LHS, std::string RHS);
  void addSubString(int stmtNum, std::string subString);
  std::unordered_set<std::string> subStringsAt(int stmtNum);
  std::string getLHSAssign(int stmtNum);
  std::string getRHSAssign(int stmtNum);

  std::unordered_map<int, std::pair<std::string, std::string>> getAllPatternsAssignStmts();
  std::unordered_map<int, std::unordered_set<std::string>> getAllIfWhilePatterns();

 private:
  std::unordered_map<int, std::pair<std::string, std::string>> assignPatternList;
  std::unordered_map<int, std::unordered_set<std::string>> subStringList;
};

#endif //AUTOTESTER_PATTERNCOMPONENT_H
