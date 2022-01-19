//
// Created by Jazer on 4/11/2021.
//

#include "catch.hpp"
#include <vector>
#include <string>
#include "QueryEvaluator.h"
#include "../../Utils/TestUtils.h"

TEST_CASE("AffectsBip both Synonyms positive test") {
  std::string program = "assign a1, a2; Select a1 such that AffectsBip(a1, a2)";
  auto queryParser = new QueryParser(program);

  queryParser->parse();
  auto q = queryParser->getQuery();
  REQUIRE_FALSE(q.isInvalidQuery());

  auto evaluator = new QueryEvaluator(queryParser, generateBipPKB());
  auto rows = evaluator->evaluate()->getRows();
  auto a1 = QuerySynonym(DesignEntityType::Assign, "a1");
  auto a2 = QuerySynonym(DesignEntityType::Assign, "a2");
  RowList expected = createRowList(
      {
          {{a1, "1"}, {a2, "3"}},
          {{a1, "1"}, {a2, "4"}},
          {{a1, "3"}, {a2, "4"}},
      }
  );
  REQUIRE(expected.size() == rows.size());
  for (auto &r: expected) {
    REQUIRE(std::find(rows.begin(), rows.end(), r) != rows.end());
  }
}

TEST_CASE("AffectsBip both Synonyms negative test") {
  std::string program = "assign a1, a2; Select a1 such that AffectsBip(a1, a2)";
  auto queryParser = new QueryParser(program);

  queryParser->parse();
  auto q = queryParser->getQuery();
  REQUIRE_FALSE(q.isInvalidQuery());

  auto evaluator = new QueryEvaluator(queryParser, generateNoNextBipPKB());
  auto rows = evaluator->evaluate()->getRows();
  REQUIRE(rows.empty());
}

TEST_CASE("AffectsBip left Synonym right Wildcard positive test") {
  std::string program = "assign a1; Select a1 such that AffectsBip(a1, _)";
  auto queryParser = new QueryParser(program);

  queryParser->parse();
  auto q = queryParser->getQuery();
  REQUIRE_FALSE(q.isInvalidQuery());

  auto evaluator = new QueryEvaluator(queryParser, generateBipPKB());
  auto rows = evaluator->evaluate()->getRows();
  auto a1 = QuerySynonym(DesignEntityType::Assign, "a1");
  RowList expected = createRowList(
      {
          {{a1, "1"}},
          {{a1, "3"}}
      }
  );
  REQUIRE(expected.size() == rows.size());
  for (auto &r: expected) {
    REQUIRE(std::find(rows.begin(), rows.end(), r) != rows.end());
  }
}

TEST_CASE("AffectsBip left Synonym right Wildcard negative test") {
  std::string program = "assign a1; Select a1 such that AffectsBip(a1, _)";
  auto queryParser = new QueryParser(program);

  queryParser->parse();
  auto q = queryParser->getQuery();
  REQUIRE_FALSE(q.isInvalidQuery());

  auto evaluator = new QueryEvaluator(queryParser, generateNoNextBipPKB());
  auto rows = evaluator->evaluate()->getRows();
  REQUIRE(rows.empty());
}

TEST_CASE("AffectsBip left Synonym right Constant positive test") {
  std::string program = "assign a1; Select a1 such that AffectsBip(a1, 3)";
  auto queryParser = new QueryParser(program);

  queryParser->parse();
  auto q = queryParser->getQuery();
  REQUIRE_FALSE(q.isInvalidQuery());

  auto evaluator = new QueryEvaluator(queryParser, generateBipPKB());
  auto rows = evaluator->evaluate()->getRows();
  auto a1 = QuerySynonym(DesignEntityType::Assign, "a1");
  RowList expected = createRowList(
      {
          {{a1, "1"}}
      }
  );
  REQUIRE(expected.size() == rows.size());
  for (auto &r: expected) {
    REQUIRE(std::find(rows.begin(), rows.end(), r) != rows.end());
  }
}

TEST_CASE("AffectsBip left Synonym right Constant negative test") {
  std::string program = "assign a1; Select a1 such that AffectsBip(a1, 3)";
  auto queryParser = new QueryParser(program);

  queryParser->parse();
  auto q = queryParser->getQuery();
  REQUIRE_FALSE(q.isInvalidQuery());

  auto evaluator = new QueryEvaluator(queryParser, generateNoNextBipPKB());
  auto rows = evaluator->evaluate()->getRows();
  REQUIRE(rows.empty());
}

TEST_CASE("AffectsBip left Wildcard right Synonym positive test") {
  std::string program = "assign a1; Select a1 such that AffectsBip(_, a1)";
  auto queryParser = new QueryParser(program);

  queryParser->parse();
  auto q = queryParser->getQuery();
  REQUIRE_FALSE(q.isInvalidQuery());

  auto evaluator = new QueryEvaluator(queryParser, generateBipPKB());
  auto rows = evaluator->evaluate()->getRows();
  auto a1 = QuerySynonym(DesignEntityType::Assign, "a1");
  RowList expected = createRowList(
      {
          {{a1, "3"}},
          {{a1, "4"}}
      }
  );
  REQUIRE(expected.size() == rows.size());
  for (auto &r: expected) {
    REQUIRE(std::find(rows.begin(), rows.end(), r) != rows.end());
  }
}

TEST_CASE("AffectsBip left Wildcard right Synonym negative test") {
  std::string program = "assign a1; Select a1 such that AffectsBip(_, a1)";
  auto queryParser = new QueryParser(program);

  queryParser->parse();
  auto q = queryParser->getQuery();
  REQUIRE_FALSE(q.isInvalidQuery());

  auto evaluator = new QueryEvaluator(queryParser, generateNoAffectsBipPKB());
  auto rows = evaluator->evaluate()->getRows();
  REQUIRE(rows.empty());
}

TEST_CASE("AffectsBip both Wildcard positive test") {
  std::string program = "assign a1; Select a1 such that AffectsBip(_, _)";
  auto queryParser = new QueryParser(program);

  queryParser->parse();
  auto q = queryParser->getQuery();
  REQUIRE_FALSE(q.isInvalidQuery());

  auto evaluator = new QueryEvaluator(queryParser, generateBipPKB());
  auto rows = evaluator->evaluate()->getRows();
  auto a1 = QuerySynonym(DesignEntityType::Assign, "a1");
  RowList expected = createRowList(
      {
          {{a1, "1"}},
          {{a1, "3"}},
          {{a1, "4"}}
      }
  );
  REQUIRE(expected.size() == rows.size());
  for (auto &r: expected) {
    REQUIRE(std::find(rows.begin(), rows.end(), r) != rows.end());
  }
}

TEST_CASE("AffectsBip both Wildcard negative test") {
  std::string program = "assign a1; Select a1 such that AffectsBip(_, _)";
  auto queryParser = new QueryParser(program);

  queryParser->parse();
  auto q = queryParser->getQuery();
  REQUIRE_FALSE(q.isInvalidQuery());

  auto evaluator = new QueryEvaluator(queryParser, generateNoAffectsBipPKB());
  auto rows = evaluator->evaluate()->getRows();

  REQUIRE(rows.empty());
}

TEST_CASE("AffectsBip left Wildcard right Constant positive test") {
  std::string program = "assign a1; Select a1 such that AffectsBip(_, 3)";
  auto queryParser = new QueryParser(program);

  queryParser->parse();
  auto q = queryParser->getQuery();
  REQUIRE_FALSE(q.isInvalidQuery());

  auto evaluator = new QueryEvaluator(queryParser, generateBipPKB());
  auto rows = evaluator->evaluate()->getRows();
  auto a1 = QuerySynonym(DesignEntityType::Assign, "a1");
  RowList expected = createRowList(
      {
          {
              {a1, "1"},

          }, {
              {a1, "3"},

          }, {
              {a1, "4"},
          }
      }
  );
  REQUIRE(expected.size() == rows.size());
  for (auto &r: expected) {
    REQUIRE(std::find(rows.begin(), rows.end(), r) != rows.end());
  }
}

TEST_CASE("AffectsBip left Wildcard right Constant negative test") {
  std::string program = "assign a1; Select a1 such that AffectsBip(_, 3)";
  auto queryParser = new QueryParser(program);

  queryParser->parse();
  auto q = queryParser->getQuery();
  REQUIRE_FALSE(q.isInvalidQuery());

  auto evaluator = new QueryEvaluator(queryParser, generateNoNextBipPKB());
  auto rows = evaluator->evaluate()->getRows();
  REQUIRE(rows.empty());
}

TEST_CASE("AffectsBip left Constant right Synonym positive test") {
  std::string program = "assign a1; Select a1 such that AffectsBip(3, a1)";
  auto queryParser = new QueryParser(program);

  queryParser->parse();
  auto q = queryParser->getQuery();
  REQUIRE_FALSE(q.isInvalidQuery());

  auto evaluator = new QueryEvaluator(queryParser, generateBipPKB());
  auto rows = evaluator->evaluate()->getRows();
  auto a1 = QuerySynonym(DesignEntityType::Assign, "a1");
  RowList expected = createRowList(
      {
          {{a1, "4"}}
      }
  );
  REQUIRE(expected.size() == rows.size());
  for (auto &r: expected) {
    REQUIRE(std::find(rows.begin(), rows.end(), r) != rows.end());
  }
}

TEST_CASE("AffectsBip left Constant right Synonym negative test") {
  std::string program = "assign a1; Select a1 such that AffectsBip(1, a1)";
  auto queryParser = new QueryParser(program);

  queryParser->parse();
  auto q = queryParser->getQuery();
  REQUIRE_FALSE(q.isInvalidQuery());

  auto evaluator = new QueryEvaluator(queryParser, generateNoNextBipPKB());
  auto rows = evaluator->evaluate()->getRows();
  REQUIRE(rows.empty());
}

TEST_CASE("AffectsBip left Constant right Wildcard positive test") {
  std::string program = "assign a1; Select a1 such that AffectsBip(3, _)";
  auto queryParser = new QueryParser(program);

  queryParser->parse();
  auto q = queryParser->getQuery();
  REQUIRE_FALSE(q.isInvalidQuery());

  auto evaluator = new QueryEvaluator(queryParser, generateBipPKB());
  auto rows = evaluator->evaluate()->getRows();
  auto a1 = QuerySynonym(DesignEntityType::Assign, "a1");
  RowList expected = createRowList(
      {
          {{a1, "1"}},
          {{a1, "3"}},
          {{a1, "4"}}
      }
  );
  REQUIRE(expected.size() == rows.size());
  for (auto &r: expected) {
    REQUIRE(std::find(rows.begin(), rows.end(), r) != rows.end());
  }
}

TEST_CASE("AffectsBip left Constant right Wildcard negative test") {
  std::string program = "assign a1; Select a1 such that AffectsBip(1, _)";
  auto queryParser = new QueryParser(program);

  queryParser->parse();
  auto q = queryParser->getQuery();
  REQUIRE_FALSE(q.isInvalidQuery());

  auto evaluator = new QueryEvaluator(queryParser, generateNoNextBipPKB());
  auto rows = evaluator->evaluate()->getRows();
  REQUIRE(rows.empty());
}

TEST_CASE("AffectsBip both Constant positive test") {
  std::string program = "assign a1; Select a1 such that AffectsBip(3, 4)";
  auto queryParser = new QueryParser(program);

  queryParser->parse();
  auto q = queryParser->getQuery();
  REQUIRE_FALSE(q.isInvalidQuery());

  auto evaluator = new QueryEvaluator(queryParser, generateBipPKB());
  auto rows = evaluator->evaluate()->getRows();
  auto a1 = QuerySynonym(DesignEntityType::Assign, "a1");
  RowList expected = createRowList(
      {
          {{a1, "1"}},
          {{a1, "3"}},
          {{a1, "4"}}
      }
  );
  REQUIRE(expected.size() == rows.size());
  for (auto &r: expected) {
    REQUIRE(std::find(rows.begin(), rows.end(), r) != rows.end());
  }
}

TEST_CASE("AffectsBip both Constant negative test") {
  std::string program = "assign a1; Select a1 such that AffectsBip(1, 2)";
  auto queryParser = new QueryParser(program);

  queryParser->parse();
  auto q = queryParser->getQuery();
  REQUIRE_FALSE(q.isInvalidQuery());

  auto evaluator = new QueryEvaluator(queryParser, generateNoNextBipPKB());
  auto rows = evaluator->evaluate()->getRows();
  REQUIRE(rows.empty());
}
