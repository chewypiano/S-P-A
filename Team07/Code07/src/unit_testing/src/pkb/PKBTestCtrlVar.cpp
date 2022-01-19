//
// Created by Christopher Mervyn on 29/9/21.
//

#include "catch.hpp"
#include "PKB/PKB.h"

TEST_CASE("get if stmt") {
  PKB storage1;
  storage1.addStatement(1, read, "ASample");
  storage1.addStatement(2, print, "ASample");
  storage1.addStatement(3, assign, "ASample");
  storage1.addStatement(4, call, "toyota");
  storage1.addCall(4, "toyota", "merc");
  storage1.addAssignPattern(3, "x", "yz*");
  storage1.addIfStmt(5, 6, 0);
  storage1.addIfStmt(5, 7, 0);
  storage1.addIfStmt(5, 8, 0);
  storage1.addIfStmt(5, 9, 0);
  storage1.addIfCtrlVar(5, "x");
  REQUIRE(storage1.getAllIf() == std::unordered_set<int>{5});
}

TEST_CASE("negative get if stmt") {
  PKB storage1;
  storage1.addStatement(1, read, "ASample");
  storage1.addStatement(2, print, "ASample");
  storage1.addStatement(3, assign, "ASample");
  storage1.addStatement(4, call, "toyota");
  storage1.addCall(4, "toyota", "merc");
  storage1.addAssignPattern(3, "x", "yz*");
  storage1.addIfStmt(5, 6, 0);
  storage1.addIfStmt(5, 7, 0);
  storage1.addIfStmt(5, 8, 0);
  storage1.addIfStmt(5, 9, 0);
  REQUIRE(storage1.getAllIf().empty());
}

TEST_CASE("get while stmt") {
  PKB storage1;
  storage1.addStatement(1, read, "ASample");
  storage1.addStatement(2, print, "ASample");
  storage1.addStatement(3, assign, "ASample");
  storage1.addStatement(4, call, "toyota");
  storage1.addCall(4, "toyota", "merc");
  storage1.addAssignPattern(3, "x", "yz*");
  storage1.addIfStmt(5, 6, 0);
  storage1.addIfStmt(5, 7, 0);
  storage1.addIfStmt(5, 8, 0);
  storage1.addIfStmt(5, 9, 0);
  storage1.addIfStmt(5, 10, 0);
  storage1.addWhileStmt(7, 8);
  storage1.addWhileStmt(7, 9);
  storage1.addWhileCtrlVar(7, "y");
  storage1.addIfCtrlVar(5, "x");
  REQUIRE(storage1.getAllWhile() == std::unordered_set<int>{7});
}

TEST_CASE("negative get while stmt") {
  PKB storage1;
  storage1.addStatement(1, read, "ASample");
  storage1.addStatement(2, print, "ASample");
  storage1.addStatement(3, assign, "ASample");
  storage1.addStatement(4, call, "toyota");
  storage1.addCall(4, "toyota", "merc");
  storage1.addAssignPattern(3, "x", "yz*");
  storage1.addIfStmt(5, 6, 0);
  storage1.addIfStmt(5, 7, 0);
  storage1.addIfStmt(5, 8, 0);
  storage1.addIfStmt(5, 9, 0);
  storage1.addIfStmt(5, 10, 0);
  storage1.addWhileStmt(7, 8);
  storage1.addWhileStmt(7, 9);
  storage1.addIfCtrlVar(5, "x");
  REQUIRE(storage1.getAllWhile().empty());
}

TEST_CASE("get if var") {
  PKB storage1;
  storage1.addStatement(1, read, "ASample");
  storage1.addStatement(2, print, "ASample");
  storage1.addStatement(3, assign, "ASample");
  storage1.addStatement(4, call, "toyota");
  storage1.addCall(4, "toyota", "merc");
  storage1.addAssignPattern(3, "x", "yz*");
  storage1.addIfStmt(5, 6, 0);
  storage1.addIfStmt(5, 7, 0);
  storage1.addIfStmt(5, 8, 0);
  storage1.addIfStmt(5, 9, 0);
  storage1.addIfStmt(5, 10, 0);
  storage1.addWhileStmt(7, 8);
  storage1.addWhileStmt(7, 9);
  storage1.addWhileCtrlVar(7, "y");
  storage1.addIfCtrlVar(5, "x");
  storage1.addIfCtrlVar(10, "x");
  REQUIRE(storage1.getIfCtrlVar("x") == std::unordered_set<int>{5, 10});
}

TEST_CASE("negative get if var") {
  PKB storage1;
  storage1.addStatement(1, read, "ASample");
  storage1.addStatement(2, print, "ASample");
  storage1.addStatement(3, assign, "ASample");
  storage1.addStatement(4, call, "toyota");
  storage1.addCall(4, "toyota", "merc");
  storage1.addAssignPattern(3, "x", "yz*");
  storage1.addIfStmt(5, 6, 0);
  storage1.addIfStmt(5, 7, 0);
  storage1.addIfStmt(5, 8, 0);
  storage1.addIfStmt(5, 9, 0);
  storage1.addIfStmt(5, 10, 0);
  storage1.addWhileStmt(7, 8);
  storage1.addWhileStmt(7, 9);
  storage1.addWhileCtrlVar(7, "y");
  storage1.addIfCtrlVar(5, "x");
  storage1.addIfCtrlVar(10, "x");
  REQUIRE(storage1.getIfCtrlVar("z").empty());
}

TEST_CASE("get while var") {
  PKB storage1;
  storage1.addStatement(1, read, "ASample");
  storage1.addStatement(2, print, "ASample");
  storage1.addStatement(3, assign, "ASample");
  storage1.addStatement(4, call, "toyota");
  storage1.addCall(4, "toyota", "merc");
  storage1.addAssignPattern(3, "x", "yz*");
  storage1.addIfStmt(5, 6, 0);
  storage1.addIfStmt(5, 7, 0);
  storage1.addIfStmt(5, 8, 0);
  storage1.addIfStmt(5, 9, 0);
  storage1.addIfStmt(5, 10, 0);
  storage1.addWhileStmt(7, 8);
  storage1.addWhileStmt(7, 9);
  storage1.addWhileCtrlVar(7, "y");
  storage1.addWhileCtrlVar(8, "y");
  storage1.addIfCtrlVar(5, "x");
  REQUIRE(storage1.getWhileCtrlVar("y") == std::unordered_set<int>{7, 8});
}

TEST_CASE("negative get while var") {
  PKB storage1;
  storage1.addStatement(1, read, "ASample");
  storage1.addStatement(2, print, "ASample");
  storage1.addStatement(3, assign, "ASample");
  storage1.addStatement(4, call, "toyota");
  storage1.addCall(4, "toyota", "merc");
  storage1.addAssignPattern(3, "x", "yz*");
  storage1.addIfStmt(5, 6, 0);
  storage1.addIfStmt(5, 7, 0);
  storage1.addIfStmt(5, 8, 0);
  storage1.addIfStmt(5, 9, 0);
  storage1.addIfStmt(5, 10, 0);
  storage1.addWhileStmt(7, 10);
  storage1.addWhileCtrlVar(7, "y");
  storage1.addWhileCtrlVar(8, "y");
  storage1.addIfCtrlVar(5, "x");
  REQUIRE(storage1.getWhileCtrlVar("z").empty());
}

TEST_CASE("get while end stmt") {
  PKB storage1;
  storage1.addStatement(1, read, "ASample");
  storage1.addStatement(2, print, "ASample");
  storage1.addStatement(3, assign, "ASample");
  storage1.addStatement(4, call, "toyota");
  storage1.addCall(4, "toyota", "merc");
  storage1.addAssignPattern(3, "x", "yz*");
  storage1.addIfStmt(5, 6, 0);
  storage1.addIfStmt(5, 7, 0);
  storage1.addIfStmt(5, 8, 0);
  storage1.addIfStmt(5, 9, 0);
  storage1.addIfStmt(5, 10, 0);
  storage1.addWhileStmt(7, 10);
  storage1.addWhileCtrlVar(7, "y");
  storage1.addWhileCtrlVar(8, "y");
  storage1.addIfCtrlVar(5, "x");
  REQUIRE(storage1.getWhileEndStmt(7) == 10);
}

TEST_CASE("negative get while end stmt") {
  PKB storage1;
  storage1.addStatement(1, read, "ASample");
  storage1.addStatement(2, print, "ASample");
  storage1.addStatement(3, assign, "ASample");
  storage1.addStatement(4, call, "toyota");
  storage1.addCall(4, "toyota", "merc");
  storage1.addAssignPattern(3, "x", "yz*");
  storage1.addIfStmt(5, 6, 0);
  storage1.addIfStmt(5, 7, 0);
  storage1.addIfStmt(5, 8, 0);
  storage1.addIfStmt(5, 9, 0);
  storage1.addIfStmt(5, 10, 0);
  storage1.addWhileStmt(7, 10);
  storage1.addWhileCtrlVar(7, "y");
  storage1.addWhileCtrlVar(8, "y");
  storage1.addIfCtrlVar(5, "x");
  REQUIRE(storage1.getWhileEndStmt(8) == -1);
}

TEST_CASE("get else stmt") {
  PKB storage1;
  storage1.addStatement(1, read, "ASample");
  storage1.addStatement(2, print, "ASample");
  storage1.addStatement(3, assign, "ASample");
  storage1.addStatement(4, call, "toyota");
  storage1.addCall(4, "toyota", "merc");
  storage1.addAssignPattern(3, "x", "yz*");
  storage1.addIfStmt(5, 10, 7);
  storage1.addIfStmt(6, 9, 8);
  storage1.addWhileStmt(7, 10);
  storage1.addWhileCtrlVar(7, "y");
  storage1.addWhileCtrlVar(8, "y");
  storage1.addIfCtrlVar(5, "x");
  REQUIRE(storage1.getElseStmt(5) == 7);
}

TEST_CASE("negative get else stmt") {
  PKB storage1;
  storage1.addStatement(1, read, "ASample");
  storage1.addStatement(2, print, "ASample");
  storage1.addStatement(3, assign, "ASample");
  storage1.addStatement(4, call, "toyota");
  storage1.addCall(4, "toyota", "merc");
  storage1.addAssignPattern(3, "x", "yz*");
  storage1.addIfStmt(5, 10, 7);
  storage1.addIfStmt(6, 9, 8);
  storage1.addWhileStmt(7, 10);
  storage1.addWhileCtrlVar(7, "y");
  storage1.addWhileCtrlVar(8, "y");
  storage1.addIfCtrlVar(5, "x");
  REQUIRE(storage1.getElseStmt(7) == -1);
}

TEST_CASE("get if end stmt") {
  PKB storage1;
  storage1.addStatement(1, read, "ASample");
  storage1.addStatement(2, print, "ASample");
  storage1.addStatement(3, assign, "ASample");
  storage1.addStatement(4, call, "toyota");
  storage1.addCall(4, "toyota", "merc");
  storage1.addAssignPattern(3, "x", "yz*");
  storage1.addIfStmt(5, 10, 7);
  storage1.addIfStmt(6, 9, 8);
  storage1.addWhileStmt(7, 10);
  storage1.addWhileCtrlVar(7, "y");
  storage1.addWhileCtrlVar(8, "y");
  storage1.addIfCtrlVar(5, "x");
  REQUIRE(storage1.getIfEndStmt(5) == 10);
}

TEST_CASE("negative get if end stmt") {
  PKB storage1;
  storage1.addStatement(1, read, "ASample");
  storage1.addStatement(2, print, "ASample");
  storage1.addStatement(3, assign, "ASample");
  storage1.addStatement(4, call, "toyota");
  storage1.addCall(4, "toyota", "merc");
  storage1.addAssignPattern(3, "x", "yz*");
  storage1.addIfStmt(5, 10, 7);
  storage1.addIfStmt(6, 9, 8);
  storage1.addWhileStmt(7, 10);
  storage1.addWhileCtrlVar(7, "y");
  storage1.addWhileCtrlVar(8, "y");
  storage1.addIfCtrlVar(5, "x");
  REQUIRE(storage1.getIfEndStmt(7) == -1);
}