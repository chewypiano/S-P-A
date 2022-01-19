//
// Created by Junhua on 6/11/2021.
//

#include "catch.hpp"
#include <vector>
#include <string>
#include "QueryEvaluator.h"
#include "../../Utils/TestUtils.h"

PKB *createCallsPKB() {
  auto pkb = new PKB();

  // proc eg
  pkb->addProc("eg");

  // call eg2
  pkb->addStatement(1, call, "eg");
  pkb->addCall(1, "eg", "eg2");

  // proc eg2
  pkb->addProc("eg2");

  // call eg3
  pkb->addStatement(2, call, "eg2");
  pkb->addCall(2, "eg2", "eg3");

  //proc eg3
  pkb->addProc("eg3");

  // read a
  pkb->addStatement(3, read, "eg3");
  pkb->addModifyProc("eg3", "a");
  pkb->addModifyStmt(3, "a");
  return pkb;
}

PKB *createNoCallsPKB() {
  auto pkb = new PKB();

  // proc eg
  pkb->addProc("eg");

  // read eg2
  pkb->addStatement(1, read, "eg");
  pkb->addModifyProc("eg", "eg");
  pkb->addModifyStmt(1, "eg");

  // proc eg2
  pkb->addProc("eg2");

  // call eg3
  pkb->addStatement(2, read, "eg2");
  pkb->addModifyProc("eg2", "eg2");
  pkb->addModifyStmt(3, "eg2");

  //proc eg3
  pkb->addProc("eg3");

  // read a
  pkb->addStatement(3, read, "eg3");
  pkb->addModifyProc("eg3", "a");
  pkb->addModifyStmt(3, "a");

  return pkb;
}

TEST_CASE("Calls both const positive test") {
  std::string program = R"(Select BOOLEAN such that Calls("eg", "eg2"))";
  auto queryParser = new QueryParser(program);

  queryParser->parse();
  auto a = queryParser->getQuery();
  REQUIRE_FALSE(a.isInvalidQuery());

  auto evaluator = new QueryEvaluator(queryParser, createCallsPKB());
  auto clauseResult = evaluator->evaluate();
  auto rows = clauseResult->getRows();
  REQUIRE(rows.empty());
  REQUIRE(clauseResult->getIdent());
}

TEST_CASE("Calls both const negative test") {
  std::string program = R"(Select BOOLEAN such that Calls("eg", "eg2"))";
  auto queryParser = new QueryParser(program);

  queryParser->parse();
  auto a = queryParser->getQuery();
  REQUIRE_FALSE(a.isInvalidQuery());

  auto evaluator = new QueryEvaluator(queryParser, createNoCallsPKB());
  auto clauseResult = evaluator->evaluate();
  auto rows = clauseResult->getRows();
  REQUIRE(rows.empty());
  REQUIRE_FALSE(clauseResult->getIdent());

}