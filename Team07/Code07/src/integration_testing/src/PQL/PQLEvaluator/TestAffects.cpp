//
// Created by Junhua on 4/11/2021.
//

#include "catch.hpp"
#include <vector>
#include <string>
#include "QueryEvaluator.h"
#include "../../Utils/TestUtils.h"

TEST_CASE("Affects left wildcard right constant positive Test") {
  std::string program = "stmt p, p1; Select p such that Affects(_, 2)";
  auto queryParser = new QueryParser(program);

  queryParser->parse();
  auto a = queryParser->getQuery();

  REQUIRE(a.toString() != "Query(\n"
                          "  INVALID QUERY\n"
                          ")");

  auto evaluator = new QueryEvaluator(queryParser, generateAffectsPKB());
  auto rows = evaluator->evaluate()->getRows();
  RowList expected = createRowList(
      {
          {
              {QuerySynonym(DesignEntityType::Stmt, "p"), "1"}
          }, {
              {QuerySynonym(DesignEntityType::Stmt, "p"), "2"}
          }, {
              {QuerySynonym(DesignEntityType::Stmt, "p"), "3"}
          }
      }
  );
  REQUIRE(expected.size() == rows.size());
  for (auto &r: expected) {
    REQUIRE(std::find(rows.begin(), rows.end(), r) != rows.end());
  }
}

TEST_CASE("Affects left wildcard right constant negative Test") {
  std::string program = "stmt p, p1; Select p such that Affects(_, 1)";
  auto queryParser = new QueryParser(program);

  queryParser->parse();
  auto a = queryParser->getQuery();

  REQUIRE(a.toString() != "Query(\n"
                          "  INVALID QUERY\n"
                          ")");

  auto evaluator = new QueryEvaluator(queryParser, generateAffectsPKB());
  auto rows = evaluator->evaluate()->getRows();
  RowList expected = createRowList({});
  REQUIRE(expected.size() == rows.size());
  for (auto &r: expected) {
    REQUIRE(std::find(rows.begin(), rows.end(), r) != rows.end());
  }
}

TEST_CASE("Affects right wildcard left constant negative Test") {
  std::string program = "stmt p, p1; Select p such that Affects(3, _)";
  auto queryParser = new QueryParser(program);

  queryParser->parse();
  auto a = queryParser->getQuery();

  REQUIRE(a.toString() != "Query(\n"
                          "  INVALID QUERY\n"
                          ")");

  auto evaluator = new QueryEvaluator(queryParser, generateAffectsPKB());
  auto rows = evaluator->evaluate()->getRows();
  RowList expected = createRowList({});
  REQUIRE(expected.size() == rows.size());
  for (auto &r: expected) {
    REQUIRE(std::find(rows.begin(), rows.end(), r) != rows.end());
  }
}

TEST_CASE("Affects right wildcard left constant positive Test") {
  std::string program = "stmt p, p1; Select p such that Affects(1, _)";
  auto queryParser = new QueryParser(program);

  queryParser->parse();
  auto a = queryParser->getQuery();

  REQUIRE(a.toString() != "Query(\n"
                          "  INVALID QUERY\n"
                          ")");

  auto evaluator = new QueryEvaluator(queryParser, generateAffectsPKB());
  auto rows = evaluator->evaluate()->getRows();
  RowList expected = createRowList(
      {
          {
              {QuerySynonym(DesignEntityType::Stmt, "p"), "1"}
          }, {
              {QuerySynonym(DesignEntityType::Stmt, "p"), "2"}
          }, {
              {QuerySynonym(DesignEntityType::Stmt, "p"), "3"}
          }
      }
  );
  REQUIRE(expected.size() == rows.size());
  for (auto &r: expected) {
    REQUIRE(std::find(rows.begin(), rows.end(), r) != rows.end());
  }
}

TEST_CASE("Affects right constant left constant positive Test") {
  std::string program = "stmt p, p1; Select p such that Affects(1, 2)";
  auto queryParser = new QueryParser(program);

  queryParser->parse();
  auto a = queryParser->getQuery();

  REQUIRE(a.toString() != "Query(\n"
                          "  INVALID QUERY\n"
                          ")");

  auto evaluator = new QueryEvaluator(queryParser, generateAffectsPKB());
  auto rows = evaluator->evaluate()->getRows();
  RowList expected = createRowList(
      {
          {
              {QuerySynonym(DesignEntityType::Stmt, "p"), "1"}
          }, {
              {QuerySynonym(DesignEntityType::Stmt, "p"), "2"}
          }, {
              {QuerySynonym(DesignEntityType::Stmt, "p"), "3"}
          }
      }
  );
  REQUIRE(expected.size() == rows.size());
  for (auto &r: expected) {
    REQUIRE(std::find(rows.begin(), rows.end(), r) != rows.end());
  }
}

TEST_CASE("Affects right constant left constant negative Test") {
  std::string program = "stmt p, p1; Select p such that Affects(2, 1)";
  auto queryParser = new QueryParser(program);

  queryParser->parse();
  auto a = queryParser->getQuery();

  REQUIRE(a.toString() != "Query(\n"
                          "  INVALID QUERY\n"
                          ")");

  auto evaluator = new QueryEvaluator(queryParser, generateAffectsPKB());
  auto rows = evaluator->evaluate()->getRows();
  RowList expected = createRowList({});
  REQUIRE(expected.size() == rows.size());
  for (auto &r: expected) {
    REQUIRE(std::find(rows.begin(), rows.end(), r) != rows.end());
  }
}

TEST_CASE("Affects left syn right constant positive Test") {
  std::string program = "assign a; Select a such that Affects(a, 2)";
  auto queryParser = new QueryParser(program);

  queryParser->parse();
  auto a = queryParser->getQuery();

  REQUIRE(a.toString() != "Query(\n"
                          "  INVALID QUERY\n"
                          ")");

  auto evaluator = new QueryEvaluator(queryParser, generateAffectsPKB());
  auto rows = evaluator->evaluate()->getRows();
  RowList expected = createRowList(
      {
          {
              {QuerySynonym(DesignEntityType::Assign, "a"), "1"}
          },
      }
  );
  REQUIRE(expected.size() == rows.size());
  for (auto &r: expected) {
    REQUIRE(std::find(rows.begin(), rows.end(), r) != rows.end());
  }
}

TEST_CASE("Affects left syn right constant negative Test") {
  std::string program = "assign a; Select a such that Affects(a, 1)";
  auto queryParser = new QueryParser(program);

  queryParser->parse();
  auto a = queryParser->getQuery();

  REQUIRE(a.toString() != "Query(\n"
                          "  INVALID QUERY\n"
                          ")");

  auto evaluator = new QueryEvaluator(queryParser, generateAffectsPKB());
  auto rows = evaluator->evaluate()->getRows();
  RowList expected = createRowList({});
  REQUIRE(expected.size() == rows.size());
  for (auto &r: expected) {
    REQUIRE(std::find(rows.begin(), rows.end(), r) != rows.end());
  }
}

TEST_CASE("Affects left constant right syn positive Test") {
  std::string program = "assign a; Select a such that Affects(1, a)";
  auto queryParser = new QueryParser(program);

  queryParser->parse();
  auto a = queryParser->getQuery();

  REQUIRE(a.toString() != "Query(\n"
                          "  INVALID QUERY\n"
                          ")");

  auto evaluator = new QueryEvaluator(queryParser, generateAffectsPKB());
  auto rows = evaluator->evaluate()->getRows();
  RowList expected = createRowList(
      {
          {
              {QuerySynonym(DesignEntityType::Assign, "a"), "2"}
          }, {
              {QuerySynonym(DesignEntityType::Assign, "a"), "3"}
          },
      }
  );
  REQUIRE(expected.size() == rows.size());
  for (auto &r: expected) {
    REQUIRE(std::find(rows.begin(), rows.end(), r) != rows.end());
  }
}

TEST_CASE("Affects left constant right syn negative Test") {
  std::string program = "assign a; Select a such that Affects(3, a)";
  auto queryParser = new QueryParser(program);

  queryParser->parse();
  auto a = queryParser->getQuery();

  REQUIRE(a.toString() != "Query(\n"
                          "  INVALID QUERY\n"
                          ")");

  auto evaluator = new QueryEvaluator(queryParser, generateAffectsPKB());
  auto rows = evaluator->evaluate()->getRows();
  RowList expected = createRowList({});
  REQUIRE(expected.size() == rows.size());
  for (auto &r: expected) {
    REQUIRE(std::find(rows.begin(), rows.end(), r) != rows.end());
  }
}

TEST_CASE("Affects left syn right wildcard positive Test") {
  std::string program = "assign a; Select a such that Affects(a, _)";
  auto queryParser = new QueryParser(program);

  queryParser->parse();
  auto a = queryParser->getQuery();

  REQUIRE(a.toString() != "Query(\n"
                          "  INVALID QUERY\n"
                          ")");

  auto evaluator = new QueryEvaluator(queryParser, generateAffectsPKB());
  auto rows = evaluator->evaluate()->getRows();
  RowList expected = createRowList(
      {
          {
              {
                  {QuerySynonym(DesignEntityType::Assign, "a"), "1"}
              }, {
                  {QuerySynonym(DesignEntityType::Assign, "a"), "2"}
              },
          }
      }
  );
  REQUIRE(expected.size() == rows.size());
  for (auto &r: expected) {
    REQUIRE(std::find(rows.begin(), rows.end(), r) != rows.end());
  }
}

TEST_CASE("Affects left syn right wildcard negative Test") {
  std::string program = "assign a; Select a such that Affects(a, _)";
  auto queryParser = new QueryParser(program);

  queryParser->parse();
  auto a = queryParser->getQuery();

  REQUIRE(a.toString() != "Query(\n"
                          "  INVALID QUERY\n"
                          ")");

  auto evaluator = new QueryEvaluator(queryParser, generateNoAffectsPKB());
  auto rows = evaluator->evaluate()->getRows();
  RowList expected = createRowList({});
  REQUIRE(expected.size() == rows.size());
  for (auto &r: expected) {
    REQUIRE(std::find(rows.begin(), rows.end(), r) != rows.end());
  }
}

TEST_CASE("Affects left wildcard right syn positive Test") {
  std::string program = "assign a; Select a such that Affects(_, a)";
  auto queryParser = new QueryParser(program);

  queryParser->parse();
  auto a = queryParser->getQuery();

  REQUIRE(a.toString() != "Query(\n"
                          "  INVALID QUERY\n"
                          ")");

  auto evaluator = new QueryEvaluator(queryParser, generateAffectsPKB());
  auto rows = evaluator->evaluate()->getRows();
  RowList expected = createRowList(
      {
          {
              {
                  {QuerySynonym(DesignEntityType::Assign, "a"), "2"}
              }, {
                  {QuerySynonym(DesignEntityType::Assign, "a"), "3"}
              },
          }
      }
  );
  REQUIRE(expected.size() == rows.size());
  for (auto &r: expected) {
    REQUIRE(std::find(rows.begin(), rows.end(), r) != rows.end());
  }
}

TEST_CASE("Affects left wildcard right syn Negative Test") {
  std::string program = "assign a; Select a such that Affects(_, a)";
  auto queryParser = new QueryParser(program);

  queryParser->parse();
  auto a = queryParser->getQuery();

  REQUIRE(a.toString() != "Query(\n"
                          "  INVALID QUERY\n"
                          ")");

  auto evaluator = new QueryEvaluator(queryParser, generateNoAffectsPKB());
  auto rows = evaluator->evaluate()->getRows();
  RowList expected = createRowList({});
  REQUIRE(expected.size() == rows.size());
  for (auto &r: expected) {
    REQUIRE(std::find(rows.begin(), rows.end(), r) != rows.end());
  }
}

TEST_CASE("Affects both syn positive Test") {
  std::string program = "assign a, b; Select <a,b> such that Affects(a, b)";
  auto queryParser = new QueryParser(program);

  queryParser->parse();
  auto q = queryParser->getQuery();
  REQUIRE(q.toString() != "Query(\n"
                          "  INVALID QUERY\n"
                          ")");

  auto evaluator = new QueryEvaluator(queryParser, generateAffectsPKB());
  auto rows = evaluator->evaluate()->getRows();
  auto a = QuerySynonym(DesignEntityType::Assign, "a");
  auto b = QuerySynonym(DesignEntityType::Assign, "b");
  RowList expected = createRowList(
      {
          {
              {a, "1"},
              {b, "2"}
          },
          {
              {a, "1"},
              {b, "3"}
          }, {
              {a, "2"},
              {b, "3"}
          },
      }
  );
  REQUIRE(expected.size() == rows.size());
  for (auto &r: expected) {
    REQUIRE(std::find(rows.begin(), rows.end(), r) != rows.end());
  }
}

TEST_CASE("Affects both syn negative Test") {
  std::string program = "assign a, b; Select <a,b> such that Affects(a, b)";
  auto queryParser = new QueryParser(program);

  queryParser->parse();
  auto q = queryParser->getQuery();
  REQUIRE(q.toString() != "Query(\n"
                          "  INVALID QUERY\n"
                          ")");

  auto evaluator = new QueryEvaluator(queryParser, generateNoAffectsPKB());
  auto rows = evaluator->evaluate()->getRows();
  auto a = QuerySynonym(DesignEntityType::Assign, "a");
  auto b = QuerySynonym(DesignEntityType::Assign, "b");
  RowList expected = createRowList({});
  REQUIRE(expected.size() == rows.size());
  for (auto &r: expected) {
    REQUIRE(std::find(rows.begin(), rows.end(), r) != rows.end());
  }
}

TEST_CASE("Affects both syn wildcard positive test") {
  std::string program = "assign a; Select a such that Affects(_, _)";
  auto queryParser = new QueryParser(program);

  queryParser->parse();
  auto q = queryParser->getQuery();
  REQUIRE(q.toString() != "Query(\n"
                          "  INVALID QUERY\n"
                          ")");

  auto evaluator = new QueryEvaluator(queryParser, generateAffectsPKB());
  auto rows = evaluator->evaluate()->getRows();
  auto a = QuerySynonym(DesignEntityType::Assign, "a");
  RowList expected = createRowList(
      {
          {
              {
                  {a, "1"},

              },
              {
                  {a, "3"},

              }, {
                  {a, "2"},
              },
          }
      }
  );
  REQUIRE(expected.size() == rows.size());
  for (auto &r: expected) {
    REQUIRE(std::find(rows.begin(), rows.end(), r) != rows.end());
  }
}

TEST_CASE("Affects both syn wildcard negative test") {
  std::string program = "assign a; Select a such that Affects(_, _)";
  auto queryParser = new QueryParser(program);

  queryParser->parse();
  auto q = queryParser->getQuery();
  REQUIRE(q.toString() != "Query(\n"
                          "  INVALID QUERY\n"
                          ")");

  auto evaluator = new QueryEvaluator(queryParser, generateNoAffectsPKB());
  auto rows = evaluator->evaluate()->getRows();
  auto a = QuerySynonym(DesignEntityType::Assign, "a");
  RowList expected = createRowList({});
  REQUIRE(expected.size() == rows.size());
  for (auto &r: expected) {
    REQUIRE(std::find(rows.begin(), rows.end(), r) != rows.end());
  }
}

TEST_CASE("Affects both constants out of range test") {
  std::string program = "assign a1; Select a1 such that Affects(4, 4)";
  auto queryParser = new QueryParser(program);

  queryParser->parse();
  auto a = queryParser->getQuery();
  REQUIRE_FALSE(a.isInvalidQuery());

  QuerySynonym a1 = QuerySynonym(DesignEntityType::Assign, "a1");
  auto evaluator = new QueryEvaluator(queryParser, generateAffectsPKB());
  auto rows = evaluator->evaluate()->getRows();
  REQUIRE(rows.empty());
}

TEST_CASE("Affects left wildcard out of range test") {
  std::string program = "assign a1; Select a1 such that Affects(_, 4)";
  auto queryParser = new QueryParser(program);

  queryParser->parse();
  auto a = queryParser->getQuery();
  REQUIRE_FALSE(a.isInvalidQuery());

  QuerySynonym a1 = QuerySynonym(DesignEntityType::Assign, "a1");
  auto evaluator = new QueryEvaluator(queryParser, generateAffectsPKB());
  auto rows = evaluator->evaluate()->getRows();
  REQUIRE(rows.empty());
}

TEST_CASE("Affects right wildcard out of range test") {
  std::string program = "assign a1; Select a1 such that Affects(4, _)";
  auto queryParser = new QueryParser(program);

  queryParser->parse();
  auto a = queryParser->getQuery();
  REQUIRE_FALSE(a.isInvalidQuery());

  QuerySynonym a1 = QuerySynonym(DesignEntityType::Assign, "a1");
  auto evaluator = new QueryEvaluator(queryParser, generateAffectsPKB());
  auto rows = evaluator->evaluate()->getRows();
  REQUIRE(rows.empty());
}

TEST_CASE("Affects left syn out of range test") {
  std::string program = "assign a1; Select a1 such that Affects(a1, 4)";
  auto queryParser = new QueryParser(program);

  queryParser->parse();
  auto a = queryParser->getQuery();
  REQUIRE_FALSE(a.isInvalidQuery());

  QuerySynonym a1 = QuerySynonym(DesignEntityType::Assign, "a1");
  auto evaluator = new QueryEvaluator(queryParser, generateAffectsPKB());
  auto rows = evaluator->evaluate()->getRows();
  REQUIRE(rows.empty());
}

TEST_CASE("Affects right syn out of range test") {
  std::string program = "assign a1; Select a1 such that Affects(4, a1)";
  auto queryParser = new QueryParser(program);

  queryParser->parse();
  auto a = queryParser->getQuery();
  REQUIRE_FALSE(a.isInvalidQuery());

  QuerySynonym a1 = QuerySynonym(DesignEntityType::Assign, "a1");
  auto evaluator = new QueryEvaluator(queryParser, generateAffectsPKB());
  auto rows = evaluator->evaluate()->getRows();
  REQUIRE(rows.empty());
}
