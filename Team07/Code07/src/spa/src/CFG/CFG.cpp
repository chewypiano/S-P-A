//
// Created by Kamil Gimik on 8/10/21.
//

#include "CFG.h"
#include <iostream>

CFG::CFG(std::unordered_map<int, std::unordered_set<int>> nextList) {
  this->nextList = nextList;
  this->nextStarCache = {};
  this->cachedCheck = {};
  this->hasBeenCached = false;
}

void CFG::addToCache(int s1, int s2) {
  hasBeenCached = true;
  if (this->nextStarCache.count(s1) == 0) {
    this->nextStarCache[s1] = {s2};
  } else {
    this->nextStarCache[s1].insert(s2);
  }
}

void CFG::setAssignList(std::unordered_map<int, std::pair<std::string, std::unordered_set<std::string>>> assignList) {
  this->assignList = assignList;
}

void CFG::setModifiesList(std::unordered_map<int, std::unordered_set<std::string>> modifiesList) {
  this->modifiesList = modifiesList;
}

void CFG::clearCache() {
  this->nextStarCache.clear();
  this->affectsCache.clear();
  this->affectsStarCache.clear();
}

void CFG::resetCacheVar() {
  if (this->hasBeenCached) {
    this->clearCache();
    this->hasBeenCached = false;
  }
}

std::unordered_map<int, std::unordered_set<int>> CFG::getNextList() {
  return this->nextList;
}

bool CFG::isNextStar(int s1, int s2) {
  bool isInNextStarCache = this->nextStarCache.count(s1) != 0 && this->nextStarCache[s1].count(s2) != 0;
  bool isInNextList = this->nextList.count(s1) != 0 && this->nextList[s1].count(s2) != 0;
  if (isInNextStarCache || isInNextList) {
    return true;
  }
  return this->bfsNextStar(s1, s2);
}

bool CFG::isAffects(int s1, int s2) {
  if (!isNextStar(s1, s2)) {
    return false;
  }
  // make use of cached
  bool isInAffectsCache = this->affectsCache.count(s1) != 0 && this->affectsCache[s1].count(s2) != 0;
  if (isInAffectsCache) {
    return true;
  }

  return this->bfsAffects(s1, s2);
}

bool CFG::isAffectsStar(int s1, int s2) {
  if (!isNextStar(s1, s2)) {
    return false;
  }
  // make use of cached
  bool isInAffectsStarCache = this->affectsStarCache.count(s1) != 0 && this->affectsStarCache[s1].count(s2) != 0;
  if (isInAffectsStarCache) {
    return true;
  }
  return this->bfsAffectsStar(s1, s2);
}
std::unordered_set<int> CFG::getNextStarList(int stmt) {
  this->bfsNextStar(stmt, 0);
  this->cachedCheck.insert(stmt);

  std::unordered_set<int> placeholder = {};
  for(int num:this->nextStarCache[stmt]){
    if(num>0){
      placeholder.insert(num);
    }
  }
  return placeholder;
}


std::unordered_set<int> CFG::getAffectsList(int stmt) {
  this->bfsAffects(stmt, 0);
  return this->affectsCache[stmt];
}

std::unordered_set<int> CFG::getAffectsStarList(int stmt) {
  this->bfsAffectsStar(stmt, 0);
  return this->affectsStarCache[stmt];
}


bool CFG::bfsNextStar(int s1, int s2) {
  std::unordered_set<int> visited;
  visited.insert(s1);
  std::queue<int> bfsQ;
  bfsQ.push(s1);
  while (!bfsQ.empty()) {
    int vertex = bfsQ.front();
    bfsQ.pop();
    if (this->nextList.count(vertex) == 0) {
      continue;
    }
    std::unordered_set<int> neighbourVertices = this->nextList[vertex];
    for (int neighbour: neighbourVertices) {
      this->addToCache(s1, neighbour);
      this->addToCache(vertex, neighbour);
      if (neighbour == s2) {
        return true;
      }
      if (visited.count(neighbour) != 0) {
        continue;
      }
      visited.insert(neighbour);
      bfsQ.push(neighbour);
    }
  }
  return false;
}

bool CFG::bfsAffects(int s1, int s2) {
  std::unordered_set<int> visited;
  visited.insert(s1);
  std::queue<int> bfsQ;
  bfsQ.push(s1);
  std::string modifiesOfS1 = this->assignList[s1].first;
  while (!bfsQ.empty()) {
    int vertex = bfsQ.front();
    bfsQ.pop();
    if (this->nextList.count(vertex) == 0) {
      continue;
    }
    std::unordered_set<int> neighbourVertices = this->nextList[vertex];

    for (int neighbour: neighbourVertices) {
      if (this->assignList[neighbour].second.count(modifiesOfS1) != 0) {
        if (this->affectsCache.count(s1) == 0) {
          affectsCache[s1] = {};
        }
        affectsCache[s1].insert(neighbour);
        // premature ending for true stmts
        if (s2 == -1) {
          return true;
        }
      }
      if (neighbour == s2) {
        if (this->assignList[s2].second.count(modifiesOfS1) != 0) {
          return true;
        }
      }
      if (visited.count(neighbour) != 0) {
        continue;
      }
      // checking if statement is modified on the way
      if (this->modifiesList.count(neighbour)) {
        if (this->modifiesList[neighbour].count(modifiesOfS1)) {
          continue;
        }
      }
      visited.insert(neighbour);
      bfsQ.push(neighbour);
    }
  }
  return false;

}

bool CFG::bfsAffectsStar(int s1, int s2) {
  // stores visited for current variable being modified
  std::unordered_map<std::string, std::unordered_set<int> > visited;
  if (this->assignList.count(s1) == 0) {
    return false;
  }
  // create pair to push to bfs node
  std::pair<int, std::string> startingPoint = {s1, this->assignList[s1].first};
  // assign to visited
  std::queue<std::pair<int, std::string>> bfsQ;
  bfsQ.push(startingPoint);
  while (!bfsQ.empty()) {
    std::pair<int, std::string> currVertex = bfsQ.front();
    int currStmtNum = currVertex.first;
    std::string lastModded = currVertex.second;
    bfsQ.pop();
    if (this->nextList.count(currVertex.first) == 0) {
      continue;
    }
    // stop when already visited
    if (visited.count(lastModded) != 0) {
      if (visited[lastModded].count(currStmtNum) != 0) { // already visited
        continue;
      }
    }
    // add to visited list
    if (visited.count(lastModded) == 0) {
      visited[lastModded] = {};
    }
    visited[lastModded].insert(currStmtNum);
    std::unordered_set<int> neighbourVertices = this->nextList[currStmtNum];
    // neighbour adding logic
    for (int neighbour: neighbourVertices) {
      // caching logic
      if (this->assignList[neighbour].second.count(lastModded) != 0) {
        if (this->affectsStarCache.count(s1) == 0) {
          affectsStarCache[s1] = {};
        }
        affectsStarCache[s1].insert(neighbour);
      }
      // last modded affects s2 , return true
      if (neighbour == s2) {
        if (this->assignList[s2].second.count(lastModded) != 0) {
          return true;
        }
      }
      // if its an assignment statement that gets affected, add extra node
      if (this->assignList.count(neighbour) != 0) { // assign statement
        if (assignList[neighbour].second.count(lastModded) != 0) { // neighbour is affected
          std::pair<int, std::string> toAdd = {neighbour, assignList[neighbour].first};
          bfsQ.push(toAdd);
        }
      }
      // dont add to q if it modifies last modified. ie continue
      if (this->modifiesList.count(neighbour) != 0) { // last stmt is modified here , short circuit
        // last modded is what neighbour modifies, dont add to q
        if (this->modifiesList[neighbour].count(lastModded) != 0) {
          continue;
        }
      }
      std::pair<int, std::string> toAddNeigh = {neighbour, lastModded};
      bfsQ.push(toAddNeigh);
    }
  }
  return false;
}

bool CFG::hasNext(int s1) {
  return this->nextList.count(s1) != 0 && this->nextList[s1].size() != 0;
}

bool CFG::hasAffects(int s1) {
  if (affectsCache.count(s1) != 0 && affectsCache[s1].size() != 0) {
    return true;
  }
  this->bfsAffects(s1, -1);
  return affectsCache.count(s1) != 0 && affectsCache[s1].size() != 0;
}

