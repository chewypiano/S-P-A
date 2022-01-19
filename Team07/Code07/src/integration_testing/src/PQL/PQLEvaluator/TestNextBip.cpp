//
// Created by Junhua on 4/11/2021.
//

#include "catch.hpp"
#include "QueryEvaluator.h"
#include "../../Utils/TestUtils.h"

TEST_CASE("NextBip both wildcard positive test") {
  std::string program = "assign a; Select a such that NextBip(_, _)";
  auto queryParser = new QueryParser(program);

  queryParser->parse();
  auto q = queryParser->getQuery();
  REQUIRE(q.toString() != "Query(\n"
                          "  INVALID QUERY\n"
                          ")");

  auto evaluator = new QueryEvaluator(queryParser, generateBipPKB());
  auto rows = evaluator->evaluate()->getRows();
  auto a = QuerySynonym(DesignEntityType::Assign, "a");
  RowList expected = createRowList(
      {
          {
              {a, "1"}
          }, {
              {a, "3"}
          }, {
              {a, "4"}
          }
      }
  );
  REQUIRE(expected.size() == rows.size());
  for (auto &r: expected) {
    REQUIRE(std::find(rows.begin(), rows.end(), r) != rows.end());
  }
}

TEST_CASE("NextBip both wildcard negative test") {
  std::string program = "assign a; Select a such that NextBip(_, _)";
  auto queryParser = new QueryParser(program);

  queryParser->parse();
  auto q = queryParser->getQuery();
  REQUIRE(q.toString() != "Query(\n"
                          "  INVALID QUERY\n"
                          ")");

  auto evaluator = new QueryEvaluator(queryParser, generateNoNextBipPKB());
  auto rows = evaluator->evaluate()->getRows();
  auto a = QuerySynonym(DesignEntityType::Assign, "a");
  RowList expected = createRowList({});
  REQUIRE(expected.size() == rows.size());
  for (auto &r: expected) {
    REQUIRE(std::find(rows.begin(), rows.end(), r) != rows.end());
  }
}

TEST_CASE("NextBip both constant positive test") {
  std::string program = "assign a; Select a such that NextBip(2, 3)";
  auto queryParser = new QueryParser(program);

  queryParser->parse();
  auto q = queryParser->getQuery();
  REQUIRE(q.toString() != "Query(\n"
                          "  INVALID QUERY\n"
                          ")");

  auto evaluator = new QueryEvaluator(queryParser, generateBipPKB());
  auto rows = evaluator->evaluate()->getRows();
  auto a = QuerySynonym(DesignEntityType::Assign, "a");
  RowList expected = createRowList(
      {
          {
              {a, "1"}
          }, {
              {a, "3"}
          }, {
              {a, "4"}
          }
      }
  );
  REQUIRE(expected.size() == rows.size());
  for (auto &r: expected) {
    REQUIRE(std::find(rows.begin(), rows.end(), r) != rows.end());
  }
}

TEST_CASE("NextBip both constant negative test") {
  std::string program = "assign a; Select a such that NextBip(2, 3)";
  auto queryParser = new QueryParser(program);

  queryParser->parse();
  auto q = queryParser->getQuery();
  REQUIRE(q.toString() != "Query(\n"
                          "  INVALID QUERY\n"
                          ")");

  auto evaluator = new QueryEvaluator(queryParser, generateNoNextBipPKB());
  auto rows = evaluator->evaluate()->getRows();
  auto a = QuerySynonym(DesignEntityType::Assign, "a");
  RowList expected = createRowList({});
  REQUIRE(expected.size() == rows.size());
  for (auto &r: expected) {
    REQUIRE(std::find(rows.begin(), rows.end(), r) != rows.end());
  }
}

TEST_CASE("NextBip left const right wildcard positive test") {
  std::string program = "assign a; Select a such that NextBip(2, _)";
  auto queryParser = new QueryParser(program);

  queryParser->parse();
  auto q = queryParser->getQuery();
  REQUIRE(q.toString() != "Query(\n"
                          "  INVALID QUERY\n"
                          ")");

  auto evaluator = new QueryEvaluator(queryParser, generateBipPKB());
  auto rows = evaluator->evaluate()->getRows();
  auto a = QuerySynonym(DesignEntityType::Assign, "a");
  RowList expected = createRowList(
      {
          {
              {a, "1"}
          }, {
              {a, "3"}
          }, {
              {a, "4"}
          }
      }
  );
  REQUIRE(expected.size() == rows.size());
  for (auto &r: expected) {
    REQUIRE(std::find(rows.begin(), rows.end(), r) != rows.end());
  }
}

TEST_CASE("NextBip left const right wildcard negative test") {
  std::string program = "assign a; Select a such that NextBip(4, _)";
  auto queryParser = new QueryParser(program);

  queryParser->parse();
  auto q = queryParser->getQuery();
  REQUIRE(q.toString() != "Query(\n"
                          "  INVALID QUERY\n"
                          ")");

  auto evaluator = new QueryEvaluator(queryParser, generateBipPKB());
  auto rows = evaluator->evaluate()->getRows();
  auto a = QuerySynonym(DesignEntityType::Assign, "a");
  RowList expected = createRowList({});
  REQUIRE(expected.size() == rows.size());
  for (auto &r: expected) {
    REQUIRE(std::find(rows.begin(), rows.end(), r) != rows.end());
  }
}

TEST_CASE("NextBip left wildcard right const positive test") {
  std::string program = "assign a; Select a such that NextBip(_, 4)";
  auto queryParser = new QueryParser(program);

  queryParser->parse();
  auto q = queryParser->getQuery();
  REQUIRE(q.toString() != "Query(\n"
                          "  INVALID QUERY\n"
                          ")");

  auto evaluator = new QueryEvaluator(queryParser, generateBipPKB());
  auto rows = evaluator->evaluate()->getRows();
  auto a = QuerySynonym(DesignEntityType::Assign, "a");
  RowList expected = createRowList(
      {
          {
              {a, "1"}
          },
          {
              {a, "3"}
          }, {
              {a, "4"}
          },
      }
  );
  REQUIRE(expected.size() == rows.size());
  for (auto &r: expected) {
    REQUIRE(std::find(rows.begin(), rows.end(), r) != rows.end());
  }
}

TEST_CASE("NextBip left wildcard right const negative test") {
  std::string program = "assign a; Select a such that NextBip(4, _)";
  auto queryParser = new QueryParser(program);

  queryParser->parse();
  auto q = queryParser->getQuery();
  REQUIRE(q.toString() != "Query(\n"
                          "  INVALID QUERY\n"
                          ")");

  auto evaluator = new QueryEvaluator(queryParser, generateBipPKB());
  auto rows = evaluator->evaluate()->getRows();
  RowList expected = createRowList({});
  REQUIRE(expected.size() == rows.size());
  for (auto &r: expected) {
    REQUIRE(std::find(rows.begin(), rows.end(), r) != rows.end());
  }
}

TEST_CASE("NextBip left const right syn positive test") {
  std::string program = "assign a; Select a such that NextBip(3, a)";
  auto queryParser = new QueryParser(program);

  queryParser->parse();
  auto q = queryParser->getQuery();
  REQUIRE(q.toString() != "Query(\n"
                          "  INVALID QUERY\n"
                          ")");

  auto evaluator = new QueryEvaluator(queryParser, generateBipPKB());
  auto rows = evaluator->evaluate()->getRows();
  auto a = QuerySynonym(DesignEntityType::Assign, "a");
  RowList expected = createRowList(
      {
          {
              {a, "4"}
          },
      });
  REQUIRE(expected.size() == rows.size());
  for (auto &r: expected) {
    REQUIRE(std::find(rows.begin(), rows.end(), r) != rows.end());
  }
}

TEST_CASE("NextBip left const right syn negative test") {
  std::string program = "assign a; Select a such that NextBip(4, a)";
  auto queryParser = new QueryParser(program);

  queryParser->parse();
  auto q = queryParser->getQuery();
  REQUIRE(q.toString() != "Query(\n"
                          "  INVALID QUERY\n"
                          ")");

  auto evaluator = new QueryEvaluator(queryParser, generateBipPKB());
  auto rows = evaluator->evaluate()->getRows();
  auto a = QuerySynonym(DesignEntityType::Assign, "a");
  RowList expected = createRowList({});
  REQUIRE(expected.size() == rows.size());
  for (auto &r: expected) {
    REQUIRE(std::find(rows.begin(), rows.end(), r) != rows.end());
  }
}

TEST_CASE("NextBip left wildcard right syn positive test") {
  std::string program = "assign a; Select a such that NextBip(_, a)";
  auto queryParser = new QueryParser(program);

  queryParser->parse();
  auto q = queryParser->getQuery();
  REQUIRE(q.toString() != "Query(\n"
                          "  INVALID QUERY\n"
                          ")");

  auto evaluator = new QueryEvaluator(queryParser, generateBipPKB());
  auto rows = evaluator->evaluate()->getRows();
  auto a = QuerySynonym(DesignEntityType::Assign, "a");
  RowList expected = createRowList(
      {
          {
              {a, "4"}
          },
          {
              {a, "3"}
          },
      }
  );
  REQUIRE(expected.size() == rows.size());
  for (auto &r: expected) {
    REQUIRE(std::find(rows.begin(), rows.end(), r) != rows.end());
  }
}

TEST_CASE("NextBip left wildcard right syn negative test") {
  std::string program = "assign a; Select a such that NextBip(_, a)";
  auto queryParser = new QueryParser(program);

  queryParser->parse();
  auto q = queryParser->getQuery();
  REQUIRE(q.toString() != "Query(\n"
                          "  INVALID QUERY\n"
                          ")");

  auto evaluator = new QueryEvaluator(queryParser, generateNoNextBipPKB());
  auto rows = evaluator->evaluate()->getRows();
  auto a = QuerySynonym(DesignEntityType::Assign, "a");
  RowList expected = createRowList({});
  REQUIRE(expected.size() == rows.size());
  for (auto &r: expected) {
    REQUIRE(std::find(rows.begin(), rows.end(), r) != rows.end());
  }
}

TEST_CASE("NextBip left syn right const positive test") {
  std::string program = "assign a; Select a such that NextBip(a, 2)";
  auto queryParser = new QueryParser(program);

  queryParser->parse();
  auto q = queryParser->getQuery();
  REQUIRE(q.toString() != "Query(\n"
                          "  INVALID QUERY\n"
                          ")");

  auto evaluator = new QueryEvaluator(queryParser, generateBipPKB());
  auto rows = evaluator->evaluate()->getRows();
  auto a = QuerySynonym(DesignEntityType::Assign, "a");
  RowList expected = createRowList(
      {
          {
              {a, "1"}
          },
      }
  );
  REQUIRE(expected.size() == rows.size());
  for (auto &r: expected) {
    REQUIRE(std::find(rows.begin(), rows.end(), r) != rows.end());
  }
}

TEST_CASE("NextBip left syn right const negative test") {
  std::string program = "assign a; Select a such that NextBip(a, 1)";
  auto queryParser = new QueryParser(program);

  queryParser->parse();
  auto q = queryParser->getQuery();
  REQUIRE(q.toString() != "Query(\n"
                          "  INVALID QUERY\n"
                          ")");

  auto evaluator = new QueryEvaluator(queryParser, generateNoNextBipPKB());
  auto rows = evaluator->evaluate()->getRows();
  auto a = QuerySynonym(DesignEntityType::Assign, "a");
  RowList expected = createRowList({});
  REQUIRE(expected.size() == rows.size());
  for (auto &r: expected) {
    REQUIRE(std::find(rows.begin(), rows.end(), r) != rows.end());
  }
}

TEST_CASE("NextBip left syn right wildcard positive test") {
  std::string program = "assign a; Select a such that NextBip(a, _)";
  auto queryParser = new QueryParser(program);

  queryParser->parse();
  auto q = queryParser->getQuery();
  REQUIRE(q.toString() != "Query(\n"
                          "  INVALID QUERY\n"
                          ")");

  auto evaluator = new QueryEvaluator(queryParser, generateBipPKB());
  auto rows = evaluator->evaluate()->getRows();
  auto a = QuerySynonym(DesignEntityType::Assign, "a");
  RowList expected = createRowList(
      {
          {
              {a, "1"}
          },
          {
              {a, "3"}
          },
      }
  );
  REQUIRE(expected.size() == rows.size());
  for (auto &r: expected) {
    REQUIRE(std::find(rows.begin(), rows.end(), r) != rows.end());
  }
}

TEST_CASE("NextBip left syn right wildcard negative test") {
  std::string program = "assign a; Select a such that NextBip(a, _)";
  auto queryParser = new QueryParser(program);

  queryParser->parse();
  auto q = queryParser->getQuery();
  REQUIRE(q.toString() != "Query(\n"
                          "  INVALID QUERY\n"
                          ")");

  auto evaluator = new QueryEvaluator(queryParser, generateNoNextBipPKB());
  auto rows = evaluator->evaluate()->getRows();
  auto a = QuerySynonym(DesignEntityType::Assign, "a");
  RowList expected = createRowList({});
  REQUIRE(expected.size() == rows.size());
  for (auto &r: expected) {
    REQUIRE(std::find(rows.begin(), rows.end(), r) != rows.end());
  }
}

TEST_CASE("NextBip both syn positive test") {
  std::string program = "assign a, b; Select a such that NextBip(a, b)";
  auto queryParser = new QueryParser(program);

  queryParser->parse();
  auto q = queryParser->getQuery();
  REQUIRE(q.toString() != "Query(\n"
                          "  INVALID QUERY\n"
                          ")");

  auto evaluator = new QueryEvaluator(queryParser, generateBipPKB());
  auto rows = evaluator->evaluate()->getRows();
  auto a = QuerySynonym(DesignEntityType::Assign, "a");
  auto b = QuerySynonym(DesignEntityType::Assign, "b");
  RowList expected = createRowList(
      {
          {
              {a, "3"},
              {b, "4"}
          },
      }
  );
  REQUIRE(expected.size() == rows.size());
  for (auto &r: expected) {
    REQUIRE(std::find(rows.begin(), rows.end(), r) != rows.end());
  }
}

TEST_CASE("NextBip both syn negative test") {
  std::string program = "assign a, b; Select a such that NextBip(a, b)";
  auto queryParser = new QueryParser(program);

  queryParser->parse();
  auto q = queryParser->getQuery();
  REQUIRE(q.toString() != "Query(\n"
                          "  INVALID QUERY\n"
                          ")");

  auto evaluator = new QueryEvaluator(queryParser, generateNoNextBipPKB());
  auto rows = evaluator->evaluate()->getRows();
  RowList expected = createRowList({});
  REQUIRE(expected.size() == rows.size());
  for (auto &r: expected) {
    REQUIRE(std::find(rows.begin(), rows.end(), r) != rows.end());
  }
}

