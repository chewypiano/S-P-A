//
// Created by Christopher Mervyn on 1/9/21.
//
#include "catch.hpp"
#include "PKB/PKB.h"

TEST_CASE("IGNOREStorageCreation") {
  PKB storage1;
  storage1.addProc("ASample");

  storage1.addStatement(1, read, "ASample");
  storage1.addStatement(2, print, "ASample");
  storage1.addStatement(3, assign, "ASample");

  storage1.addVariable("car");
  storage1.addVariable("wheel");
  storage1.addVariable("bike");

  storage1.addConst("6");
  storage1.addConst("8");

  REQUIRE(1 == 1);
}

TEST_CASE("getProcList") {
  PKB storage2;
  storage2.addProc("ASample");
  REQUIRE(storage2.getProcList() == std::unordered_set<std::string>{"ASample"});
}

TEST_CASE("negative getProcList") {
  PKB storage2;
  REQUIRE(storage2.getProcList().empty());
}

TEST_CASE("getStmtType") {
  PKB storage3;
  storage3.addStatement(1, read, "ASample");
  storage3.addStatement(2, print, "ASample");
  storage3.addStatement(3, assign, "ASample");
  REQUIRE(storage3.getStmtType(2) == print);
}

TEST_CASE("negative getStmtType") {
  PKB storage3;
  storage3.addStatement(1, read, "ASample");
  storage3.addStatement(2, print, "ASample");
  storage3.addStatement(3, assign, "ASample");
  REQUIRE(storage3.getStmtType(4) == stmtType::nullValue);
}

TEST_CASE("getVarList") {
  PKB storage4;
  storage4.addVariable("car");
  storage4.addVariable("wheel");
  storage4.addVariable("bike");
  REQUIRE(storage4.getVarList() == std::unordered_set<std::string>{"car", "wheel", "bike"});
}

TEST_CASE("negative getVarList") {
  PKB storage4;
  REQUIRE(storage4.getVarList().empty());
}

TEST_CASE("getConstList") {
  PKB storage5;
  storage5.addConst("6");
  storage5.addConst("8");
  REQUIRE(storage5.getConstList() == std::unordered_set<std::string>{"6", "8"});
}

TEST_CASE("negative getConstList") {
  PKB storage5;
  REQUIRE(storage5.getConstList().empty());
}

TEST_CASE("getReadStmt") {
  PKB storage6;
  storage6.addStatement(1, read, "ASample");
  storage6.addStatement(2, print, "ASample");
  storage6.addStatement(3, assign, "ASample");
  REQUIRE(storage6.getReadStmts() == std::unordered_set<int>{1});
}

TEST_CASE("negative getReadStmt") {
  PKB storage6;
  storage6.addStatement(2, print, "ASample");
  storage6.addStatement(3, assign, "ASample");
  REQUIRE(storage6.getReadStmts().empty());
}

TEST_CASE("getPrintStmt") {
  PKB storage8;
  storage8.addStatement(1, read, "ASample");
  storage8.addStatement(2, print, "ASample");
  storage8.addStatement(3, assign, "ASample");
  REQUIRE(storage8.getPrintStmts() == std::unordered_set<int>{2});
}

TEST_CASE("negative getPrintStmt") {
  PKB storage8;
  storage8.addStatement(1, read, "ASample");
  storage8.addStatement(3, assign, "ASample");
  REQUIRE(storage8.getPrintStmts().empty());
}

TEST_CASE("getAssignStmt") {
  PKB storage9;
  storage9.addStatement(1, read, "ASample");
  storage9.addStatement(2, print, "ASample");
  storage9.addStatement(3, assign, "ASample");
  REQUIRE(storage9.getAssignStmts() == std::unordered_set<int>{3});
}

TEST_CASE("negative getAssignStmt") {
  PKB storage9;
  storage9.addStatement(1, read, "ASample");
  storage9.addStatement(2, print, "ASample");
  REQUIRE(storage9.getAssignStmts().empty());
}

TEST_CASE("getCallStmt") {
  PKB storage;
  storage.addStatement(1, read, "ASample");
  storage.addStatement(2, print, "ASample");
  storage.addStatement(3, assign, "ASample");
  storage.addStatement(4, call, "ASample");
  REQUIRE(storage.getCallStmts() == std::unordered_set<int>{4});
}

TEST_CASE("negative getCallStmt") {
  PKB storage;
  storage.addStatement(1, read, "ASample");
  storage.addStatement(2, print, "ASample");
  storage.addStatement(3, assign, "ASample");
  REQUIRE(storage.getCallStmts().empty());
}

TEST_CASE("getTotalStmtNum") {
  PKB storage7;
  storage7.addStatement(1, read, "ASample");
  storage7.addStatement(2, print, "ASample");
  storage7.addStatement(3, assign, "ASample");
  REQUIRE(storage7.getTotalStmtNum() == 3);
}

TEST_CASE("empty getTotalStmtNum") {
  PKB storage7;
  REQUIRE(storage7.getTotalStmtNum() == 0);
}

TEST_CASE("getStmtList") {
  PKB storage;
  storage.addStatement(1, read, "ASample");
  storage.addStatement(2, print, "ASample");
  storage.addStatement(3, assign, "ASample");
  storage.addStatement(4, read, "BS");
  storage.addStatement(5, print, "BS");
  storage.addStatement(6, assign, "ASample");
  REQUIRE(storage.getStmtList("ASample") == std::unordered_set<int>{1, 2, 3, 6});
}

TEST_CASE("negative getStmtList") {
  PKB storage;
  storage.addStatement(4, read, "BS");
  storage.addStatement(5, print, "BS");
  REQUIRE(storage.getStmtList("ASample").empty());
}

TEST_CASE("getStmtProc") {
  PKB storage;
  storage.addStatement(1, read, "ASample");
  storage.addStatement(2, print, "ASample");
  storage.addStatement(3, assign, "ASample");
  storage.addStatement(4, read, "BS");
  storage.addStatement(5, print, "BS");
  storage.addStatement(6, assign, "ASample");
  REQUIRE(storage.getStmtProc(4) == "BS");

}

TEST_CASE("negative getStmtProc") {
  PKB storage;
  storage.addStatement(1, read, "ASample");
  storage.addStatement(2, print, "ASample");
  storage.addStatement(3, assign, "ASample");
  storage.addStatement(4, read, "BS");
  storage.addStatement(5, print, "BS");
  storage.addStatement(6, assign, "ASample");
  REQUIRE(storage.getStmtProc(7).empty());

}

TEST_CASE("getAllStmtNum") {
  PKB storage1;
  storage1.addProc("ASample");

  storage1.addStatement(1, read, "ASample");
  storage1.addStatement(2, print, "ASample");
  storage1.addStatement(3, assign, "ASample");

  storage1.addVariable("car");
  storage1.addVariable("wheel");
  storage1.addVariable("bike");

  storage1.addConst("6");
  storage1.addConst("8");

  REQUIRE(storage1.getAllStmt() == std::unordered_set<int>{1, 2, 3});
}

TEST_CASE("affects List") {
  PKB storage1;
  storage1.addProc("ASample");

  storage1.addStatement(1, read, "ASample");
  storage1.addStatement(2, print, "ASample");
  storage1.addStatement(3, assign, "ASample");
  storage1.addStatement(4, assign, "ASample");
  storage1.addStatement(5, assign, "ASample");

  storage1.addModifyStmt(3, "a");
  storage1.addUseStmt(3, "b");
  storage1.addUseStmt(3, "c");

  storage1.addModifyStmt(4, "x");
  storage1.addUseStmt(4, "y");
  storage1.addUseStmt(4, "z");

  storage1.addModifyStmt(5, "p");

  std::unordered_map<int, std::pair<std::string, std::unordered_set<std::string>>> res;
  res.emplace(3, std::pair<std::string, std::unordered_set<std::string>>("a", {"b", "c"}));
  res.emplace(4, std::pair<std::string, std::unordered_set<std::string>>("x", {"y", "z"}));
  res.emplace(5, std::pair<std::string, std::unordered_set<std::string>>("p", {}));

  REQUIRE(storage1.getAssignList() == res);
}

TEST_CASE("getAllProcedureWithStartAndEnd") {
  PKB storage1;
  storage1.addProc("ASample");

  storage1.addStatement(1, read, "ASample");
  storage1.addStatement(2, print, "ASample");
  storage1.addStatement(3, assign, "ASample");
  storage1.addStatement(4, assign, "BSample");
  storage1.addStatement(5, assign, "BSample");
  storage1.addStatement(6, print, "CSample");
  storage1.addStatement(7, assign, "CSample");
  storage1.addStatement(8, assign, "CSample");
  storage1.addProc("ASample");
  storage1.addProc("BSample");
  storage1.addProc("CSample");

  std::unordered_map<std::string, std::pair<int, std::unordered_set<int>>> res;
  res.emplace("ASample", std::pair<int, std::unordered_set<int>>(1,std::unordered_set<int>{1,2,3}));
  res.emplace("BSample", std::pair<int, std::unordered_set<int>>(4,std::unordered_set<int>{4,5}));
  res.emplace("CSample", std::pair<int, std::unordered_set<int>>(6,std::unordered_set<int>{6,7,8}));

  REQUIRE(storage1.getAllProcedureWithStartAndEnd() == res);
}



