//
// Created by Christopher Mervyn on 2/11/21.
//

#include "CFGBipComponent.h"

#include <system_error>


CFGBipComponent::CFGBipComponent() {
  CFGBipObject = nullptr;
}

bool CFGBipComponent::addCFGBip(CFGBip *cfgBip) {
  if (cfgBip != nullptr) {
    CFGBipObject = cfgBip;
    return true;
  }
  return false;
}

CFGBip *CFGBipComponent::getCFGBip() {
  if (CFGBipObject != nullptr) {
    return CFGBipObject;
  }
  throw std::invalid_argument("CFG not found");
}

void CFGBipComponent::clearCFGBipCache() {
  if(CFGBipObject != nullptr) {
    CFGBipObject->resetCacheVar();
  }
}
