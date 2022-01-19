//
// Created by Christopher Mervyn on 4/10/21.
//
#include "catch.hpp"
#include "PKB/PKB.h"

TEST_CASE("getLHSAssignPattern") {
  PKB storage1;
  storage1.addProc("ASample");

  storage1.addStatement(1, read, "ASample");
  storage1.addStatement(2, print, "ASample");
  storage1.addStatement(3, assign, "ASample");
  storage1.addAssignPattern(3, "x", "yz*");

  storage1.addVariable("car");
  storage1.addVariable("wheel");
  storage1.addVariable("bike");

  storage1.addConst("6");
  storage1.addConst("8");

  REQUIRE(storage1.getLHSAssign(3) == "x");
}

TEST_CASE("getRHSAssignPattern") {
  PKB storage1;
  storage1.addProc("ASample");

  storage1.addStatement(1, read, "ASample");
  storage1.addStatement(2, print, "ASample");
  storage1.addStatement(3, assign, "ASample");
  storage1.addAssignPattern(3, "x", "yz*");

  storage1.addVariable("car");
  storage1.addVariable("wheel");
  storage1.addVariable("bike");

  storage1.addConst("6");
  storage1.addConst("8");

  REQUIRE(storage1.getRHSAssign(3) == "yz*");
}

TEST_CASE("get LHS out of range") {
  PKB storage1;
  storage1.addStatement(1, read, "ASample");
  storage1.addStatement(2, print, "ASample");
  storage1.addStatement(3, assign, "ASample");
  storage1.addStatement(4, call, "toyota");
  storage1.addCall(4, "toyota", "merc");
  storage1.addAssignPattern(3, "x", "yz*");

  REQUIRE(storage1.getLHSAssign(1).empty());
}

TEST_CASE("get LHS negative number") {
  PKB storage1;
  storage1.addStatement(1, read, "ASample");
  storage1.addStatement(2, print, "ASample");
  storage1.addStatement(3, assign, "ASample");
  storage1.addStatement(4, call, "toyota");
  storage1.addCall(4, "toyota", "merc");
  storage1.addAssignPattern(3, "x", "yz*");
  REQUIRE(storage1.getLHSAssign(-1).empty());
}

TEST_CASE("get RHS out of range") {
  PKB storage1;
  storage1.addStatement(1, read, "ASample");
  storage1.addStatement(2, print, "ASample");
  storage1.addStatement(3, assign, "ASample");
  storage1.addStatement(4, call, "toyota");
  storage1.addCall(4, "toyota", "merc");
  storage1.addAssignPattern(3, "x", "yz*");
  REQUIRE(storage1.getRHSAssign(1).empty());
}

TEST_CASE("get RHS negative number") {
  PKB storage1;
  storage1.addStatement(1, read, "ASample");
  storage1.addStatement(2, print, "ASample");
  storage1.addStatement(3, assign, "ASample");
  storage1.addStatement(4, call, "toyota");
  storage1.addCall(4, "toyota", "merc");
  storage1.addAssignPattern(3, "x", "yz*");
  REQUIRE(storage1.getRHSAssign(-1).empty());
}

TEST_CASE("get subString") {
  PKB storage1;
  storage1.addStatement(1, read, "ASample");
  storage1.addStatement(2, print, "ASample");
  storage1.addStatement(3, assign, "ASample");
  storage1.addStatement(4, call, "toyota");
  storage1.addCall(4, "toyota", "merc");
  storage1.addAssignPattern(3, "x", "yz* bc+");
  storage1.addSubString(3, "y");
  storage1.addSubString(3, "yz*");
  storage1.addSubString(3, "yz* bc+");
  REQUIRE(storage1.subStringsAt(3) == std::unordered_set<std::string>{"y", "yz*", "yz* bc+"});
}

TEST_CASE("negative get subString") {
  PKB storage1;
  storage1.addStatement(1, read, "ASample");
  storage1.addStatement(2, print, "ASample");
  storage1.addStatement(3, assign, "ASample");
  storage1.addStatement(4, call, "toyota");
  storage1.addCall(4, "toyota", "merc");
  storage1.addAssignPattern(3, "x", "yz* bc+");
  storage1.addSubString(3, "y");
  storage1.addSubString(3, "yz*");
  storage1.addSubString(3, "yz* bc+");
  REQUIRE(storage1.subStringsAt(4).empty());
}


