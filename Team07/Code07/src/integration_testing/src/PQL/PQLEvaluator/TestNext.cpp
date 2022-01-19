//
// Created by Junhua on 6/11/2021.
//

#include <string>
#include "catch.hpp"
#include "PKB/PKB.h"
#include "QueryParser.h"
#include "QueryEvaluator.h"
#include "../../Utils/TestUtils.h"

TEST_CASE("Next clause") {
  auto pkb = new PKB();
  pkb->addProc("eg");
  pkb->addStatement(1, assign, "eg");
  pkb->addStatement(2, print, "eg");
  pkb->addStatement(3, print, "eg");
  pkb->addNext(1, 2);
  pkb->addNext(2, 3);

  std::string program = "assign a; print p, p1; Select p such that Next(a, p)";
  auto queryParser = new QueryParser(program);

  queryParser->parse();
  auto a = queryParser->getQuery();
  REQUIRE(a.toString() != "Query(\n"
                          "  INVALID QUERY\n"
                          ")");

  auto evaluator = new QueryEvaluator(queryParser, pkb);
  auto rows = evaluator->evaluate()->getRows();
  RowList expected = createRowList(
      {
          {
              {
                  {QuerySynonym(DesignEntityType::Assign, "a"), "1"},
                  {QuerySynonym(DesignEntityType::Print, "p"), "2"}
              }
          }
      }
  );
  REQUIRE(expected.size() == rows.size());
  for (auto &r: expected) {
    REQUIRE(std::find(rows.begin(), rows.end(), r) != rows.end());
  }
}

TEST_CASE("Next clause with and") {
  auto pkb = new PKB();
  pkb->addProc("eg");
  pkb->addStatement(1, assign, "eg");
  pkb->addStatement(2, print, "eg");
  pkb->addStatement(3, print, "eg");
  pkb->addNext(1, 2);
  pkb->addNext(2, 3);

  std::string program = "assign a; print p, p1; Select p such that Next(a, p) and Next(p, p1)";
  auto queryParser = new QueryParser(program);

  queryParser->parse();
  auto a = queryParser->getQuery();
  REQUIRE(a.toString() != "Query(\n"
                          "  INVALID QUERY\n"
                          ")");

  auto evaluator = new QueryEvaluator(queryParser, pkb);
  auto rows = evaluator->evaluate()->getRows();
  RowList expected = createRowList(
      {
          {
              {
                  {QuerySynonym(DesignEntityType::Assign, "a"), "1"},
                  {QuerySynonym(DesignEntityType::Print, "p"), "2"},
                  {QuerySynonym(DesignEntityType::Print, "p1"), "3"}
              }
          }
      }
  );
  REQUIRE(expected.size() == rows.size());
  for (auto &r: expected) {
    REQUIRE(std::find(rows.begin(), rows.end(), r) != rows.end());
  }
}

TEST_CASE("Next with 2 wildcards") {
  auto pkb = new PKB();
  pkb->addProc("eg");
  pkb->addStatement(1, assign, "eg");
  pkb->addStatement(2, print, "eg");
  pkb->addStatement(3, print, "eg");
  pkb->addNext(1, 2);
  pkb->addNext(2, 3);

  std::string program = "assign a; Select a such that Next(_, _)";
  auto queryParser = new QueryParser(program);

  queryParser->parse();
  auto a = queryParser->getQuery();
  REQUIRE(a.toString() != "Query(\n"
                          "  INVALID QUERY\n"
                          ")");

  auto evaluator = new QueryEvaluator(queryParser, pkb);
  auto rows = evaluator->evaluate()->getRows();
  RowList expected = createRowList(
      {
          {
              {
                  {QuerySynonym(DesignEntityType::Assign, "a"), "1"},
              }
          }
      }
  );
  REQUIRE(expected.size() == rows.size());
  for (auto &r: expected) {
    REQUIRE(std::find(rows.begin(), rows.end(), r) != rows.end());
  }
}

TEST_CASE("Test next left wildcard") {
  auto pkb = new PKB();
  pkb->addProc("eg");
  pkb->addStatement(1, assign, "eg");
  pkb->addStatement(2, print, "eg");
  pkb->addStatement(3, print, "eg");
  pkb->addNext(1, 2);
  pkb->addNext(2, 3);

  std::string program = "assign a; print p, p1; Select p such that Next(_, p)";
  auto queryParser = new QueryParser(program);

  queryParser->parse();
  auto a = queryParser->getQuery();
  REQUIRE(a.toString() != "Query(\n"
                          "  INVALID QUERY\n"
                          ")");

  auto evaluator = new QueryEvaluator(queryParser, pkb);
  auto rows = evaluator->evaluate()->getRows();
  RowList expected = createRowList(
      {
          {
              {
                  {QuerySynonym(DesignEntityType::Print, "p"), "2"},

              }, {
                  {QuerySynonym(DesignEntityType::Print, "p"), "3"}
              }
          }
      }
  );
  REQUIRE(expected.size() == rows.size());
  for (auto &r: expected) {
    REQUIRE(std::find(rows.begin(), rows.end(), r) != rows.end());
  }
}

TEST_CASE("Test next right wildcard") {
  auto pkb = new PKB();
  pkb->addProc("eg");
  pkb->addStatement(1, assign, "eg");
  pkb->addStatement(2, print, "eg");
  pkb->addStatement(3, print, "eg");
  pkb->addNext(1, 2);
  pkb->addNext(2, 3);

  std::string program = "assign a; print p, p1; Select p such that Next(p, _)";
  auto queryParser = new QueryParser(program);

  queryParser->parse();
  auto a = queryParser->getQuery();
  REQUIRE(a.toString() != "Query(\n"
                          "  INVALID QUERY\n"
                          ")");

  auto evaluator = new QueryEvaluator(queryParser, pkb);
  auto rows = evaluator->evaluate()->getRows();
  RowList expected = createRowList(
      {
          {
              {
                  {QuerySynonym(DesignEntityType::Print, "p"), "2"},

              }
          }
      }
  );
  REQUIRE(expected.size() == rows.size());
  for (auto &r: expected) {
    REQUIRE(std::find(rows.begin(), rows.end(), r) != rows.end());
  }
}

TEST_CASE("Test next * right wildcard") {
  auto pkb = new PKB();
  pkb->addProc("eg");
  pkb->addStatement(1, assign, "eg");
  pkb->addStatement(2, print, "eg");
  pkb->addStatement(3, print, "eg");
  pkb->addNext(1, 2);
  pkb->addNext(2, 3);

  std::string program = "stmt p; Select p such that Next*(p, _)";
  auto queryParser = new QueryParser(program);

  queryParser->parse();
  auto a = queryParser->getQuery();
  REQUIRE(a.toString() != "Query(\n"
                          "  INVALID QUERY\n"
                          ")");

  auto evaluator = new QueryEvaluator(queryParser, pkb);
  auto rows = evaluator->evaluate()->getRows();
  RowList expected = createRowList(
      {
          {
              {
                  {QuerySynonym(DesignEntityType::Stmt, "p"), "2"},
              }, {
                  {QuerySynonym(DesignEntityType::Stmt, "p"), "1"},
              }
          }
      }
  );
  REQUIRE(expected.size() == rows.size());
  for (auto &r: expected) {
    REQUIRE(std::find(rows.begin(), rows.end(), r) != rows.end());
  }
}
TEST_CASE("Test next * left wildcard") {
  auto pkb = new PKB();
  pkb->addProc("eg");
  pkb->addStatement(1, assign, "eg");
  pkb->addStatement(2, print, "eg");
  pkb->addStatement(3, print, "eg");
  pkb->addNext(1, 2);
  pkb->addNext(2, 3);

  std::string program = "stmt p; Select p such that Next*(_, p)";
  auto queryParser = new QueryParser(program);

  queryParser->parse();
  auto a = queryParser->getQuery();
  REQUIRE(a.toString() != "Query(\n"
                          "  INVALID QUERY\n"
                          ")");

  auto evaluator = new QueryEvaluator(queryParser, pkb);
  auto rows = evaluator->evaluate()->getRows();
  RowList expected = createRowList(
      {
          {
              {
                  {QuerySynonym(DesignEntityType::Stmt, "p"), "2"},
              }, {
                  {QuerySynonym(DesignEntityType::Stmt, "p"), "3"},
              }
          }
      }
  );
  REQUIRE(expected.size() == rows.size());
  for (auto &r: expected) {
    REQUIRE(std::find(rows.begin(), rows.end(), r) != rows.end());
  }
}

TEST_CASE("Test next * no wildcard") {
  auto pkb = new PKB();
  pkb->addProc("eg");
  pkb->addStatement(1, assign, "eg");
  pkb->addStatement(2, print, "eg");
  pkb->addStatement(3, print, "eg");
  pkb->addNext(1, 2);
  pkb->addNext(2, 3);
  pkb->addCFG("eg", new CFG(pkb->getNextListForProc("eg")));

  std::string program = "stmt p, p1; Select <p, p1> such that Next*(p1, p)";
  auto queryParser = new QueryParser(program);

  queryParser->parse();
  auto a = queryParser->getQuery();
  REQUIRE(a.toString() != "Query(\n"
                          "  INVALID QUERY\n"
                          ")");

  auto evaluator = new QueryEvaluator(queryParser, pkb);
  auto rows = evaluator->evaluate()->getRows();
  RowList expected = createRowList(
      {
          {
              {
                  {QuerySynonym(DesignEntityType::Stmt, "p"), "2"},
                  {QuerySynonym(DesignEntityType::Stmt, "p1"), "1"},
              }, {
                  {QuerySynonym(DesignEntityType::Stmt, "p1"), "2"},
                  {QuerySynonym(DesignEntityType::Stmt, "p"), "3"},
              }, {
                  {QuerySynonym(DesignEntityType::Stmt, "p1"), "1"},
                  {QuerySynonym(DesignEntityType::Stmt, "p"), "3"},
              }
          }
      }
  );
  REQUIRE(expected.size() == rows.size());
  for (auto &r: expected) {
    REQUIRE(std::find(rows.begin(), rows.end(), r) != rows.end());
  }
}

TEST_CASE("Test next * both constants true") {
  auto pkb = new PKB();
  pkb->addProc("eg");
  pkb->addStatement(1, assign, "eg");
  pkb->addStatement(2, print, "eg");
  pkb->addStatement(3, print, "eg");
  pkb->addNext(1, 2);
  pkb->addNext(2, 3);
  pkb->addCFG("eg", new CFG(pkb->getNextListForProc("eg")));

  std::string program = "stmt p, p1; Select p such that Next*(1, 3)";
  auto queryParser = new QueryParser(program);

  queryParser->parse();
  auto a = queryParser->getQuery();
  REQUIRE(a.toString() != "Query(\n"
                          "  INVALID QUERY\n"
                          ")");

  auto evaluator = new QueryEvaluator(queryParser, pkb);
  auto rows = evaluator->evaluate()->getRows();
  RowList expected = createRowList(
      {
          {
              {
                  {QuerySynonym(DesignEntityType::Stmt, "p"), "2"},
              }, {
                  {QuerySynonym(DesignEntityType::Stmt, "p"), "3"},
              }, {
                  {QuerySynonym(DesignEntityType::Stmt, "p"), "1"},
              }
          }
      }
  );
  REQUIRE(expected.size() == rows.size());
  for (auto &r: expected) {
    REQUIRE(std::find(rows.begin(), rows.end(), r) != rows.end());
  }
}

TEST_CASE("Test next * both constants false") {
  auto pkb = new PKB();
  pkb->addProc("eg");
  pkb->addStatement(1, assign, "eg");
  pkb->addStatement(2, print, "eg");
  pkb->addStatement(3, print, "eg");
  pkb->addNext(1, 2);
  pkb->addNext(2, 3);
  pkb->addCFG("eg", new CFG(pkb->getNextListForProc("eg")));

  std::string program = "stmt p, p1; Select p such that Next*(3, 1)";
  auto queryParser = new QueryParser(program);

  queryParser->parse();
  auto a = queryParser->getQuery();
  REQUIRE(a.toString() != "Query(\n"
                          "  INVALID QUERY\n"
                          ")");

  auto evaluator = new QueryEvaluator(queryParser, pkb);
  auto rows = evaluator->evaluate()->getRows();
  RowList expected = createRowList({});
  REQUIRE(expected.size() == rows.size());
}

TEST_CASE("Test next * constants with right wildcard true") {
  auto pkb = new PKB();
  pkb->addProc("eg");
  pkb->addStatement(1, assign, "eg");
  pkb->addStatement(2, print, "eg");
  pkb->addStatement(3, print, "eg");
  pkb->addNext(1, 2);
  pkb->addNext(2, 3);
  pkb->addCFG("eg", new CFG(pkb->getNextListForProc("eg")));

  std::string program = "stmt p, p1; Select p such that Next*(1, _)";
  auto queryParser = new QueryParser(program);

  queryParser->parse();
  auto a = queryParser->getQuery();
  REQUIRE(a.toString() != "Query(\n"
                          "  INVALID QUERY\n"
                          ")");

  auto evaluator = new QueryEvaluator(queryParser, pkb);
  auto rows = evaluator->evaluate()->getRows();
  RowList expected = createRowList(
      {
          {
              {
                  {QuerySynonym(DesignEntityType::Stmt, "p"), "2"},
              }, {
                  {QuerySynonym(DesignEntityType::Stmt, "p"), "3"},
              }, {
                  {QuerySynonym(DesignEntityType::Stmt, "p"), "1"},
              }
          }
      }
  );
  REQUIRE(expected.size() == rows.size());
  for (auto &r: expected) {
    REQUIRE(std::find(rows.begin(), rows.end(), r) != rows.end());
  }
}

TEST_CASE("Test next * constants with right wildcard false") {
  auto pkb = new PKB();
  pkb->addProc("eg");
  pkb->addStatement(1, assign, "eg");
  pkb->addStatement(2, print, "eg");
  pkb->addStatement(3, print, "eg");
  pkb->addNext(1, 2);
  pkb->addNext(2, 3);
  pkb->addCFG("eg", new CFG(pkb->getNextListForProc("eg")));

  std::string program = "stmt p, p1; Select p such that Next*(3, _)";
  auto queryParser = new QueryParser(program);

  queryParser->parse();
  auto a = queryParser->getQuery();
  REQUIRE(a.toString() != "Query(\n"
                          "  INVALID QUERY\n"
                          ")");

  auto evaluator = new QueryEvaluator(queryParser, pkb);
  auto rows = evaluator->evaluate()->getRows();
  RowList expected = createRowList({});
  REQUIRE(expected.size() == rows.size());
  for (auto &r: expected) {
    REQUIRE(std::find(rows.begin(), rows.end(), r) != rows.end());
  }
}

TEST_CASE("Test next * constants with left wildcard true") {
  auto pkb = new PKB();
  pkb->addProc("eg");
  pkb->addStatement(1, assign, "eg");
  pkb->addStatement(2, print, "eg");
  pkb->addStatement(3, print, "eg");
  pkb->addNext(1, 2);
  pkb->addNext(2, 3);
  pkb->addCFG("eg", new CFG(pkb->getNextListForProc("eg")));

  std::string program = "stmt p, p1; Select p such that Next*(_, 2)";
  auto queryParser = new QueryParser(program);

  queryParser->parse();
  auto a = queryParser->getQuery();
  REQUIRE(a.toString() != "Query(\n"
                          "  INVALID QUERY\n"
                          ")");

  auto evaluator = new QueryEvaluator(queryParser, pkb);
  auto rows = evaluator->evaluate()->getRows();
  RowList expected = createRowList(
      {
          {
              {
                  {QuerySynonym(DesignEntityType::Stmt, "p"), "2"},
              }, {
                  {QuerySynonym(DesignEntityType::Stmt, "p"), "3"},
              }, {
                  {QuerySynonym(DesignEntityType::Stmt, "p"), "1"},
              }
          }
      }
  );
  REQUIRE(expected.size() == rows.size());
  for (auto &r: expected) {
    REQUIRE(std::find(rows.begin(), rows.end(), r) != rows.end());
  }
}

TEST_CASE("Test next * constants with left wildcard false") {
  auto pkb = new PKB();
  pkb->addProc("eg");
  pkb->addStatement(1, assign, "eg");
  pkb->addStatement(2, print, "eg");
  pkb->addStatement(3, print, "eg");
  pkb->addNext(1, 2);
  pkb->addNext(2, 3);
  pkb->addCFG("eg", new CFG(pkb->getNextListForProc("eg")));

  std::string program = "stmt p, p1; Select p such that Next*(_, 1)";
  auto queryParser = new QueryParser(program);

  queryParser->parse();
  auto a = queryParser->getQuery();
  REQUIRE(a.toString() != "Query(\n"
                          "  INVALID QUERY\n"
                          ")");

  auto evaluator = new QueryEvaluator(queryParser, pkb);
  auto rows = evaluator->evaluate()->getRows();
  RowList expected = createRowList({});
  REQUIRE(expected.size() == rows.size());
  for (auto &r: expected) {
    REQUIRE(std::find(rows.begin(), rows.end(), r) != rows.end());
  }
}
