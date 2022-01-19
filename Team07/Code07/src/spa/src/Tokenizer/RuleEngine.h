//
// Created by Junhua on 28/8/2021.
//

#ifndef TEAM07_RULEENGINE_H
#define TEAM07_RULEENGINE_H

#include <regex>
#include <vector>
#include "Token.h"
#include "Rules.h"

class RuleEngine {
  RuleList currentRules;
 public:
  explicit RuleEngine(RuleList rules);
  Token *stringToToken(const std::string &input);
};

#endif //TEAM07_RULEENGINE_H
