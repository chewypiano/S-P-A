//
// Created by Christopher Mervyn on 29/9/21.
//

#include "catch.hpp"
#include "PKB/PKB.h"

TEST_CASE("getReadList for addModifyingStmt") {
  PKB storage;
  storage.addStatement(1, read, "ASample");
  storage.addModifyStmt(1, "x");
  storage.addStatement(2, print, "ASample");
  storage.addStatement(3, assign, "ASample");
  std::unordered_map<int, std::string> resultMap = {};
  resultMap.emplace(1, "x");

  REQUIRE(storage.getReadList() == resultMap);
}

TEST_CASE("addModifyingStmt") {
  PKB storage;
  storage.addStatement(1, read, "ASample");
  storage.addModifyStmt(1, "x");
  storage.addStatement(2, print, "ASample");
  storage.addStatement(3, assign, "ASample");

  REQUIRE(storage.isStmtModifyingVar(1, "x") == true);
}

TEST_CASE("negative addModifyingStmt") {
  PKB storage;
  storage.addStatement(1, read, "ASample");
  storage.addModifyStmt(1, "x");
  storage.addStatement(2, print, "ASample");
  storage.addStatement(3, assign, "ASample");

  REQUIRE(storage.isStmtModifyingVar(3, "z") == false);
}

TEST_CASE("add modifies relation") {
  PKB storage;
  storage.addStatement(1, read, "ASample");
  storage.addModifyStmt(1, "a");
  REQUIRE(storage.getVarModifiedByStmt(1) == std::unordered_set<std::string>{"a"});
}

TEST_CASE("negative add modifies relation") {
  PKB storage;
  storage.addStatement(1, read, "ASample");
  REQUIRE(storage.getVarUsedByStmt(1).empty());
}

TEST_CASE("addModifyingProc") {
  PKB storage;
  storage.addStatement(1, read, "ASample");
  storage.addModifyProc("ferrari", "x");
  storage.addStatement(2, print, "ASample");
  storage.addUseProc("porsche", "y");
  storage.addStatement(3, assign, "ASample");

  REQUIRE(storage.isProcModifyingVar("ferrari", "x") == true);
}

TEST_CASE("negative addModifyingProc") {
  PKB storage;
  storage.addStatement(1, read, "ASample");
  storage.addModifyProc("ferrari", "x");
  storage.addStatement(2, print, "ASample");
  storage.addUseProc("porsche", "y");
  storage.addStatement(3, assign, "ASample");

  REQUIRE(storage.isProcUsingVar("porsche", "x") == false);
}

TEST_CASE("getStmtListofVar") {
  PKB storage;
  storage.addStatement(1, read, "ASample");
  storage.addModifyProc("ferrari", "x");
  storage.addStatement(2, print, "ASample");
  storage.addUseProc("porsche", "y");
  storage.addStatement(3, assign, "ASample");
  storage.addStatement(4, call, "ASample");
  storage.addStatement(5, ifStmt, "ASample");
  storage.addStatement(6, assign, "ASample");
  storage.addStatement(7, assign, "ASample");

  storage.addModifyStmt(1, "x");
  storage.addModifyStmt(3, "a");
  storage.addModifyStmt(4, "x");
  storage.addModifyStmt(4, "b");
  storage.addModifyStmt(4, "c");
  storage.addModifyStmt(5, "d");
  storage.addModifyStmt(5, "e");
  storage.addModifyStmt(6, "d");
  storage.addModifyStmt(7, "e");

  std::unordered_map<int, std::unordered_set<std::string>> res;
  res.emplace(1, std::unordered_set<std::string>{"x"});
  res.emplace(3, std::unordered_set<std::string>{"a"});
  res.emplace(4, std::unordered_set<std::string>{"x", "b", "c"});
  res.emplace(5, std::unordered_set<std::string>{"d", "e"});
  res.emplace(6, std::unordered_set<std::string>{"d"});
  res.emplace(7, std::unordered_set<std::string>{"e"});

  REQUIRE(storage.getStmtListofVar() == res);
}

TEST_CASE("getStmtListofVar2") {
  PKB storage;
  storage.addStatement(1, read, "ASample");
  storage.addModifyProc("ferrari", "x");
  storage.addStatement(2, print, "ASample");
  storage.addUseProc("porsche", "y");
  storage.addStatement(3, assign, "ASample");
  storage.addStatement(4, call, "ASample");
  storage.addStatement(5, ifStmt, "ASample");
  storage.addStatement(6, assign, "ASample");
  storage.addStatement(7, assign, "ASample");
  storage.addStatement(8, whileStmt, "ASample");
  storage.addStatement(9, assign, "ASample");


  storage.addModifyStmt(1, "x");
  storage.addModifyStmt(3, "a");
  storage.addModifyStmt(4, "x");
  storage.addModifyStmt(4, "b");
  storage.addModifyStmt(4, "c");
  storage.addModifyStmt(5, "d");
  storage.addModifyStmt(5, "e");
  storage.addModifyStmt(6, "d");
  storage.addModifyStmt(7, "e");
  storage.addModifyStmt(8, "f");
  storage.addModifyStmt(9, "f");

  storage.addIfStmt(5, 6, 0);
  storage.addIfStmt(5, 7, 0);
  storage.addWhileStmt(8, 9);

  storage.addIfCtrlVar(5, "ppp");
  storage.addWhileCtrlVar(8, "uuu");


  std::unordered_map<int, std::unordered_set<std::string>> res;
  res.emplace(1, std::unordered_set<std::string>{"x"});
  res.emplace(3, std::unordered_set<std::string>{"a"});
  res.emplace(4, std::unordered_set<std::string>{"x", "b", "c"});
  res.emplace(6, std::unordered_set<std::string>{"d"});
  res.emplace(7, std::unordered_set<std::string>{"e"});
  res.emplace(9, std::unordered_set<std::string>{"f"});

  REQUIRE(storage.getStmtListofVar2() == res);
}

TEST_CASE("getAllModifiesProc") {
  PKB storage;
  storage.addStatement(1, read, "ferrari");
  storage.addModifyProc("ferrari", "x");
  storage.addStatement(2, print, "porsche");
  storage.addUseProc("porsche", "y");
  storage.addStatement(3, ifStmt, "ASample");
  storage.addStatement(4, assign, "toyota");
  storage.addModifyProc("toyota", "y");

  std::unordered_set<std::string> res;
  res.emplace("ferrari");
  res.emplace("toyota");
  REQUIRE(storage.getAllModifiesProc() == res);
}