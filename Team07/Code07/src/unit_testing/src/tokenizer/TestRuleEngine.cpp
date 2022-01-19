
#include <regex>
#include "TokenType.h"
#include "RuleEngine.h"
#include "Rules.h"
#include "catch.hpp"

TEST_CASE("RuleEngineSanityCheck") {
  RuleList rules = {
      Rule(new std::regex("^\\d+$"), Number)
  };
  auto engine = new RuleEngine(rules);
  REQUIRE(engine->stringToToken("1243")->getType() == Number);
  delete engine;
}

TEST_CASE("RuleEngineFirstMatch") {
  RuleList rules = {
      Rule(new std::regex("^\\d+$"), Number),
      Rule(new std::regex("^[A-Za-z][A-Za-z0-9]*"), Variable)
  };
  auto engine = new RuleEngine(rules);
  REQUIRE(engine->stringToToken("NameFirst123 12345")->getType() == Variable);
  delete engine;
}

TEST_CASE("RuleEngineNoRules") {
  RuleList rules;
  auto engine = new RuleEngine(rules);
  auto token = engine->stringToToken("Anything here");
  REQUIRE(token->getType() == Error);
  REQUIRE(token->getValue() == "Anything here");
  delete engine;
}

TEST_CASE("RuleEngineNoMatch") {
  RuleList rules = {
      Rule(new std::regex("^\\d+$"), Number),
  };
  auto engine = new RuleEngine(rules);
  auto token = engine->stringToToken("Word here");
  REQUIRE(token->getType() == Error);
  delete engine;
}

TEST_CASE("RuleEnginePriorityMatch") {
  RuleList rules = {
      Rule(new std::regex("^\\d+$"), Variable),
      Rule(new std::regex("^\\d+$"), Number),
  };
  auto engine = new RuleEngine(rules);
  REQUIRE(engine->stringToToken("1234453")->getType() == Variable);
  delete engine;
}

TEST_CASE("RuleEngineMultipleMatch") {
  RuleList rules = {
      Rule(new std::regex("^[A-Za-z][A-Za-z0-9]*"), Variable),
      Rule(new std::regex("^\\d+"), Number)
  };

  auto engine = new RuleEngine(rules);
  auto token = engine->stringToToken("12345 StringHere");
  bool result;
  REQUIRE(token != nullptr);
  result = (token->getType() == Number) && (token->getValue() == "12345");
  REQUIRE(result);
  token = engine->stringToToken("StringHere 123412");
  REQUIRE(token != nullptr);
  result = token->getType() == Variable && token->getValue() == "StringHere";
  REQUIRE(result);
  delete engine;
}

TEST_CASE("Invalid Symbols") {
  RuleList rules = {
      Rule(new std::regex("^[A-Za-z][A-Za-z0-9]*"), Variable),
      Rule(new std::regex("^\\d+"), Number)
  };

  auto engine = new RuleEngine(rules);
  std::string testString = "~ :` hello world; procedure Hello {}";
  auto token = engine->stringToToken(testString);
  REQUIRE(token != nullptr);
  REQUIRE(token->getType() == Error);
  REQUIRE(token->getValue() == testString);
}

TEST_CASE("Unterminated Quotes") {
  auto engine = new RuleEngine(Rules::getPQLRules());
  std::string testString = "\" test";
  auto token = engine->stringToToken(testString);
  REQUIRE(token != nullptr);
  REQUIRE(token->getType() == Error);
  REQUIRE(token->getValue() == testString);
}
