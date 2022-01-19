//
// Created by Junhua on 6/11/2021.
//


#include "catch.hpp"
#include <vector>
#include <string>
#include "QueryEvaluator.h"
#include "../../Utils/TestUtils.h"

PKB *createModifiesPKB() {
  auto pkb = new PKB();

  // proc eg
  pkb->addProc("eg");

  // a = b
  pkb->addVariable("a");
  pkb->addVariable("b");
  pkb->addStatement(1, assign, "eg");
  pkb->addUseStmt(1, "b");
  pkb->addUseProc("eg", "b");
  pkb->addModifyProc("eg", "a");
  pkb->addModifyStmt(1, "a");

  // c = a + b
  pkb->addVariable("c");
  pkb->addStatement(2, assign, "eg");
  pkb->addUseStmt(2, "a");
  pkb->addUseStmt(2, "b");
  pkb->addUseProc("eg", "a");
  pkb->addUseProc("eg", "c");
  pkb->addModifyStmt(2, "c");
  pkb->addModifyProc("eg", "c");

  // read c;
  pkb->addStatement(3, read, "eg");
  pkb->addModifyStmt(3, "c");
  pkb->addModifyProc("eg", "c");

  return pkb;
}

PKB *createNoModifiesPKB() {
  auto pkb = new PKB();

  // proc eg
  pkb->addProc("eg");

  // print c;
  pkb->addStatement(1, print, "eg");
  pkb->addUseStmt(1, "c");
  pkb->addUseProc("eg", "c");

  return pkb;
}

TEST_CASE("ModifiesP syn wildcard positive test") {

  std::string program = "procedure p; Select p such that Modifies(p, _)";
  auto queryParser = new QueryParser(program);

  queryParser->parse();
  auto a = queryParser->getQuery();
  REQUIRE_FALSE(a.isInvalidQuery());

  auto evaluator = new QueryEvaluator(queryParser, createModifiesPKB());
  auto rows = evaluator->evaluate()->getRows();
  auto q = QuerySynonym(DesignEntityType::Procedure, "p");
  RowList expected = createRowList(
      {
          {
              {q, "eg"},
          }
      }
  );
  REQUIRE(expected.size() == rows.size());
  for (auto &r: expected) {
    REQUIRE(std::find(rows.begin(), rows.end(), r) != rows.end());
  }
}

TEST_CASE("ModifiesP syn wildcard negative test") {
  std::string program = "procedure p; Select p such that Modifies(p, _)";
  auto queryParser = new QueryParser(program);

  queryParser->parse();
  auto a = queryParser->getQuery();
  REQUIRE_FALSE(a.isInvalidQuery());

  auto evaluator = new QueryEvaluator(queryParser, createNoModifiesPKB());
  auto rows = evaluator->evaluate()->getRows();
  REQUIRE(rows.empty());
}

TEST_CASE("ModifiesP syn constant positive test") {

  std::string program = "procedure p; Select p such that Modifies(p, \"a\")";
  auto queryParser = new QueryParser(program);

  queryParser->parse();
  auto a = queryParser->getQuery();
  REQUIRE_FALSE(a.isInvalidQuery());

  auto evaluator = new QueryEvaluator(queryParser, createModifiesPKB());
  auto rows = evaluator->evaluate()->getRows();
  auto q = QuerySynonym(DesignEntityType::Procedure, "p");
  RowList expected = createRowList(
      {
          {
              {q, "eg"},
          }
      }
  );
  REQUIRE(expected.size() == rows.size());
  for (auto &r: expected) {
    REQUIRE(std::find(rows.begin(), rows.end(), r) != rows.end());
  }
}

TEST_CASE("ModifiesP syn constant negative test") {
  std::string program = "procedure p; Select p such that Modifies(p, \"a\")";
  auto queryParser = new QueryParser(program);

  queryParser->parse();
  auto a = queryParser->getQuery();
  REQUIRE_FALSE(a.isInvalidQuery());

  auto evaluator = new QueryEvaluator(queryParser, createNoModifiesPKB());
  auto rows = evaluator->evaluate()->getRows();
  REQUIRE(rows.empty());
}

TEST_CASE("ModifiesP syn syn positive test") {
  std::string program = "procedure p; variable v; Select p such that Modifies(p, v)";
  auto queryParser = new QueryParser(program);

  queryParser->parse();
  auto a = queryParser->getQuery();
  REQUIRE_FALSE(a.isInvalidQuery());

  auto evaluator = new QueryEvaluator(queryParser, createModifiesPKB());
  auto rows = evaluator->evaluate()->getRows();
  auto p = QuerySynonym(DesignEntityType::Procedure, "p");
  auto v = QuerySynonym(DesignEntityType::Variable, "v");
  RowList expected = createRowList(
      {
          {
              {p, "eg"},
              {v, "a"},
          }, {
              {p, "eg"},
              {v, "c"},
          }
      }
  );
  REQUIRE(expected.size() == rows.size());
  for (auto &r: expected) {
    REQUIRE(std::find(rows.begin(), rows.end(), r) != rows.end());
  }
}

TEST_CASE("ModifiesP syn syn negative test") {

  std::string program = "procedure p; variable v; Select p such that Modifies(p, v)";
  auto queryParser = new QueryParser(program);

  queryParser->parse();
  auto a = queryParser->getQuery();
  REQUIRE_FALSE(a.isInvalidQuery());

  auto evaluator = new QueryEvaluator(queryParser, createNoModifiesPKB());
  auto rows = evaluator->evaluate()->getRows();
  REQUIRE(rows.empty());
}

TEST_CASE("ModifiesS syn wildcard positive test") {

  std::string program = "stmt p; Select p such that Modifies(p, _)";
  auto queryParser = new QueryParser(program);

  queryParser->parse();
  auto a = queryParser->getQuery();
  REQUIRE_FALSE(a.isInvalidQuery());

  auto evaluator = new QueryEvaluator(queryParser, createModifiesPKB());
  auto rows = evaluator->evaluate()->getRows();
  auto s = QuerySynonym(DesignEntityType::Stmt, "p");
  RowList expected = createRowList(
      {
          {
              {s, "1"},
          },
          {
              {s, "2"},
          }, {
              {s, "3"},
          }
      }
  );
  REQUIRE(expected.size() == rows.size());
  for (auto &r: expected) {
    REQUIRE(std::find(rows.begin(), rows.end(), r) != rows.end());
  }
}

TEST_CASE("ModifiesS syn wildcard negative test") {
  std::string program = "stmt p; Select p such that Modifies(p, _)";
  auto queryParser = new QueryParser(program);

  queryParser->parse();
  auto a = queryParser->getQuery();
  REQUIRE_FALSE(a.isInvalidQuery());

  auto evaluator = new QueryEvaluator(queryParser, createNoModifiesPKB());
  auto rows = evaluator->evaluate()->getRows();
  REQUIRE(rows.empty());
}

TEST_CASE("ModifiesS syn constant positive test") {

  std::string program = "stmt p; Select p such that Modifies(p, \"a\")";
  auto queryParser = new QueryParser(program);

  queryParser->parse();
  auto a = queryParser->getQuery();
  REQUIRE_FALSE(a.isInvalidQuery());

  auto evaluator = new QueryEvaluator(queryParser, createModifiesPKB());
  auto rows = evaluator->evaluate()->getRows();
  auto q = QuerySynonym(DesignEntityType::Stmt, "p");
  RowList expected = createRowList(
      {
          {
              {q, "1"},
          }
      }
  );
  REQUIRE(expected.size() == rows.size());
  for (auto &r: expected) {
    REQUIRE(std::find(rows.begin(), rows.end(), r) != rows.end());
  }
}

TEST_CASE("ModifiesS syn constant negative test") {
  std::string program = "procedure p; Select p such that Modifies(p, \"a\")";
  auto queryParser = new QueryParser(program);

  queryParser->parse();
  auto a = queryParser->getQuery();
  REQUIRE_FALSE(a.isInvalidQuery());

  auto evaluator = new QueryEvaluator(queryParser, createNoModifiesPKB());
  auto rows = evaluator->evaluate()->getRows();
  REQUIRE(rows.empty());
}

TEST_CASE("ModifiesS syn syn positive test") {
  std::string program = "stmt p; variable v; Select p such that Modifies(p, v)";
  auto queryParser = new QueryParser(program);

  queryParser->parse();
  auto a = queryParser->getQuery();
  REQUIRE_FALSE(a.isInvalidQuery());

  auto evaluator = new QueryEvaluator(queryParser, createModifiesPKB());
  auto rows = evaluator->evaluate()->getRows();
  auto p = QuerySynonym(DesignEntityType::Stmt, "p");
  auto v = QuerySynonym(DesignEntityType::Variable, "v");
  RowList expected = createRowList(
      {
          {
              {p, "1"},
              {v, "a"},
          }, {
              {p, "2"},
              {v, "c"},
          }, {
              {p, "3"},
              {v, "c"},
          }
      }
  );
  REQUIRE(expected.size() == rows.size());
  for (auto &r: expected) {
    REQUIRE(std::find(rows.begin(), rows.end(), r) != rows.end());
  }
}

TEST_CASE("ModifiesS syn syn negative test") {

  std::string program = "procedure p; variable v; Select p such that Modifies(p, v)";
  auto queryParser = new QueryParser(program);

  queryParser->parse();
  auto a = queryParser->getQuery();
  REQUIRE_FALSE(a.isInvalidQuery());

  auto evaluator = new QueryEvaluator(queryParser, createNoModifiesPKB());
  auto rows = evaluator->evaluate()->getRows();
  REQUIRE(rows.empty());
}