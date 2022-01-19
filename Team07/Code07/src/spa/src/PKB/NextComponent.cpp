//
// Created by Christopher Mervyn on 4/10/21.
//

#include "NextComponent.h"

NextComponent::NextComponent() {}

bool NextComponent::addNext(int stmtNum1, int stmtNum2) {
  //checks if the relation next(stmtNum1,stmtNum2) exists already
  if (!stmtPairs.emplace(std::pair<int, int>(stmtNum1, stmtNum2)).second) {
    return false;
  }

  if (!nextList.emplace(stmtNum1, std::unordered_set<int>{stmtNum2}).second) {
    nextList.at(stmtNum1).emplace(stmtNum2);
  }

  if (!prevList.emplace(stmtNum2, std::unordered_set<int>{stmtNum1}).second) {
    prevList.at(stmtNum2).emplace(stmtNum1);
  }

  return true;
}

std::unordered_set<int> NextComponent::getNextStatementsOf(int stmtNum) {
  if (nextList.find(stmtNum) != nextList.end()) {
    return nextList.at(stmtNum);
  }
  return {};
}

std::unordered_set<int> NextComponent::getPrevStatementsOf(int stmtNum) {
  if (prevList.find(stmtNum) != prevList.end()) {
    return prevList.at(stmtNum);
  }
  return {};
}

std::unordered_set<std::pair<int, int>, std::hash_pair> NextComponent::getAllStatementsPairs() {
  return stmtPairs;
}

