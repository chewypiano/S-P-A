//
// Created by Junhua on 6/9/2021.
//


#include <regex>
#include "catch.hpp"
#include "Utils/string_utils.h"
#include "Rules.h"

TEST_CASE("Check unterminated quotes with regex") {
  auto reg = new std::regex("(\".*\")");
  std::string res = reMatch(reg, "\" test");
  REQUIRE(res.empty());
  delete reg;
}

TEST_CASE("Check different rules for same string") {
  std::string line = ";";
  auto rules = Rules::getPQLRules();
  for (auto &rule: rules) {
    auto result = reMatch(rule.first, line);
    if (result.empty()) {
      continue;
    }
    REQUIRE(result == ";");
  }
}
