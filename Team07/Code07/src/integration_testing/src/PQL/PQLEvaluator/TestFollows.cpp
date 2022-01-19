//
// Created by Junhua on 6/11/2021.
//

#include "catch.hpp"
#include <vector>
#include <string>
#include "QueryEvaluator.h"
#include "../../Utils/TestUtils.h"

TEST_CASE("Test such that follows(a,a)") {
  std::string program = "assign a; Select a such that Follows(a,a)";
  auto pkb = new PKB();
  pkb->addProc("Unused");
  pkb->addStatement(1, assign, "Unused");
  pkb->addStatement(2, assign, "Unused");
  pkb->addStatement(3, assign, "Unused");
  pkb->addVariable("x");
  pkb->addFollow(1, 2);
  pkb->addFollow(2, 3);
  pkb->addFollowStar(1, 2);
  pkb->addFollowStar(2, 3);
  pkb->addFollowStar(1, 3);
  pkb->addUseStmt(1, "x");
  pkb->addModifyStmt(3, "x");
  pkb->addAssignPattern(3, "x", "x + 3");
  auto queryParser = new QueryParser(program);
  queryParser->parse();

  auto a = (queryParser->getQuery());
  REQUIRE(a.toString() != "Query(\n"
                          "  INVALID QUERY\n"
                          ")");

  auto evaluator = new QueryEvaluator(queryParser, pkb);
  auto expectedOutput = createRowList({});

  auto result = evaluator->evaluate()->getRows();
  REQUIRE(result.size() == expectedOutput.size());
  for (auto &r: expectedOutput) {
    REQUIRE(std::find(result.begin(), result.end(), r) != result.end());
  }
}