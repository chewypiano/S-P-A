//
// Created by Christopher Mervyn on 29/9/21.
//

#include "catch.hpp"
#include "PKB/PKB.h"

TEST_CASE("getCallerProc") {
  PKB storage1;
  storage1.addProc("ASample");

  storage1.addStatement(1, read, "ASample");
  storage1.addStatement(2, print, "ASample");
  storage1.addStatement(3, assign, "ASample");
  storage1.addStatement(4, call, "toyota");
  storage1.addCall(4, "toyota", "merc");
  storage1.addAssignPattern(3, "x", "yz*");

  storage1.addVariable("car");
  storage1.addVariable("wheel");
  storage1.addVariable("bike");

  storage1.addConst("6");
  storage1.addConst("8");

  REQUIRE(storage1.getCallerProc(4) == "toyota");
}

TEST_CASE("negative getCallerProc") {
  PKB storage1;
  storage1.addProc("ASample");

  storage1.addStatement(1, read, "ASample");
  storage1.addStatement(2, print, "ASample");
  storage1.addStatement(3, assign, "ASample");
  storage1.addStatement(4, call, "toyota");
  storage1.addCall(4, "toyota", "merc");
  storage1.addAssignPattern(3, "x", "yz*");

  storage1.addVariable("car");
  storage1.addVariable("wheel");
  storage1.addVariable("bike");

  storage1.addConst("6");
  storage1.addConst("8");

  REQUIRE(storage1.getCallerProc(5).empty());
}

TEST_CASE("Get Caller proc number out of range") {
  PKB storage1;
  storage1.addStatement(1, read, "ASample");
  storage1.addStatement(2, print, "ASample");
  storage1.addStatement(3, assign, "ASample");
  storage1.addStatement(4, call, "toyota");
  storage1.addCall(4, "toyota", "merc");
  storage1.addAssignPattern(3, "x", "yz*");
  REQUIRE(storage1.getCallerProc(1).empty());
}

TEST_CASE("Get Caller proc negative number") {
  PKB storage1;
  storage1.addStatement(1, read, "ASample");
  storage1.addStatement(2, print, "ASample");
  storage1.addStatement(3, assign, "ASample");
  storage1.addStatement(4, call, "toyota");
  storage1.addCall(4, "toyota", "merc");
  storage1.addAssignPattern(3, "x", "yz*");
  REQUIRE(storage1.getCallerProc(-1).empty());
}

TEST_CASE("getCalledProc") {
  PKB storage1;
  storage1.addProc("ASample");

  storage1.addStatement(1, read, "ASample");
  storage1.addStatement(2, print, "ASample");
  storage1.addStatement(3, assign, "ASample");
  storage1.addStatement(4, call, "toyota");
  storage1.addCall(4, "toyota", "merc");
  storage1.addAssignPattern(3, "x", "yz*");

  storage1.addVariable("car");
  storage1.addVariable("wheel");
  storage1.addVariable("bike");

  storage1.addConst("6");
  storage1.addConst("8");

  REQUIRE(storage1.getCalledProc(4) == "merc");
}

TEST_CASE("negative getCalledProc") {
  PKB storage1;
  storage1.addProc("ASample");

  storage1.addStatement(1, read, "ASample");
  storage1.addStatement(2, print, "ASample");
  storage1.addStatement(3, assign, "ASample");
  storage1.addStatement(4, call, "toyota");
  storage1.addCall(4, "toyota", "merc");
  storage1.addAssignPattern(3, "x", "yz*");

  storage1.addVariable("car");
  storage1.addVariable("wheel");
  storage1.addVariable("bike");

  storage1.addConst("6");
  storage1.addConst("8");

  REQUIRE(storage1.getCalledProc(45).empty());
}

TEST_CASE("Get Called proc number out of range") {
  PKB storage1;
  storage1.addStatement(1, read, "ASample");
  storage1.addStatement(2, print, "ASample");
  storage1.addStatement(3, assign, "ASample");
  storage1.addStatement(4, call, "toyota");
  storage1.addCall(4, "toyota", "merc");
  storage1.addAssignPattern(3, "x", "yz*");
  REQUIRE(storage1.getCalledProc(1).empty());
}

TEST_CASE("Get Called proc negative number") {
  PKB storage1;
  storage1.addStatement(1, read, "ASample");
  storage1.addStatement(2, print, "ASample");
  storage1.addStatement(3, assign, "ASample");
  storage1.addStatement(4, call, "toyota");
  storage1.addCall(4, "toyota", "merc");
  storage1.addAssignPattern(3, "x", "yz*");
  REQUIRE(storage1.getCalledProc(-1).empty());
}

TEST_CASE("get caller stmt") {
  PKB storage1;
  storage1.addStatement(1, read, "ASample");
  storage1.addStatement(2, print, "ASample");
  storage1.addStatement(3, assign, "ASample");
  storage1.addStatement(4, call, "toyota");
  storage1.addCall(4, "toyota", "merc");
  storage1.addCall(5, "merc", "honda");
  storage1.addAssignPattern(3, "x", "yz*");
  storage1.addIfStmt(5, 6, 0);
  storage1.addIfStmt(5, 7, 0);
  storage1.addIfStmt(5, 8, 0);
  storage1.addIfStmt(5, 9, 0);
  storage1.addIfCtrlVar(5, "x");
  REQUIRE(storage1.getProcsCalledFromProc("merc") == std::unordered_set<std::string>{"toyota"});
}

TEST_CASE("negative get caller stmt") {
  PKB storage1;
  storage1.addStatement(1, read, "ASample");
  storage1.addStatement(2, print, "ASample");
  storage1.addStatement(3, assign, "ASample");
  storage1.addStatement(4, call, "toyota");
  storage1.addCall(4, "toyota", "merc");
  storage1.addCall(5, "merc", "honda");
  storage1.addAssignPattern(3, "x", "yz*");
  storage1.addIfStmt(5, 6, 0);
  storage1.addIfStmt(5, 7, 0);
  storage1.addIfStmt(5, 8, 0);
  storage1.addIfStmt(5, 9, 0);
  storage1.addIfCtrlVar(5, "x");
  REQUIRE(storage1.getProcsCalledFromProc("ferrari").empty());
}

TEST_CASE("get called stmt") {
  PKB storage1;
  storage1.addStatement(1, read, "ASample");
  storage1.addStatement(2, print, "ASample");
  storage1.addStatement(3, assign, "ASample");
  storage1.addStatement(4, call, "toyota");
  storage1.addCall(4, "toyota", "merc");
  storage1.addCall(5, "merc", "honda");
  storage1.addAssignPattern(3, "x", "yz*");
  storage1.addIfStmt(5, 6, 0);
  storage1.addIfStmt(5, 7, 0);
  storage1.addIfStmt(5, 8, 0);
  storage1.addIfStmt(5, 9, 0);
  storage1.addIfCtrlVar(5, "x");
  REQUIRE(storage1.getProcsCalledByProc("merc") == std::unordered_set<std::string>{"honda"});
}

TEST_CASE("negative get called stmt") {
  PKB storage1;
  storage1.addStatement(1, read, "ASample");
  storage1.addStatement(2, print, "ASample");
  storage1.addStatement(3, assign, "ASample");
  storage1.addStatement(4, call, "toyota");
  storage1.addCall(4, "toyota", "merc");
  storage1.addCall(5, "merc", "honda");
  storage1.addAssignPattern(3, "x", "yz*");
  storage1.addIfStmt(5, 6, 0);
  storage1.addIfStmt(5, 7, 0);
  storage1.addIfStmt(5, 8, 0);
  storage1.addIfStmt(5, 9, 0);
  storage1.addIfCtrlVar(5, "x");
  REQUIRE(storage1.getProcsCalledByProc("porsche").empty());
}

TEST_CASE("check call relation") {
  PKB storage1;
  storage1.addStatement(1, read, "ASample");
  storage1.addStatement(2, print, "ASample");
  storage1.addStatement(3, assign, "ASample");
  storage1.addStatement(4, call, "toyota");
  storage1.addCall(4, "toyota", "merc");
  storage1.addCall(5, "merc", "honda");
  storage1.addAssignPattern(3, "x", "yz*");
  storage1.addIfStmt(5, 6, 0);
  storage1.addIfStmt(5, 7, 0);
  storage1.addIfStmt(5, 8, 0);
  storage1.addIfStmt(5, 9, 0);
  storage1.addIfCtrlVar(5, "x");
  REQUIRE(storage1.isCallRelation("toyota", "merc") == true);
}

TEST_CASE("negative check call relation") {
  PKB storage1;
  storage1.addStatement(1, read, "ASample");
  storage1.addStatement(2, print, "ASample");
  storage1.addStatement(3, assign, "ASample");
  storage1.addStatement(4, call, "toyota");
  storage1.addCall(4, "toyota", "merc");
  storage1.addCall(5, "merc", "honda");
  storage1.addAssignPattern(3, "x", "yz*");
  storage1.addIfStmt(5, 6, 0);
  storage1.addIfStmt(5, 7, 0);
  storage1.addIfStmt(5, 8, 0);
  storage1.addIfStmt(5, 9, 0);
  storage1.addIfCtrlVar(5, "x");
  REQUIRE(storage1.isCallRelation("toyota", "honda") == false);
}

TEST_CASE("check call star relation") {
  PKB storage1;
  storage1.addStatement(1, read, "ASample");
  storage1.addStatement(2, print, "ASample");
  storage1.addStatement(3, assign, "ASample");
  storage1.addStatement(4, call, "toyota");
  storage1.addCall(4, "toyota", "merc");
  storage1.addCall(5, "merc", "honda");
  storage1.addCallStar("toyota", "honda");
  storage1.addAssignPattern(3, "x", "yz*");
  storage1.addIfStmt(5, 6, 0);
  storage1.addIfStmt(5, 7, 0);
  storage1.addIfStmt(5, 8, 0);
  storage1.addIfStmt(5, 9, 0);
  storage1.addIfCtrlVar(5, "x");
  REQUIRE(storage1.isCallStarRelation("toyota", "honda") == true);
}

TEST_CASE("negative check call star relation") {
  PKB storage1;
  storage1.addStatement(1, read, "ASample");
  storage1.addStatement(2, print, "ASample");
  storage1.addStatement(3, assign, "ASample");
  storage1.addStatement(4, call, "toyota");
  storage1.addCall(4, "toyota", "merc");
  storage1.addCall(5, "merc", "honda");
  storage1.addAssignPattern(3, "x", "yz*");
  storage1.addIfStmt(5, 6, 0);
  storage1.addIfStmt(5, 7, 0);
  storage1.addIfStmt(5, 8, 0);
  storage1.addIfStmt(5, 9, 0);
  storage1.addIfCtrlVar(5, "x");
  REQUIRE(storage1.isCallStarRelation("honda", "toyota") == false);
}

TEST_CASE("get called* stmt by") {
  PKB storage1;
  storage1.addStatement(1, read, "ASample");
  storage1.addStatement(2, print, "ASample");
  storage1.addStatement(3, assign, "ASample");
  storage1.addStatement(4, call, "toyota");
  storage1.addCall(4, "toyota", "merc");
  storage1.addCall(5, "merc", "honda");
  storage1.addCall(5, "honda", "ferrari");
  storage1.addCallStar("toyota", "merc");
  storage1.addCallStar("toyota", "honda");
  storage1.addCallStar("toyota", "ferrari");
  storage1.addAssignPattern(3, "x", "yz*");
  storage1.addIfStmt(5, 6, 0);
  storage1.addIfStmt(5, 7, 0);
  storage1.addIfStmt(5, 8, 0);
  storage1.addIfStmt(5, 9, 0);
  storage1.addIfCtrlVar(5, "x");
  REQUIRE(storage1.getProcsCalledStarByProc("toyota") == std::unordered_set<std::string>{"merc", "honda", "ferrari"});
}

TEST_CASE("negative get called* stmt by") {
  PKB storage1;
  storage1.addStatement(1, read, "ASample");
  storage1.addStatement(2, print, "ASample");
  storage1.addStatement(3, assign, "ASample");
  storage1.addStatement(4, call, "toyota");
  storage1.addCall(4, "toyota", "merc");
  storage1.addCall(5, "merc", "honda");
  storage1.addCall(5, "honda", "ferrari");
  storage1.addCallStar("toyota", "merc");
  storage1.addCallStar("toyota", "honda");
  storage1.addCallStar("toyota", "ferrari");
  storage1.addAssignPattern(3, "x", "yz*");
  storage1.addIfStmt(5, 6, 0);
  storage1.addIfStmt(5, 7, 0);
  storage1.addIfStmt(5, 8, 0);
  storage1.addIfStmt(5, 9, 0);
  storage1.addIfCtrlVar(5, "x");
  REQUIRE(storage1.getProcsCalledStarByProc("merc") == std::unordered_set<std::string>{});
}

TEST_CASE("get called* stmt from") {
  PKB storage1;
  storage1.addStatement(1, read, "ASample");
  storage1.addStatement(2, print, "ASample");
  storage1.addStatement(3, assign, "ASample");
  storage1.addStatement(4, call, "toyota");
  storage1.addCall(4, "toyota", "merc");
  storage1.addCall(5, "merc", "honda");
  storage1.addCall(5, "honda", "ferrari");
  storage1.addCallStar("toyota", "merc");
  storage1.addCallStar("toyota", "honda");
  storage1.addCallStar("toyota", "ferrari");
  storage1.addCallStar("merc", "ferrari");
  storage1.addCallStar("merc", "honda");
  storage1.addCallStar("honda", "ferrari");
  REQUIRE(storage1.getProcsCalledStarFromProc("ferrari") == std::unordered_set<std::string>{"toyota", "merc", "honda"});
}

TEST_CASE("negative get called* stmt from") {
  PKB storage1;
  storage1.addStatement(1, read, "ASample");
  storage1.addStatement(2, print, "ASample");
  storage1.addStatement(3, assign, "ASample");
  storage1.addStatement(4, call, "toyota");
  storage1.addCall(4, "toyota", "merc");
  storage1.addCall(5, "merc", "honda");
  storage1.addCall(5, "honda", "ferrari");
  storage1.addCallStar("toyota", "merc");
  storage1.addCallStar("toyota", "honda");
  storage1.addCallStar("toyota", "ferrari");
  storage1.addCallStar("merc", "ferrari");
  storage1.addCallStar("merc", "honda");
  storage1.addCallStar("honda", "ferrari");
  REQUIRE(storage1.getProcsCalledStarFromProc("toyota") == std::unordered_set<std::string>{});
}


TEST_CASE("getAllCallStmts") {
  PKB storage1;
  storage1.addStatement(1, read, "ASample");
  storage1.addStatement(2, print, "ASample");
  storage1.addStatement(3, assign, "ASample");
  storage1.addStatement(4, call, "toyota");
  storage1.addStatement(5, call, "toyota");
  storage1.addStatement(6, call, "lexus");
  storage1.addCall(4, "toyota", "merc");
  storage1.addCall(5, "toyota", "honda");
  storage1.addCall(6, "lexus", "ferrari");

  std::unordered_map<int, std::string> res;
  res.emplace(5, "honda");
  res.emplace(4, "merc");
  res.emplace(6, "ferrari");

  REQUIRE(storage1.getAllCallStmts() == res);
}

