//
// Created by Jazer on 4/11/2021.
//

#include "catch.hpp"
#include "QueryEvaluator.h"
#include "../../Utils/TestUtils.h"

TEST_CASE("NextBip* both Synonyms positive test") {
  std::string program = "stmt s1, s2; Select s1 such that NextBip*(s1, s2)";
  auto queryParser = new QueryParser(program);

  queryParser->parse();
  auto q = queryParser->getQuery();
  REQUIRE_FALSE(q.isInvalidQuery());

  auto evaluator = new QueryEvaluator(queryParser, generateBipPKB());
  auto rows = evaluator->evaluate()->getRows();
  auto s1 = QuerySynonym(DesignEntityType::Stmt, "s1");
  auto s2 = QuerySynonym(DesignEntityType::Stmt, "s2");
  RowList expected = createRowList(
      {
          {{s1, "1"}, {s2, "2"}},
          {{s1, "1"}, {s2, "3"}},
          {{s1, "1"}, {s2, "4"}},
          {{s1, "2"}, {s2, "3"}},
          {{s1, "2"}, {s2, "4"}},
          {{s1, "3"}, {s2, "4"}},
      }
  );
  REQUIRE(expected.size() == rows.size());
  for (auto &r: expected) {
    REQUIRE(std::find(rows.begin(), rows.end(), r) != rows.end());
  }
}

TEST_CASE("NextBip* both Synonyms negative test") {
  std::string program = "stmt s1, s2; Select s1 such that NextBip*(s1, s2)";
  auto queryParser = new QueryParser(program);

  queryParser->parse();
  auto q = queryParser->getQuery();
  REQUIRE_FALSE(q.isInvalidQuery());

  auto evaluator = new QueryEvaluator(queryParser, generateNoNextBipPKB());
  auto rows = evaluator->evaluate()->getRows();
  REQUIRE(rows.empty());
}

TEST_CASE("NextBip* left Synonym right Wildcard positive test") {
  std::string program = "stmt s1; Select s1 such that NextBip*(s1, _)";
  auto queryParser = new QueryParser(program);

  queryParser->parse();
  auto q = queryParser->getQuery();
  REQUIRE_FALSE(q.isInvalidQuery());

  auto evaluator = new QueryEvaluator(queryParser, generateBipPKB());
  auto rows = evaluator->evaluate()->getRows();
  auto s1 = QuerySynonym(DesignEntityType::Stmt, "s1");
  RowList expected = createRowList(
      {
          {{s1, "1"}},
          {{s1, "2"}},
          {{s1, "3"}},
      });
  REQUIRE(expected.size() == rows.size());
  for (auto &r: expected) {
    REQUIRE(std::find(rows.begin(), rows.end(), r) != rows.end());
  }
}

TEST_CASE("NextBip* left Synonym right Wildcard negative test") {
  std::string program = "stmt s1; Select s1 such that NextBip*(s1, _)";
  auto queryParser = new QueryParser(program);

  queryParser->parse();
  auto q = queryParser->getQuery();
  REQUIRE_FALSE(q.isInvalidQuery());

  auto evaluator = new QueryEvaluator(queryParser, generateNoNextBipPKB());
  auto rows = evaluator->evaluate()->getRows();
  REQUIRE(rows.empty());
}

TEST_CASE("NextBip* left Synonym right Constant positive test") {
  std::string program = "stmt s1; Select s1 such that NextBip*(s1, 3)";
  auto queryParser = new QueryParser(program);

  queryParser->parse();
  auto q = queryParser->getQuery();
  REQUIRE_FALSE(q.isInvalidQuery());

  auto evaluator = new QueryEvaluator(queryParser, generateBipPKB());
  auto rows = evaluator->evaluate()->getRows();
  auto s1 = QuerySynonym(DesignEntityType::Stmt, "s1");
  RowList expected = createRowList(
      {
          {{s1, "1"}},
          {{s1, "2"}},
      });
  REQUIRE(expected.size() == rows.size());
  for (auto &r: expected) {
    REQUIRE(std::find(rows.begin(), rows.end(), r) != rows.end());
  }
}

TEST_CASE("NextBip* left Synonym right Constant negative test") {
  std::string program = "stmt s1; Select s1 such that NextBip*(s1, 1)";
  auto queryParser = new QueryParser(program);

  queryParser->parse();
  auto q = queryParser->getQuery();
  REQUIRE_FALSE(q.isInvalidQuery());

  auto evaluator = new QueryEvaluator(queryParser, generateBipPKB());
  auto rows = evaluator->evaluate()->getRows();
  REQUIRE(rows.empty());
}

TEST_CASE("NextBip* left Wildcard right Synonym positive test") {
  std::string program = "stmt s1; Select s1 such that NextBip*(_, s1)";
  auto queryParser = new QueryParser(program);

  queryParser->parse();
  auto q = queryParser->getQuery();
  REQUIRE_FALSE(q.isInvalidQuery());

  auto evaluator = new QueryEvaluator(queryParser, generateBipPKB());
  auto rows = evaluator->evaluate()->getRows();
  auto s1 = QuerySynonym(DesignEntityType::Stmt, "s1");
  RowList expected = createRowList(
      {
          {{s1, "2"}},
          {{s1, "3"}},
          {{s1, "4"}},
      });
  REQUIRE(expected.size() == rows.size());
  for (auto &r: expected) {
    REQUIRE(std::find(rows.begin(), rows.end(), r) != rows.end());
  }
}

TEST_CASE("NextBip* left Wildcard right Synonym negative test") {
  std::string program = "stmt s1; Select s1 such that NextBip*(_, s1)";
  auto queryParser = new QueryParser(program);

  queryParser->parse();
  auto q = queryParser->getQuery();
  REQUIRE_FALSE(q.isInvalidQuery());

  auto evaluator = new QueryEvaluator(queryParser, generateNoNextBipPKB());
  auto rows = evaluator->evaluate()->getRows();
  REQUIRE(rows.empty());
}

TEST_CASE("NextBip* both wildcard positive test") {
  std::string program = "stmt s1; Select s1 such that NextBip*(_, _)";
  auto queryParser = new QueryParser(program);

  queryParser->parse();
  auto q = queryParser->getQuery();
  REQUIRE_FALSE(q.isInvalidQuery());

  auto evaluator = new QueryEvaluator(queryParser, generateBipPKB());
  auto rows = evaluator->evaluate()->getRows();
  auto s1 = QuerySynonym(DesignEntityType::Stmt, "s1");
  RowList expected = createRowList(
      {
          {{s1, "1"}},
          {{s1, "2"}},
          {{s1, "3"}},
          {{s1, "4"}},
      });
  REQUIRE(expected.size() == rows.size());
  for (auto &r: expected) {
    REQUIRE(std::find(rows.begin(), rows.end(), r) != rows.end());
  }
}

TEST_CASE("NextBip* both wildcard negative test") {
  std::string program = "stmt s1; Select s1 such that NextBip*(_, _)";
  auto queryParser = new QueryParser(program);

  queryParser->parse();
  auto q = queryParser->getQuery();
  REQUIRE_FALSE(q.isInvalidQuery());

  auto evaluator = new QueryEvaluator(queryParser, generateNoNextBipPKB());
  auto rows = evaluator->evaluate()->getRows();
  REQUIRE(rows.empty());
}

TEST_CASE("NextBip* left Wildcard right Constant positive test") {
  std::string program = "stmt s1; Select s1 such that NextBip*(_, 3)";
  auto queryParser = new QueryParser(program);

  queryParser->parse();
  auto q = queryParser->getQuery();
  REQUIRE_FALSE(q.isInvalidQuery());

  auto evaluator = new QueryEvaluator(queryParser, generateBipPKB());
  auto rows = evaluator->evaluate()->getRows();
  auto s1 = QuerySynonym(DesignEntityType::Stmt, "s1");
  RowList expected = createRowList(
      {
          {{s1, "1"}},
          {{s1, "2"}},
          {{s1, "3"}},
          {{s1, "4"}},
      });
  REQUIRE(expected.size() == rows.size());
  for (auto &r: expected) {
    REQUIRE(std::find(rows.begin(), rows.end(), r) != rows.end());
  }
}

TEST_CASE("NextBip* left Wildcard right Constant negative test") {
  std::string program = "stmt s1; Select s1 such that NextBip*(_, 3)";
  auto queryParser = new QueryParser(program);

  queryParser->parse();
  auto q = queryParser->getQuery();
  REQUIRE_FALSE(q.isInvalidQuery());

  auto evaluator = new QueryEvaluator(queryParser, generateNoNextBipPKB());
  auto rows = evaluator->evaluate()->getRows();
  REQUIRE(rows.empty());
}

TEST_CASE("NextBip* left Constant right Synonym positive test") {
  std::string program = "stmt s1; Select s1 such that NextBip*(2, s1)";
  auto queryParser = new QueryParser(program);

  queryParser->parse();
  auto q = queryParser->getQuery();
  REQUIRE_FALSE(q.isInvalidQuery());

  auto evaluator = new QueryEvaluator(queryParser, generateBipPKB());
  auto rows = evaluator->evaluate()->getRows();
  auto s1 = QuerySynonym(DesignEntityType::Stmt, "s1");
  RowList expected = createRowList(
      {
          {{s1, "3"}},
          {{s1, "4"}},
      });
  REQUIRE(expected.size() == rows.size());
  for (auto &r: expected) {
    REQUIRE(std::find(rows.begin(), rows.end(), r) != rows.end());
  }
}

TEST_CASE("NextBip* left Constant right Synonym negative test") {
  std::string program = "stmt s1; Select s1 such that NextBip*(2, s1)";
  auto queryParser = new QueryParser(program);

  queryParser->parse();
  auto q = queryParser->getQuery();
  REQUIRE_FALSE(q.isInvalidQuery());

  auto evaluator = new QueryEvaluator(queryParser, generateNoNextBipPKB());
  auto rows = evaluator->evaluate()->getRows();
  REQUIRE(rows.empty());
}

TEST_CASE("NextBip* left Constant right Wildcard positive test") {
  std::string program = "stmt s1; Select s1 such that NextBip*(2, _)";
  auto queryParser = new QueryParser(program);

  queryParser->parse();
  auto q = queryParser->getQuery();
  REQUIRE_FALSE(q.isInvalidQuery());

  auto evaluator = new QueryEvaluator(queryParser, generateBipPKB());
  auto rows = evaluator->evaluate()->getRows();
  auto s1 = QuerySynonym(DesignEntityType::Stmt, "s1");
  RowList expected = createRowList(
      {
          {{s1, "1"}},
          {{s1, "2"}},
          {{s1, "3"}},
          {{s1, "4"}},
      });
  REQUIRE(expected.size() == rows.size());
  for (auto &r: expected) {
    REQUIRE(std::find(rows.begin(), rows.end(), r) != rows.end());
  }
}

TEST_CASE("NextBip* left Constant right Wildcard negative test") {
  std::string program = "stmt s1; Select s1 such that NextBip*(4, _)";
  auto queryParser = new QueryParser(program);

  queryParser->parse();
  auto q = queryParser->getQuery();
  REQUIRE_FALSE(q.isInvalidQuery());

  auto evaluator = new QueryEvaluator(queryParser, generateBipPKB());
  auto rows = evaluator->evaluate()->getRows();
  REQUIRE(rows.empty());
}

TEST_CASE("NextBip* both Constant positive test") {
  std::string program = "stmt s1; Select s1 such that NextBip*(2, 3)";
  auto queryParser = new QueryParser(program);

  queryParser->parse();
  auto q = queryParser->getQuery();
  REQUIRE_FALSE(q.isInvalidQuery());

  auto evaluator = new QueryEvaluator(queryParser, generateBipPKB());
  auto rows = evaluator->evaluate()->getRows();
  auto s1 = QuerySynonym(DesignEntityType::Stmt, "s1");
  RowList expected = createRowList(
      {
          {{s1, "1"}},
          {{s1, "2"}},
          {{s1, "3"}},
          {{s1, "4"}},
      });
  REQUIRE(expected.size() == rows.size());
  for (auto &r: expected) {
    REQUIRE(std::find(rows.begin(), rows.end(), r) != rows.end());
  }
}

TEST_CASE("NextBip* both Constant negative test") {
  std::string program = "stmt s1; Select s1 such that NextBip*(1, 2)";
  auto queryParser = new QueryParser(program);

  queryParser->parse();
  auto q = queryParser->getQuery();
  REQUIRE_FALSE(q.isInvalidQuery());

  auto evaluator = new QueryEvaluator(queryParser, generateNoNextBipPKB());
  auto rows = evaluator->evaluate()->getRows();
  REQUIRE(rows.empty());
}