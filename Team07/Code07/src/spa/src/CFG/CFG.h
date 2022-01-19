//
// Created by Kamil Gimik on 8/10/21.
//

#ifndef AUTOTESTER_TEAM07_CODE07_SRC_SPA_SRC_CFG_CFG_H_
#define AUTOTESTER_TEAM07_CODE07_SRC_SPA_SRC_CFG_CFG_H_

#include <queue>
#include <string>
#include <unordered_set>
#include <unordered_map>


class CFG {

 protected:

  std::unordered_map<int, std::unordered_set<int>> nextList;
  std::unordered_map<int, std::pair<std::string, std::unordered_set<std::string>>> assignList;
  std::unordered_map<int, std::unordered_set<std::string> > modifiesList; // include assign , read , call
  std::unordered_map<int, std::unordered_set<int>> nextStarCache;

  std::unordered_set<int> cachedCheck;

  std::unordered_map<int, std::unordered_set<int>> affectsCache;
  std::unordered_map<int, std::unordered_set<int>> affectsStarCache;
  std::unordered_set<int> terminals;


  void addToCache(int s1, int s2);
  bool bfsNextStar(int s1, int s2);
  bool bfsAffects(int s1, int s2);
  bool bfsAffectsStar(int s1, int s2);

 public:

  void setModifiesList(std::unordered_map<int, std::unordered_set<std::string>> modifiesList);
  void setAssignList(std::unordered_map<int, std::pair<std::string, std::unordered_set<std::string>>> assignList);
  bool hasBeenCached;
  explicit CFG(std::unordered_map<int, std::unordered_set<int>> nextList);

  virtual std::unordered_map<int, std::unordered_set<int>> getNextList();
  void resetCacheVar();

  // API BELOW

  // method when you provide s1 and s2
  bool isNextStar(int s1, int s2);
  bool isAffects(int s1, int s2);
  bool isAffectsStar(int s1, int s2);

  // provide s1 and get all s2
  std::unordered_set<int> getNextStarList(int stmt);
  std::unordered_set<int> getAffectsList(int stmt);
  std::unordered_set<int> getAffectsStarList(int stmt);

  // provide s1 and return true prematurely if exists s2
  bool hasNext(int stmt);
  bool hasAffects(int stmt);
  void clearCache();

};

#endif //AUTOTESTER_TEAM07_CODE07_SRC_SPA_SRC_CFG_CFG_H_
