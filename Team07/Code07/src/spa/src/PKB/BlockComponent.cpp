//
// Created by Christopher Mervyn on 13/9/21.
//

#include "BlockComponent.h"

BlockComponent::BlockComponent() {}

bool BlockComponent::addIfStmt(int startStmtNum, int lastStmtNum, int elseStmtNum) {
  if (!ifBlockStmtList.emplace(startStmtNum, std::pair<int, int>{lastStmtNum, elseStmtNum}).second) {
    return false;
  }
  return true;
}

bool BlockComponent::addWhileStmt(int startStmtNum, int lastStmtNum) {
  if (!whileBlockStmtList.emplace(startStmtNum, lastStmtNum).second) {
    return false;
  }
  return true;
}

void BlockComponent::addIfBlock(int startStmtNum, std::string ctrlVar) {
  if (!ifBlock.emplace(ctrlVar, std::unordered_set<int>{startStmtNum}).second) {
    ifBlock.at(ctrlVar).emplace(startStmtNum);
  }
}

void BlockComponent::addWhileBlock(int startStmtNum, std::string ctrlVar) {
  if (!whileBlock.emplace(ctrlVar, std::unordered_set<int>{startStmtNum}).second) {
    whileBlock.at(ctrlVar).emplace(startStmtNum);
  }
}

int BlockComponent::getWhileEndStmt(int stmtNum) {
  if (whileBlockStmtList.find(stmtNum) != whileBlockStmtList.end()) {
    return whileBlockStmtList.at(stmtNum);
  }
  return -1;
}

int BlockComponent::getElseStmt(int stmtNum) {
  if (ifBlockStmtList.find(stmtNum) != ifBlockStmtList.end()) {
    return ifBlockStmtList.at(stmtNum).second;
  }
  return -1;
}

int BlockComponent::getIfEndStmt(int stmtNum) {
  if (ifBlockStmtList.find(stmtNum) != ifBlockStmtList.end()) {
    return ifBlockStmtList.at(stmtNum).first;
  }
  return -1;
}

std::unordered_set<int> BlockComponent::getAllIfBlock() {
  std::unordered_set<int> ifStmtList;
  for (auto kv: ifBlock) {
    for (auto kj: kv.second) {
      ifStmtList.emplace(kj);
    }
  }
  return ifStmtList;
}

std::unordered_set<int> BlockComponent::getAllWhileBlock() {
  std::unordered_set<int> whileStmtList;
  for (auto kv: whileBlock) {
    for (auto kj: kv.second) {
      whileStmtList.emplace(kj);
    }
  }
  return whileStmtList;
}

std::unordered_set<int> BlockComponent::getAllIfCtrlVar(std::string ctrlVar) {
  if (ifBlock.find(ctrlVar) == ifBlock.end()) {
    return {};
  }
  return ifBlock.at(ctrlVar);
}

std::unordered_set<int> BlockComponent::getAllWhileCtrlVar(std::string ctrlVar) {
  if (whileBlock.find(ctrlVar) == whileBlock.end()) {
    return {};
  }
  return whileBlock.at(ctrlVar);
}

std::unordered_set<int> BlockComponent::getAllIfStmts() {
  std::unordered_set<int> ifStmtList;
  for (auto const& ev: ifBlockStmtList) {
    ifStmtList.emplace(ev.first);
  }
  return ifStmtList;
}

std::unordered_set<int> BlockComponent::getAllWhileStmts() {
  std::unordered_set<int> whileStmtList;
  for (auto const& ev: whileBlockStmtList) {
    whileStmtList.emplace(ev.first);
  }
  return whileStmtList;
}



