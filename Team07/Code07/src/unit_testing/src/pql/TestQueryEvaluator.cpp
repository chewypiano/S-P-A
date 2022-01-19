//
// Created by Jazer on 1/10/2021.
//

#include "catch.hpp"
#include "PQLUtil.h"
#include "QueryEvaluator.h"

TEST_CASE("Select all variables") {
  auto pkb = new PKB();
  pkb->addVariable("a");
  pkb->addVariable("b");
  pkb->addVariable("c");
  QuerySynonym outputSyn = QuerySynonym(DesignEntityType::Variable, "v");
  QueryOutput output = QueryOutput(QueryOutputType::Synonym, &outputSyn);

  std::string program;
  auto queryParser = new QueryParser(program);
  auto eval = QueryEvaluator(queryParser, pkb);

  Query query = Query();
  query.insertSynonym(outputSyn);
  query.insertOutput(&output);

  auto res = eval.evaluateQuery(query);

  REQUIRE(res->getOutput() == std::vector<QueryOutput *>{&output});

  Row a = Row();
  a.addSynonym(outputSyn, "a");
  Row b = Row();
  b.addSynonym(outputSyn, "b");
  Row c = Row();
  c.addSynonym(outputSyn, "c");

  auto rows = res->getRows();
  REQUIRE(res->size() == 3);
  REQUIRE(std::find(rows.begin(), rows.end(), a) != rows.end());
  REQUIRE(std::find(rows.begin(), rows.end(), b) != rows.end());
  REQUIRE(std::find(rows.begin(), rows.end(), c) != rows.end());
}

TEST_CASE("Select all statements") {
  auto pkb = new PKB();
  pkb->addProc("proc");
  pkb->addStatement(1, assign, "proc");
  pkb->addStatement(2, assign, "proc");
  pkb->addStatement(3, assign, "proc");
  QuerySynonym output = QuerySynonym(DesignEntityType::Stmt, "v");

  std::string program;
  auto queryParser = new QueryParser(program);
  auto eval = QueryEvaluator(queryParser, pkb);

  Query query = Query();
  query.insertSynonym(output);
  query.insertOutputSynonym(&output);

  auto res = eval.evaluateQuery(query);

  Row a = Row();
  a.addSynonym(output, "1");
  Row b = Row();
  b.addSynonym(output, "2");
  Row c = Row();
  c.addSynonym(output, "3");

  auto rows = res->getRows();
  REQUIRE(res->size() == 3);
  REQUIRE(std::find(rows.begin(), rows.end(), a) != rows.end());
  REQUIRE(std::find(rows.begin(), rows.end(), b) != rows.end());
  REQUIRE(std::find(rows.begin(), rows.end(), c) != rows.end());
}

TEST_CASE("Select all statements that use any variable") {
  auto pkb = new PKB();
  pkb->addProc("proc");
  pkb->addStatement(1, assign, "proc");
  pkb->addUseStmt(1, "x");
  pkb->addStatement(2, assign, "proc");
  pkb->addUseStmt(2, "x");
  pkb->addStatement(3, assign, "proc");
  QuerySynonym output = QuerySynonym(DesignEntityType::Assign, "a");

  std::string program = "assign a; Select a such that Uses(a, _)";
  auto queryParser = new QueryParser(program);
  queryParser->parse();
  Query query = queryParser->getQuery();

  auto eval = QueryEvaluator(queryParser, pkb);

  ClauseResult *res = eval.evaluateQuery(query);

  Row a = Row();
  a.addSynonym(output, "1");
  Row b = Row();
  b.addSynonym(output, "2");
  Row c = Row();
  c.addSynonym(output, "3");

  auto rows = res->getRows();
  REQUIRE(res->size() == 2);
  REQUIRE(std::find(rows.begin(), rows.end(), a) != rows.end());
  REQUIRE(std::find(rows.begin(), rows.end(), b) != rows.end());
  REQUIRE(std::find(rows.begin(), rows.end(), c) == rows.end());
}

TEST_CASE("Select all statements that use a specific variable") {
  auto pkb = new PKB();
  pkb->addProc("proc");
  pkb->addStatement(1, assign, "proc");
  pkb->addUseStmt(1, "x");
  pkb->addStatement(2, assign, "proc");
  pkb->addUseStmt(2, "y");
  pkb->addStatement(3, assign, "proc");
  pkb->addUseStmt(3, "z");
  QuerySynonym output = QuerySynonym(DesignEntityType::Assign, "a");

  std::string program = "assign a; Select a such that Uses(a, \"x\")";
  auto queryParser = new QueryParser(program);
  queryParser->parse();
  Query query = queryParser->getQuery();

  auto eval = QueryEvaluator(queryParser, pkb);

  ClauseResult *res = eval.evaluateQuery(query);

  Row a = Row();
  a.addSynonym(output, "1");
  Row b = Row();
  b.addSynonym(output, "2");
  Row c = Row();
  c.addSynonym(output, "3");

  auto rows = res->getRows();
  REQUIRE(res->size() == 1);
  REQUIRE(std::find(rows.begin(), rows.end(), a) != rows.end());
  REQUIRE(std::find(rows.begin(), rows.end(), b) == rows.end());
  REQUIRE(std::find(rows.begin(), rows.end(), c) == rows.end());
}

TEST_CASE("Select all statements that match a wildcard pattern") {
  auto pkb = new PKB();
  pkb->addProc("proc");
  // Essentially x = x, y = y, z = z
  // TODO: May need to change this when shunting yard is implemented
  pkb->addStatement(1, assign, "proc");
  pkb->addAssignPattern(1, "x", "x");
  pkb->addUseStmt(1, "x");
  pkb->addModifyStmt(1, "x");
  pkb->addStatement(2, assign, "proc");
  pkb->addAssignPattern(2, "y", "y");
  pkb->addUseStmt(2, "y");
  pkb->addModifyStmt(2, "y");
  pkb->addStatement(3, assign, "proc");
  pkb->addAssignPattern(3, "z", "z");
  pkb->addUseStmt(3, "z");
  pkb->addModifyStmt(3, "z");
  QuerySynonym output = QuerySynonym(DesignEntityType::Assign, "a");

  std::string program = "assign a; Select a pattern a(_, _)";
  auto queryParser = new QueryParser(program);
  queryParser->parse();
  Query query = queryParser->getQuery();

  auto eval = QueryEvaluator(queryParser, pkb);

  ClauseResult *res = eval.evaluateQuery(query);

  Row a = Row();
  a.addSynonym(output, "1");
  Row b = Row();
  b.addSynonym(output, "2");
  Row c = Row();
  c.addSynonym(output, "3");

  auto rows = res->getRows();
  REQUIRE(res->size() == 3);
  REQUIRE(std::find(rows.begin(), rows.end(), a) != rows.end());
  REQUIRE(std::find(rows.begin(), rows.end(), b) != rows.end());
  REQUIRE(std::find(rows.begin(), rows.end(), c) != rows.end());
}

TEST_CASE("Select all statements that match a synonym pattern") {
  auto pkb = new PKB();
  pkb->addProc("proc");
  // Essentially x = x, y = y, z = z

  pkb->addVariable("x");
  pkb->addVariable("y");
  pkb->addVariable("z");
  pkb->addStatement(1, assign, "proc");
  pkb->addAssignPattern(1, "x", "x");
  pkb->addUseStmt(1, "x");
  pkb->addModifyStmt(1, "x");
  pkb->addStatement(2, assign, "proc");
  pkb->addAssignPattern(2, "y", "y");
  pkb->addUseStmt(2, "y");
  pkb->addModifyStmt(2, "y");
  pkb->addStatement(3, assign, "proc");
  pkb->addAssignPattern(3, "z", "z");
  pkb->addUseStmt(3, "z");
  pkb->addModifyStmt(3, "z");
  QuerySynonym output = QuerySynonym(DesignEntityType::Assign, "a");

  std::string program = "assign a; variable v; Select a pattern a(v, _)";
  auto queryParser = new QueryParser(program);
  queryParser->parse();
  Query query = queryParser->getQuery();

  auto eval = QueryEvaluator(queryParser, pkb);

  ClauseResult *res = eval.evaluateQuery(query);
  QuerySynonym v = QuerySynonym(DesignEntityType::Variable, "v");

  Row a = Row();
  a.addSynonym(output, "1");
  a.addSynonym(v, "x");
  Row b = Row();
  b.addSynonym(output, "2");
  b.addSynonym(v, "y");
  Row c = Row();
  c.addSynonym(output, "3");
  c.addSynonym(v, "z");

  auto rows = res->getRows();
  REQUIRE(res->size() == 3);
  REQUIRE(std::find(rows.begin(), rows.end(), a) != rows.end());
  REQUIRE(std::find(rows.begin(), rows.end(), b) != rows.end());
  REQUIRE(std::find(rows.begin(), rows.end(), c) != rows.end());
}

TEST_CASE("Select all statements that match a synonym pattern and basic such that clause") {
  auto pkb = new PKB();
  pkb->addProc("proc");
  // Essentially x = x, y = y, z = z

  pkb->addVariable("x");
  pkb->addVariable("y");
  pkb->addVariable("z");
  pkb->addStatement(1, assign, "proc");
  pkb->addAssignPattern(1, "x", "x");
  pkb->addUseStmt(1, "x");
  pkb->addModifyStmt(1, "x");
  pkb->addStatement(2, assign, "proc");
  pkb->addAssignPattern(2, "y", "y");
  pkb->addUseStmt(2, "y");
  pkb->addModifyStmt(2, "y");
  pkb->addStatement(3, assign, "proc");
  pkb->addAssignPattern(3, "z", "z");
  pkb->addUseStmt(3, "z");
  pkb->addModifyStmt(3, "z");
  pkb->addFollow(1, 2);
  pkb->addFollowStar(1, 2);
  pkb->addFollowStar(1, 3);
  pkb->addFollow(2, 3);
  pkb->addFollowStar(2, 3);

  QuerySynonym output = QuerySynonym(DesignEntityType::Assign, "a");

  std::string program = "assign a; variable v; Select a such that Follows(1, a) pattern a(v, _)";
  auto queryParser = new QueryParser(program);
  queryParser->parse();
  Query query = queryParser->getQuery();

  auto eval = QueryEvaluator(queryParser, pkb);

  ClauseResult *res = eval.evaluateQuery(query);
  QuerySynonym v = QuerySynonym(DesignEntityType::Variable, "v");

  Row a = Row();
  a.addSynonym(output, "1");
  a.addSynonym(v, "x");
  Row b = Row();
  b.addSynonym(output, "2");
  b.addSynonym(v, "y");
  Row c = Row();
  c.addSynonym(output, "3");
  c.addSynonym(v, "z");

  auto rows = res->getRows();
  REQUIRE(res->size() == 1);
  REQUIRE(std::find(rows.begin(), rows.end(), a) == rows.end());
  REQUIRE(std::find(rows.begin(), rows.end(), b) != rows.end());
  REQUIRE(std::find(rows.begin(), rows.end(), c) == rows.end());
}

TEST_CASE("Select all procedures") {
  std::vector<std::string> expectedOutput = {"proc1", "proc2"};
  std::string program;
  auto pkb = new PKB();
  Query query = Query();
  // procedure p; Select p
  QuerySynonym output = QuerySynonym(DesignEntityType::Procedure, "p");
  query.insertSynonym(output);
  query.insertOutputSynonym(&output);
  ClauseResult expected = ClauseResult();
  for (const std::string &s: expectedOutput) {
    pkb->addProc(s);
    Row r = Row();
    r.addSynonym(output, s);
    expected.addRow(r);
  }

  auto parser = QueryParser(program);
  QueryEvaluator evaluator = QueryEvaluator(&parser, pkb);

  ClauseResult *result = evaluator.evaluateQuery(query);

  REQUIRE(result->size() == expected.size());
  auto r = result->getRows();
  REQUIRE(std::find(r.begin(), r.end(), expected.getRows()[0]) != r.end());
  REQUIRE(std::find(r.begin(), r.end(), expected.getRows()[1]) != r.end());
}

TEST_CASE("Select all constants") {
  std::vector<std::string> expectedOutput = {"const1", "const2"};
  std::string program;
  auto pkb = new PKB();
  Query query = Query();
  QuerySynonym output = QuerySynonym(DesignEntityType::Constant, "c");
  query.insertSynonym(output);
  query.insertOutputSynonym(&output);

  ClauseResult expected = ClauseResult();
  for (const std::string &s: expectedOutput) {
    pkb->addConst(s);
    Row r = Row();
    r.addSynonym(output, s);
    expected.addRow(r);
  }

  auto parser = QueryParser(program);
  QueryEvaluator evaluator = QueryEvaluator(&parser, pkb);

  ClauseResult *result = evaluator.evaluateQuery(query);

  REQUIRE(result->size() == expected.size());
  auto r = result->getRows();
  REQUIRE(
      std::find(r.begin(), r.end(), expected.getRows()[0]) != r.end());
  REQUIRE(
      std::find(r.begin(), r.end(), expected.getRows()[1]) != r.end());
}

TEST_CASE("No constants should return none") {
  std::string program;
  auto pkb = new PKB();
  Query query = Query();
  QuerySynonym output = QuerySynonym(DesignEntityType::Constant, "c");
  query.insertSynonym(output);
  query.insertOutputSynonym(&output);

  auto parser = QueryParser(program);
  QueryEvaluator evaluator = QueryEvaluator(&parser, pkb);
  ClauseResult *result = evaluator.evaluateQuery(query);

  REQUIRE(result->size() == 0);
}

TEST_CASE("No statements should return none") {
  std::string program;
  auto pkb = new PKB();
  Query query = Query();
  QuerySynonym output = QuerySynonym(DesignEntityType::Stmt, "s");
  query.insertSynonym(output);
  query.insertOutputSynonym(&output);

  auto parser = QueryParser(program);
  QueryEvaluator evaluator = QueryEvaluator(&parser, pkb);
  ClauseResult *result = evaluator.evaluateQuery(query);

  REQUIRE(result->size() == 0);
}

TEST_CASE("Invalid queries should output none") {
  auto pkb = new PKB();
  std::string program;
  auto queryParser = QueryParser(program);
  QueryEvaluator evaluator = QueryEvaluator(&queryParser, pkb);
  Query query = Query();
  QuerySynonym output = QuerySynonym(DesignEntityType::Stmt, "s");
  query.insertOutputSynonym(&output);
  query.setInvalidQuery();
  ClauseResult *result = evaluator.evaluateQuery(query);
  REQUIRE(result->size() == 0);
}

TEST_CASE("Select uses with specific variable") {
  // Select a such that Uses(a, x);
  auto pkb = new PKB();
  pkb->addProc("Unused");
  pkb->addStatement(1, assign, "Unused");
  pkb->addStatement(2, assign, "Unused");
  pkb->addVariable("x");
  pkb->addVariable("y");
  pkb->addUseStmt(1, "x");
  pkb->addUseStmt(2, "y");

  Query query = Query();
  QuerySynonym a = QuerySynonym(DesignEntityType::Stmt, "a");
  query.insertSynonym(a);
  query.insertOutputSynonym(&a);
  auto stc = new SuchThatClause(ClauseType::UsesS, "a", "\"x\"");
  query.insertClause(stc);

  std::string program;
  auto queryParser = QueryParser(program);
  queryParser.parse();
  auto evaluator = new QueryEvaluator(&queryParser, pkb);

  ClauseResult *result = evaluator->evaluateQuery(query);

  Row expected = Row();
  expected.addSynonym(a, "1");

  REQUIRE(result->getRows().size() == 1);
  auto r = result->getRows();
  REQUIRE(std::find(r.begin(), r.end(), expected) != r.end());
}

TEST_CASE("Uses relation test with wildcard v2") {
  // Select a such that Uses(a, _);
  auto pkb = new PKB();
  pkb->addProc("Unused");
  pkb->addStatement(1, assign, "Unused");
  pkb->addStatement(2, assign, "Unused");
  pkb->addVariable("x");
  pkb->addVariable("y");
  pkb->addUseStmt(1, "x");
  pkb->addUseStmt(2, "y");

  Query query = Query();
  QuerySynonym a = QuerySynonym(DesignEntityType::Stmt, "a");
  query.insertSynonym(a);
  query.insertOutputSynonym(&a);
  auto stc = new SuchThatClause(ClauseType::UsesS, "a", "_");
  query.insertClause(stc);

  std::string program;
  auto queryParser = QueryParser(program);
  queryParser.parse();
  auto evaluator = new QueryEvaluator(&queryParser, pkb);

  ClauseResult *result = evaluator->evaluateQuery(query);

  Row expected1 = Row();
  expected1.addSynonym(a, "1");
  Row expected2 = Row();
  expected2.addSynonym(a, "2");

  REQUIRE(result->getRows().size() == 2);
  auto r = result->getRows();
  REQUIRE(std::find(r.begin(), r.end(), expected1) != r.end());
  REQUIRE(std::find(r.begin(), r.end(), expected2) != r.end());
}

TEST_CASE("evaluateModifies_leftStmtNumRightWildcard_true v2") {

  auto pkb = new PKB();
  std::string testVar = "a";
  pkb->addProc("Unused");
  pkb->addStatement(2, assign, "Unused");
  pkb->addVariable(testVar);
  pkb->addModifyStmt(2, testVar);

  // This clause should always evaluate to True, but would not constrain 'a' in any way.
  std::string program = "assign a;\nSelect a such that Modifies(2, _)";
  auto queryParser = QueryParser(program);
  queryParser.parse();

  Row expected = Row();
  QuerySynonym syn = QuerySynonym(DesignEntityType::Assign, "a");
  expected.addSynonym(syn, "2");
  auto evaluator = QueryEvaluator(&queryParser, pkb);
  auto query = queryParser.getQuery();
  auto result = evaluator.evaluateQuery(query);

  REQUIRE(result->size() == 1);
  REQUIRE(result->getRows()[0] == expected);
}

TEST_CASE("selectFollows_betweenVariablesValidFollows_success v2") {
  auto pkb = new PKB();
  pkb->addProc("Unused");
  pkb->addStatement(1, assign, "Unused");
  pkb->addStatement(2, assign, "Unused");
  pkb->addFollow(1, 2);

  // assign a, b; Select a such that Follows(a, b)
  Query query = Query();
  QuerySynonym a = QuerySynonym(DesignEntityType::Stmt, "a");
  QuerySynonym b = QuerySynonym(DesignEntityType::Stmt, "b");
  query.insertSynonym(a);
  query.insertSynonym(b);
  query.insertOutputSynonym(&a);
  auto stc = new SuchThatClause(ClauseType::Follows, "a", "b");
  query.insertClause(stc);

  std::string program;
  auto queryParser = QueryParser(program);
  queryParser.parse();
  auto evaluator = new QueryEvaluator(&queryParser, pkb);

  ClauseResult *result = evaluator->evaluateQuery(query);

  ClauseResult expected = ClauseResult();
  Row r = Row();
  r.addSynonym(a, "1");
  r.addSynonym(b, "2");
  expected.addRow(r);
  REQUIRE(expected.getRows() == result->getRows());
}

TEST_CASE("Parent* Relation test v2") {
  auto pkb = new PKB();
  pkb->addProc("Unused");
  pkb->addStatement(1, assign, "Unused");
  pkb->addStatement(2, assign, "Unused");
  pkb->addParentStar(1, 2);
  pkb->addParent(1, 2);

  // assign a, b; Select a such that Parent*(a, b);
  Query query = Query();
  QuerySynonym a = QuerySynonym(DesignEntityType::Stmt, "a");
  QuerySynonym b = QuerySynonym(DesignEntityType::Stmt, "b");
  query.insertSynonym(a);
  query.insertSynonym(b);
  query.insertOutputSynonym(&a);
  auto stc = new SuchThatClause(ClauseType::ParentT, "a", "b");
  query.insertClause(stc);

  std::string program;
  auto queryParser = new QueryParser(program);
  queryParser->parse();
  auto evaluator = new QueryEvaluator(queryParser, pkb);

  ClauseResult *result = evaluator->evaluateQuery(query);

  ClauseResult expected = ClauseResult();
  Row r = Row();
  r.addSynonym(a, "1");
  r.addSynonym(b, "2");
  expected.addRow(r);

  REQUIRE(expected.getRows() == result->getRows());
}

TEST_CASE("Follow* Relation test v2") {
  auto pkb = new PKB();
  pkb->addProc("Unused");
  pkb->addStatement(1, assign, "Unused");
  pkb->addStatement(2, assign, "Unused");
  pkb->addStatement(3, assign, "Unused");
  pkb->addFollow(1, 2);
  pkb->addFollow(2, 3);
  pkb->addFollowStar(1, 2);
  pkb->addFollowStar(2, 3);
  pkb->addFollowStar(1, 3);

  std::string program = "stmt s, s1; Select s such that Follows*(s,s1)";
  auto queryParser = new QueryParser(program);
  queryParser->parse();
  auto evaluator = new QueryEvaluator(queryParser, pkb);
  // {1, 2}, {1,3}, {2,3}
  QuerySynonym s = QuerySynonym(DesignEntityType::Stmt, "s");
  QuerySynonym s1 = QuerySynonym(DesignEntityType::Stmt, "s1");

  auto result = evaluator->evaluate();

  Row r1 = Row();
  r1.addSynonym(s, "1");
  r1.addSynonym(s1, "2");
  Row r2 = Row();
  r2.addSynonym(s, "1");
  r2.addSynonym(s1, "3");
  Row r3 = Row();
  r3.addSynonym(s, "2");
  r3.addSynonym(s1, "3");

  REQUIRE(result->size() == 3);
  auto r = result->getRows();
  REQUIRE(std::find(r.begin(), r.end(), r1) != r.end());
  REQUIRE(std::find(r.begin(), r.end(), r2) != r.end());
  REQUIRE(std::find(r.begin(), r.end(), r3) != r.end());
}

TEST_CASE("Test pattern with 1 wildcards v2") {
  auto pkb = new PKB();
  pkb->addProc("example");
  pkb->addAssignPattern(1, "a", "b+c");
  pkb->addAssignPattern(2, "c", "b");
  pkb->addUseStmt(1, "b");
  pkb->addUseStmt(1, "c");
  pkb->addUseStmt(2, "b");
  pkb->addStatement(1, assign, "example");
  pkb->addStatement(2, assign, "example");

  std::string program = "assign a; Select a pattern a(\"a\",_)";
  auto queryParser = new QueryParser(program);
  queryParser->parse();
  auto evaluator = new QueryEvaluator(queryParser, pkb);
  auto result = evaluator->evaluate();
  Row r = Row();
  QuerySynonym syn = QuerySynonym(DesignEntityType::Assign, "a");
  r.addSynonym(syn, "1");

  REQUIRE(result->size() == 1);
  auto re = result->getRows();
  REQUIRE(std::find(re.begin(), re.end(), r) != re.end());
}

TEST_CASE("Test pattern with 2 wildcards v2") {
  auto pkb = new PKB();
  pkb->addProc("example");
  pkb->addAssignPattern(1, "a", "b+c");
  pkb->addAssignPattern(2, "c", "b");
  pkb->addStatement(1, assign, "example");
  pkb->addStatement(2, assign, "example");
  pkb->addUseStmt(1, "b");
  pkb->addUseStmt(1, "c");
  pkb->addUseStmt(2, "b");

  std::string program = "assign a; Select a pattern a(_,_)";
  auto queryParser = new QueryParser(program);
  queryParser->parse();
  auto evaluator = new QueryEvaluator(queryParser, pkb);
  auto result = evaluator->evaluate();

  QuerySynonym syn = QuerySynonym(DesignEntityType::Assign, "a");
  Row r1 = Row();
  r1.addSynonym(syn, "1");
  Row r2 = Row();
  r2.addSynonym(syn, "2");
  REQUIRE(result->size() == 2);
  auto r = result->getRows();
  REQUIRE(std::find(r.begin(), r.end(), r1) != r.end());
  REQUIRE(std::find(r.begin(), r.end(), r2) != r.end());
}

TEST_CASE("Test pattern with such that v2") {
  auto pkb = new PKB();
  pkb->addProc("example");
  pkb->addStatement(1, assign, "example");
  pkb->addStatement(2, assign, "example");
  pkb->addUseStmt(1, "b");
  pkb->addUseStmt(1, "c");
  pkb->addUseStmt(2, "b");
  pkb->addModifyStmt(1, "a");
  pkb->addModifyStmt(2, "c");
  pkb->addVariable("a");
  pkb->addVariable("b");
  pkb->addVariable("c");
  pkb->addAssignPattern(1, "a", "b+c");
  pkb->addAssignPattern(2, "c", "b");

  std::string program = "assign a; variable v, v1; Select v such that Uses(a, v1) pattern a(v,_)";
  auto queryParser = new QueryParser(program);
  queryParser->parse();
  auto evaluator = new QueryEvaluator(queryParser, pkb);
  std::vector<std::string> expectedOutput = {"a", "c"};
  auto result = evaluator->evaluate();

  QuerySynonym a = QuerySynonym(DesignEntityType::Assign, "a");
  QuerySynonym v = QuerySynonym(DesignEntityType::Variable, "v");
  QuerySynonym v1 = QuerySynonym(DesignEntityType::Variable, "v1");

  Row r1 = createRow({{v, "a"}, {v1, "b"}, {a, "1"}});
  Row r2 = createRow({{v, "a"}, {v1, "b"}, {a, "1"}});
  Row r3 = createRow({{v, "a"}, {v1, "b"}, {a, "1"}});

  REQUIRE(result->size() == 3);
  auto r = result->getRows();
  REQUIRE(std::find(r.begin(), r.end(), r1) != r.end());
  REQUIRE(std::find(r.begin(), r.end(), r2) != r.end());
  REQUIRE(std::find(r.begin(), r.end(), r3) != r.end());
}

TEST_CASE("Test Follows with wildcard and syn v2") {
  auto pkb = new PKB();
  pkb->addProc("example");
  pkb->addStatement(1, print, "example");
  pkb->addStatement(2, assign, "example");
  pkb->addFollow(1, 2);

  std::string program = "assign a; Select a such that Follows(_, a)";
  auto queryParser = new QueryParser(program);
  queryParser->parse();
  auto evaluator = new QueryEvaluator(queryParser, pkb);
  auto result = evaluator->evaluate();
  QuerySynonym a = QuerySynonym(DesignEntityType::Assign, "a");
  Row expected = createRow({{a, "2"}});
  REQUIRE(result->size() == 1);
  REQUIRE(result->getRows()[0] == expected);
}

TEST_CASE("Test Follows with wildcard statement v2") {
  auto pkb = new PKB();
  pkb->addProc("example");
  pkb->addStatement(1, print, "example");
  pkb->addStatement(2, assign, "example");
  pkb->addFollow(1, 2);

  std::string program = "assign a; Select a such that Follows(_, 2)";
  auto queryParser = new QueryParser(program);
  queryParser->parse();
  auto evaluator = new QueryEvaluator(queryParser, pkb);
  auto result = evaluator->evaluate();
  QuerySynonym a = QuerySynonym(DesignEntityType::Assign, "a");
  Row expected = createRow({{a, "2"}});
  REQUIRE(result->size() == 1);
  REQUIRE(result->getRows()[0] == expected);
}

TEST_CASE("Uses statement with syn v2") {
  auto pkb = new PKB();
  pkb->addProc("example");
  pkb->addStatement(1, assign, "example");
  pkb->addVariable("a");
  pkb->addVariable("c");
  pkb->addUseStmt(1, "c");
  pkb->addModifyStmt(1, "a");

  std::string program = "variable v; Select v such that Uses(1, v)";
  auto queryParser = new QueryParser(program);
  queryParser->parse();
  auto evaluator = QueryEvaluator(queryParser, pkb);
  auto result = evaluator.evaluate();
  QuerySynonym v = QuerySynonym(DesignEntityType::Variable, "v");
  Row expected = createRow({{v, "c"}});
  REQUIRE(result->size() == 1);
  REQUIRE(result->getRows()[0] == expected);
}

TEST_CASE("Uses with pattern v2") {
  auto pkb = new PKB();
  pkb->addProc("example");
  pkb->addStatement(1, assign, "example");
  pkb->addAssignPattern(1, "a", "a + 1");
  pkb->addSubString(1, " a 1 + ");
  pkb->addUseStmt(1, "a");
  pkb->addModifyStmt(1, "a");
  pkb->addUseStmt(1, "a");
  pkb->addVariable("a");
  pkb->addConst("1");

  std::string program = R"(assign a; Select a such that Uses(a,"a") pattern a(_, _"a"_))";
  auto queryParser = new QueryParser(program);
  queryParser->parse();
  auto evaluator = QueryEvaluator(queryParser, pkb);
  std::vector<std::string> expectedAnswer = {"1"};
  QuerySynonym a = QuerySynonym(DesignEntityType::Assign, "a");
  Row expected = createRow({{a, "1"}});
  auto result = evaluator.evaluate();
  REQUIRE(result->size() == 1);
  REQUIRE(result->getRows()[0] == expected);
}

TEST_CASE("Test pattern double wildcard v2") {
  auto pkb = new PKB();
  pkb->addProc("example");
  pkb->addStatement(1, assign, "example");
  pkb->addStatement(2, assign, "example");
  pkb->addAssignPattern(1, "a", "a+1");
  pkb->addSubString(1, " a 1 + ");
  pkb->addAssignPattern(2, "b", "b+a");
  pkb->addSubString(2, " b a + ");
  pkb->addUseStmt(1, "a");
  pkb->addModifyStmt(1, "a");
  pkb->addUseStmt(2, "b");
  pkb->addUseStmt(2, "a");
  pkb->addModifyStmt(2, "b");

  std::string program = "assign a; Select a pattern a(_, _\"b\"_)";
  auto queryParser = new QueryParser(program);
  queryParser->parse();
  auto evaluator = QueryEvaluator(queryParser, pkb);
  auto result = evaluator.evaluate();
  QuerySynonym a = QuerySynonym(DesignEntityType::Assign, "a");
  Row expected = createRow({{a, "2"}});
  REQUIRE(result->size() == 1);
  REQUIRE(expected == result->getRows()[0]);
}

TEST_CASE("Test pattern double wildcard with extra characters after name v2") {
  // Test for whether 'bb' also shows up in the check for 'b'
  auto pkb = new PKB();
  pkb->addProc("example");
  pkb->addStatement(1, assign, "example");
  pkb->addStatement(2, assign, "example");
  pkb->addAssignPattern(1, "a", "bc+1");
  pkb->addSubString(1, " bc 1 + ");
  pkb->addAssignPattern(2, "b", "b+a");
  pkb->addSubString(2, " b a + ");
  pkb->addUseStmt(1, "bb");
  pkb->addModifyStmt(1, "a");
  pkb->addUseStmt(2, "b");
  pkb->addUseStmt(2, "a");
  pkb->addModifyStmt(2, "b");

  std::string program = "assign a; Select a pattern a(_, _\"b\"_)";
  auto queryParser = new QueryParser(program);
  queryParser->parse();
  auto evaluator = QueryEvaluator(queryParser, pkb);
  auto result = evaluator.evaluate();
  QuerySynonym a = QuerySynonym(DesignEntityType::Assign, "a");
  Row expected = createRow({{a, "2"}});
  REQUIRE(result->size() == 1);
  REQUIRE(expected == result->getRows()[0]);
}

TEST_CASE("Test pattern double wildcard with extra characters before v2") {
  // Test for whether 'bb' also shows up in the check for 'b'
  auto pkb = new PKB();
  pkb->addProc("example");
  pkb->addStatement(1, assign, "example");
  pkb->addStatement(2, assign, "example");
  pkb->addAssignPattern(1, "a", "ab+1");
  pkb->addSubString(1, " ab 1 + ");
  pkb->addAssignPattern(2, "b", "b+a");
  pkb->addSubString(2, " b a +");
  pkb->addUseStmt(1, "ab");
  pkb->addModifyStmt(1, "a");
  pkb->addUseStmt(2, "b");
  pkb->addUseStmt(2, "a");
  pkb->addModifyStmt(2, "b");

  std::string program = "assign a; Select a pattern a(_, _\"b\"_)";
  auto queryParser = new QueryParser(program);
  queryParser->parse();
  auto evaluator = QueryEvaluator(queryParser, pkb);
  auto result = evaluator.evaluate();
  QuerySynonym a = QuerySynonym(DesignEntityType::Assign, "a");
  Row expected = createRow({{a, "2"}});
  REQUIRE(result->size() == 1);
  REQUIRE(expected == result->getRows()[0]);
}

TEST_CASE("Modifies with n rhs v2") {
  auto pkb = new PKB();
  pkb->addProc("example");
  pkb->addStatement(1, assign, "example");
  pkb->addStatement(2, assign, "example");
  pkb->addAssignPattern(1, "a", "ab+1+n");
  pkb->addAssignPattern(2, "n", "b+a");
  pkb->addUseStmt(1, "ab");
  pkb->addUseStmt(1, "n");
  pkb->addModifyStmt(1, "a");
  pkb->addUseStmt(2, "b");
  pkb->addUseStmt(2, "a");
  pkb->addModifyStmt(2, "n");

  std::string program = "assign a; Select a such that Modifies (a, \"n\")";
  auto queryParser = new QueryParser(program);
  queryParser->parse();
  auto evaluator = QueryEvaluator(queryParser, pkb);
  auto result = evaluator.evaluate();
  QuerySynonym a = QuerySynonym(DesignEntityType::Assign, "a");
  Row expected = createRow({{a, "2"}});
  REQUIRE(result->size() == 1);
  REQUIRE(expected == result->getRows()[0]);
}

TEST_CASE("Modifies with n rhs and lhs v2") {
  auto pkb = new PKB();
  pkb->addProc("example");
  pkb->addStatement(1, assign, "example");
  pkb->addStatement(2, assign, "example");
  pkb->addAssignPattern(1, "a", "ab+1+n");
  pkb->addAssignPattern(2, "n", "b+a-n");
  pkb->addUseStmt(1, "ab");
  pkb->addUseStmt(1, "n");
  pkb->addModifyStmt(1, "a");
  pkb->addUseStmt(2, "b");
  pkb->addUseStmt(2, "a");
  pkb->addModifyStmt(2, "n");

  std::string program = "assign a; Select a such that Modifies (a, \"n\")";
  auto queryParser = new QueryParser(program);
  queryParser->parse();
  auto evaluator = QueryEvaluator(queryParser, pkb);
  auto result = evaluator.evaluate();
  QuerySynonym a = QuerySynonym(DesignEntityType::Assign, "a");
  Row expected = createRow({{a, "2"}});
  REQUIRE(result->size() == 1);
  REQUIRE(expected == result->getRows()[0]);
}

TEST_CASE("Uses with n lhs v2") {
  auto pkb = new PKB();
  pkb->addProc("example");
  pkb->addStatement(1, assign, "example");
  pkb->addStatement(2, assign, "example");
  pkb->addAssignPattern(1, "a", "ab+1+n");
  pkb->addAssignPattern(2, "n", "b+a");
  pkb->addUseStmt(1, "ab");
  pkb->addUseStmt(1, "n");
  pkb->addModifyStmt(1, "a");
  pkb->addUseStmt(2, "b");
  pkb->addUseStmt(2, "a");
  pkb->addModifyStmt(2, "n");

  std::string program = "assign a; Select a such that Uses(a, \"n\")";
  auto queryParser = new QueryParser(program);
  queryParser->parse();
  auto evaluator = QueryEvaluator(queryParser, pkb);
  auto result = evaluator.evaluate();
  QuerySynonym a = QuerySynonym(DesignEntityType::Assign, "a");
  Row expected = createRow({{a, "1"}});
  REQUIRE(result->size() == 1);
  REQUIRE(expected == result->getRows()[0]);
}

TEST_CASE("Invalid Modifies with n rhs v2") {
  auto pkb = new PKB();
  pkb->addProc("example");
  pkb->addStatement(1, assign, "example");
  pkb->addStatement(2, assign, "example");
  pkb->addAssignPattern(1, "a", "ab+1+n");
  pkb->addAssignPattern(2, "a", "b+a");
  pkb->addUseStmt(1, "ab");
  pkb->addUseStmt(1, "n");
  pkb->addModifyStmt(1, "a");
  pkb->addUseStmt(2, "b");
  pkb->addUseStmt(2, "a");
  pkb->addModifyStmt(2, "a");

  std::string program = "assign a; Select a such that Modifies (a, \"n\")";
  auto queryParser = new QueryParser(program);
  queryParser->parse();
  auto evaluator = QueryEvaluator(queryParser, pkb);
  auto result = evaluator.evaluate();
  REQUIRE(result->size() == 0);
}

TEST_CASE("Invalid Uses with n lhs v2") {
  auto pkb = new PKB();
  pkb->addProc("example");
  pkb->addStatement(1, assign, "example");
  pkb->addStatement(2, assign, "example");
  pkb->addAssignPattern(1, "a", "ab+1");
  pkb->addAssignPattern(2, "n", "b+a");
  pkb->addUseStmt(1, "ab");
  pkb->addModifyStmt(1, "a");
  pkb->addUseStmt(2, "b");
  pkb->addUseStmt(2, "a");
  pkb->addModifyStmt(2, "n");

  std::string program = "assign a; Select a such that Uses(a, \"n\")";
  auto queryParser = new QueryParser(program);
  queryParser->parse();
  auto evaluator = QueryEvaluator(queryParser, pkb);
  auto result = evaluator.evaluate();
  REQUIRE(result->size() == 0);
}

TEST_CASE("Modifies with just n on lhs and rhs v2") {
  auto pkb = new PKB();
  pkb->addProc("example");
  pkb->addStatement(1, assign, "example");
  pkb->addStatement(2, assign, "example");
  pkb->addAssignPattern(1, "a", "ab+1+n");
  pkb->addAssignPattern(2, "n", "n");
  pkb->addUseStmt(1, "ab");
  pkb->addUseStmt(1, "n");
  pkb->addModifyStmt(1, "a");
  pkb->addUseStmt(2, "n");
  pkb->addModifyStmt(2, "n");

  std::string program = "assign a; Select a such that Modifies (a, \"n\")";
  auto queryParser = new QueryParser(program);
  queryParser->parse();
  auto evaluator = QueryEvaluator(queryParser, pkb);
  auto result = evaluator.evaluate();
  QuerySynonym a = QuerySynonym(DesignEntityType::Assign, "a");
  Row expected = createRow({{a, "2"}});
  REQUIRE(result->size() == 1);
  REQUIRE(expected == result->getRows()[0]);
}

TEST_CASE("Evaluate With Statement lhs attr") {
  auto pkb = new PKB();
  pkb->addProc("example");

// a = 3
  pkb->addStatement(1, assign, "example");
  pkb->addAssignPattern(1, "a", "3");
  pkb->addConst("3");
  pkb->addVariable("a");
  pkb->addModifyStmt(1, "a");

  std::string program = "constant c; assign a; Select a with c.value = 3";
  auto queryParser = new QueryParser(program);
  queryParser->parse();
  INFO(queryParser->parsedQueryToString())
  auto evaluator = QueryEvaluator(queryParser, pkb);
  auto result = evaluator.evaluate();
  QuerySynonym a = QuerySynonym(DesignEntityType::Assign, "a");
  Row expected = createRow(
      {
          {a, "1"},
      }
  );
  REQUIRE(result->size() == 1);
  REQUIRE(expected == result->getRows()[0]);
}

TEST_CASE("Evaluate With Statement rhs attr") {
  auto pkb = new PKB();
  pkb->addProc("example");

// a = 3
  pkb->addStatement(1, assign, "example");
  pkb->addAssignPattern(1, "a", "3");
  pkb->addConst("3");
  pkb->addVariable("a");
  pkb->addModifyStmt(1, "a");

  std::string program = "constant c; assign a; Select a with 3 = c.value";
  auto queryParser = new QueryParser(program);
  queryParser->parse();
  INFO(queryParser->parsedQueryToString())
  auto evaluator = QueryEvaluator(queryParser, pkb);
  auto result = evaluator.evaluate();

  QuerySynonym a = QuerySynonym(DesignEntityType::Assign, "a");
  Row expected = createRow(
      {
          {a, "1"},
      }
  );
  REQUIRE(result->size() == 1);
  REQUIRE(expected == result->getRows()[0]);
}

TEST_CASE("Evaluate With Statement both const") {
  auto pkb = new PKB();
  pkb->addProc("example");

  // a = 3
  pkb->addStatement(1, assign, "example");
  pkb->addAssignPattern(1, "a", "3");
  pkb->addConst("3");
  pkb->addVariable("a");
  pkb->addModifyStmt(1, "a");

  // a = 3
  pkb->addStatement(2, assign, "example");
  pkb->addAssignPattern(2, "a", "3");
  pkb->addConst("3");
  pkb->addVariable("a");
  pkb->addModifyStmt(2, "a");

  std::string program = "constant c; assign a; Select a with 3 = 3";
  auto queryParser = new QueryParser(program);
  queryParser->
      parse();
  INFO(queryParser->parsedQueryToString())
  auto evaluator = QueryEvaluator(queryParser, pkb);
  auto result = evaluator.evaluate();

  QuerySynonym a = QuerySynonym(DesignEntityType::Assign, "a");
  auto expected = createRowList(
      {
          {
              {a, "1"},
          }, {
              {a, "2"},
          }
      }
  );
  REQUIRE(result->size() == expected.size());
  auto res = result->getRows();
  for (const auto &r: expected) {
    REQUIRE(std::count(res.begin(), res.end(), r) != 0);
  }
}

TEST_CASE("Calls from procedure synonym to wildcard") {
  auto pkb = new PKB();
  pkb->addProc("a");
  pkb->addProc("b");
  pkb->addProc("c");
  pkb->addStatement(1, call, "a");
  pkb->addStatement(2, call, "c");
  pkb->addCall(1, "a", "b");
  pkb->addCall(2, "c", "b");
  // procedure a {call b;} procedure b {} procedure c {call b;}


  std::string program = "procedure p; Select p such that Calls(p, _)";
  auto queryParser = new QueryParser(program);
  queryParser->parse();
  auto evaluator = QueryEvaluator(queryParser, pkb);
  auto result = evaluator.evaluate();
  QuerySynonym p = QuerySynonym(DesignEntityType::Procedure, "p");
  RowList expected = createRowList(
      {
          {{p, "a"}},
          {{p, "c"}}
      }
  );
  auto rows = result->getRows();

  REQUIRE(result->size() == 2);
  for (auto &r: expected) {
    REQUIRE(std::find(rows.begin(), rows.end(), r) != rows.end());
  }
}

TEST_CASE("Calls from procedure synonym to procedure synonym") {
  auto pkb = new PKB();
  pkb->addProc("a");
  pkb->addProc("b");
  pkb->addProc("c");
  pkb->addStatement(1, call, "a");
  pkb->addStatement(2, call, "c");
  pkb->addCall(1, "a", "b");
  pkb->addCall(2, "c", "b");
  // procedure a {call b;} procedure b {} procedure c {call b;}


  std::string program = "procedure p, q; Select p such that Calls(p, q)";
  auto queryParser = new QueryParser(program);
  queryParser->parse();
  auto evaluator = QueryEvaluator(queryParser, pkb);
  auto result = evaluator.evaluate();
  QuerySynonym p = QuerySynonym(DesignEntityType::Procedure, "p");
  QuerySynonym q = QuerySynonym(DesignEntityType::Procedure, "q");
  RowList expected = createRowList({
                                       {{p, "a"}, {q, "b"}},
                                       {{p, "c"}, {q, "b"}}
                                   });
  auto rows = result->getRows();

  REQUIRE(result->size() == 2);
  for (auto &r: expected) {
    REQUIRE(std::find(rows.begin(), rows.end(), r) != rows.end());
  }
}

TEST_CASE("Calls from wildcard to procedure synonym") {
  auto pkb = new PKB();
  pkb->addProc("a");
  pkb->addProc("b");
  pkb->addProc("c");
  pkb->addStatement(1, call, "a");
  pkb->addStatement(2, call, "c");
  pkb->addCall(1, "a", "b");
  pkb->addCall(2, "c", "b");
  // procedure a {call b;} procedure b {} procedure c {call b;}


  std::string program = "procedure p; Select p such that Calls(_, p)";
  auto queryParser = new QueryParser(program);
  queryParser->parse();
  auto evaluator = QueryEvaluator(queryParser, pkb);
  auto result = evaluator.evaluate();
  QuerySynonym p = QuerySynonym(DesignEntityType::Procedure, "p");
  RowList expected = createRowList(
      {
          {{p, "b"}}
      }
  );
  auto rows = result->getRows();
  REQUIRE(result->size() == expected.size());
  for (auto &r: expected) {
    REQUIRE(std::find(rows.begin(), rows.end(), r) != rows.end());
  }
}

TEST_CASE("Calls between wildcards") {
  auto pkb = new PKB();
  pkb->addProc("a");
  pkb->addProc("b");
  pkb->addProc("c");
  pkb->addStatement(1, call, "a");
  pkb->addStatement(2, call, "c");
  pkb->addCall(1, "a", "b");
  pkb->addCall(2, "c", "b");
  // procedure a {call b;} procedure b {} procedure c {call b;}


  std::string program = "procedure p; Select p such that Calls(_, _)";
  auto queryParser = new QueryParser(program);
  queryParser->parse();
  auto evaluator = QueryEvaluator(queryParser, pkb);
  auto result = evaluator.evaluate();
  QuerySynonym p = QuerySynonym(DesignEntityType::Procedure, "p");
  RowList expected = createRowList({
                                       {{p, "a"}},
                                       {{p, "b"}},
                                       {{p, "c"}}
                                   });
  auto rows = result->getRows();

  REQUIRE(result->size() == 3);
  for (auto &r: expected) {
    REQUIRE(std::find(rows.begin(), rows.end(), r) != rows.end());
  }
}

TEST_CASE("Calls* from procedure synonym to procedure synonym") {
  auto pkb = new PKB();
  pkb->addProc("a");
  pkb->addProc("b");
  pkb->addProc("c");
  pkb->addStatement(1, call, "a");
  pkb->addStatement(2, call, "b");
  pkb->addCall(1, "a", "b");
  pkb->addCall(2, "b", "c");

  pkb->addCallStar("a", "b");
  pkb->addCallStar("a", "c");
  pkb->addCallStar("b", "c");
  // a calls b, b calls c


  std::string program = "procedure p, q; Select p such that Calls*(p, q)";
  auto queryParser = new QueryParser(program);
  queryParser->parse();
  auto evaluator = QueryEvaluator(queryParser, pkb);
  auto result = evaluator.evaluate();
  QuerySynonym p = QuerySynonym(DesignEntityType::Procedure, "p");
  QuerySynonym q = QuerySynonym(DesignEntityType::Procedure, "q");
  RowList expected = createRowList({
                                       {{p, "a"}, {q, "b"}},
                                       {{p, "b"}, {q, "c"}},
                                       {{p, "a"}, {q, "c"}}
                                   });
  auto rows = result->getRows();

  REQUIRE(result->size() == 3);
  for (auto &r: expected) {
    REQUIRE(std::find(rows.begin(), rows.end(), r) != rows.end());
  }
}

TEST_CASE("Test with constant") {
  auto pkb = new PKB();
  pkb->addProc("a");
  pkb->addProc("b");
  pkb->addProc("c");

  std::string program = "procedure p; Select p with p.procName = \"a\"";
  auto queryParser = new QueryParser(program);
  queryParser->parse();
  auto evaluator = QueryEvaluator(queryParser, pkb);
  auto result = evaluator.evaluate();
  QuerySynonym p = QuerySynonym(DesignEntityType::Procedure, "p");
  RowList expected = createRowList({
                                       {{p, "a"}}
                                   });
  auto rows = result->getRows();

  REQUIRE(result->size() == 1);
  for (auto &r: expected) {
    REQUIRE(std::find(rows.begin(), rows.end(), r) != rows.end());
  }
}

TEST_CASE("Test with attributes") {
  auto pkb = new PKB();
  pkb->addProc("a");
  pkb->addProc("b");
  pkb->addProc("c");
  pkb->addStatement(1, assign, "a");
  pkb->addVariable("x");
  pkb->addModifyStmt(1, "x");
  pkb->addModifyProc("a", "x");

  std::string program = "procedure p, q; Select p with p.procName = q.procName such that Modifies(q, _)";
  auto queryParser = new QueryParser(program);
  queryParser->parse();
  auto evaluator = QueryEvaluator(queryParser, pkb);
  auto result = evaluator.evaluate();
  QuerySynonym p = QuerySynonym(DesignEntityType::Procedure, "p");
  QuerySynonym q = QuerySynonym(DesignEntityType::Procedure, "q");
  RowList expected = createRowList({
                                       {{p, "a"}, {q, "a"}}
                                   });
  auto rows = result->getRows();

  REQUIRE(result->size() == 1);
  for (auto &r: expected) {
    REQUIRE(std::find(rows.begin(), rows.end(), r) != rows.end());
  }
}

TEST_CASE("Test selecting prog_line") {
  auto pkb = new PKB();
  pkb->addProc("a");
  pkb->addStatement(1, assign, "a");
  pkb->addStatement(2, assign, "a");
  pkb->addStatement(3, assign, "a");

  std::string program = "prog_line n; Select n";
  auto queryParser = new QueryParser(program);
  queryParser->parse();
  auto evaluator = QueryEvaluator(queryParser, pkb);
  auto result = evaluator.evaluate();
  QuerySynonym n = QuerySynonym(DesignEntityType::ProgLine, "n");
  RowList expected = createRowList({
                                       {{n, "1"}},
                                       {{n, "2"}},
                                       {{n, "3"}}
                                   });
  auto rows = result->getRows();

  REQUIRE(result->size() == 3);
  for (auto &r: expected) {
    REQUIRE(std::find(rows.begin(), rows.end(), r) != rows.end());
  }
}