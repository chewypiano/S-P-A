//
// Created by Junhua on 28/8/2021.
//

#ifndef TEAM07_RULES_H
#define TEAM07_RULES_H

#include <regex>
#include <vector>
#include "TokenType.h"

typedef std::pair<std::regex *, TokenType> Rule;
typedef std::vector<Rule> RuleList;

class Rules {
  static RuleList pqlRules;
  static RuleList simpleRules;

 public:
  static RuleList getPQLRules();
  static RuleList getSimpleRules();

};

#endif //TEAM07_RULES_H