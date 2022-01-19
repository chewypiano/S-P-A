//
// Created by Christopher Mervyn on 8/10/21.
//

#include "CFGComponent.h"

CFGComponent::CFGComponent() {}

bool CFGComponent::addCFG(std::string procName, CFG *cfg) {
  if (CFGTable.find(procName) == CFGTable.end()) {
    CFGTable.emplace(procName, cfg);
    CFGList.emplace(cfg);
    return true;
  }
  return false;
}

CFG *CFGComponent::getCFG(std::string procName) {
  if (CFGTable.find(procName) != CFGTable.end()) {
    return CFGTable.at(procName);
  }
  return nullptr;
}

std::unordered_set<CFG *> CFGComponent::getCFGList() {
  return CFGList;
}

