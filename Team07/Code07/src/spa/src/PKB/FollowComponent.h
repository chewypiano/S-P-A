//
// Created by Christopher Mervyn on 8/9/21.
//

#ifndef AUTOTESTER_FOLLOWCOMPONENT_H
#define AUTOTESTER_FOLLOWCOMPONENT_H

#include <string>
#include <unordered_set>
#include <unordered_map>
#include <PKB/PairHasher.h>

class FollowComponent {
 public:
  FollowComponent();

  //Adds the relation Follow(stmtNum, stmtNum1) to the table
  //Returns false if it already exists in the table
  bool addFollow(int stmtParent, int stmtChild);

  //Adds the relation Follow*(stmtParent, stmtChild) to the table
  //Returns false if it already exists in the table
  bool addFollowStar(int stmtParent, int stmtChild);

  //Checks if the stmtNum is being directly followed
  bool isBeingFollowed(int stmtNum);

  //Checks if the stmtNum directly follows another statement
  bool isFollowing(int stmtNum);

  //For Follow(s1, s2) relation, return the followed by statement s1 of the specified statement
  int getFollowedBy(int s2);

  //For Follow(s1, s2) relation, return the followed statement s2 of the specified statement
  int getFollower(int s1);

  //For Follow*(s1, s2), return the list of following statements {s2} of the specified statement
  std::unordered_set<int> getFollowerStar(int s1);

  //For Follow*(s1, s2), return the followed by statement {s1} of the specified statement
  std::unordered_set<int> getFollowedByStar(int s2);

  //Returns a list of all statements that follows another statement
  std::unordered_set<int> getAllFollowers();

  //Returns a list of all statements that is followed by another statement
  std::unordered_set<int> getAllFollowedBy();

  //Checks if the relation Follow(s1,s2) exists
  bool isFollow(int s1, int s2);

  //Checks if the relation Follows*(s1,s2) exists
  bool isFollowStar(int s1, int s2);

  //Clears the followComponent
  void clear();

 private:

  std::unordered_set<std::pair<int, int>, std::hash_pair> followPair;
  std::unordered_set<std::pair<int, int>, std::hash_pair> followStarPair;
  std::unordered_map<int, int> followByTable;
  std::unordered_map<int, int> followTable;
  std::unordered_map<int, std::unordered_set<int>> followStarTable;
  std::unordered_map<int, std::unordered_set<int>> followedByStarTable;

};

#endif //AUTOTESTER_FOLLOWCOMPONENT_H
