//
// Created by Sean Chan  on 1/11/21.
//

#ifndef AUTOTESTER_TEAM07_CODE07_SRC_SPA_SRC_CFG_CFGBIP_H_
#define AUTOTESTER_TEAM07_CODE07_SRC_SPA_SRC_CFG_CFGBIP_H_

#include "CFG.h"
#include <unordered_set>
#include <unordered_map>
#include <string>
#include <algorithm>

class CFGBip : public CFG {

 private:
  std::unordered_map<std::string, std::pair<int, std::unordered_set<int>>> procedureList;
  std::unordered_map<int, std::string> callList;
  std::unordered_map<std::string, int> procedureTerminals;
  std::unordered_map<int, std::unordered_set<int>> nextBipList;

 public:
  explicit CFGBip(std::unordered_map<int, std::unordered_set<int>> nextList);

  bool isNextBip(int s1, int s2);
  std::unordered_set<int> getNextBipList(int stmt);
  std::unordered_map<int, std::unordered_set<int>> getNextBipList();
  bool hasNextBip(int stmt);

  void setProcedureList(std::unordered_map<std::string, std::pair<int, std::unordered_set<int>>> procedureList);
  void setCallList(std::unordered_map<int, std::string> callList);
  void turnToBip();
  std::unordered_map<int, std::unordered_set<int>> getNextList() override;

};

#endif //AUTOTESTER_TEAM07_CODE07_SRC_SPA_SRC_CFG_CFGBIP_H_
