//
// Created by Christopher Mervyn on 7/9/21.
//


#include "ParentComponent.h"

ParentComponent::ParentComponent() {}

bool ParentComponent::addParent(int stmtParent, int stmtChild) {
  //Checks if the parent-child pair already exists
  if (!parentChildPair.emplace(std::pair<int, int>(stmtParent, stmtChild)).second) {
    return false;
  }

  if (!parentTable.emplace(stmtParent, std::unordered_set<int>{stmtChild}).second) {
    parentTable.at(stmtParent).emplace(stmtChild);
  }
  parentList.emplace(stmtParent);
  childList.emplace(stmtChild);
  return true;

}

bool ParentComponent::addParentStar(int stmtParent, int stmtChild) {
  //Checks if the parent-child pair already exists
  if (!parentStarPair.emplace(std::pair<int, int>(stmtParent, stmtChild)).second) {
    return false;
  }

  if (!parentStarTable.emplace(stmtParent, std::unordered_set<int>{stmtChild}).second) {
    parentStarTable.at(stmtParent).emplace(stmtChild);
  }

  if (!childStarTable.emplace(stmtChild, std::unordered_set<int>{stmtParent}).second) {
    childStarTable.at(stmtChild).emplace(stmtParent);
  }

  parentList.emplace(stmtParent);
  childList.emplace(stmtChild);
  return true;
}

bool ParentComponent::isParent(int stmtNum) {
  return parentList.find(stmtNum) != parentList.end();
}

bool ParentComponent::isChild(int stmtNum) {
  return childList.find(stmtNum) != childList.end();
}

int ParentComponent::getParent(int stmtNum) {
  if (parentChildPair.end() == find_if(parentChildPair.begin(), parentChildPair.end(),
                                       [&](const std::pair<int, int> &element) { return element.second == stmtNum; })) {
    return 0;
  }
  auto iter = *find_if(parentChildPair.begin(), parentChildPair.end(),
                       [&](const std::pair<int, int> &element) { return element.second == stmtNum; });
  return iter.first;

}

std::unordered_set<int> ParentComponent::getParentStar(int stmtNum) {
  if (childStarTable.find(stmtNum) != childStarTable.end()) {
    return childStarTable.at(stmtNum);
  }
  return {};
}

std::unordered_set<int> ParentComponent::getChild(int stmtNum) {
  if (parentTable.find(stmtNum) != parentTable.end()) {
    return parentTable.at(stmtNum);
  }
  return {};
}

std::unordered_set<int> ParentComponent::getChildStar(int stmtNum) {
  if (parentStarTable.find(stmtNum) != parentStarTable.end()) {
    return parentStarTable.at(stmtNum);
  }
  return {};
}

std::unordered_set<int> ParentComponent::getParentList() {
  return parentList;
}

std::unordered_set<int> ParentComponent::getChildList() {
  return childList;
}

std::unordered_map<int, std::unordered_set<int>> ParentComponent::getParentTable() {
  return parentTable;
}

std::unordered_map<int, std::unordered_set<int>> ParentComponent::getParentStarTable() {
  return parentStarTable;
}

bool ParentComponent::isParentPair(int s1, int s2) {
  for (auto kv: parentChildPair) {
    if (parentChildPair.find(std::pair<int, int>{s1, s2}) != parentChildPair.end()) {
      return true;
    }
  }
  return false;
}

bool ParentComponent::isParentStarPair(int s1, int s2) {
  for (auto kv: parentChildPair) {
    if (parentStarPair.find(std::pair<int, int>{s1, s2}) != parentStarPair.end()) {
      return true;
    }
  }
  return false;
}

void ParentComponent::clear() {
  parentTable.clear();
  parentChildPair.clear();
  parentStarPair.clear();
  parentList.clear();
  childList.clear();
  parentStarTable.clear();
}


