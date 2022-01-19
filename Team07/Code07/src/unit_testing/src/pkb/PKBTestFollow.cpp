//
// Created by Christopher Mervyn on 29/9/21.
//

#include "catch.hpp"
#include "PKB/PKB.h"

TEST_CASE("isFollowing") {
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

  REQUIRE(storage.isFollowing(2) == true);
}

TEST_CASE("negative isFollowing") {
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

  REQUIRE(storage.isFollowing(1) == false);
}

TEST_CASE("isBeingFollowed") {
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

  REQUIRE(storage.isBeingFollowed(1) == true);
}

TEST_CASE("negative isBeingFollowed") {
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

  REQUIRE(storage.isBeingFollowed(4) == false);
}

TEST_CASE("getFollower") {
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

  REQUIRE(storage.getFollower(1) == 2);
}
TEST_CASE("negative getFollower") {
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

  REQUIRE(storage.getFollower(4) == 0);
}

TEST_CASE("getFollowedBy") {
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

  REQUIRE(storage.getFollowedBy(4) == 3);
}

TEST_CASE("negative getFollowedBy") {
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

  REQUIRE(storage.getFollowedBy(1) == 0);
}

TEST_CASE("getFollowerStar") {
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

  REQUIRE(storage.getFollowerStar(1) == std::unordered_set<int>{3, 4});
}

TEST_CASE("getFollowedByStar") {
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
  storage.addFollowStar(2, 3);

  REQUIRE(storage.getFollowedByStar(3) == std::unordered_set<int>{1, 2});
}

TEST_CASE("getAllFollowers") {
  PKB storage;
  storage.addFollow(1, 2);
  storage.addFollow(2, 3);
  storage.addFollow(3, 4);
  storage.addFollowStar(1, 3);
  storage.addFollowStar(1, 4);
  storage.addFollowStar(2, 3);

  REQUIRE(storage.getAllFollowers() == std::unordered_set<int>{2, 3, 4});
}

TEST_CASE("getAllFollowedBy") {
  PKB storage;
  storage.addFollow(1, 2);
  storage.addFollow(2, 3);
  storage.addFollow(3, 4);
  storage.addFollowStar(1, 3);
  storage.addFollowStar(1, 4);
  storage.addFollowStar(2, 3);

  REQUIRE(storage.getAllFollowedBy() == std::unordered_set<int>{1, 2, 3});
}

TEST_CASE("add follow star relation") {
  PKB storage;
  storage.addStatement(1, read, "ASample");
  storage.addStatement(2, print, "ASample");
  storage.addStatement(3, print, "ASample");
  storage.addFollowStar(1, 2);
  storage.addFollowStar(1, 3);
  storage.addFollowStar(2, 3);
  storage.addFollow(1, 2);
  storage.addFollow(2, 3);
  REQUIRE(storage.getFollowedByStar(2) == std::unordered_set<int>{1});
  REQUIRE(storage.getFollowedByStar(3) == std::unordered_set<int>{1, 2});
}

TEST_CASE("negative add follow star relation") {
  PKB storage;
  storage.addStatement(1, read, "ASample");
  storage.addStatement(2, print, "ASample");
  storage.addStatement(3, print, "ASample");
  storage.addFollowStar(1, 2);
  storage.addFollowStar(1, 3);
  storage.addFollowStar(2, 3);
  storage.addFollow(1, 2);
  storage.addFollow(2, 3);
  REQUIRE(storage.getFollowedByStar(1).empty());
}

TEST_CASE("isFollow pair") {
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
  REQUIRE(storage.isFollow(2, 3) == true);
}

TEST_CASE("negative isFollow pair") {
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
  REQUIRE(storage.isFollow(4, 5) == false);
}

TEST_CASE("isFollowStar pair") {
  PKB storage;
  storage.addStatement(1, read, "ASample");
  storage.addModifyStmt(1, "x");
  storage.addStatement(2, print, "ASample");
  storage.addUseStmt(2, "y");
  storage.addStatement(3, assign, "ASample");
  storage.addStatement(4, read, "BSample");
  storage.addStatement(5, print, "CSample");
  storage.addFollow(1, 2);
  storage.addFollow(2, 3);
  storage.addFollow(3, 4);
  storage.addFollow(4, 5);

  storage.addFollowStar(1, 3);
  storage.addFollowStar(1, 4);
  storage.addFollowStar(1, 5);
  REQUIRE(storage.isFollowStar(1, 3) == true);
}

TEST_CASE("negative isFollowStar pair") {
  PKB storage;
  storage.addStatement(1, read, "ASample");
  storage.addModifyStmt(1, "x");
  storage.addStatement(2, print, "ASample");
  storage.addUseStmt(2, "y");
  storage.addStatement(3, assign, "ASample");
  storage.addStatement(4, read, "BSample");
  storage.addStatement(5, print, "CSample");
  storage.addFollow(1, 2);
  storage.addFollow(2, 3);
  storage.addFollow(3, 4);
  storage.addFollow(4, 5);

  storage.addFollowStar(1, 3);
  storage.addFollowStar(1, 4);
  storage.addFollowStar(1, 5);
  REQUIRE(storage.isFollowStar(2, 6) == false);
}