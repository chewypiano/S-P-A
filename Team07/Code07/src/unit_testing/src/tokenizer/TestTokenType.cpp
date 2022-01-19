//
// Created by Junhua on 1/9/2021.
//

#include "TokenType.h"
#include "catch.hpp"
#include <sstream>

#define REQUIRE_MESSAGE(cond, msg) do { INFO(msg); REQUIRE(cond); } while((void)0, 0)

TEST_CASE("Test all token type has conversion") {
  for (int i = LBrace; i <= Negate; ++i) {
    auto type = static_cast<TokenType>(i);

    std::ostringstream out;
    out << "Token number: " << i;

    REQUIRE_MESSAGE(
        !TokenToName::tokenToName(type).empty(),
        out.str()
    );
  }
}