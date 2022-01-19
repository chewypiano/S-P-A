//
// Created by Christopher Mervyn on 8/10/21.
//

#ifndef AUTOTESTER_CFGCOMPONENT_H
#define AUTOTESTER_CFGCOMPONENT_H

#include <string>
#include <unordered_set>
#include <unordered_map>
#include <PKB/PairHasher.h>
#include <CFG.h>

class CFGComponent {
 public:
  CFGComponent();

  bool addCFG(std::string procName, CFG *cfg);

  CFG *getCFG(std::string procName);

  std::unordered_set<CFG *> getCFGList();

  std::unordered_map<std::string, CFG *> CFGTable;

  std::unordered_set<CFG *> CFGList;

};

#endif //AUTOTESTER_CFGCOMPONENT_H
