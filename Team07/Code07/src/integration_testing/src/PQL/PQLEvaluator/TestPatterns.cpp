//
// Created by Junhua on 6/11/2021.
//

#include <string>
#include "catch.hpp"
#include "PKB/PKB.h"
#include "QueryParser.h"
#include "QueryEvaluator.h"
#include "../../Utils/TestUtils.h"

TEST_CASE("Test assign pattern with 2 wildcards v2") {
  std::string program = "assign a; Select a pattern a(_,_)";
  auto pkb = new PKB();
  pkb->addProc("Unused");
  pkb->addStatement(1, assign, "Unused");
  pkb->addStatement(2, assign, "Unused");
  pkb->addVariable("x");
  pkb->addUseStmt(1, "x");
  pkb->addUseStmt(2, "x");

  auto queryParser = new QueryParser(program);
  queryParser->
      parse();
  auto evaluator = new QueryEvaluator(queryParser, pkb);

  RowList expectedOutput = createRowList(
      {
          {
              {QuerySynonym(DesignEntityType::Assign, "a"), "1"},

          }, {
              {QuerySynonym(DesignEntityType::Assign, "a"), "2"},
          }
      }
  );

  auto result = evaluator->evaluate()->getRows();
  REQUIRE(result
              .
                  size()
              == expectedOutput.
                  size()
  );
  for (
    auto &r
      : expectedOutput) {
    REQUIRE(std::find(result.begin(), result.end(), r)
                != result.
                    end()
    );
  }
}

TEST_CASE("Test assign pattern with (ident, _)") {
  std::string program = "assign a; Select a pattern a(\"x\",_)";
  auto pkb = new PKB();
  pkb->addProc("Unused");
  pkb->addStatement(1, assign, "Unused");
  pkb->addStatement(2, assign, "Unused");
  pkb->addStatement(3, assign, "Unused");
  pkb->addVariable("x");
  pkb->addUseStmt(1, "x");
  pkb->addUseStmt(2, "x");
  pkb->addModifyStmt(3, "x");
  pkb->addAssignPattern(3, "x", "x + 3");
  auto queryParser = new QueryParser(program);
  queryParser->
      parse();
  auto evaluator = new QueryEvaluator(queryParser, pkb);

  RowList expectedOutput = createRowList(
      {
          {
              {QuerySynonym(DesignEntityType::Assign, "a"), "3"}
          }
      }
  );

  auto result = evaluator->evaluate()->getRows();
  REQUIRE(result
              .
                  size()
              == expectedOutput.
                  size()
  );
  for (
    auto &r
      : expectedOutput) {
    REQUIRE(std::find(result.begin(), result.end(), r)
                != result.
                    end()
    );
  }
}

TEST_CASE("Test assign pattern with (ident, _ident_)") {
  std::string program = R"(assign a; Select a pattern a("x",_"x"_))";
  auto pkb = new PKB();
  pkb->addProc("Unused");
  pkb->addStatement(1, assign, "Unused");
  pkb->addStatement(2, assign, "Unused");
  pkb->addStatement(3, assign, "Unused");
  pkb->addVariable("x");
  pkb->addUseStmt(1, "x");
  pkb->addModifyStmt(3, "x");
  pkb->addAssignPattern(3, "x", "x + 3");
  pkb->addSubString(3, " x 3 + ");
  auto queryParser = new QueryParser(program);
  queryParser->
      parse();

  auto evaluator = new QueryEvaluator(queryParser, pkb);
  RowList expectedOutput = createRowList(
      {
          {
              {QuerySynonym(DesignEntityType::Assign, "a"), "3"}
          }
      }
  );

  auto result = evaluator->evaluate()->getRows();
  REQUIRE(result
              .
                  size()
              == expectedOutput.
                  size()
  );
  for (
    auto &r
      : expectedOutput) {
    REQUIRE(std::find(result.begin(), result.end(), r)
                != result.
                    end()
    );
  }
}

TEST_CASE("Test assign pattern (ident, ident)") {
  std::string program = R"(assign a; Select a pattern a("x","x"))";
  auto pkb = new PKB();
  pkb->addProc("Unused");
  pkb->addStatement(1, assign, "Unused");
  pkb->addStatement(2, assign, "Unused");
  pkb->addStatement(3, assign, "Unused");
  pkb->addVariable("x");
  pkb->addUseStmt(1, "x");
  pkb->addModifyStmt(3, "x");
  pkb->addAssignPattern(3, "x", "x + 3");
  auto queryParser = new QueryParser(program);
  queryParser->
      parse();

  auto evaluator = new QueryEvaluator(queryParser, pkb);

  RowList expectedOutput = createRowList({});
  auto result = evaluator->evaluate()->getRows();
  REQUIRE(result
              .
                  size()
              == expectedOutput.
                  size()
  );
  for (
    auto &r
      : expectedOutput) {
    REQUIRE(std::find(result.begin(), result.end(), r)
                != result.
                    end()
    );
  }
}

TEST_CASE("Test pattern assign (_,_)") {
  std::string program = R"(assign a; Select a pattern a(_,_))";
  auto pkb = new PKB();
  pkb->addProc("Unused");
  pkb->addStatement(1, assign, "Unused");
  pkb->addStatement(2, assign, "Unused");
  pkb->addStatement(3, assign, "Unused");
  pkb->addVariable("x");
  pkb->addUseStmt(1, "x");
  pkb->addModifyStmt(3, "x");
  pkb->addAssignPattern(3, "x", "x + 3");
  auto queryParser = new QueryParser(program);
  queryParser->
      parse();

  auto evaluator = new QueryEvaluator(queryParser, pkb);

  RowList expected = createRowList(
      {
          {
              {QuerySynonym(DesignEntityType::Assign, "a"), "1"},
          }, {
              {QuerySynonym(DesignEntityType::Assign, "a"), "2"},
          }, {
              {QuerySynonym(DesignEntityType::Assign, "a"), "3"},
          }
      }
  );

  auto result = evaluator->evaluate()->getRows();
  REQUIRE(expected
              .
                  size()
              == result.
                  size()
  );
  for (
    auto &item
      : expected) {
    REQUIRE(std::count(result.begin(), result.end(), item)
                == 1);
  }
}
TEST_CASE("Test pattern clause lower case Evaluator") {
  std::string program = R"(assign a; Select a pattern a(_,_"x"_))";
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
  pkb->addSubString(1, " x ");

  // print x
  pkb->addUseStmt(2, "x");

  // x = x + 3 + y
  pkb->addModifyStmt(3, "x");
  pkb->addUseStmt(3, "x");
  pkb->addUseStmt(3, "y");
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
              {QuerySynonym(DesignEntityType::Assign, "a"), "1"},
          },

          {
              {QuerySynonym(DesignEntityType::Assign, "a"), "3"},
          }
      }
  );

  auto rows = evaluator->evaluate()->getRows();
  REQUIRE(expected.size() == rows.size());
}

TEST_CASE("Test pattern clause casing Evaluator") {
  std::string program = R"(variable v; assign a; Select v pattern a(_,_"Right"_))";
  auto pkb = new PKB();
  pkb->addProc("Unused");
  pkb->addStatement(1, assign, "Unused");
  pkb->addStatement(2, print, "Unused");
  pkb->addStatement(3, assign, "Unused");
  pkb->addVariable("x");
  pkb->addVariable("right");
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
  pkb->addUseStmt(3, "right");
  pkb->addAssignPattern(3, "x", "x + 3 + right");
  auto queryParser = new QueryParser(program);
  queryParser->parse();

  auto a = (queryParser->getQuery());
  REQUIRE(a.toString() != "Query(\n"
                          "  INVALID QUERY\n"
                          ")");

  auto evaluator = new QueryEvaluator(queryParser, pkb);

  RowList expected = createRowList({});

  auto rows = evaluator->evaluate()->getRows();
  REQUIRE(expected.size() == rows.size());
}

TEST_CASE("Empty PKB no match") {
  auto pkb = new PKB();
  pkb->addProc("unused");
  pkb->addStatement(1, print, "unused");
  std::string program = "assign a; print p; Select p pattern a(_,_)";
  auto queryParser = new QueryParser(program);
  queryParser->parse();
  auto a = queryParser->getQuery();
  REQUIRE(a.toString() != "Query(\n"
                          "  INVALID QUERY\n"
                          ")");

  auto evaluator = new QueryEvaluator(queryParser, pkb);
  auto rows = evaluator->evaluate()->getRows();
  RowList expected = {};
  REQUIRE(expected.size() == rows.size());
  for (auto &r: expected) {
    REQUIRE(std::find(rows.begin(), rows.end(), r) != rows.end());
  }
}

TEST_CASE("Test if pattern synonym success") {
  auto pkb = new PKB();
  pkb->addProc("eg");
  pkb->addStatement(1, ifStmt, "eg");
  pkb->addIfStmt(1, 2, 3);
  pkb->addIfCtrlVar(1, "a");
  pkb->addVariable("a");

  std::string program = "if p; variable a; Select p pattern p(a, _, _)";
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
              {QuerySynonym(DesignEntityType::If, "p"), "1"},
              {QuerySynonym(DesignEntityType::Variable, "a"), "a"},
          }
      }
  );
  REQUIRE(expected.size() == rows.size());
  for (auto &r: expected) {
    REQUIRE(std::find(rows.begin(), rows.end(), r) != rows.end());
  }
}

TEST_CASE("Test if pattern wildcard success") {
  auto pkb = new PKB();
  pkb->addProc("eg");
  pkb->addStatement(1, ifStmt, "eg");
  pkb->addIfStmt(1, 2, 3);
  pkb->addIfCtrlVar(1, "a");
  pkb->addVariable("a");

  std::string program = "if p; variable a; Select p pattern p(_, _, _)";
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
              {QuerySynonym(DesignEntityType::If, "p"), "1"},
          }
      }
  );
  REQUIRE(expected.size() == rows.size());
  for (auto &r: expected) {
    REQUIRE(std::find(rows.begin(), rows.end(), r) != rows.end());
  }
}

TEST_CASE("Test if pattern wildcard failure") {
  auto pkb = new PKB();

  std::string program = "if p; variable a; Select p pattern p(_, _, _)";
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

TEST_CASE("Test if pattern constant success") {
  auto pkb = new PKB();
  pkb->addProc("eg");
  pkb->addStatement(1, ifStmt, "eg");
  pkb->addIfStmt(1, 2, 3);
  pkb->addIfCtrlVar(1, "a");
  pkb->addVariable("a");

  std::string program = "if p; variable a; Select p pattern p(\"a\", _, _)";
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
              {QuerySynonym(DesignEntityType::If, "p"), "1"},
          }
      }
  );
  REQUIRE(expected.size() == rows.size());
  for (auto &r: expected) {
    REQUIRE(std::find(rows.begin(), rows.end(), r) != rows.end());
  }
}

TEST_CASE("Test if pattern constant failure") {
  auto pkb = new PKB();
  pkb->addProc("eg");
  pkb->addStatement(1, ifStmt, "eg");
  pkb->addIfStmt(1, 2, 3);
  pkb->addIfCtrlVar(1, "a");
  pkb->addVariable("a");

  std::string program = "if p; variable a; Select p pattern p(\"b\", _, _)";
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

TEST_CASE("Test while pattern constant success") {
  auto pkb = new PKB();
  pkb->addProc("eg");
  pkb->addStatement(1, whileStmt, "eg");
  pkb->addWhileStmt(1, 2);
  pkb->addWhileCtrlVar(1, "a");
  pkb->addVariable("a");

  std::string program = "while p; variable a; Select p pattern p(\"a\", _)";
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
              {QuerySynonym(DesignEntityType::While, "p"), "1"}
          }
      }
  );
  REQUIRE(expected.size() == rows.size());
  for (auto &r: expected) {
    REQUIRE(std::find(rows.begin(), rows.end(), r) != rows.end());
  }
}

TEST_CASE("Test while pattern constant failure") {
  auto pkb = new PKB();
  pkb->addProc("eg");
  pkb->addStatement(1, whileStmt, "eg");
  pkb->addWhileStmt(1, 2);
  pkb->addWhileCtrlVar(1, "a");
  pkb->addVariable("a");

  std::string program = "while p; variable a; Select p pattern p(\"b\", _)";
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

TEST_CASE("Test while pattern synonym success") {
  auto pkb = new PKB();
  pkb->addProc("eg");
  pkb->addStatement(1, whileStmt, "eg");
  pkb->addWhileStmt(1, 2);
  pkb->addWhileCtrlVar(1, "a");
  pkb->addVariable("a");

  std::string program = "while p; variable a; Select p pattern p(a, _)";
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
              {QuerySynonym(DesignEntityType::While, "p"), "1"},
              {QuerySynonym(DesignEntityType::Variable, "a"), "a"},
          }
      }
  );
  REQUIRE(expected.size() == rows.size());
  for (auto &r: expected) {
    REQUIRE(std::find(rows.begin(), rows.end(), r) != rows.end());
  }
}

TEST_CASE("Test while pattern wildcard success") {
  auto pkb = new PKB();
  pkb->addProc("eg");
  pkb->addStatement(1, whileStmt, "eg");
  pkb->addWhileStmt(1, 2);
  pkb->addWhileCtrlVar(1, "a");
  pkb->addVariable("a");

  std::string program = "while p; variable a; Select p pattern p(_, _)";
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
              {QuerySynonym(DesignEntityType::While, "p"), "1"},
          }
      }
  );
  REQUIRE(expected.size() == rows.size());
  for (auto &r: expected) {
    REQUIRE(std::find(rows.begin(), rows.end(), r) != rows.end());
  }
}

TEST_CASE("Test while pattern wildcard failure") {
  auto pkb = new PKB();

  std::string program = "while p; variable a; Select p pattern p(_, _)";
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