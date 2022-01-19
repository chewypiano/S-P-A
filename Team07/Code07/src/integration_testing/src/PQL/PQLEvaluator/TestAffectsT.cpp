//
// Created by Jazer on 4/11/2021.
//


#include "catch.hpp"
#include <vector>
#include <string>
#include "QueryEvaluator.h"
#include "../../Utils/TestUtils.h"

TEST_CASE("Affects* both Synonym positive test") {
  std::string program = "assign a1, a2; Select a1 such that Affects*(a1, a2)";
  auto queryParser = new QueryParser(program);

  queryParser->parse();
  auto a = queryParser->getQuery();
  REQUIRE_FALSE(a.isInvalidQuery());

  QuerySynonym a1 = QuerySynonym(DesignEntityType::Assign, "a1");
  QuerySynonym a2 = QuerySynonym(DesignEntityType::Assign, "a2");
  auto evaluator = new QueryEvaluator(queryParser, generateAffectsPKB());
  auto rows = evaluator->evaluate()->getRows();
  RowList expected = createRowList(
      {
          {
              {a1, "1"},
              {a2, "2"},
          },
          {
              {a1, "2"},
              {a2, "3"},
          },
          {
              {a1, "1"},
              {a2, "3"},
          }
      }
  );
  REQUIRE(expected.size() == rows.size());
  for (auto &r: expected) {
    REQUIRE(std::find(rows.begin(), rows.end(), r) != rows.end());
  }
}

TEST_CASE("Affects* both Synonym negative test") {
  std::string program = "assign a1, a2; Select a1 such that Affects*(a1, a2)";
  auto queryParser = new QueryParser(program);

  queryParser->parse();
  auto a = queryParser->getQuery();
  REQUIRE_FALSE(a.isInvalidQuery());

  auto evaluator = new QueryEvaluator(queryParser, generateNoAffectsPKB());
  auto rows = evaluator->evaluate()->getRows();
  REQUIRE(rows.empty());
}

TEST_CASE("Affects* left Synonym right Wildcard positive test") {
  std::string program = "assign a1; Select a1 such that Affects*(a1, _)";
  auto queryParser = new QueryParser(program);

  queryParser->parse();
  auto a = queryParser->getQuery();
  REQUIRE_FALSE(a.isInvalidQuery());

  QuerySynonym a1 = QuerySynonym(DesignEntityType::Assign, "a1");
  auto evaluator = new QueryEvaluator(queryParser, generateAffectsPKB());
  auto rows = evaluator->evaluate()->getRows();
  RowList expected = createRowList({
                                       {{a1, "1"}},
                                       {{a1, "2"}},
                                   });
  REQUIRE(expected.size() == rows.size());
  for (auto &r: expected) {
    REQUIRE(std::find(rows.begin(), rows.end(), r) != rows.end());
  }
}

TEST_CASE("Affects* left Synonym right Wildcard negative test") {
  std::string program = "assign a1; Select a1 such that Affects*(a1, _)";
  auto queryParser = new QueryParser(program);

  queryParser->parse();
  auto a = queryParser->getQuery();
  REQUIRE_FALSE(a.isInvalidQuery());

  QuerySynonym a1 = QuerySynonym(DesignEntityType::Assign, "a1");
  auto evaluator = new QueryEvaluator(queryParser, generateNoAffectsPKB());
  auto rows = evaluator->evaluate()->getRows();
  REQUIRE(rows.empty());
}

TEST_CASE("Affects* left Synonym right constant positive test") {
  std::string program = "assign a1; Select a1 such that Affects*(a1, 3)";
  auto queryParser = new QueryParser(program);

  queryParser->parse();
  auto a = queryParser->getQuery();
  REQUIRE_FALSE(a.isInvalidQuery());

  QuerySynonym a1 = QuerySynonym(DesignEntityType::Assign, "a1");
  auto evaluator = new QueryEvaluator(queryParser, generateAffectsPKB());
  auto rows = evaluator->evaluate()->getRows();
  RowList expected = createRowList({
                                       {{a1, "1"}},
                                       {{a1, "2"}},
                                   });
  REQUIRE(expected.size() == rows.size());
  for (auto &r: expected) {
    REQUIRE(std::find(rows.begin(), rows.end(), r) != rows.end());
  }
}

TEST_CASE("Affects* left Synonym right constant negative test") {
  std::string program = "assign a1; Select a1 such that Affects*(a1, 1)";
  auto queryParser = new QueryParser(program);

  queryParser->parse();
  auto a = queryParser->getQuery();
  REQUIRE_FALSE(a.isInvalidQuery());

  QuerySynonym a1 = QuerySynonym(DesignEntityType::Assign, "a1");
  auto evaluator = new QueryEvaluator(queryParser, generateAffectsPKB());
  auto rows = evaluator->evaluate()->getRows();
  REQUIRE(rows.empty());
}

TEST_CASE("Affects* left Wildcard right Synonym positive test") {
  std::string program = "assign a1; Select a1 such that Affects*(1, a1)";
  auto queryParser = new QueryParser(program);

  queryParser->parse();
  auto a = queryParser->getQuery();
  REQUIRE_FALSE(a.isInvalidQuery());

  QuerySynonym a1 = QuerySynonym(DesignEntityType::Assign, "a1");
  auto evaluator = new QueryEvaluator(queryParser, generateAffectsPKB());
  auto rows = evaluator->evaluate()->getRows();
  RowList expected = createRowList({
                                       {{a1, "2"}},
                                       {{a1, "3"}},
                                   });
  REQUIRE(expected.size() == rows.size());
  for (auto &r: expected) {
    REQUIRE(std::find(rows.begin(), rows.end(), r) != rows.end());
  }
}

TEST_CASE("Affects* left Wildcard right Synonym negative test") {
  std::string program = "assign a1; Select a1 such that Affects*(3, a1)";
  auto queryParser = new QueryParser(program);

  queryParser->parse();
  auto a = queryParser->getQuery();
  REQUIRE_FALSE(a.isInvalidQuery());

  QuerySynonym a1 = QuerySynonym(DesignEntityType::Assign, "a1");
  auto evaluator = new QueryEvaluator(queryParser, generateAffectsPKB());
  auto rows = evaluator->evaluate()->getRows();
  REQUIRE(rows.empty());
}

TEST_CASE("Affects* both wildcard positive test") {
  std::string program = "assign a1; Select a1 such that Affects*(_, _)";
  auto queryParser = new QueryParser(program);

  queryParser->parse();
  auto a = queryParser->getQuery();
  REQUIRE_FALSE(a.isInvalidQuery());

  QuerySynonym a1 = QuerySynonym(DesignEntityType::Assign, "a1");
  auto evaluator = new QueryEvaluator(queryParser, generateAffectsPKB());
  auto rows = evaluator->evaluate()->getRows();
  RowList expected = createRowList({
                                       {{a1, "1"}},
                                       {{a1, "2"}},
                                       {{a1, "3"}},
                                   });
  REQUIRE(expected.size() == rows.size());
  for (auto &r: expected) {
    REQUIRE(std::find(rows.begin(), rows.end(), r) != rows.end());
  }
}

TEST_CASE("Affects* both wildcard negative test") {
  std::string program = "assign a1; Select a1 such that Affects*(_, _)";
  auto queryParser = new QueryParser(program);

  queryParser->parse();
  auto a = queryParser->getQuery();
  REQUIRE_FALSE(a.isInvalidQuery());

  QuerySynonym a1 = QuerySynonym(DesignEntityType::Assign, "a1");
  auto evaluator = new QueryEvaluator(queryParser, generateNoAffectsPKB());
  auto rows = evaluator->evaluate()->getRows();
  REQUIRE(rows.empty());
}

TEST_CASE("Affects* left Wildcard right constant positive test") {
  std::string program = "assign a1; Select a1 such that Affects*(_, 3)";
  auto queryParser = new QueryParser(program);

  queryParser->parse();
  auto a = queryParser->getQuery();
  REQUIRE_FALSE(a.isInvalidQuery());

  QuerySynonym a1 = QuerySynonym(DesignEntityType::Assign, "a1");
  auto evaluator = new QueryEvaluator(queryParser, generateAffectsPKB());
  auto rows = evaluator->evaluate()->getRows();
  RowList expected = createRowList({
                                       {{a1, "1"}},
                                       {{a1, "2"}},
                                       {{a1, "3"}},
                                   });
  REQUIRE(expected.size() == rows.size());
  for (auto &r: expected) {
    REQUIRE(std::find(rows.begin(), rows.end(), r) != rows.end());
  }
}

TEST_CASE("Affects* left Wildcard right constant negative test") {
  std::string program = "assign a1; Select a1 such that Affects*(_, 1)";
  auto queryParser = new QueryParser(program);

  queryParser->parse();
  auto a = queryParser->getQuery();
  REQUIRE_FALSE(a.isInvalidQuery());

  QuerySynonym a1 = QuerySynonym(DesignEntityType::Assign, "a1");
  auto evaluator = new QueryEvaluator(queryParser, generateAffectsPKB());
  auto rows = evaluator->evaluate()->getRows();
  REQUIRE(rows.empty());
}

TEST_CASE("Affects* left Constant right Synonym positive test") {
  std::string program = "assign a1; Select a1 such that Affects*(1, a1)";
  auto queryParser = new QueryParser(program);

  queryParser->parse();
  auto a = queryParser->getQuery();
  REQUIRE_FALSE(a.isInvalidQuery());

  QuerySynonym a1 = QuerySynonym(DesignEntityType::Assign, "a1");
  auto evaluator = new QueryEvaluator(queryParser, generateAffectsPKB());
  auto rows = evaluator->evaluate()->getRows();
  RowList expected = createRowList({
                                       {{a1, "2"}},
                                       {{a1, "3"}},
                                   });
  REQUIRE(expected.size() == rows.size());
  for (auto &r: expected) {
    REQUIRE(std::find(rows.begin(), rows.end(), r) != rows.end());
  }
}

TEST_CASE("Affects* left Constant right Synonym negative test") {
  std::string program = "assign a1; Select a1 such that Affects*(3, a1)";
  auto queryParser = new QueryParser(program);

  queryParser->parse();
  auto a = queryParser->getQuery();
  REQUIRE_FALSE(a.isInvalidQuery());

  QuerySynonym a1 = QuerySynonym(DesignEntityType::Assign, "a1");
  auto evaluator = new QueryEvaluator(queryParser, generateAffectsPKB());
  auto rows = evaluator->evaluate()->getRows();
  REQUIRE(rows.empty());
}

TEST_CASE("Affects* left Constant right Wildcard positive test") {
  std::string program = "assign a1; Select a1 such that Affects*(1, _)";
  auto queryParser = new QueryParser(program);

  queryParser->parse();
  auto a = queryParser->getQuery();
  REQUIRE_FALSE(a.isInvalidQuery());

  QuerySynonym a1 = QuerySynonym(DesignEntityType::Assign, "a1");
  auto evaluator = new QueryEvaluator(queryParser, generateAffectsPKB());
  auto rows = evaluator->evaluate()->getRows();
  RowList expected = createRowList({
                                       {{a1, "1"}},
                                       {{a1, "2"}},
                                       {{a1, "3"}},
                                   });
  REQUIRE(expected.size() == rows.size());
  for (auto &r: expected) {
    REQUIRE(std::find(rows.begin(), rows.end(), r) != rows.end());
  }
}

TEST_CASE("Affects* left Constant right Wildcard negative test") {
  std::string program = "assign a1; Select a1 such that Affects*(3, _)";
  auto queryParser = new QueryParser(program);

  queryParser->parse();
  auto a = queryParser->getQuery();
  REQUIRE_FALSE(a.isInvalidQuery());

  QuerySynonym a1 = QuerySynonym(DesignEntityType::Assign, "a1");
  auto evaluator = new QueryEvaluator(queryParser, generateAffectsPKB());
  auto rows = evaluator->evaluate()->getRows();
  REQUIRE(rows.empty());
}

TEST_CASE("Affects* both constants positive test") {
  std::string program = "assign a1; Select a1 such that Affects*(1, 3)";
  auto queryParser = new QueryParser(program);

  queryParser->parse();
  auto a = queryParser->getQuery();
  REQUIRE_FALSE(a.isInvalidQuery());

  QuerySynonym a1 = QuerySynonym(DesignEntityType::Assign, "a1");
  auto evaluator = new QueryEvaluator(queryParser, generateAffectsPKB());
  auto rows = evaluator->evaluate()->getRows();
  RowList expected = createRowList({
                                       {{a1, "1"}},
                                       {{a1, "2"}},
                                       {{a1, "3"}},
                                   });
  REQUIRE(expected.size() == rows.size());
  for (auto &r: expected) {
    REQUIRE(std::find(rows.begin(), rows.end(), r) != rows.end());
  }
}

TEST_CASE("Affects* both constants negative test") {
  std::string program = "assign a1; Select a1 such that Affects*(3, 1)";
  auto queryParser = new QueryParser(program);

  queryParser->parse();
  auto a = queryParser->getQuery();
  REQUIRE_FALSE(a.isInvalidQuery());

  QuerySynonym a1 = QuerySynonym(DesignEntityType::Assign, "a1");
  auto evaluator = new QueryEvaluator(queryParser, generateAffectsPKB());
  auto rows = evaluator->evaluate()->getRows();
  REQUIRE(rows.empty());
}

TEST_CASE("Affects* both constants out of range test") {
  std::string program = "assign a1; Select a1 such that Affects*(4, 4)";
  auto queryParser = new QueryParser(program);

  queryParser->parse();
  auto a = queryParser->getQuery();
  REQUIRE_FALSE(a.isInvalidQuery());

  QuerySynonym a1 = QuerySynonym(DesignEntityType::Assign, "a1");
  auto evaluator = new QueryEvaluator(queryParser, generateAffectsPKB());
  auto rows = evaluator->evaluate()->getRows();
  REQUIRE(rows.empty());
}

TEST_CASE("Affects* left wildcard out of range test") {
  std::string program = "assign a1; Select a1 such that Affects*(_, 4)";
  auto queryParser = new QueryParser(program);

  queryParser->parse();
  auto a = queryParser->getQuery();
  REQUIRE_FALSE(a.isInvalidQuery());

  QuerySynonym a1 = QuerySynonym(DesignEntityType::Assign, "a1");
  auto evaluator = new QueryEvaluator(queryParser, generateAffectsPKB());
  auto rows = evaluator->evaluate()->getRows();
  REQUIRE(rows.empty());
}

TEST_CASE("Affects* right wildcard out of range test") {
  std::string program = "assign a1; Select a1 such that Affects*(4, _)";
  auto queryParser = new QueryParser(program);

  queryParser->parse();
  auto a = queryParser->getQuery();
  REQUIRE_FALSE(a.isInvalidQuery());

  QuerySynonym a1 = QuerySynonym(DesignEntityType::Assign, "a1");
  auto evaluator = new QueryEvaluator(queryParser, generateAffectsPKB());
  auto rows = evaluator->evaluate()->getRows();
  REQUIRE(rows.empty());
}

TEST_CASE("Affects* left syn out of range test") {
  std::string program = "assign a1; Select a1 such that Affects*(a1, 4)";
  auto queryParser = new QueryParser(program);

  queryParser->parse();
  auto a = queryParser->getQuery();
  REQUIRE_FALSE(a.isInvalidQuery());

  QuerySynonym a1 = QuerySynonym(DesignEntityType::Assign, "a1");
  auto evaluator = new QueryEvaluator(queryParser, generateAffectsPKB());
  auto rows = evaluator->evaluate()->getRows();
  REQUIRE(rows.empty());
}

TEST_CASE("Affects* right syn out of range test") {
  std::string program = "assign a1; Select a1 such that Affects*(4, a1)";
  auto queryParser = new QueryParser(program);

  queryParser->parse();
  auto a = queryParser->getQuery();
  REQUIRE_FALSE(a.isInvalidQuery());

  QuerySynonym a1 = QuerySynonym(DesignEntityType::Assign, "a1");
  auto evaluator = new QueryEvaluator(queryParser, generateAffectsPKB());
  auto rows = evaluator->evaluate()->getRows();
  REQUIRE(rows.empty());
}