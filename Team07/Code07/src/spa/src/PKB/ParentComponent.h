//
// Created by Christopher Mervyn on 7/9/21.
//

#ifndef AUTOTESTER_PARENTCOMPONENT_H
#define AUTOTESTER_PARENTCOMPONENT_H

#include <string>
#include <unordered_set>
#include <unordered_map>
#include <PKB/PairHasher.h>

class ParentComponent {
 public:
  ParentComponent();

  //Adds the relation Parent(stmtNum, stmtNum1) to the table
  //Returns false if it already exists in the table
  bool addParent(int stmtParent, int stmtChild);

  //Adds the relation Parent*(stmtParent, stmtChild) to the table
  //Returns false if it already exists in the table
  bool addParentStar(int stmtParent, int stmtChild);

  //Return true if the specified statement is a parent of another statement
  bool isParent(int stmtNum);

  //Return true if the specified statement is a child of another statement
  bool isChild(int stmtNum);

  //For Parent(s1, s2) relation, return the parent statement s1 of the specified statement
  int getParent(int stmtNum);

  //For Parent*(s1, s2), return the list of parent statements {s1} of the specified statement
  std::unordered_set<int> getParentStar(int stmtNum);

  //For Parent(s1, s2), return the child statement s2 of the specified statement
  std::unordered_set<int> getChild(int stmtNum);

  //For Parent*(s1, s2), return the child statement s2 of the specified statement
  std::unordered_set<int> getChildStar(int stmtNum);

  //Returns all parent statements
  std::unordered_set<int> getParentList();

  //Returns all child statements
  std::unordered_set<int> getChildList();

  //Returns a table of all the parent statements with their child
  std::unordered_map<int, std::unordered_set<int>> getParentTable();

  //Returns a table of all the parent* statements with their child
  std::unordered_map<int, std::unordered_set<int>> getParentStarTable();

  //Returns true if the parent pair exists
  bool isParentPair(int s1, int s2);

  //Returns true if the parent star pair exists
  bool isParentStarPair(int s1, int s2);

  //Clears the parentComponent
  void clear();

 private:
  std::unordered_map<int, std::unordered_set<int>> parentTable;
  std::unordered_set<std::pair<int, int>, std::hash_pair> parentChildPair;
  std::unordered_set<std::pair<int, int>, std::hash_pair> parentStarPair;
  std::unordered_set<int> parentList;
  std::unordered_set<int> childList;
  std::unordered_map<int, std::unordered_set<int>> parentStarTable;
  std::unordered_map<int, std::unordered_set<int>> childStarTable;

};

#endif //AUTOTESTER_PARENTCOMPONENT_H
