//
// Created by Junhua on 28/8/2021.
//

#include <string>
#include <utility>
#include "RuleEngine.h"
#include "Utils/string_utils.h"

RuleEngine::RuleEngine(RuleList rules) {
  this->currentRules = std::move(rules);
}

Token *RuleEngine::stringToToken(const std::string &input) {
  // Match to the different rules and see which one it fits
  for (auto &currentRule: currentRules) {
    std::string value = reMatch(currentRule.first, input);
    if (value.empty()) {
      continue;
    }
    return new Token(currentRule.second, value);
  }
  return new Token(Error, input);
}
