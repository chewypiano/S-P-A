//
// Created by Junhua on 6/11/2021.
//

#include "catch.hpp"
#include <vector>
#include <string>
#include "QueryEvaluator.h"
#include "../../Utils/TestUtils.h"

TEST_CASE("Test QE no common between such that and pattern") {
  std::string program = R"(stmt a; assign a1; Select a such that Uses(a, "x") pattern a1("x",_"y"_))";
  auto pkb = new PKB();
  pkb->addProc("Unused");
  pkb->addStatement(1, assign, "Unused");
  pkb->addStatement(2, assign, "Unused");
  pkb->addStatement(3, assign, "Unused");
  pkb->addVariable("x");
  pkb->addVariable("y");
  pkb->addFollow(1, 2);
  pkb->addFollow(2, 3);
  pkb->addFollowStar(1, 2);
  pkb->addFollowStar(2, 3);
  pkb->addFollowStar(1, 3);
  pkb->addUseStmt(1, "x");
  pkb->addUseStmt(3, "x");
  pkb->addUseStmt(3, "y");
  pkb->addModifyStmt(3, "x");
  pkb->addAssignPattern(3, "x", "x + 3 + y");
  pkb->addSubString(3, " x 3 + y + ");
  auto queryParser = new QueryParser(program);
  queryParser->parse();

  auto a = (queryParser->getQuery());
  REQUIRE(a.toString() != "Query(\n"
                          "  INVALID QUERY\n"
                          ")");

  auto evaluator = new QueryEvaluator(queryParser, pkb);

  RowList expected = createRowList(
      {
          {
              {QuerySynonym(DesignEntityType::Stmt, "a"), "1"},

          },
          {
              {QuerySynonym(DesignEntityType::Stmt, "a"), "3"},
          }
      }
  );

  auto rows = evaluator->evaluate()->getRows();
  REQUIRE(expected.size() == rows.size());
  for (auto &r: expected) {
    REQUIRE(std::find(rows.begin(), rows.end(), r) != rows.end());
  }
}

TEST_CASE("Test QE no common between select and others") {
  std::string program = R"(stmt c; assign a; variable v; Select c such that Uses(a,v) pattern a(v,_))";
  auto pkb = new PKB();
  pkb->addProc("Unused");
  pkb->addStatement(1, assign, "Unused");
  pkb->addStatement(2, assign, "Unused");
  pkb->addStatement(3, assign, "Unused");
  pkb->addStatement(4, call, "Unused");
  pkb->addVariable("x");
  pkb->addVariable("y");
  pkb->addFollow(1, 2);
  pkb->addFollow(2, 3);
  pkb->addFollowStar(1, 2);
  pkb->addFollowStar(2, 3);
  pkb->addFollowStar(1, 3);
  pkb->addUseStmt(1, "x");
  pkb->addUseStmt(3, "x");
  pkb->addUseStmt(3, "y");
  pkb->addModifyStmt(3, "x");
  pkb->addAssignPattern(3, "x", "x + 3 + y");
  auto queryParser = new QueryParser(program);
  queryParser->parse();

  auto a = (queryParser->getQuery());
  REQUIRE(a.toString() != "Query(\n"
                          "  INVALID QUERY\n"
                          ")");

  auto evaluator = new QueryEvaluator(queryParser, pkb);

  RowList expected = createRowList(
      {
          {
              {QuerySynonym(DesignEntityType::Stmt, "c"), "3"},
              {QuerySynonym(DesignEntityType::Assign, "a"), "3"},
              {QuerySynonym(DesignEntityType::Variable, "v"), "x"},
          },
          {
              {QuerySynonym(DesignEntityType::Stmt, "c"), "4"},
              {QuerySynonym(DesignEntityType::Assign, "a"), "3"},
              {QuerySynonym(DesignEntityType::Variable, "v"), "x"},
          }, {
              {QuerySynonym(DesignEntityType::Stmt, "c"), "1"},
              {QuerySynonym(DesignEntityType::Assign, "a"), "3"},
              {QuerySynonym(DesignEntityType::Variable, "v"), "x"},
          }, {
              {QuerySynonym(DesignEntityType::Stmt, "c"), "2"},
              {QuerySynonym(DesignEntityType::Assign, "a"), "3"},
              {QuerySynonym(DesignEntityType::Variable, "v"), "x"},
          }
      }
  );

  auto rows = evaluator->evaluate()->getRows();
  REQUIRE(expected.size() == rows.size());
  for (auto &r: expected) {
    REQUIRE(std::find(rows.begin(), rows.end(), r) != rows.end());
  }
}

TEST_CASE("Test QE no common between select and others original error query") {
  std::string program = R"(constant c; assign a; variable v; Select c such that Uses(a,v) pattern a(v,_))";
  auto pkb = new PKB();
  pkb->addProc("Unused");
  pkb->addStatement(1, assign, "Unused");
  pkb->addStatement(2, print, "Unused");
  pkb->addStatement(3, assign, "Unused");
  pkb->addVariable("x");
  pkb->addVariable("y");
  pkb->addConst("3");
  pkb->addFollow(1, 2);
  pkb->addFollow(2, 3);
  pkb->addFollowStar(1, 2);
  pkb->addFollowStar(2, 3);
  pkb->addFollowStar(1, 3);

// x = x
  pkb->addUseStmt(1, "x");
  pkb->addModifyStmt(1, "x");
  pkb->addAssignPattern(1, "x", "x");

// print x
  pkb->addUseStmt(2, "x");

// x = x + 3 + y
  pkb->addModifyStmt(3, "x");
  pkb->addUseStmt(3, "x");
  pkb->addUseStmt(3, "y");
  pkb->addAssignPattern(3, "x", "x + 3 + y");
  auto queryParser = new QueryParser(program);
  queryParser->parse();

  auto a = (queryParser->getQuery());
  REQUIRE(a.toString() != "Query(\n"
                          "  INVALID QUERY\n"
                          ")");

  auto evaluator = new QueryEvaluator(queryParser, pkb);

  RowList expected = createRowList(
      {
          {
              {QuerySynonym(DesignEntityType::Constant, "c"), "3"},
              {QuerySynonym(DesignEntityType::Assign, "a"), "1"},
              {QuerySynonym(DesignEntityType::Variable, "v"), "x"},
          },

          {
              {QuerySynonym(DesignEntityType::Constant, "c"), "3"},
              {QuerySynonym(DesignEntityType::Assign, "a"), "3"},
              {QuerySynonym(DesignEntityType::Variable, "v"), "x"},
          }
      }
  );

  auto rows = evaluator->evaluate()->getRows();
  REQUIRE(expected.size() == rows.size());
  for (auto &r: expected) {
    REQUIRE(std::find(rows.begin(), rows.end(), r) != rows.end());
  }
}

TEST_CASE("Test multiple clauses") {
  std::string program =
      R"(stmt a, b, c; Select a such that Follows(a, b) and Follows*(a,c) such that Follows(b, c) and Follows*(a, c) and Follows*(b,c))";
  auto pkb = new PKB();
  pkb->addProc("Unused");
  pkb->addStatement(1, assign, "Unused");
  pkb->addStatement(2, print, "Unused");
  pkb->addStatement(3, assign, "Unused");
  pkb->addVariable("x");
  pkb->addVariable("y");
  pkb->addConst("3");
  pkb->addFollow(1, 2);
  pkb->addFollow(2, 3);
  pkb->addFollowStar(1, 2);
  pkb->addFollowStar(2, 3);
  pkb->addFollowStar(1, 3);

  // x = x
  pkb->addUseStmt(1, "x");
  pkb->addModifyStmt(1, "x");
  pkb->addAssignPattern(1, "x", "x");

  // print x
  pkb->addUseStmt(2, "x");

  // x = x + 3 + y
  pkb->addModifyStmt(3, "x");
  pkb->addUseStmt(3, "x");
  pkb->addUseStmt(3, "y");
  pkb->addAssignPattern(3, "x", "x + 3 + y");
  auto queryParser = new QueryParser(program);
  queryParser->parse();

  auto a = (queryParser->getQuery());
  REQUIRE(a.toString() != "Query(\n"
                          "  INVALID QUERY\n"
                          ")");

  auto evaluator = new QueryEvaluator(queryParser, pkb);

  RowList expected = createRowList(
      {
          {
              {QuerySynonym(DesignEntityType::Stmt, "a"), "1"},
              {QuerySynonym(DesignEntityType::Stmt, "b"), "2"},
              {QuerySynonym(DesignEntityType::Stmt, "c"), "3"}
          }
      }
  );

  auto rows = evaluator->evaluate()->getRows();
  REQUIRE(expected.size() == rows.size());
  for (auto &r: expected) {
    REQUIRE(std::find(rows.begin(), rows.end(), r) != rows.end());
  }
}