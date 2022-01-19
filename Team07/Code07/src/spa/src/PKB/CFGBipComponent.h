//
// Created by Christopher Mervyn on 2/11/21.
//

#ifndef AUTOTESTER_TEAM07_CODE07_SRC_SPA_SRC_PKB_CFGBIPCOMPONENT_H_
#define AUTOTESTER_TEAM07_CODE07_SRC_SPA_SRC_PKB_CFGBIPCOMPONENT_H_

#include <string>
#include <unordered_set>
#include <unordered_map>
#include <PKB/PairHasher.h>
#include <CFGBip.h>

class CFGBipComponent {
 public:
  CFGBipComponent();

  bool addCFGBip(CFGBip *cfgBip);

  CFGBip *getCFGBip();

  std::unordered_set<CFGBip *> getCFGBipList();

  void clearCFGBipCache();

  CFGBip *CFGBipObject;


};

#endif //AUTOTESTER_TEAM07_CODE07_SRC_SPA_SRC_PKB_CFGBIPCOMPONENT_H_
