//
// Created by Junhua on 6/11/2021.
//

#include <string>
#include "catch.hpp"
#include "PKB/PKB.h"
#include "QueryParser.h"
#include "QueryEvaluator.h"
#include "../../Utils/TestUtils.h"

TEST_CASE("Test basic Select assign statement") {
  std::string program = "assign a; Select a";
  auto pkb = new PKB();
  pkb->addProc("Unused");
  pkb->addStatement(1, assign, "Unused");
  pkb->addStatement(2, assign, "Unused");
  pkb->addVariable("x");
  pkb->addUseStmt(1, "x");
  pkb->addUseStmt(2, "x");

  const QuerySynonym pQuerySynonym = QuerySynonym(DesignEntityType::Assign, "a");

  RowList expectedOutput = createRowList(
      {
          {
              {pQuerySynonym, "1"},
          }, {
              {pQuerySynonym, "2"}
          }
      }
  );

  auto queryParser = new QueryParser(program);
  queryParser->parse();
  auto evaluator = new QueryEvaluator(queryParser, pkb);
  auto result = evaluator->evaluate()->getRows();
  REQUIRE(result.size() == expectedOutput.size());
  for (auto &r: expectedOutput) {
    REQUIRE(std::find(result.begin(), result.end(), r) != result.end());
  }
}

TEST_CASE("Test basic Select variable statement") {
  std::string program = "variable a; Select a";
  auto pkb = new PKB();
  pkb->addProc("Unused");
  pkb->addStatement(1, print, "Unused");
  pkb->addStatement(2, print, "Unused");
  pkb->addVariable("x");
  pkb->addUseStmt(1, "x");
  pkb->addUseStmt(2, "x");

  const QuerySynonym pQuerySynonym = QuerySynonym(DesignEntityType::Variable, "a");

  RowList expectedOutput = createRowList(
      {
          {
              {pQuerySynonym, "x"},
          }
      }
  );

  auto queryParser = new QueryParser(program);
  queryParser->parse();
  auto evaluator = new QueryEvaluator(queryParser, pkb);
  auto result = evaluator->evaluate()->getRows();
  REQUIRE(result.size() == expectedOutput.size());
  for (auto &r: expectedOutput) {
    REQUIRE(std::find(result.begin(), result.end(), r) != result.end());
  }

}

TEST_CASE("Test Select procedure") {
  std::string program = "procedure a; Select a";
  auto pkb = new PKB();
  pkb->addProc("Unused");
  pkb->addStatement(1, print, "Unused");
  pkb->addStatement(2, print, "Unused");
  pkb->addVariable("x");
  pkb->addUseStmt(1, "x");
  pkb->addUseStmt(2, "x");

  const QuerySynonym pQuerySynonym = QuerySynonym(DesignEntityType::Procedure, "a");

  RowList expectedOutput = createRowList(
      {
          {
              {pQuerySynonym, "Unused"},
          }
      }
  );

  auto queryParser = new QueryParser(program);
  queryParser->parse();
  auto evaluator = new QueryEvaluator(queryParser, pkb);
  auto result = evaluator->evaluate()->getRows();
  REQUIRE(result.size() == expectedOutput.size());
  for (auto &r: expectedOutput) {
    REQUIRE(std::find(result.begin(), result.end(), r) != result.end());
  }
}

TEST_CASE("Test invalid query no semicolon") {
  std::string program = "assign a Select a";
  auto pkb = new PKB();
  pkb->addProc("Unused");
  pkb->addStatement(1, assign, "Unused");
  pkb->addStatement(2, assign, "Unused");
  pkb->addVariable("x");
  pkb->addUseStmt(1, "x");
  pkb->addUseStmt(2, "x");

  auto queryParser = new QueryParser(program);
  queryParser->parse();
  auto evaluator = new QueryEvaluator(queryParser, pkb);
  RowList expectedOutput = {};

  auto result = evaluator->evaluate()->getRows();
  REQUIRE(result.size() == expectedOutput.size());
  for (auto &r: expectedOutput) {
    REQUIRE(std::find(result.begin(), result.end(), r) != result.end());
  }
}

TEST_CASE("Test Select procedure v2ss") {
  std::string program = "call a; Select a";
  auto pkb = new PKB();
  pkb->addProc("Unused");
  pkb->addStatement(1, print, "Unused");
  pkb->addStatement(2, print, "Unused");
  pkb->addVariable("x");
  pkb->addUseStmt(1, "x");
  pkb->addUseStmt(2, "x");

  RowList expectedOutput = createRowList(
      {}
  );

  auto queryParser = new QueryParser(program);
  queryParser->parse();
  auto evaluator = new QueryEvaluator(queryParser, pkb);
  REQUIRE(evaluator->evaluate()->getRows() == expectedOutput);
}