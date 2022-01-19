//
// Created by Christopher Mervyn on 13/9/21.
//

#ifndef AUTOTESTER_BLOCKCOMPONENT_H
#define AUTOTESTER_BLOCKCOMPONENT_H

#include <string>
#include <unordered_set>
#include <unordered_map>

#include <PKB/PairHasher.h>

class BlockComponent {
 public:
  BlockComponent();

  bool addIfStmt(int startStmtNum, int stmtInBlock, int elseStmtNum);

  bool addWhileStmt(int startStmtNum, int lastStmtNum);

  void addIfBlock(int startStmtNum, std::string ctrlVar);

  void addWhileBlock(int startStmtNum, std::string ctrlVar);

  int getWhileEndStmt(int stmtNum);

  int getElseStmt(int stmtNum);

  int getIfEndStmt(int stmtNum);

  std::unordered_set<int> getAllIfBlock();

  std::unordered_set<int> getAllWhileBlock();

  std::unordered_set<int> getAllIfCtrlVar(std::string ctrlVar);

  std::unordered_set<int> getAllWhileCtrlVar(std::string ctrlVar);

  std::unordered_set<int> getAllIfStmts();

  std::unordered_set<int> getAllWhileStmts();

 private:
  std::unordered_map<std::string, std::unordered_set<int>> ifBlock;
  std::unordered_map<std::string, std::unordered_set<int>> whileBlock;
  std::unordered_map<int, std::pair<int, int>> ifBlockStmtList;
  std::unordered_map<int, int> whileBlockStmtList;

};

#endif //AUTOTESTER_BLOCKCOMPONENT_H
