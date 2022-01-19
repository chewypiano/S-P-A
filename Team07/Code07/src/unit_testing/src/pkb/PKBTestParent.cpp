//
// Created by Christopher Mervyn on 29/9/21.
//

#include "catch.hpp"
#include "PKB/PKB.h"

TEST_CASE("isParentStmt") {
  PKB storage;
  storage.addStatement(1, read, "ASample");
  storage.addModifyStmt(1, "x");
  storage.addStatement(2, print, "ASample");
  storage.addUseStmt(2, "y");
  storage.addStatement(3, assign, "ASample");
  storage.addStatement(4, read, "BSample");
  storage.addParent(1, 2);
  storage.addParent(2, 3);
  storage.addParent(3, 4);
  storage.addParentStar(1, 3);
  storage.addParentStar(1, 4);

  REQUIRE(storage.isParent(3) == true);
}

TEST_CASE("negative isParentStmt") {
  PKB storage;
  storage.addStatement(1, whileStmt, "ASample");
  storage.addModifyStmt(1, "x");
  storage.addStatement(2, ifStmt, "ASample");
  storage.addUseStmt(2, "y");
  storage.addStatement(3, ifStmt, "ASample");
  storage.addStatement(4, read, "ASample");
  storage.addParent(1, 2);
  storage.addParent(2, 3);
  storage.addParent(3, 4);
  storage.addParentStar(1, 3);
  storage.addParentStar(1, 4);

  REQUIRE(storage.isParent(4) == false);
}

TEST_CASE("isChildStmt") {
  PKB storage;
  storage.addStatement(1, read, "ASample");
  storage.addModifyStmt(1, "x");
  storage.addStatement(2, print, "ASample");
  storage.addUseStmt(2, "y");
  storage.addStatement(3, assign, "ASample");
  storage.addStatement(4, read, "BSample");
  storage.addParent(1, 2);
  storage.addParent(2, 3);
  storage.addParent(3, 4);
  storage.addParentStar(1, 3);
  storage.addParentStar(1, 4);

  REQUIRE(storage.isChild(3) == true);
}

TEST_CASE("negative isChildStmt") {
  PKB storage;
  storage.addStatement(1, read, "ASample");
  storage.addModifyStmt(1, "x");
  storage.addStatement(2, print, "ASample");
  storage.addUseStmt(2, "y");
  storage.addStatement(3, assign, "ASample");
  storage.addStatement(4, read, "BSample");
  storage.addParent(1, 2);
  storage.addParent(2, 3);
  storage.addParent(3, 4);
  storage.addParentStar(1, 3);
  storage.addParentStar(1, 4);

  REQUIRE(storage.isChild(5) == false);
}

TEST_CASE("getParentStmt") {
  PKB storage;
  storage.addStatement(1, read, "ASample");
  storage.addModifyStmt(1, "x");
  storage.addStatement(2, print, "ASample");
  storage.addUseStmt(2, "y");
  storage.addStatement(3, assign, "ASample");
  storage.addStatement(4, read, "BSample");
  storage.addStatement(5, print, "CSample");
  storage.addParent(1, 2);
  storage.addParent(2, 3);
  storage.addParent(2, 5);
  storage.addParent(3, 4);
  storage.addParentStar(1, 3);
  storage.addParentStar(1, 4);

  REQUIRE(storage.getParent(3) == 2);
}

TEST_CASE("negative getParentStmt") {
  PKB storage;
  storage.addStatement(1, read, "ASample");
  storage.addModifyStmt(1, "x");
  storage.addStatement(2, print, "ASample");
  storage.addUseStmt(2, "y");
  storage.addStatement(3, assign, "ASample");
  storage.addStatement(4, read, "BSample");
  storage.addStatement(5, print, "CSample");
  storage.addParent(1, 2);
  storage.addParent(2, 3);
  storage.addParent(2, 5);
  storage.addParent(3, 4);
  storage.addParentStar(1, 3);
  storage.addParentStar(1, 4);

  REQUIRE(storage.getParent(1) == 0);
}

TEST_CASE("getChildList") {
  PKB storage;
  storage.addStatement(1, read, "ASample");
  storage.addModifyStmt(1, "x");
  storage.addStatement(2, print, "ASample");
  storage.addUseStmt(2, "y");
  storage.addStatement(3, assign, "ASample");
  storage.addStatement(4, read, "BSample");
  storage.addStatement(5, print, "CSample");
  storage.addParent(1, 2);
  storage.addParent(2, 3);
  storage.addParent(2, 5);
  storage.addParent(3, 4);
  storage.addParentStar(1, 3);
  storage.addParentStar(1, 4);

  REQUIRE(storage.getChild(2) == std::unordered_set<int>{3, 5});
}

TEST_CASE("negative getChildList") {
  PKB storage;
  storage.addStatement(1, read, "ASample");
  storage.addModifyStmt(1, "x");
  storage.addStatement(2, print, "ASample");
  storage.addUseStmt(2, "y");
  storage.addStatement(3, assign, "ASample");
  storage.addStatement(4, read, "BSample");
  storage.addStatement(5, print, "CSample");
  storage.addParent(1, 2);
  storage.addParent(2, 3);
  storage.addParent(2, 5);
  storage.addParent(3, 4);
  storage.addParentStar(1, 3);
  storage.addParentStar(1, 4);

  REQUIRE(storage.getChild(4).empty());
}

TEST_CASE("getParentStar") {
  PKB storage;
  storage.addStatement(1, read, "ASample");
  storage.addModifyStmt(1, "x");
  storage.addStatement(2, print, "ASample");
  storage.addUseStmt(2, "y");
  storage.addStatement(3, assign, "ASample");
  storage.addStatement(4, read, "BSample");
  storage.addStatement(5, print, "CSample");
  storage.addParent(1, 2);
  storage.addParent(2, 3);
  storage.addParent(2, 5);
  storage.addParent(3, 4);
  storage.addParentStar(1, 4);
  storage.addParentStar(2, 4);

  REQUIRE(storage.getParentStar(4) == std::unordered_set<int>{1, 2});
}

TEST_CASE("negative getParentStar") {
  PKB storage;
  storage.addStatement(1, read, "ASample");
  storage.addModifyStmt(1, "x");
  storage.addStatement(2, print, "ASample");
  storage.addUseStmt(2, "y");
  storage.addStatement(3, assign, "ASample");
  storage.addStatement(4, read, "BSample");
  storage.addStatement(5, print, "CSample");
  storage.addParent(1, 2);
  storage.addParent(2, 3);
  storage.addParent(2, 5);
  storage.addParent(3, 4);
  storage.addParentStar(1, 4);
  storage.addParentStar(2, 4);

  REQUIRE(storage.getParentStar(1).empty());
}

TEST_CASE("add parent star relation") {
  PKB storage;
  storage.addStatement(1, read, "ASample");
  storage.addStatement(2, print, "ASample");
  storage.addStatement(3, print, "ASample");
  storage.addParentStar(1, 2);
  storage.addParentStar(1, 3);
  storage.addParentStar(2, 3);
  storage.addParent(1, 2);
  storage.addParent(2, 3);
  REQUIRE(storage.getParentStar(2) == std::unordered_set<int>{1});
  REQUIRE(storage.getParentStar(3) == std::unordered_set<int>{1, 2});
}

TEST_CASE("negative add parents star relation") {
  PKB storage;
  storage.addStatement(1, read, "ASample");
  storage.addStatement(2, print, "ASample");
  storage.addStatement(3, print, "ASample");
  storage.addParent(1, 2);
  storage.addParent(2, 3);
  REQUIRE(storage.getParentStar(2).empty());
  REQUIRE(storage.getParentStar(3).empty());
}

TEST_CASE("isParentPair") {
  PKB storage;
  storage.addStatement(1, read, "ASample");
  storage.addModifyStmt(1, "x");
  storage.addStatement(2, print, "ASample");
  storage.addUseStmt(2, "y");
  storage.addStatement(3, assign, "ASample");
  storage.addStatement(4, read, "BSample");
  storage.addStatement(5, print, "CSample");
  storage.addParent(1, 2);
  storage.addParent(2, 3);
  storage.addParent(2, 5);
  storage.addParent(3, 4);
  storage.addParentStar(1, 3);
  storage.addParentStar(1, 4);
  storage.addFollow(1, 2);
  storage.addFollow(2, 3);
  storage.addFollow(3, 4);
  storage.addFollowStar(1, 3);
  storage.addFollowStar(1, 4);
  REQUIRE(storage.isParentPair(1, 2) == true);
}

TEST_CASE("negative isParentPair") {
  PKB storage;
  storage.addStatement(1, read, "ASample");
  storage.addModifyStmt(1, "x");
  storage.addStatement(2, print, "ASample");
  storage.addUseStmt(2, "y");
  storage.addStatement(3, assign, "ASample");
  storage.addStatement(4, read, "BSample");
  storage.addStatement(5, print, "CSample");
  storage.addParent(1, 2);
  storage.addParent(2, 3);
  storage.addParent(2, 5);
  storage.addParent(3, 4);
  storage.addParentStar(1, 3);
  storage.addParentStar(1, 4);
  storage.addFollow(1, 2);
  storage.addFollow(2, 3);
  storage.addFollow(3, 4);
  storage.addFollowStar(1, 3);
  storage.addFollowStar(1, 4);
  REQUIRE(storage.isParentPair(1, 3) == false);
}

TEST_CASE("isParentStar pair") {
  PKB storage;
  storage.addStatement(1, read, "ASample");
  storage.addModifyStmt(1, "x");
  storage.addStatement(2, print, "ASample");
  storage.addUseStmt(2, "y");
  storage.addStatement(3, assign, "ASample");
  storage.addStatement(4, read, "BSample");
  storage.addStatement(5, print, "CSample");
  storage.addParent(1, 2);
  storage.addParent(2, 3);
  storage.addParent(2, 5);
  storage.addParent(3, 4);
  storage.addParentStar(1, 3);
  storage.addParentStar(1, 4);
  storage.addFollow(1, 2);
  storage.addFollow(2, 3);
  storage.addFollow(3, 4);
  storage.addFollowStar(1, 3);
  storage.addFollowStar(1, 4);
  REQUIRE(storage.isParentStarPair(1, 3) == true);
}

TEST_CASE("negative isParentStar pair") {
  PKB storage;
  storage.addStatement(1, read, "ASample");
  storage.addModifyStmt(1, "x");
  storage.addStatement(2, print, "ASample");
  storage.addUseStmt(2, "y");
  storage.addStatement(3, assign, "ASample");
  storage.addStatement(4, read, "BSample");
  storage.addStatement(5, print, "CSample");
  storage.addParent(1, 2);
  storage.addParent(2, 3);
  storage.addParent(2, 5);
  storage.addParent(3, 4);
  storage.addParentStar(1, 3);
  storage.addParentStar(1, 4);
  storage.addFollow(1, 2);
  storage.addFollow(2, 3);
  storage.addFollow(3, 4);
  storage.addFollowStar(1, 3);
  storage.addFollowStar(1, 4);
  REQUIRE(storage.isParentStarPair(1, 5) == false);
}



