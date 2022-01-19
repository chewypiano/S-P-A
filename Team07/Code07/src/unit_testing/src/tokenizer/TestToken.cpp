//
// Created by Junhua on 1/9/2021.
//

#include "Token.h"
#include "TokenType.h"
#include "catch.hpp"

TEST_CASE("Token get type method") {
  auto token = new Token(Variable, "Test");
  REQUIRE(token->getType() == Variable);
}

TEST_CASE("Token get value method") {
  auto token = new Token(Number, "1234");
  REQUIRE(token->getValue() == "1234");
}
