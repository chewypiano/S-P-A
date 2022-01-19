//
// Created by Christopher Mervyn on 4/10/21.
//

#ifndef AUTOTESTER_NEXTCOMPONENT_H
#define AUTOTESTER_NEXTCOMPONENT_H

#include <string>
#include <unordered_set>
#include <unordered_map>
#include <PKB/PairHasher.h>

class NextComponent {
 public:
  NextComponent();

  bool addNext(int stmtNum1, int stmtNum2);

  std::unordered_set<int> getNextStatementsOf(int stmtNum);
  std::unordered_set<int> getPrevStatementsOf(int stmtNum);
  std::unordered_set<std::pair<int, int>, std::hash_pair> getAllStatementsPairs();


 private:
  std::unordered_set<std::pair<int, int>, std::hash_pair> stmtPairs;
  std::unordered_map<int, std::unordered_set<int>> nextList;
  std::unordered_map<int, std::unordered_set<int>> prevList;
  std::unordered_map<int, std::unordered_set<int>> combinedNextList;

};

#endif //AUTOTESTER_NEXTCOMPONENT_H
