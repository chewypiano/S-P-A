//
// Created by Christopher Mervyn on 29/9/21.
//

#include "catch.hpp"
#include "PKB/PKB.h"

TEST_CASE("getPrintList for addUsingStmt") {
  PKB storage;
  storage.addStatement(1, read, "ASample");
  storage.addModifyStmt(1, "x");
  storage.addStatement(2, print, "ASample");
  storage.addUseStmt(2, "y");
  storage.addStatement(3, assign, "ASample");
  std::unordered_map<int, std::string> resultMap = {};
  resultMap.emplace(2, "y");

  REQUIRE(storage.getPrintList() == resultMap);
}

TEST_CASE("addUsingStmt") {
  PKB storage;
  storage.addStatement(1, read, "ASample");
  storage.addModifyStmt(1, "x");
  storage.addStatement(2, print, "ASample");
  storage.addUseStmt(2, "y");
  storage.addStatement(3, assign, "ASample");

  REQUIRE(storage.isStmtUsingVar(2, "y") == true);
}

TEST_CASE("negative addUsingStmt") {
  PKB storage;
  storage.addStatement(1, read, "ASample");
  storage.addModifyStmt(1, "x");
  storage.addStatement(2, print, "ASample");
  storage.addUseStmt(2, "y");
  storage.addStatement(3, assign, "ASample");

  REQUIRE(storage.isStmtUsingVar(3, "y") == false);
}

TEST_CASE("add use relation") {
  PKB storage;
  storage.addStatement(1, print, "ASample");
  storage.addUseStmt(1, "a");
  REQUIRE(storage.getVarUsedByStmt(1) == std::unordered_set<std::string>{"a"});
}

TEST_CASE("negative add use relation") {
  PKB storage;
  storage.addStatement(1, print, "ASample");
  REQUIRE(storage.getVarUsedByStmt(1).empty());
}

TEST_CASE("addUsingProc") {
  PKB storage;
  storage.addStatement(1, read, "ASample");
  storage.addModifyProc("ferrari", "x");
  storage.addStatement(2, print, "ASample");
  storage.addUseProc("porsche", "y");
  storage.addStatement(3, assign, "ASample");

  REQUIRE(storage.isProcUsingVar("porsche", "y") == true);
}

TEST_CASE("negative addUsingProc") {
  PKB storage;
  storage.addStatement(1, read, "ASample");
  storage.addModifyProc("ferrari", "x");
  storage.addStatement(2, print, "ASample");
  storage.addUseProc("porsche", "y");
  storage.addStatement(3, assign, "ASample");

  REQUIRE(storage.isProcUsingVar("lamborghini", "x") == false);
}

