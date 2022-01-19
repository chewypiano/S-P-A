//
// Created by Christopher Mervyn on 8/9/21.
//



#include "FollowComponent.h"

FollowComponent::FollowComponent() {}

bool FollowComponent::addFollow(int stmtParent, int stmtChild) {

  //Checks if the follow relation already exists
  if (!followPair.emplace(std::pair<int, int>(stmtParent, stmtChild)).second) {
    return false;
  }
  followTable.emplace(stmtParent, stmtChild);
  followByTable.emplace(stmtChild, stmtParent);
  return true;
}

bool FollowComponent::addFollowStar(int stmtParent, int stmtChild) {

  //Checks if the follow star relation already exists
  if (!followStarPair.emplace(std::pair<int, int>(stmtParent, stmtChild)).second) {
    return false;
  }

  if (!followStarTable.emplace(stmtParent, std::unordered_set<int>{stmtChild}).second) {
    followStarTable.at(stmtParent).emplace(stmtChild);
  }

  if (!followedByStarTable.emplace(stmtChild, std::unordered_set<int>{stmtParent}).second) {
    followedByStarTable.at(stmtChild).emplace(stmtParent);
  }
  return true;
}

bool FollowComponent::isBeingFollowed(int stmtNum) {
  return followByTable.find(stmtNum) != followByTable.end();
}

bool FollowComponent::isFollowing(int stmtNum) {
  return followTable.find(stmtNum) != followTable.end();
}

int FollowComponent::getFollower(int s1) {
  if (this->isFollowing(s1)) {
    return followTable.at(s1);
  }
  return 0;
}

int FollowComponent::getFollowedBy(int s2) {
  if (this->isBeingFollowed(s2)) {
    return followByTable.at(s2);
  }
  return 0;
}

std::unordered_set<int> FollowComponent::getFollowerStar(int s1) {
  if (followStarTable.find(s1) != followStarTable.end()) {
    return followStarTable.at(s1);
  }
  return {};
}

std::unordered_set<int> FollowComponent::getFollowedByStar(int s2) {
  if (followedByStarTable.find(s2) != followedByStarTable.end()) {
    return followedByStarTable.at(s2);
  }
  return {};
}

std::unordered_set<int> FollowComponent::getAllFollowers() {
  std::unordered_set<int> followerList;
  for (auto kv: followByTable) {
    followerList.emplace(kv.first);
  }
  return followerList;
}

std::unordered_set<int> FollowComponent::getAllFollowedBy() {
  std::unordered_set<int> followedByList;
  for (auto kv: followTable) {
    followedByList.emplace(kv.first);
  }
  return followedByList;
}

bool FollowComponent::isFollow(int s1, int s2) {
  for (auto kv: followPair) {
    if (followPair.find(std::pair<int, int>{s1, s2}) != followPair.end()) {
      return true;
    }
  }
  return false;
}

bool FollowComponent::isFollowStar(int s1, int s2) {
  for (auto kv: followStarPair) {
    if (followStarPair.find(std::pair<int, int>{s1, s2}) != followStarPair.end()) {
      return true;
    }
  }
  return false;
}

void FollowComponent::clear() {
  followPair.clear();
  followStarPair.clear();
  followByTable.clear();
  followTable.clear();
  followStarTable.clear();
  followedByStarTable.clear();
}





