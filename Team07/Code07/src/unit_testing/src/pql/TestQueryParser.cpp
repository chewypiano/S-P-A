
//// Created by vaish on 5/9/2021.

#include "catch.hpp"
#include <string>
#include "QueryParser.h"

const char *invalidQueryOutputString = R""""(Parsed: [
Query(
  INVALID QUERY
)
])"""";

TEST_CASE("Parser Sanity Check") {
  REQUIRE(1 == 1);
}

TEST_CASE("Single basic valid query") {
  std::string program = "assign a; Select a";
  const char *expectedString = R""""(Parsed: [
Query(
  Output: a
  QuerySynonym(Assign, a)
)
])"""";
  QueryParser queryParser = QueryParser(program);
  queryParser.parse();
  REQUIRE(queryParser.parsedQueryToString() == expectedString);
}

TEST_CASE("Query with multiple synonyms") {
  std::string program = "assign a; stmt s1; Select a such that Modifies(s1,\"a\")";
  const char *expectedString = R""""(Parsed: [
Query(
  Output: a
  QuerySynonym(Assign, a)
  QuerySynonym(Stmt, s1)
  -----
  SuchThat ModifiesS(s1, "a")
)
])"""";
  QueryParser queryParser = QueryParser(program);
  queryParser.parse();
  REQUIRE(queryParser.parsedQueryToString() == expectedString);
}

TEST_CASE("Query with multiple synonyms of a single type") {
  std::string program = "assign a, a1; stmt s1; variable v; Select a such that Modifies(s1,v)";
  const char *expectedString = R""""(Parsed: [
Query(
  Output: a
  QuerySynonym(Assign, a)
  QuerySynonym(Assign, a1)
  QuerySynonym(Stmt, s1)
  QuerySynonym(Variable, v)
  -----
  SuchThat ModifiesS(s1, v)
)
])"""";
  QueryParser queryParser = QueryParser(program);
  queryParser.parse();
  REQUIRE(queryParser.parsedQueryToString() == expectedString);
}

TEST_CASE("Query with multiple synonyms of a single type, for multiple types") {
  std::string program = "assign a, a1; stmt s1, s2; Select a";
  const char *expectedString = R""""(Parsed: [
Query(
  Output: a
  QuerySynonym(Assign, a)
  QuerySynonym(Assign, a1)
  QuerySynonym(Stmt, s1)
  QuerySynonym(Stmt, s2)
)
])"""";
  QueryParser queryParser = QueryParser(program);
  queryParser.parse();
  REQUIRE(queryParser.parsedQueryToString() == expectedString);
}

TEST_CASE("Query with multiple synonyms of a single type, for multiple types with such that clause") {
  std::string program = "assign a, a1; stmt s1, s2; Select a such that Parent(s1, s2)";
  const char *expectedString = R""""(Parsed: [
Query(
  Output: a
  QuerySynonym(Assign, a)
  QuerySynonym(Assign, a1)
  QuerySynonym(Stmt, s1)
  QuerySynonym(Stmt, s2)
  -----
  SuchThat Parent(_, _)
)
])"""";
  QueryParser queryParser = QueryParser(program);
  queryParser.parse();
  REQUIRE(queryParser.parsedQueryToString() == expectedString);
}

TEST_CASE("Query with such that clause and pattern clause") {
  std::string program = "assign a, a1; stmt s1, s2; Select a such that Parent(s1, s2) pattern a(\"x\",_)";
  const char *expectedString = R""""(Parsed: [
Query(
  Output: a
  QuerySynonym(Assign, a)
  QuerySynonym(Assign, a1)
  QuerySynonym(Stmt, s1)
  QuerySynonym(Stmt, s2)
  Pattern a("x", _)
  -----
  SuchThat Parent(_, _)
)
])"""";
  QueryParser queryParser = QueryParser(program);
  queryParser.parse();
  REQUIRE(queryParser.parsedQueryToString() == expectedString);
}

TEST_CASE("Query with more that 2 such that and pattern clauses") {
  std::string program =
      "assign a, a1; stmt s1, s2; if i; variable v; Select a such that Parent(i, s2) pattern a(\"x\",_\"y\"_) such that Modifies(s1, v)";
  const char *expectedString = R""""(Parsed: [
Query(
  Output: a
  QuerySynonym(Assign, a)
  QuerySynonym(Assign, a1)
  QuerySynonym(Stmt, s1)
  QuerySynonym(Stmt, s2)
  QuerySynonym(If, i)
  QuerySynonym(Variable, v)
  Pattern a("x", _" y "_)
  -----
  SuchThat Parent(i, _)
  SuchThat ModifiesS(s1, v)
)
])"""";
  QueryParser queryParser = QueryParser(program);
  queryParser.parse();
  REQUIRE(queryParser.parsedQueryToString() == expectedString);
}

TEST_CASE("Query with all Iteration 1 such that clauses") {
  std::string program =
      "assign a, a1; stmt s1, s2; if i1, i2; variable v; Select a such that Parent(i1, s2) such that Parent*(i2, s2) such that Modifies(s1, v) such that Follows(s1,s2) such that Follows*(s1,s2) such that Uses(s1, v)";
  const char *expectedString = R""""(Parsed: [
Query(
  Output: a
  QuerySynonym(Assign, a)
  QuerySynonym(Assign, a1)
  QuerySynonym(Stmt, s1)
  QuerySynonym(Stmt, s2)
  QuerySynonym(If, i1)
  QuerySynonym(If, i2)
  QuerySynonym(Variable, v)
  SuchThat Parent(i1, s2)
  SuchThat Parent*(i2, s2)
  SuchThat ModifiesS(s1, v)
  SuchThat Follows(s1, s2)
  SuchThat Follows*(s1, s2)
  SuchThat UsesS(s1, v)
)
])"""";
  QueryParser queryParser = QueryParser(program);
  queryParser.parse();
  REQUIRE(queryParser.parsedQueryToString() == expectedString);
}

TEST_CASE("Invalid query, Multiple basic queries") {
  std::string program = "assign a; Select a\n   variable v;";
  QueryParser queryParser = QueryParser(program);
  queryParser.parse();
  REQUIRE(queryParser.parsedQueryToString() == invalidQueryOutputString);
}

TEST_CASE("Test invalid PQL with no semicolon") {
  std::string program = "assign a1 Select a1";
  auto queryParser = QueryParser(program);
  REQUIRE_NOTHROW(queryParser.parse());
  REQUIRE(queryParser.parsedQueryToString() == invalidQueryOutputString);
}

TEST_CASE("Test invalid PQL with 1 word assign") {
  std::string program = "assign";
  auto queryParser = QueryParser(program);
  REQUIRE_NOTHROW(queryParser.parse());
  REQUIRE(queryParser.parsedQueryToString() == invalidQueryOutputString);
}

TEST_CASE("Test invalid PQL with 1 word procedure") {
  std::string program = "procedure";
  auto queryParser = QueryParser(program);
  REQUIRE_NOTHROW(queryParser.parse());
  REQUIRE(queryParser.parsedQueryToString() == invalidQueryOutputString);
}

TEST_CASE("Test invalid PQL with gibberish") {
  std::string program = "!@#$!%@#";
  auto queryParser = QueryParser(program);
  REQUIRE_NOTHROW(queryParser.parse());
  REQUIRE(queryParser.parsedQueryToString() == invalidQueryOutputString);
}

TEST_CASE("Test valid PQL with 2 keywords") {
  std::string program = "assign assign;\n Select assign such that Modifies(assign,_)";
  auto queryParser = QueryParser(program);
  REQUIRE_NOTHROW(queryParser.parse());
  REQUIRE(queryParser.parsedQueryToString() != invalidQueryOutputString);
}

TEST_CASE("Test invalid PQL assign missing semi colon") {
  std::string program = "assign a";
  auto queryParser = QueryParser(program);
  REQUIRE_NOTHROW(queryParser.parse());
  REQUIRE(queryParser.parsedQueryToString() == invalidQueryOutputString);
}

TEST_CASE("Test invalid PQL ends at such") {
  std::string program = "assign a; Select a such";
  auto queryParser = QueryParser(program);
  REQUIRE_NOTHROW(queryParser.parse());
  REQUIRE(queryParser.parsedQueryToString() == invalidQueryOutputString);
}

TEST_CASE("Test invalid PQL ends at such that") {
  std::string program = "assign a; Select a such that";
  auto queryParser = QueryParser(program);
  REQUIRE_NOTHROW(queryParser.parse());
  REQUIRE(queryParser.parsedQueryToString() == invalidQueryOutputString);
}

TEST_CASE("Test invalid PQL with invalid assign 2nd name string") {
  std::string program = "assign a, !abc ; Select a";
  auto queryParser = QueryParser(program);
  REQUIRE_NOTHROW(queryParser.parse());
  REQUIRE(queryParser.parsedQueryToString() == invalidQueryOutputString);
}

TEST_CASE("Test invalid PQL with invalid assign name string") {
  std::string program = "assign !abc ; Select a";
  auto queryParser = QueryParser(program);
  REQUIRE_NOTHROW(queryParser.parse());
  REQUIRE(queryParser.parsedQueryToString() == invalidQueryOutputString);
}

TEST_CASE("Test invalid PQL empty") {
  std::string program;
  auto queryParser = QueryParser(program);
  REQUIRE_NOTHROW(queryParser.parse());
  REQUIRE(queryParser.parsedQueryToString() == invalidQueryOutputString);
}

TEST_CASE("Test invalid PQL Select only") {
  std::string program = "assign a; Select";
  auto queryParser = QueryParser(program);
  REQUIRE_NOTHROW(queryParser.parse());
  REQUIRE(queryParser.parsedQueryToString() == invalidQueryOutputString);
}

TEST_CASE("Test invalid PQL Select with invalid name") {
  std::string program = "assign a; Select !(a)";
  auto queryParser = QueryParser(program);
  REQUIRE_NOTHROW(queryParser.parse());
  REQUIRE(queryParser.parsedQueryToString() == invalidQueryOutputString);
}

TEST_CASE("Test invalid PQL Pattern only") {
  std::string program = "assign a; Select a pattern";
  auto queryParser = QueryParser(program);
  REQUIRE_NOTHROW(queryParser.parse());
  REQUIRE(queryParser.parsedQueryToString() == invalidQueryOutputString);
}

TEST_CASE("Test invalid PQL Pattern invalid symbols after pattern") {
  std::string program = "assign a; Select a pattern !@#$";
  auto queryParser = QueryParser(program);
  REQUIRE_NOTHROW(queryParser.parse());
  REQUIRE(queryParser.parsedQueryToString() == invalidQueryOutputString);
}

TEST_CASE("Test invalid PQL empty Bracketed contents") {
  std::string program = "assign a; Select a pattern a()";
  auto queryParser = QueryParser(program);
  REQUIRE_NOTHROW(queryParser.parse());
  REQUIRE(queryParser.parsedQueryToString() == invalidQueryOutputString);
}

TEST_CASE("Test invalid PQL invalid Bracketed contents") {
  std::string program = "assign a; Select a pattern a(!@#$)";
  auto queryParser = QueryParser(program);
  REQUIRE_NOTHROW(queryParser.parse());
  REQUIRE(queryParser.parsedQueryToString() == invalidQueryOutputString);
}

TEST_CASE("Test invalid PQL invalid unclosed bracket") {
  std::string program = "assign a; Select a pattern a(";
  auto queryParser = QueryParser(program);
  REQUIRE_NOTHROW(queryParser.parse());
  REQUIRE(queryParser.parsedQueryToString() == invalidQueryOutputString);
}

TEST_CASE("Test invalid PQL invalid bracket with missing 2nd token") {
  std::string program = "assign a; Select a pattern a(_,)";
  auto queryParser = QueryParser(program);
  REQUIRE_NOTHROW(queryParser.parse());
  REQUIRE(queryParser.parsedQueryToString() == invalidQueryOutputString);
}

TEST_CASE("Test invalid PQL invalid bracket with missing first token") {
  std::string program = "assign a; Select a pattern a(,_)";
  auto queryParser = QueryParser(program);
  REQUIRE_NOTHROW(queryParser.parse());
  REQUIRE(queryParser.parsedQueryToString() == invalidQueryOutputString);
}

TEST_CASE("Test invalid PQL invalid arguments with additional token") {
  std::string program = "assign a; Select a pattern a(_,_,_)";
  auto queryParser = QueryParser(program);
  REQUIRE_NOTHROW(queryParser.parse());
  REQUIRE(queryParser.parsedQueryToString() == invalidQueryOutputString);
}

TEST_CASE("Test valid PQL pattern with wildcard token") {
  std::string program = "assign a; Select a pattern a(_,_\"x\"_)";
  auto queryParser = QueryParser(program);
  REQUIRE_NOTHROW(queryParser.parse());
  REQUIRE(queryParser.parsedQueryToString() != invalidQueryOutputString);
}

TEST_CASE("Test valid PQL pattern with constant") {
  std::string program = "constant c; Select c";
  auto queryParser = QueryParser(program);
  REQUIRE_NOTHROW(queryParser.parse());
  REQUIRE(queryParser.parsedQueryToString() != invalidQueryOutputString);
}

TEST_CASE("Test invalid PQL invalid synonym name as number in declaration") {
  std::string program = "assign 9; Select a pattern a(_,_)";
  auto queryParser = QueryParser(program);
  REQUIRE_NOTHROW(queryParser.parse());
  REQUIRE(queryParser.parsedQueryToString() == invalidQueryOutputString);
}

TEST_CASE("Test invalid PQL invalid synonym name as wildcard in declaration") {
  std::string program = "assign _; Select a pattern a(_,_)";
  auto queryParser = QueryParser(program);
  REQUIRE_NOTHROW(queryParser.parse());
  REQUIRE(queryParser.parsedQueryToString() == invalidQueryOutputString);
}

TEST_CASE("Test invalid PQL invalid synonym name as number in select") {
  std::string program = "assign a; Select 2 pattern a(_,_)";
  auto queryParser = QueryParser(program);
  REQUIRE_NOTHROW(queryParser.parse());
  REQUIRE(queryParser.parsedQueryToString() == invalidQueryOutputString);
}

TEST_CASE("Test invalid PQL invalid synonym name as wildcard in select") {
  std::string program = "assign a; Select _ pattern a(_,_)";
  auto queryParser = QueryParser(program);
  REQUIRE_NOTHROW(queryParser.parse());
  REQUIRE(queryParser.parsedQueryToString() == invalidQueryOutputString);
}

TEST_CASE("Test valid PQL with seemingly invalid synonym name") {
  std::string program = "assign assign; if if; stmt stmt; Select assign pattern assign(_,_)";
  auto queryParser = QueryParser(program);
  const char *expectedString = R""""(Parsed: [
Query(
  Output: assign
  QuerySynonym(Assign, assign)
  QuerySynonym(If, if)
  QuerySynonym(Stmt, stmt)
  Pattern assign(_, _)
)
])"""";
  REQUIRE_NOTHROW(queryParser.parse());
  REQUIRE(queryParser.parsedQueryToString() == expectedString);
}

TEST_CASE("Test valid PQL with seemingly invalid synonym name in pattern clause") {
  std::string program = "assign assign, pattern; if if; stmt stmt; Select assign pattern pattern(_,_)";
  auto queryParser = QueryParser(program);
  const char *expectedString = R""""(Parsed: [
Query(
  Output: assign
  QuerySynonym(Assign, assign)
  QuerySynonym(Assign, pattern)
  QuerySynonym(If, if)
  QuerySynonym(Stmt, stmt)
  -----
  Pattern pattern(_, _)
)
])"""";
  REQUIRE_NOTHROW(queryParser.parse());
  REQUIRE(queryParser.parsedQueryToString() == expectedString);
}

TEST_CASE("Test invalid PQL invalid select casing") {
  std::string program = "assign a; select a pattern a(_,_)";
  auto queryParser = QueryParser(program);
  REQUIRE_NOTHROW(queryParser.parse());
  REQUIRE(queryParser.parsedQueryToString() == invalidQueryOutputString);
}

TEST_CASE("Test invalid undeclared synonym") {
  std::string program = "stmt s;\nSelect a such that Uses(s, \"a\")";
  auto queryParser = QueryParser(program);
  REQUIRE_NOTHROW(queryParser.parse());
  REQUIRE(queryParser.parsedQueryToString() == invalidQueryOutputString);
}

TEST_CASE("Test valid input from system test") {
  std::string program = "stmt a; variable v;\nSelect a such that Modifies(a,v)";
  auto queryParser = QueryParser(program);
  REQUIRE_NOTHROW(queryParser.parse());
  REQUIRE(queryParser.parsedQueryToString() != invalidQueryOutputString);
}

TEST_CASE("Test valid parent*") {
  std::string program = "stmt a, ifs; Select a such that Parent*(ifs,a)";
  auto queryParser = QueryParser(program);
  REQUIRE_NOTHROW(queryParser.parse());
  REQUIRE(queryParser.parsedQueryToString() != invalidQueryOutputString);
}

TEST_CASE("Test valid pattern with wildcard on both sides") {
  std::string program = "assign a;\nSelect a pattern a(_,_\"x\"_)";
  const char *expectedString = R""""(Parsed: [
Query(
  Output: a
  QuerySynonym(Assign, a)
  Pattern a(_, _" x "_)
)
])"""";
  auto queryParser = QueryParser(program);
  REQUIRE_NOTHROW(queryParser.parse());
  REQUIRE(queryParser.parsedQueryToString() == expectedString);
}

TEST_CASE("Test invalid pattern with invalid syntax of right std::string arg") {
  std::string program = "assign a; Select a pattern a(_,\"!\")";
  auto queryParser = QueryParser(program);
  REQUIRE_NOTHROW(queryParser.parse());
  REQUIRE(queryParser.parsedQueryToString() == invalidQueryOutputString);
}

TEST_CASE("Test invalid pattern with invalid syntax of right ident std::string arg") {
  std::string program = "assign a; Select a pattern a(_,\"alpha1@\")";
  auto queryParser = QueryParser(program);
  REQUIRE_NOTHROW(queryParser.parse());
  REQUIRE(queryParser.parsedQueryToString() == invalidQueryOutputString);
}

TEST_CASE("Test valid pattern with valid syntax of right integer std::string arg") {
  std::string program = "assign a; Select a pattern a(_,\"231%23\")";
  auto queryParser = QueryParser(program);
  REQUIRE_NOTHROW(queryParser.parse());
  REQUIRE(queryParser.parsedQueryToString() != invalidQueryOutputString);
}

TEST_CASE("Test invalid pattern with invalid type of left arg as integer") {
  std::string program = "assign a; Select a pattern a(1,\"x\")";
  auto queryParser = QueryParser(program);
  REQUIRE_NOTHROW(queryParser.parse());
  REQUIRE(queryParser.parsedQueryToString() == invalidQueryOutputString);
}

TEST_CASE("Test invalid pattern with invalid pattern synonym type") {
  std::string program = "assign a1; variable a; Select a pattern a(_,\"x\")";
  auto queryParser = QueryParser(program);
  REQUIRE_NOTHROW(queryParser.parse());
  REQUIRE(queryParser.parsedQueryToString() == invalidQueryOutputString);
}

TEST_CASE("Test invalid pattern with undeclared pattern synonym type") {
  std::string program = "assign a1; Select a pattern a(_,\"x\")";
  auto queryParser = QueryParser(program);
  REQUIRE_NOTHROW(queryParser.parse());
  REQUIRE(queryParser.parsedQueryToString() == invalidQueryOutputString);
}

TEST_CASE("Test invalid select synonym") {
  std::string program = "assign a; Select ; pattern a(_,\"x\")";
  auto queryParser = QueryParser(program);
  REQUIRE_NOTHROW(queryParser.parse());
  REQUIRE(queryParser.parsedQueryToString() == invalidQueryOutputString);
}

TEST_CASE("Test invalid ModifiesS clause with wildcard as first argument") {
  std::string program = "assign a; Select a such that Modifies(_, \"x\")";
  auto queryParser = QueryParser(program);
  REQUIRE_NOTHROW(queryParser.parse());
  REQUIRE(queryParser.parsedQueryToString() == invalidQueryOutputString);
}

TEST_CASE("Test invalid UsesS clause with wildcard as first argument") {
  std::string program = "assign a; Select a such that Uses(_, \"x\")";
  auto queryParser = QueryParser(program);
  REQUIRE_NOTHROW(queryParser.parse());
  REQUIRE(queryParser.parsedQueryToString() == invalidQueryOutputString);
}

TEST_CASE("Test valid ModifiesP clause with ident as first argument") {
  std::string program = "assign a; Select a such that Modifies(\"y\", \"x\")";
  auto queryParser = QueryParser(program);
  REQUIRE_NOTHROW(queryParser.parse());
  REQUIRE(queryParser.parsedQueryToString() != invalidQueryOutputString);
}

TEST_CASE("Test valid UsesP clause with ident as first argument") {
  std::string program = "assign a; Select a such that Uses(\"y\", \"x\")";
  auto queryParser = QueryParser(program);
  REQUIRE_NOTHROW(queryParser.parse());
  REQUIRE(queryParser.parsedQueryToString() != invalidQueryOutputString);
}

TEST_CASE("Test invalid ModifiesS clause with undeclared synonym as first arg") {
  std::string program = "assign a; Select a such that Modifies(a1, \"x\")";
  auto queryParser = QueryParser(program);
  REQUIRE_NOTHROW(queryParser.parse());
  REQUIRE(queryParser.parsedQueryToString() == invalidQueryOutputString);
}

TEST_CASE("Test invalid UsesS clause with undeclared synonym as first arg") {
  std::string program = "assign a; Select a such that Uses(a1, \"x\")";
  auto queryParser = QueryParser(program);
  REQUIRE_NOTHROW(queryParser.parse());
  REQUIRE(queryParser.parsedQueryToString() == invalidQueryOutputString);
}

TEST_CASE("Test valid ModifiesS clause with integer as first arg") {
  std::string program = "assign a; Select a such that Modifies(1, \"x\")";
  auto queryParser = QueryParser(program);
  REQUIRE_NOTHROW(queryParser.parse());
  REQUIRE(queryParser.parsedQueryToString() != invalidQueryOutputString);
}

TEST_CASE("Test valid UsesS clause with integer as first arg") {
  std::string program = "assign a; Select a such that Uses(1, \"x\")";
  auto queryParser = QueryParser(program);
  REQUIRE_NOTHROW(queryParser.parse());
  REQUIRE(queryParser.parsedQueryToString() != invalidQueryOutputString);
}

TEST_CASE("Test invalid Follows clause with ident as first arg") {
  std::string program = "assign a; Select a such that Follows(\"x\", 1)";
  auto queryParser = QueryParser(program);
  REQUIRE_NOTHROW(queryParser.parse());
  REQUIRE(queryParser.parsedQueryToString() == invalidQueryOutputString);
}

TEST_CASE("Test invalid Follows clause with ident as second arg") {
  std::string program = "assign a; Select a such that Follows(1, \"x\")";
  auto queryParser = QueryParser(program);
  REQUIRE_NOTHROW(queryParser.parse());
  REQUIRE(queryParser.parsedQueryToString() == invalidQueryOutputString);
}

TEST_CASE("Test invalid Follows clause with undeclared synonym as first arg") {
  std::string program = "assign a; Select a such that Follows(v, 1)";
  auto queryParser = QueryParser(program);
  REQUIRE_NOTHROW(queryParser.parse());
  REQUIRE(queryParser.parsedQueryToString() == invalidQueryOutputString);
}

TEST_CASE("Test invalid FollowsT clause with ident as first arg") {
  std::string program = "assign a; Select a such that Follows*(\"x\", 1)";
  auto queryParser = QueryParser(program);
  REQUIRE_NOTHROW(queryParser.parse());
  REQUIRE(queryParser.parsedQueryToString() == invalidQueryOutputString);
}

TEST_CASE("Test invalid FollowsT clause with ident as second arg") {
  std::string program = "assign a; Select a such that Follows*(1, \"x\")";
  auto queryParser = QueryParser(program);
  REQUIRE_NOTHROW(queryParser.parse());
  REQUIRE(queryParser.parsedQueryToString() == invalidQueryOutputString);
}

TEST_CASE("Test invalid FollowsT clause with undeclared synonym as first arg") {
  std::string program = "assign a; Select a such that Follows*(v, 1)";
  auto queryParser = QueryParser(program);
  REQUIRE_NOTHROW(queryParser.parse());
  REQUIRE(queryParser.parsedQueryToString() == invalidQueryOutputString);
}

TEST_CASE("Test invalid Parent clause with ident as first arg") {
  std::string program = "assign a; Select a such that Parent(\"x\", 1)";
  auto queryParser = QueryParser(program);
  REQUIRE_NOTHROW(queryParser.parse());
  REQUIRE(queryParser.parsedQueryToString() == invalidQueryOutputString);
}

TEST_CASE("Test invalid Parent clause with ident as second arg") {
  std::string program = "assign a; Select a such that Parent(1, \"x\")";
  auto queryParser = QueryParser(program);
  REQUIRE_NOTHROW(queryParser.parse());
  REQUIRE(queryParser.parsedQueryToString() == invalidQueryOutputString);
}

TEST_CASE("Test invalid Parent clause with undeclared synonym as first arg") {
  std::string program = "assign a; Select a such that Parent(v, 1)";
  auto queryParser = QueryParser(program);
  REQUIRE_NOTHROW(queryParser.parse());
  REQUIRE(queryParser.parsedQueryToString() == invalidQueryOutputString);
}

TEST_CASE("Test invalid ParentT clause with ident as first arg") {
  std::string program = "assign a; Select a such that Parent*(\"x\", 1)";
  auto queryParser = QueryParser(program);
  REQUIRE_NOTHROW(queryParser.parse());
  REQUIRE(queryParser.parsedQueryToString() == invalidQueryOutputString);
}

TEST_CASE("Test invalid ParentT clause with ident as second arg") {
  std::string program = "assign a; Select a such that Parent*(1, \"x\")";
  auto queryParser = QueryParser(program);
  REQUIRE_NOTHROW(queryParser.parse());
  REQUIRE(queryParser.parsedQueryToString() == invalidQueryOutputString);
}

TEST_CASE("Test invalid ParentT clause with undeclared synonym as first arg") {
  std::string program = "assign a; Select a such that Parent*(v, 1)";
  auto queryParser = QueryParser(program);
  REQUIRE_NOTHROW(queryParser.parse());
  REQUIRE(queryParser.parsedQueryToString() == invalidQueryOutputString);
}

TEST_CASE("Test invalid query declaration with missing synonym name") {
  std::string program = "assign ; Select a such that Parent*(a, 1)";
  auto queryParser = QueryParser(program);
  REQUIRE_NOTHROW(queryParser.parse());
  REQUIRE(queryParser.parsedQueryToString() == invalidQueryOutputString);
}

TEST_CASE("Test invalid query declaration with missing synonym name in additional synonym") {
  std::string program = "assign a1, ; Select a such that Parent*(a, 1)";
  auto queryParser = QueryParser(program);
  REQUIRE_NOTHROW(queryParser.parse());
  REQUIRE(queryParser.parsedQueryToString() == invalidQueryOutputString);
}

TEST_CASE("Test invalid query with missing whole select statement") {
  std::string program = "assign a1, a2;";
  auto queryParser = QueryParser(program);
  REQUIRE_NOTHROW(queryParser.parse());
  REQUIRE(queryParser.parsedQueryToString() == invalidQueryOutputString);
}

TEST_CASE("Test query for parent") {
  std::string program = "stmt a, ifs;\n"
                        " Select a such that Parent*(ifs, a)";
  auto queryParser = QueryParser(program);
  REQUIRE_NOTHROW(queryParser.parse());
  REQUIRE(queryParser.parsedQueryToString() != invalidQueryOutputString);
}

TEST_CASE("Test invalid query for parent with missing such that") {
  std::string program = "assign a;\n"
                        "Select a Parent(a, 14)";
  auto queryParser = QueryParser(program);
  queryParser.parse();
  REQUIRE(queryParser.parsedQueryToString() == invalidQueryOutputString);
}

TEST_CASE("Test valid query, invalid for iter 1 where multiple such that clauses") {
  std::string program = "assign a;\n"
                        "Select a such that Parent(a, 14) such that Parent(a, 12)";
  auto queryParser = QueryParser(program);
  queryParser.parse();
  REQUIRE(queryParser.parsedQueryToString() != invalidQueryOutputString);
}

TEST_CASE("Test valid query, invalid for iter 1 where multiple pattern clauses") {
  std::string program = "assign a, a1;\n"
                        "Select a pattern a(\"x\",_) pattern a1(\"y\",_)";
  auto queryParser = QueryParser(program);
  queryParser.parse();
  REQUIRE(queryParser.parsedQueryToString() != invalidQueryOutputString);
}

TEST_CASE("Test valid query, invalid for iter 1 where pattern clause before such that clause") {
  std::string program = "assign a, a1;\n"
                        "Select a pattern a(\"x\",_) such that Parent(a, 12)";
  auto queryParser = QueryParser(program);
  queryParser.parse();
  REQUIRE(queryParser.parsedQueryToString() != invalidQueryOutputString);
}

TEST_CASE("Valid query with whitespace in pattern clause rhs arg") {
  std::string
      program = "assign a, a1; stmt s1, s2; Select a such that Parent(s1, s2) pattern a(\"x\",_\"   \n \r a \"_)";
  QueryParser queryParser = QueryParser(program);
  queryParser.parse();
  REQUIRE(queryParser.parsedQueryToString() != invalidQueryOutputString);
}

TEST_CASE("Invalid query with semicolon at the end") {
  std::string program = "assign a; variable v;\nSelect a pattern a(_,_\"a\"_);";
  QueryParser queryParser = QueryParser(program);
  queryParser.parse();
  REQUIRE(queryParser.parsedQueryToString() == invalidQueryOutputString);
}

TEST_CASE("Invalid query with multiple synonyms of diff type declared with same name") {
  std::string program = "variable v; stmt v; assign a; Select v such that Uses(a, v)";
  QueryParser queryParser = QueryParser(program);
  queryParser.parse();
  REQUIRE(queryParser.parsedQueryToString() == invalidQueryOutputString);
}

TEST_CASE("Invalid query with multiple synonyms of same type declared with same name") {
  std::string program = "variable v, v; assign a; Select v such that Uses(a, v)";
  QueryParser queryParser = QueryParser(program);
  queryParser.parse();
  REQUIRE(queryParser.parsedQueryToString() == invalidQueryOutputString);
}

TEST_CASE("Invalid query with multiple additional synonyms declared with same name") {
  std::string program = "variable v, v1, v2; assign a, a1, a1; Select v such that Uses(a, v)";
  QueryParser queryParser = QueryParser(program);
  queryParser.parse();
  REQUIRE(queryParser.parsedQueryToString() == invalidQueryOutputString);
}

TEST_CASE("Invalid query with non stmt ref synonyms on left side of Follows") {
  std::string program = "variable v; stmt s; Select v such that Follows(v, s)";
  QueryParser queryParser = QueryParser(program);
  queryParser.parse();
  REQUIRE(queryParser.parsedQueryToString() == invalidQueryOutputString);
}

TEST_CASE("Invalid query with non stmt ref synonyms on right side of Follows") {
  std::string program = "variable v; stmt s; Select v such that Follows(s, v)";
  QueryParser queryParser = QueryParser(program);
  queryParser.parse();
  REQUIRE(queryParser.parsedQueryToString() == invalidQueryOutputString);
}

TEST_CASE("Valid query for Follows") {
  std::string program = "stmt s, s1; Select s such that Follows(s, s1)";
  QueryParser queryParser = QueryParser(program);
  queryParser.parse();
  REQUIRE(queryParser.parsedQueryToString() != invalidQueryOutputString);
}

TEST_CASE("Invalid query with non stmt ref synonyms on left side of Follows*") {
  std::string program = "variable v; stmt s; Select v such that Follows*(v, s)";
  QueryParser queryParser = QueryParser(program);
  queryParser.parse();
  REQUIRE(queryParser.parsedQueryToString() == invalidQueryOutputString);
}

TEST_CASE("Invalid query with non stmt ref synonyms on right side of Follows*") {
  std::string program = "variable v; stmt s; Select v such that Follows*(s, v)";
  QueryParser queryParser = QueryParser(program);
  queryParser.parse();
  REQUIRE(queryParser.parsedQueryToString() == invalidQueryOutputString);
}

TEST_CASE("Valid query for Follows*") {
  std::string program = "stmt s, s1; Select s such that Follows*(s, s1)";
  QueryParser queryParser = QueryParser(program);
  queryParser.parse();
  REQUIRE(queryParser.parsedQueryToString() != invalidQueryOutputString);
}

TEST_CASE("Invalid query with non stmt ref synonyms on left side of Parent") {
  std::string program = "variable v; stmt s; Select v such that Parent(v, s)";
  QueryParser queryParser = QueryParser(program);
  queryParser.parse();
  REQUIRE(queryParser.parsedQueryToString() == invalidQueryOutputString);
}

TEST_CASE("Invalid query with non stmt ref synonyms on right side of Parent") {
  std::string program = "variable v; stmt s; Select v such that Parent(s, v)";
  QueryParser queryParser = QueryParser(program);
  queryParser.parse();
  REQUIRE(queryParser.parsedQueryToString() == invalidQueryOutputString);
}

TEST_CASE("Valid query for Parent") {
  std::string program = "stmt s, s1; Select s such that Parent(s, s1)";
  QueryParser queryParser = QueryParser(program);
  queryParser.parse();
  REQUIRE(queryParser.parsedQueryToString() != invalidQueryOutputString);
}

TEST_CASE("Invalid query with non stmt ref synonyms on left side of Parent*") {
  std::string program = "variable v; stmt s; Select v such that Parent*(v, s)";
  QueryParser queryParser = QueryParser(program);
  queryParser.parse();
  REQUIRE(queryParser.parsedQueryToString() == invalidQueryOutputString);
}

TEST_CASE("Invalid query with non stmt ref synonyms on right side of Parent*") {
  std::string program = "variable v; stmt s; Select v such that Parent*(s, v)";
  QueryParser queryParser = QueryParser(program);
  queryParser.parse();
  REQUIRE(queryParser.parsedQueryToString() == invalidQueryOutputString);
}

TEST_CASE("Valid query for Parent*") {
  std::string program = "stmt s, s1; Select s such that Parent*(s, s1)";
  QueryParser queryParser = QueryParser(program);
  queryParser.parse();
  REQUIRE(queryParser.parsedQueryToString() != invalidQueryOutputString);
}

TEST_CASE("Invalid query with non stmt ref synonyms on left side of Uses") {
  std::string program = "variable v, v1; Select v such that Uses(v1, v)";
  QueryParser queryParser = QueryParser(program);
  queryParser.parse();
  REQUIRE(queryParser.parsedQueryToString() == invalidQueryOutputString);
}

TEST_CASE("Invalid query with non variable synonyms on right side of Uses") {
  std::string program = "assign a; stmt s; Select v such that Uses(s, a)";
  QueryParser queryParser = QueryParser(program);
  queryParser.parse();
  REQUIRE(queryParser.parsedQueryToString() == invalidQueryOutputString);
}

TEST_CASE("Valid query for Uses") {
  std::string program = "variable v; stmt s; Select v such that Uses(s, v)";
  QueryParser queryParser = QueryParser(program);
  queryParser.parse();
  REQUIRE(queryParser.parsedQueryToString() != invalidQueryOutputString);
}

TEST_CASE("Invalid query with non stmt ref synonyms on left side of Modifies") {
  std::string program = "variable v, v1; Select v such that Modifies(v1, v)";
  QueryParser queryParser = QueryParser(program);
  queryParser.parse();
  REQUIRE(queryParser.parsedQueryToString() == invalidQueryOutputString);
}

TEST_CASE("Invalid query with non variable synonyms on right side of Modifies") {
  std::string program = "assign a; stmt s; Select v such that Modifies(s, a)";
  QueryParser queryParser = QueryParser(program);
  queryParser.parse();
  REQUIRE(queryParser.parsedQueryToString() == invalidQueryOutputString);
}

TEST_CASE("Valid query for Modifies") {
  std::string program = "variable v; stmt s; Select v such that Modifies(s, v)";
  QueryParser queryParser = QueryParser(program);
  queryParser.parse();
  REQUIRE(queryParser.parsedQueryToString() != invalidQueryOutputString);
}

TEST_CASE("Invalid query for Pattern") {
  std::string program = "while w; assign a; Select s pattern a(w, \"_\")";
  QueryParser queryParser = QueryParser(program);
  queryParser.parse();
  REQUIRE(queryParser.parsedQueryToString() == invalidQueryOutputString);
}

TEST_CASE("Valid query for Pattern") {
  std::string program = "variable v ; assign a; Select v pattern a(v, _)";
  QueryParser queryParser = QueryParser(program);
  queryParser.parse();
  REQUIRE(queryParser.parsedQueryToString() != invalidQueryOutputString);
}
TEST_CASE("Valid query for Uses with const") {
  std::string program = "variable v ; assign a; Select v such that Uses(a, \"a\")";
  QueryParser queryParser = QueryParser(program);
  queryParser.parse();
  REQUIRE(queryParser.parsedQueryToString() != invalidQueryOutputString);
}

TEST_CASE("Valid query for Modifies with const") {
  std::string program = "variable v ; assign a; Select v such that Modifies(a, \"a\")";
  QueryParser queryParser = QueryParser(program);
  queryParser.parse();
  REQUIRE(queryParser.parsedQueryToString() != invalidQueryOutputString);
}

TEST_CASE("Valid random high complexity system test query") {
  std::string program = "assign a;\n Select a pattern a(\"a\", _)";
  QueryParser queryParser = QueryParser(program);
  queryParser.parse();
  REQUIRE(queryParser.parsedQueryToString() != invalidQueryOutputString);
}

TEST_CASE("Valid query with assign pattern rhs as pure string, invalid in iter 1") {
  std::string program = "assign a;\n Select a pattern a(\"a\", \"a % b * c\")";
  const char *expectedString = R""""(Parsed: [
Query(
  Output: a
  QuerySynonym(Assign, a)
  Pattern a("a", " a b % c * ")
)
])"""";
  QueryParser queryParser = QueryParser(program);
  queryParser.parse();
  REQUIRE(queryParser.parsedQueryToString() == expectedString);
}

TEST_CASE("Test valid query with WithClause stmt# lhs int rhs") {
  std::string program = "assign a;\n Select a pattern a(\"a\", \"a / 1 - 3\") with a.stmt# = 2";
  const char *expectedString = R""""(Parsed: [
Query(
  Output: a
  QuerySynonym(Assign, a)
  Pattern a("a", " a 1 / 3 - ")
  With(a.stmt#, 2)
)
])"""";
  QueryParser queryParser = QueryParser(program);
  queryParser.parse();
  REQUIRE(queryParser.parsedQueryToString() == expectedString);
}

TEST_CASE("Test valid query with WithClause stmt# lhs stmt# rhs") {
  std::string program = "assign a;\n Select a pattern a(\"a\", \"a + (3 / x)\") with a.stmt# = a.stmt#";
  const char *expectedString = R""""(Parsed: [
Query(
  Output: a
  QuerySynonym(Assign, a)
  Pattern a("a", " a 3 x / + ")
  With(a.stmt#, a.stmt#)
)
])"""";
  QueryParser queryParser = QueryParser(program);
  queryParser.parse();
  REQUIRE(queryParser.parsedQueryToString() == expectedString);
}

TEST_CASE("Test invalid query with WithClause stmt# lhs ident std::string rhs") {
  std::string program = "assign a;\n Select a pattern a(\"a\", \"a + 1\") with a.stmt# = \"lol\"";
  QueryParser queryParser = QueryParser(program);
  queryParser.parse();
  REQUIRE(queryParser.parsedQueryToString() == invalidQueryOutputString);
}

TEST_CASE("Test valid query with WithClause procName lhs varName rhs") {
  std::string program = "stmt s; procedure p; variable v; Select p with p.procName = v.varName";
  const char *expectedString = R""""(Parsed: [
Query(
  Output: p
  QuerySynonym(Stmt, s)
  QuerySynonym(Procedure, p)
  QuerySynonym(Variable, v)
  With(p.procName, v.varName)
)
])"""";
  QueryParser queryParser = QueryParser(program);
  queryParser.parse();
  REQUIRE(queryParser.parsedQueryToString() == expectedString);
}

TEST_CASE("Test valid query with WithClause procName lhs ident std::string rhs") {
  std::string program = "stmt s; procedure p; variable v; Select p with p.procName = \"yes\"";
  const char *expectedString = R""""(Parsed: [
Query(
  Output: p
  QuerySynonym(Stmt, s)
  QuerySynonym(Procedure, p)
  QuerySynonym(Variable, v)
  With(p.procName, "yes")
)
])"""";
  QueryParser queryParser = QueryParser(program);
  queryParser.parse();
  REQUIRE(queryParser.parsedQueryToString() == expectedString);
}

TEST_CASE("Test invalid query with WithClause procName lhs integer rhs") {
  std::string program = "stmt s; procedure p; variable v; Select p with p.procName = 3";
  QueryParser queryParser = QueryParser(program);
  queryParser.parse();
  REQUIRE(queryParser.parsedQueryToString() == invalidQueryOutputString);
}

TEST_CASE("Test valid query with multiple WithClause") {
  std::string program = "stmt s; procedure p; variable v; Select p with p.procName = \"yes\" and s.stmt# = 1";
  const char *expectedString = R""""(Parsed: [
Query(
  Output: p
  QuerySynonym(Stmt, s)
  QuerySynonym(Procedure, p)
  QuerySynonym(Variable, v)
  With(p.procName, "yes")
  -----
  With(s.stmt#, 1)
)
])"""";
  QueryParser queryParser = QueryParser(program);
  queryParser.parse();
  REQUIRE(queryParser.parsedQueryToString() == expectedString);
}

TEST_CASE("Test valid query with WithClause value lhs int rhs") {
  std::string program = "constant c; Select c with c.value = 2";
  const char *expectedString = R""""(Parsed: [
Query(
  Output: c
  QuerySynonym(Constant, c)
  With(c.value, 2)
)
])"""";
  QueryParser queryParser = QueryParser(program);
  queryParser.parse();
  REQUIRE(queryParser.parsedQueryToString() == expectedString);
}

TEST_CASE("Test valid query with WithClause value lhs stmt# rhs") {
  std::string program = "constant c; stmt s; Select c with c.value = s.stmt#";
  const char *expectedString = R""""(Parsed: [
Query(
  Output: c
  QuerySynonym(Constant, c)
  QuerySynonym(Stmt, s)
  With(c.value, s.stmt#)
)
])"""";
  QueryParser queryParser = QueryParser(program);
  queryParser.parse();
  REQUIRE(queryParser.parsedQueryToString() == expectedString);
}

TEST_CASE("Test invalid query with WithClause value lhs std::string rhs") {
  std::string program = "constant c; Select c with c.value = \"What\"";
  QueryParser queryParser = QueryParser(program);
  queryParser.parse();
  REQUIRE(queryParser.parsedQueryToString() == invalidQueryOutputString);
}

TEST_CASE("Test valid query with WithClause varName lhs std::string rhs") {
  std::string program = "print p; Select p with p.varName = \"mafia\"";
  const char *expectedString = R""""(Parsed: [
Query(
  Output: p
  QuerySynonym(Print, p)
  With(p.varName, "mafia")
)
])"""";
  QueryParser queryParser = QueryParser(program);
  queryParser.parse();
  REQUIRE(queryParser.parsedQueryToString() == expectedString);
}

TEST_CASE("Test invalid query with WithClause varName lhs stmt# rhs") {
  std::string program = "variable v; Call c1; Select v with v.varName = c1.stmt#";
  QueryParser queryParser = QueryParser(program);
  queryParser.parse();
  REQUIRE(queryParser.parsedQueryToString() == invalidQueryOutputString);
}

TEST_CASE("Test valid query with 'and' for with and for such that") {
  std::string program =
      "print p; assign a, a1; if i; Select p with p.varName = \"mafia\" and i.stmt# = a1.stmt# such that Parent(i, a1) and Follows(a, _)";
  const char *expectedString = R""""(Parsed: [
Query(
  Output: p
  QuerySynonym(Print, p)
  QuerySynonym(Assign, a)
  QuerySynonym(Assign, a1)
  QuerySynonym(If, i)
  With(p.varName, "mafia")
  With(i.stmt#, a1.stmt#)
  SuchThat Parent(i, a1)
  -----
  SuchThat Follows(a, _)
)
])"""";
  QueryParser queryParser = QueryParser(program);
  queryParser.parse();
  REQUIRE(queryParser.parsedQueryToString() == expectedString);
}

TEST_CASE("Test valid query with WithClause progLine lhs progLine rhs") {
  std::string program = "prog_line p1, p2, p3; Select p2 with p1 = p2 and p2 = p3";
  const char *expectedString = R""""(Parsed: [
Query(
  Output: p2
  QuerySynonym(ProgLine, p1)
  QuerySynonym(ProgLine, p2)
  QuerySynonym(ProgLine, p3)
  With(p1, p2)
  With(p2, p3)
)
])"""";
  QueryParser queryParser = QueryParser(program);
  queryParser.parse();
  REQUIRE(queryParser.parsedQueryToString() == expectedString);
}

TEST_CASE("Test valid query with WithClause progLine lhs integer rhs") {
  std::string program = "prog_line p1, p2; Select p2 with p1 = p2 and p2 = 3";
  const char *expectedString = R""""(Parsed: [
Query(
  Output: p2
  QuerySynonym(ProgLine, p1)
  QuerySynonym(ProgLine, p2)
  With(p1, p2)
  With(p2, 3)
)
])"""";
  QueryParser queryParser = QueryParser(program);
  queryParser.parse();
  REQUIRE(queryParser.parsedQueryToString() == expectedString);
}

TEST_CASE("Test invalid query with WithClause progLine lhs String rhs") {
  std::string program = "prog_line p1, p2; Select p2 with p1 = p2 and p2 = \"potatos\"";
  QueryParser queryParser = QueryParser(program);
  queryParser.parse();
  REQUIRE(queryParser.parsedQueryToString() == invalidQueryOutputString);
}

TEST_CASE("Test valid query with one 'and' between suchThats and another for Withs") {
  std::string program =
      "prog_line p1, p2; stmt s1, s2; Select p2 with p1 = p2 and p2 = s1.stmt# such that Follows(s1, s2) and Follows*(p1, s2)";
  QueryParser queryParser = QueryParser(program);
  queryParser.parse();
  REQUIRE(queryParser.parsedQueryToString() != invalidQueryOutputString);
}

TEST_CASE("Test invalid query with invalid 'and' between SuchThat and With") {
  std::string program = "prog_line p1, p2; stmt s1, s2; Select p2 with p1 = p2 and Follows(s1, s2)";
  QueryParser queryParser = QueryParser(program);
  queryParser.parse();
  REQUIRE(queryParser.parsedQueryToString() == invalidQueryOutputString);
}

TEST_CASE("Test valid query with progLine as arg for statement SuchThatClause") {
  std::string program = "prog_line n; stmt s; Select s such that Follows* (s, n) with n=10";
  const char *expectedString = R""""(Parsed: [
Query(
  Output: s
  QuerySynonym(ProgLine, n)
  QuerySynonym(Stmt, s)
  SuchThat Follows*(s, n)
  With(n, 10)
)
])"""";
  QueryParser queryParser = QueryParser(program);
  queryParser.parse();
  REQUIRE(queryParser.parsedQueryToString() == expectedString);
}

TEST_CASE("Test valid query with non-progline as arg for Next") {
  std::string program = "assign a; while w; Select a such that Modifies (a, \"x\") and Parent* (w, a) and Next* (1, a)";
  const char *expectedString = R""""(Parsed: [
Query(
  Output: a
  QuerySynonym(Assign, a)
  QuerySynonym(While, w)
  SuchThat ModifiesS(a, "x")
  SuchThat Parent*(w, a)
  SuchThat Next*(1, a)
)
])"""";
  QueryParser queryParser = QueryParser(program);
  queryParser.parse();
  REQUIRE(queryParser.parsedQueryToString() == expectedString);
}

TEST_CASE("Test valid pattern with 'and'") {
  std::string program = "assign a;\nSelect a pattern a(_,_\"x\"_) and a(\"x\", \"y\")";
  const char *expectedString = R""""(Parsed: [
Query(
  Output: a
  QuerySynonym(Assign, a)
  Pattern a(_, _" x "_)
  Pattern a("x", " y ")
)
])"""";
  auto queryParser = QueryParser(program);
  queryParser.parse();
  REQUIRE(queryParser.parsedQueryToString() == expectedString);
}

TEST_CASE("Test valid pattern with 'and' also with 'with'") {
  std::string program =
      "assign a; constant c;\nSelect a pattern a(_,_\"x\"_) and a(\"x\", \"y\") with a.stmt# = 2 and c.value = a.stmt#";
  const char *expectedString = R""""(Parsed: [
Query(
  Output: a
  QuerySynonym(Assign, a)
  QuerySynonym(Constant, c)
  Pattern a(_, _" x "_)
  Pattern a("x", " y ")
  With(a.stmt#, 2)
  With(c.value, a.stmt#)
)
])"""";
  auto queryParser = QueryParser(program);
  queryParser.parse();
  REQUIRE(queryParser.parsedQueryToString() == expectedString);
}

TEST_CASE("Test valid query with Calls SuchThatClause") {
  std::string program =
      "procedure p1, p2; call c; Select p1 such that Calls(p1, p2)";
  const char *expectedString = R""""(Parsed: [
Query(
  Output: p1
  QuerySynonym(Procedure, p1)
  QuerySynonym(Procedure, p2)
  QuerySynonym(Call, c)
  SuchThat Calls(p1, p2)
)
])"""";
  auto queryParser = QueryParser(program);
  queryParser.parse();
  REQUIRE(queryParser.parsedQueryToString() == expectedString);
}

TEST_CASE("Test valid query with CallsT SuchThatClause") {
  std::string program =
      "procedure p1, p2; call c; Select p1 such that Calls*(p1, p2)";
  const char *expectedString = R""""(Parsed: [
Query(
  Output: p1
  QuerySynonym(Procedure, p1)
  QuerySynonym(Procedure, p2)
  QuerySynonym(Call, c)
  SuchThat Calls*(p1, p2)
)
])"""";
  auto queryParser = QueryParser(program);
  queryParser.parse();
  REQUIRE(queryParser.parsedQueryToString() == expectedString);
}

TEST_CASE("Test invalid query with Calls with call Type") {
  std::string program = "procedure p1, p2; call c; Select p1 such that Calls(p1, c)";
  QueryParser queryParser = QueryParser(program);
  queryParser.parse();
  REQUIRE(queryParser.parsedQueryToString() == invalidQueryOutputString);
}

TEST_CASE("Test invalid query with Calls with read Type") {
  std::string program = "procedure p1, p2; read r; Select p1 such that Calls(r, p2)";
  QueryParser queryParser = QueryParser(program);
  queryParser.parse();
  REQUIRE(queryParser.parsedQueryToString() == invalidQueryOutputString);
}

TEST_CASE("Test invalid query with CallsT with call Type") {
  std::string program = "procedure p1, p2; call c; Select p1 such that Calls*(c, p1)";
  QueryParser queryParser = QueryParser(program);
  queryParser.parse();
  REQUIRE(queryParser.parsedQueryToString() == invalidQueryOutputString);
}

TEST_CASE("Test invalid query with CallsT with read Type") {
  std::string program = "procedure p1, p2; read r; Select p1 such that Calls*(p2, r)";
  QueryParser queryParser = QueryParser(program);
  queryParser.parse();
  REQUIRE(queryParser.parsedQueryToString() == invalidQueryOutputString);
}

TEST_CASE("Test valid query with ModifiesP and proc on lhs") {
  std::string program =
      "procedure p1, p2; variable v; Select p1 such that Modifies(p1, v)";
  const char *expectedString = R""""(Parsed: [
Query(
  Output: p1
  QuerySynonym(Procedure, p1)
  QuerySynonym(Procedure, p2)
  QuerySynonym(Variable, v)
  SuchThat ModifiesP(p1, v)
)
])"""";
  auto queryParser = QueryParser(program);
  queryParser.parse();
  REQUIRE(queryParser.parsedQueryToString() == expectedString);
}

TEST_CASE("Test valid query with ModifiesP and call on lhs") {
  std::string program =
      "procedure p1, p2; call c; variable v; Select p1 such that Modifies(c, v)";
  const char *expectedString = R""""(Parsed: [
Query(
  Output: p1
  QuerySynonym(Procedure, p1)
  QuerySynonym(Procedure, p2)
  QuerySynonym(Call, c)
  QuerySynonym(Variable, v)
  -----
  SuchThat ModifiesS(c, v)
)
])"""";
  auto queryParser = QueryParser(program);
  queryParser.parse();
  REQUIRE(queryParser.parsedQueryToString() == expectedString);
}

TEST_CASE("Test invalid query with ModifiesP and stmtRef on rhs") {
  std::string program = "procedure p1, p2; print p; Select p1 such that Modifies(p1, p)";
  QueryParser queryParser = QueryParser(program);
  queryParser.parse();
  REQUIRE(queryParser.parsedQueryToString() == invalidQueryOutputString);
}

TEST_CASE("Test valid query with UsesP and proc on lhs") {
  std::string program =
      "procedure p1, p2; variable v; Select p1 such that Uses(p1, v)";
  const char *expectedString = R""""(Parsed: [
Query(
  Output: p1
  QuerySynonym(Procedure, p1)
  QuerySynonym(Procedure, p2)
  QuerySynonym(Variable, v)
  SuchThat UsesP(p1, v)
)
])"""";
  auto queryParser = QueryParser(program);
  queryParser.parse();
  REQUIRE(queryParser.parsedQueryToString() == expectedString);
}

TEST_CASE("Test valid query with UsesP and call on lhs") {
  std::string program =
      "procedure p1, p2; call c; variable v; Select p1 such that Uses(c, v)";
  const char *expectedString = R""""(Parsed: [
Query(
  Output: p1
  QuerySynonym(Procedure, p1)
  QuerySynonym(Procedure, p2)
  QuerySynonym(Call, c)
  QuerySynonym(Variable, v)
  -----
  SuchThat UsesS(c, v)
)
])"""";
  auto queryParser = QueryParser(program);
  queryParser.parse();
  REQUIRE(queryParser.parsedQueryToString() == expectedString);
}

TEST_CASE("Test valid query with UsesS and progLine on lhs") {
  std::string program =
      "prog_line p; variable v; Select v such that Uses(p, v)";
  const char *expectedString = R""""(Parsed: [
Query(
  Output: v
  QuerySynonym(ProgLine, p)
  QuerySynonym(Variable, v)
  SuchThat UsesS(p, v)
)
])"""";
  auto queryParser = QueryParser(program);
  queryParser.parse();
  REQUIRE(queryParser.parsedQueryToString() == expectedString);
}

TEST_CASE("Test invalid query with UsesP and stmtRef on rhs") {
  std::string program = "procedure p1, p2; print p; Select p1 such that Uses(p1, p)";
  QueryParser queryParser = QueryParser(program);
  queryParser.parse();
  REQUIRE(queryParser.parsedQueryToString() == invalidQueryOutputString);
}

TEST_CASE("Test valid query with Next and both args progLines") {
  std::string program =
      "prog_line p1, p2; Select p1 such that Next(p1, p2)";
  const char *expectedString = R""""(Parsed: [
Query(
  Output: p1
  QuerySynonym(ProgLine, p1)
  QuerySynonym(ProgLine, p2)
  SuchThat Next(p1, p2)
)
])"""";
  auto queryParser = QueryParser(program);
  queryParser.parse();
  REQUIRE(queryParser.parsedQueryToString() == expectedString);
}

TEST_CASE("Test valid query with Next and both args other int types and wildcard") {
  std::string program =
      "read r; assign a; Select a such that Next(r, a) and Next(a, _)";
  const char *expectedString = R""""(Parsed: [
Query(
  Output: a
  QuerySynonym(Read, r)
  QuerySynonym(Assign, a)
  SuchThat Next(r, a)
  SuchThat Next(a, _)
)
])"""";
  auto queryParser = QueryParser(program);
  queryParser.parse();
  REQUIRE(queryParser.parsedQueryToString() == expectedString);
}

TEST_CASE("Test invalid query with Next and one arg is std::string type") {
  std::string program =
      "variable v; assign a; Select a such that Next(v, a) and Next(a, _)";
  auto queryParser = QueryParser(program);
  queryParser.parse();
  REQUIRE(queryParser.parsedQueryToString() == invalidQueryOutputString);
}

TEST_CASE("Test valid query with Next* and both args progLines") {
  std::string program =
      "prog_line p1, p2; Select p1 such that Next*(p1, p2)";
  const char *expectedString = R""""(Parsed: [
Query(
  Output: p1
  QuerySynonym(ProgLine, p1)
  QuerySynonym(ProgLine, p2)
  SuchThat Next*(p1, p2)
)
])"""";
  auto queryParser = QueryParser(program);
  queryParser.parse();
  REQUIRE(queryParser.parsedQueryToString() == expectedString);
}

TEST_CASE("Test valid query with Next* and both args other int types and wildcard") {
  std::string program =
      "read r; assign a; Select a such that Next*(r, a) and Next*(a, _)";
  const char *expectedString = R""""(Parsed: [
Query(
  Output: a
  QuerySynonym(Read, r)
  QuerySynonym(Assign, a)
  SuchThat Next*(r, a)
  SuchThat Next*(a, _)
)
])"""";
  auto queryParser = QueryParser(program);
  queryParser.parse();
  REQUIRE(queryParser.parsedQueryToString() == expectedString);
}

TEST_CASE("Test invalid query with Next* and one arg is std::string type") {
  std::string program =
      "variable v; assign a; Select a such that Next*(v, a) and Next*(a, _)";
  auto queryParser = QueryParser(program);
  queryParser.parse();
  REQUIRE(queryParser.parsedQueryToString() == invalidQueryOutputString);
}

TEST_CASE("Test valid query with multiple bracketed valid expr on assign pattern rhs arg") {
  std::string program =
      "assign a; Select a pattern a(_, \"(((((((((((((((((cal)))))))))))))))))\")";
  auto queryParser = QueryParser(program);
  queryParser.parse();
  REQUIRE(queryParser.parsedQueryToString() != invalidQueryOutputString);
}

TEST_CASE("Test invalid query with multiple bracketed but 1 extra right bracket expr on assign pattern rhs arg") {
  std::string program =
      "assign a; Select a pattern a(_, \"(((((((((((((((((cal))))))))))))))))))\")";
  auto queryParser = QueryParser(program);
  queryParser.parse();
  REQUIRE(queryParser.parsedQueryToString() == invalidQueryOutputString);
}

TEST_CASE("Test invalid query with multiple bracketed but 1 extra left bracket expr on assign pattern rhs arg") {
  std::string program =
      "assign a; Select a pattern a(_, \"((((((((((((((((((cal)))))))))))))))))\")";
  auto queryParser = QueryParser(program);
  queryParser.parse();
  REQUIRE(queryParser.parsedQueryToString() == invalidQueryOutputString);
}

TEST_CASE("Test valid query with complicated but valid expr on assign pattern rhs arg") {
  std::string program =
      "assign a; Select a pattern a(_, \"a + ((r - y - (x + bike)) * s + p % (x * v * z) % (car / bike / bus))\")";
  auto queryParser = QueryParser(program);
  queryParser.parse();
  REQUIRE(queryParser.parsedQueryToString() != invalidQueryOutputString);
}

TEST_CASE("Test invalid query with double operator on assign pattern rhs arg") {
  std::string program =
      "assign a; Select a pattern a(_, \"a + ((r - y - (x + bike)) ** s + p % (x * v * z) % (car / bike / bus))\")";
  auto queryParser = QueryParser(program);
  queryParser.parse();
  REQUIRE(queryParser.parsedQueryToString() == invalidQueryOutputString);
}

TEST_CASE("Test invalid query with empty brackets on assign pattern rhs arg") {
  std::string program =
      "assign a; Select a pattern a(_, \"a + ((r - y - () / (x + bike)) * s + p % (x * v * z) % (car / bike / bus))\")";
  auto queryParser = QueryParser(program);
  queryParser.parse();
  REQUIRE(queryParser.parsedQueryToString() == invalidQueryOutputString);
}

TEST_CASE("Test invalid query with illegal operator between brackets on assign pattern rhs arg") {
  std::string program =
      "assign a; Select a pattern a(_, \"a + ((r - y - (+) / (x + bike)) * s + p % (x * v * z) % (car / bike / bus))\")";
  auto queryParser = QueryParser(program);
  queryParser.parse();
  REQUIRE(queryParser.parsedQueryToString() == invalidQueryOutputString);
}

TEST_CASE("Test invalid query with missing expression on rhs of operator on assign pattern rhs arg") {
  std::string program =
      "assign a; Select a pattern a(_, _\"a +\"_)";
  auto queryParser = QueryParser(program);
  queryParser.parse();
  REQUIRE(queryParser.parsedQueryToString() == invalidQueryOutputString);
}

TEST_CASE("Test valid query with if pattern synonym type in pattern clause, left arg is synonym") {
  std::string program =
      "if if; variable v; Select v pattern if(v, _, _)";
  const char *expectedString = R""""(Parsed: [
Query(
  Output: v
  QuerySynonym(If, if)
  QuerySynonym(Variable, v)
  Pattern if(v, _, _)
)
])"""";
  auto queryParser = QueryParser(program);
  queryParser.parse();
  REQUIRE(queryParser.parsedQueryToString() == expectedString);
}

TEST_CASE("Test valid query with if pattern synonym type in pattern clause, left arg is wildcard") {
  std::string program =
      "if if; variable v; Select if pattern if(_, _, _)";
  const char *expectedString = R""""(Parsed: [
Query(
  Output: if
  QuerySynonym(If, if)
  QuerySynonym(Variable, v)
  Pattern if(_, _, _)
)
])"""";
  auto queryParser = QueryParser(program);
  queryParser.parse();
  REQUIRE(queryParser.parsedQueryToString() == expectedString);
}

TEST_CASE("Test valid query with if pattern synonym type in pattern clause, left arg is IDENT") {
  std::string program =
      "if if; variable v; Select if pattern if(\"juxtaposition\", _, _)";
  const char *expectedString = R""""(Parsed: [
Query(
  Output: if
  QuerySynonym(If, if)
  QuerySynonym(Variable, v)
  Pattern if("juxtaposition", _, _)
)
])"""";
  auto queryParser = QueryParser(program);
  queryParser.parse();
  REQUIRE(queryParser.parsedQueryToString() == expectedString);
}

TEST_CASE("Test invalid query with if pattern synonym type in pattern clause, left arg is integer") {
  std::string program =
      "if if; variable v; Select if pattern if(21, _, _)";
  auto queryParser = QueryParser(program);
  queryParser.parse();
  REQUIRE(queryParser.parsedQueryToString() == invalidQueryOutputString);
}

TEST_CASE("Test invalid query with if pattern synonym type in pattern clause, centre arg is not wildcard") {
  std::string program =
      "if if; variable v; Select if pattern if(v, \"_\", _)";
  auto queryParser = QueryParser(program);
  queryParser.parse();
  REQUIRE(queryParser.parsedQueryToString() == invalidQueryOutputString);
}

TEST_CASE("Test invalid query with if pattern synonym type in pattern clause, right arg is not wildcard") {
  std::string program =
      "if if; variable v; Select if pattern if(v, _, 19)";
  auto queryParser = QueryParser(program);
  queryParser.parse();
  REQUIRE(queryParser.parsedQueryToString() == invalidQueryOutputString);
}

TEST_CASE("Test invalid query with if pattern synonym type in pattern clause, left arg is non-variable synonym") {
  std::string program =
      "if if; assign a; Select if pattern if(a, _, _)";
  auto queryParser = QueryParser(program);
  queryParser.parse();
  REQUIRE(queryParser.parsedQueryToString() == invalidQueryOutputString);
}

TEST_CASE("Test invalid query with assign pattern synonym type in pattern clause, with 3 args instead of 2") {
  std::string program =
      "if if; assign a; Select if pattern a(\"what\", _, _)";
  auto queryParser = QueryParser(program);
  queryParser.parse();
  REQUIRE(queryParser.parsedQueryToString() == invalidQueryOutputString);
}

TEST_CASE("Test valid query with while pattern synonym type in pattern clause, left arg is synonym") {
  std::string program =
      "while while; variable v; Select v pattern while(v, _)";
  const char *expectedString = R""""(Parsed: [
Query(
  Output: v
  QuerySynonym(While, while)
  QuerySynonym(Variable, v)
  Pattern while(v, _)
)
])"""";
  auto queryParser = QueryParser(program);
  queryParser.parse();
  REQUIRE(queryParser.parsedQueryToString() == expectedString);
}

TEST_CASE("Test valid query with while pattern synonym type in pattern clause, left arg is wildcard") {
  std::string program =
      "while while; variable v; Select while pattern while(_, _)";
  const char *expectedString = R""""(Parsed: [
Query(
  Output: while
  QuerySynonym(While, while)
  QuerySynonym(Variable, v)
  Pattern while(_, _)
)
])"""";
  auto queryParser = QueryParser(program);
  queryParser.parse();
  REQUIRE(queryParser.parsedQueryToString() == expectedString);
}

TEST_CASE("Test valid query with while pattern synonym type in pattern clause, left arg is IDENT") {
  std::string program =
      "while while; variable v; Select while pattern while(\"juxtaposition\", _)";
  const char *expectedString = R""""(Parsed: [
Query(
  Output: while
  QuerySynonym(While, while)
  QuerySynonym(Variable, v)
  Pattern while("juxtaposition", _)
)
])"""";
  auto queryParser = QueryParser(program);
  queryParser.parse();
  REQUIRE(queryParser.parsedQueryToString() == expectedString);
}

TEST_CASE("Test invalid query with while pattern synonym type in pattern clause, left arg is integer") {
  std::string program =
      "while while; variable v; Select while pattern while(21, _)";
  auto queryParser = QueryParser(program);
  queryParser.parse();
  REQUIRE(queryParser.parsedQueryToString() == invalidQueryOutputString);
}

TEST_CASE("Test invalid query with while pattern synonym type in pattern clause, right arg is not wildcard") {
  std::string program =
      "while while; variable v; Select while pattern while(v, \"_\")";
  auto queryParser = QueryParser(program);
  queryParser.parse();
  REQUIRE(queryParser.parsedQueryToString() == invalidQueryOutputString);
}

TEST_CASE("Test invalid query with while pattern synonym type in pattern clause, left arg is non-variable synonym") {
  std::string program =
      "while while; assign a; Select while pattern while(a, _)";
  auto queryParser = QueryParser(program);
  queryParser.parse();
  REQUIRE(queryParser.parsedQueryToString() == invalidQueryOutputString);
}

TEST_CASE("Test valid query with multiple pattern clause of diff type connected by 'and'") {
  std::string program =
      "if if; variable v; while w; assign a; Select v pattern if(v, _, _) and w(v, _) and a(v, _)";
  const char *expectedString = R""""(Parsed: [
Query(
  Output: v
  QuerySynonym(If, if)
  QuerySynonym(Variable, v)
  QuerySynonym(While, w)
  QuerySynonym(Assign, a)
  Pattern if(v, _, _)
  Pattern w(v, _)
  Pattern a(v, _)
)
])"""";
  auto queryParser = QueryParser(program);
  queryParser.parse();
  REQUIRE(queryParser.parsedQueryToString() == expectedString);
}

TEST_CASE("Test valid query with 2nd pattern clause made to seem like Follows clause") {
  std::string program =
      "while while; assign Follows; Select while pattern while(_, _) and Follows(_, _)";
  auto queryParser = QueryParser(program);
  queryParser.parse();
  REQUIRE(queryParser.parsedQueryToString() != invalidQueryOutputString);
}

TEST_CASE("Test valid query with tuple output type, single synonym") {
  std::string program =
      "assign a; Select <a>";
  const char *expectedString = R""""(Parsed: [
Query(
  Output: a
  QuerySynonym(Assign, a)
)
])"""";
  auto queryParser = QueryParser(program);
  queryParser.parse();
  REQUIRE(queryParser.parsedQueryToString() == expectedString);
}

TEST_CASE("Test valid query with tuple output type, double synonym") {
  std::string program =
      "assign a, a1; Select <a, a1>";
  const char *expectedString = R""""(Parsed: [
Query(
  Output: a, a1
  QuerySynonym(Assign, a)
  QuerySynonym(Assign, a1)
)
])"""";
  auto queryParser = QueryParser(program);
  queryParser.parse();
  REQUIRE(queryParser.parsedQueryToString() == expectedString);
}

TEST_CASE("Test valid query with single attribute output type") {
  std::string program =
      "assign a, a1; Select a.stmt#";
  const char *expectedString = R""""(Parsed: [
Query(
  Output: a.stmt#
  QuerySynonym(Assign, a)
  QuerySynonym(Assign, a1)
)
])"""";
  auto queryParser = QueryParser(program);
  queryParser.parse();
  REQUIRE(queryParser.parsedQueryToString() == expectedString);
}

TEST_CASE("Test valid query with tuple output of synonym and attribute") {
  std::string program =
      "assign a, a1; Select <a.stmt#, a1>";
  const char *expectedString = R""""(Parsed: [
Query(
  Output: a.stmt#, a1
  QuerySynonym(Assign, a)
  QuerySynonym(Assign, a1)
)
])"""";
  auto queryParser = QueryParser(program);
  queryParser.parse();
  REQUIRE(queryParser.parsedQueryToString() == expectedString);
}

TEST_CASE("Test invalid query with tuple output of synonym and invalid attribute") {
  std::string program =
      "assign a, a1; Select <a.stm, a1>";
  auto queryParser = QueryParser(program);
  queryParser.parse();
  REQUIRE(queryParser.parsedQueryToString() == invalidQueryOutputString);
}

TEST_CASE("Test valid query with output of boolean") {
  std::string program =
      "assign a, a1; Select BOOLEAN";
  const char *expectedString = R""""(Parsed: [
Query(
  Output: BOOLEAN T/F
  QuerySynonym(Assign, a)
  QuerySynonym(Assign, a1)
)
])"""";
  auto queryParser = QueryParser(program);
  queryParser.parse();
  REQUIRE(queryParser.parsedQueryToString() == expectedString);
}

TEST_CASE("Test invalid query with tuple output of synonym and boolean") {
  std::string program =
      "assign a, a1; Select <a.procName, BOOLEAN>";
  auto queryParser = QueryParser(program);
  queryParser.parse();
  REQUIRE(queryParser.parsedQueryToString() == invalidQueryOutputString);
}

TEST_CASE("Test invalid query with single tuple output of boolean") {
  std::string program =
      "assign a, a1; Select <BOOLEAN>";
  auto queryParser = QueryParser(program);
  queryParser.parse();
  REQUIRE(queryParser.parsedQueryToString() == invalidQueryOutputString);
}

TEST_CASE("Test valid query with tuple output of all attributes and a synonym") {
  std::string program =
      "assign a, a1; procedure p; constant c; if i; variable v; Select <a1, i.stmt#, p.procName, v.varName, c.value>";
  const char *expectedString = R""""(Parsed: [
Query(
  Output: a1, i.stmt#, p.procName, v.varName, c.value
  QuerySynonym(Assign, a)
  QuerySynonym(Assign, a1)
  QuerySynonym(Procedure, p)
  QuerySynonym(Constant, c)
  QuerySynonym(If, i)
  QuerySynonym(Variable, v)
)
])"""";
  auto queryParser = QueryParser(program);
  queryParser.parse();
  REQUIRE(queryParser.parsedQueryToString() == expectedString);
}

TEST_CASE("Test invalid query with invalid attribute type for given synonym, value of assign") {
  std::string program =
      "assign a, a1; Select a.value";
  auto queryParser = QueryParser(program);
  queryParser.parse();
  REQUIRE(queryParser.parsedQueryToString() == invalidQueryOutputString);
}

TEST_CASE("Test invalid query with invalid attribute type for given synonym, procName of constant") {
  std::string program =
      "constant c; Select c.procName";
  auto queryParser = QueryParser(program);
  queryParser.parse();
  REQUIRE(queryParser.parsedQueryToString() == invalidQueryOutputString);
}

TEST_CASE("Test valid query with valid with clause lhs is progline and rhs is other syn stmtNo attr") {
  std::string program =
      "prog_line p; assign a; Select p with p = a.stmt#";
  auto queryParser = QueryParser(program);
  queryParser.parse();
  REQUIRE(queryParser.parsedQueryToString() != invalidQueryOutputString);
}

TEST_CASE("Test valid query with invalid with clause in terms of semantics but of matching types") {
  std::string program =
      "assign a; constant c; Select a with a.stmt# = c.stmt#";
  auto queryParser = QueryParser(program);
  queryParser.parse();
  REQUIRE(queryParser.parsedQueryToString() == invalidQueryOutputString);
}

TEST_CASE("Test valid postfix assign pattern rhs") {
  std::string program =
      "assign a; Select a pattern a(\"x\",_\"y\"_) and a(\"x\",\"y + 1\") and a(\"x\",\"   y * 1       \")";
  const char *expectedString = R""""(Parsed: [
Query(
  Output: a
  QuerySynonym(Assign, a)
  Pattern a("x", _" y "_)
  Pattern a("x", " y 1 + ")
  Pattern a("x", " y 1 * ")
)
])"""";
  QueryParser queryParser = QueryParser(program);
  queryParser.parse();
  REQUIRE(queryParser.parsedQueryToString() == expectedString);
}

TEST_CASE("Test valid query with just boolean, no synonyms and clauses") {
  std::string program = "Select BOOLEAN";
  auto queryParser = QueryParser(program);
  queryParser.parse();
  REQUIRE(queryParser.parsedQueryToString() != invalidQueryOutputString);
}

TEST_CASE("Test valid query with Affects SuchThatClause, args are assign syns") {
  std::string program =
      "assign a1, a2; Select a1 such that Affects(a1, a2)";
  const char *expectedString = R""""(Parsed: [
Query(
  Output: a1
  QuerySynonym(Assign, a1)
  QuerySynonym(Assign, a2)
  SuchThat Affects(a1, a2)
)
])"""";
  auto queryParser = QueryParser(program);
  queryParser.parse();
  REQUIRE(queryParser.parsedQueryToString() == expectedString);
}

TEST_CASE("Test valid query with Affects SuchThatClause, args are integers") {
  std::string program =
      "assign a1, a2; Select a1 such that Affects(1, 2)";
  const char *expectedString = R""""(Parsed: [
Query(
  Output: a1
  QuerySynonym(Assign, a1)
  QuerySynonym(Assign, a2)
  -----
  SuchThat Affects(1, 2)
)
])"""";
  auto queryParser = QueryParser(program);
  queryParser.parse();
  REQUIRE(queryParser.parsedQueryToString() == expectedString);
}

TEST_CASE("Test valid query with Affects SuchThatClause, args are wildcards") {
  std::string program =
      "assign a1, a2; Select a1 such that Affects(_, _)";
  const char *expectedString = R""""(Parsed: [
Query(
  Output: a1
  QuerySynonym(Assign, a1)
  QuerySynonym(Assign, a2)
  -----
  SuchThat Affects(_, _)
)
])"""";
  auto queryParser = QueryParser(program);
  queryParser.parse();
  REQUIRE(queryParser.parsedQueryToString() == expectedString);
}

TEST_CASE("Test valid query with Affects SuchThatClause, args are mix of integer and assign syn") {
  std::string program =
      "assign a1, a2; Select a1 such that Affects(3, a1)";
  const char *expectedString = R""""(Parsed: [
Query(
  Output: a1
  QuerySynonym(Assign, a1)
  QuerySynonym(Assign, a2)
  SuchThat Affects(3, a1)
)
])"""";
  auto queryParser = QueryParser(program);
  queryParser.parse();
  REQUIRE(queryParser.parsedQueryToString() == expectedString);
}

TEST_CASE("Test invalid query with Affects SuchThatClause, arg is non-assign syn") {
  std::string program = "If i; Select i such that Affects(i, 2)";
  auto queryParser = QueryParser(program);
  queryParser.parse();
  REQUIRE(queryParser.parsedQueryToString() == invalidQueryOutputString);
}

TEST_CASE("Test invalid query with Affects SuchThatClause, arg is integer but string") {
  std::string program = "If i; Select i such that Affects(2, \"9\")";
  auto queryParser = QueryParser(program);
  queryParser.parse();
  REQUIRE(queryParser.parsedQueryToString() == invalidQueryOutputString);
}

TEST_CASE("Test valid query with Affects* SuchThatClause, args are assign syns") {
  std::string program =
      "assign a1, a2; Select a1 such that Affects*(a1, a2)";
  const char *expectedString = R""""(Parsed: [
Query(
  Output: a1
  QuerySynonym(Assign, a1)
  QuerySynonym(Assign, a2)
  SuchThat Affects*(a1, a2)
)
])"""";
  auto queryParser = QueryParser(program);
  queryParser.parse();
  REQUIRE(queryParser.parsedQueryToString() == expectedString);
}

TEST_CASE("Test valid query with Affects* SuchThatClause, args are integers") {
  std::string program =
      "assign a1, a2; Select a1 such that Affects*(1, 2)";
  const char *expectedString = R""""(Parsed: [
Query(
  Output: a1
  QuerySynonym(Assign, a1)
  QuerySynonym(Assign, a2)
  -----
  SuchThat Affects*(1, 2)
)
])"""";
  auto queryParser = QueryParser(program);
  queryParser.parse();
  REQUIRE(queryParser.parsedQueryToString() == expectedString);
}

TEST_CASE("Test valid query with Affects* SuchThatClause, args are wildcards") {
  std::string program =
      "assign a1, a2; Select a1 such that Affects*(_, _)";
  const char *expectedString = R""""(Parsed: [
Query(
  Output: a1
  QuerySynonym(Assign, a1)
  QuerySynonym(Assign, a2)
  -----
  SuchThat Affects*(_, _)
)
])"""";
  auto queryParser = QueryParser(program);
  queryParser.parse();
  REQUIRE(queryParser.parsedQueryToString() == expectedString);
}

TEST_CASE("Test valid query with Affects* SuchThatClause, args are mix of integer and assign syn") {
  std::string program =
      "assign a1, a2; Select a1 such that Affects*(3, a1)";
  const char *expectedString = R""""(Parsed: [
Query(
  Output: a1
  QuerySynonym(Assign, a1)
  QuerySynonym(Assign, a2)
  SuchThat Affects*(3, a1)
)
])"""";
  auto queryParser = QueryParser(program);
  queryParser.parse();
  REQUIRE(queryParser.parsedQueryToString() == expectedString);
}

TEST_CASE("Test invalid query with Affects* SuchThatClause, arg is non-assign syn") {
  std::string program = "If i; Select i such that Affects*(i, 2)";
  auto queryParser = QueryParser(program);
  queryParser.parse();
  REQUIRE(queryParser.parsedQueryToString() == invalidQueryOutputString);
}

TEST_CASE("Test invalid query with Affects* SuchThatClause, arg is integer but string") {
  std::string program = "If i; Select i such that Affects*(2, \"9\")";
  auto queryParser = QueryParser(program);
  queryParser.parse();
  REQUIRE(queryParser.parsedQueryToString() == invalidQueryOutputString);
}

TEST_CASE("Test valid query with NextBip") {
  std::string program =
      "prog_line n1, n2; Select n1 such that NextBip(n1, 21)";
  const char *expectedString = R""""(Parsed: [
Query(
  Output: n1
  QuerySynonym(ProgLine, n1)
  QuerySynonym(ProgLine, n2)
  SuchThat NextBip(n1, 21)
)
])"""";
  auto queryParser = QueryParser(program);
  queryParser.parse();
  REQUIRE(queryParser.parsedQueryToString() == expectedString);
}

TEST_CASE("Test valid query with NextBip*") {
  std::string program =
      "prog_line n1, n2; Select n1 such that NextBip*(n1, 9)";
  const char *expectedString = R""""(Parsed: [
Query(
  Output: n1
  QuerySynonym(ProgLine, n1)
  QuerySynonym(ProgLine, n2)
  SuchThat NextBip*(n1, 9)
)
])"""";
  auto queryParser = QueryParser(program);
  queryParser.parse();
  REQUIRE(queryParser.parsedQueryToString() == expectedString);
}

TEST_CASE("Test valid query with AffectsBip") {
  std::string program =
      "assign a1; Select a1 such that AffectsBip(a1, 2)";
  const char *expectedString = R""""(Parsed: [
Query(
  Output: a1
  QuerySynonym(Assign, a1)
  SuchThat AffectsBip(a1, 2)
)
])"""";
  auto queryParser = QueryParser(program);
  queryParser.parse();
  REQUIRE(queryParser.parsedQueryToString() == expectedString);
}

TEST_CASE("Test valid query with ModifiesS and call synonym") {
  std::string program =
      "call c; Select c such that Modifies(c,\"a\")";
  const char *expectedString = R""""(Parsed: [
Query(
  Output: c
  QuerySynonym(Call, c)
  SuchThat ModifiesS(c, "a")
)
])"""";
  auto queryParser = QueryParser(program);
  queryParser.parse();
  REQUIRE(queryParser.parsedQueryToString() == expectedString);
}

TEST_CASE("Test invalid with clause with stmtNo attr for progline syn") {
  std::string program =
      "prog_line n; Select n with n.stmt# = 1";
  auto queryParser = QueryParser(program);
  queryParser.parse();
  REQUIRE(queryParser.parsedQueryToString() == invalidQueryOutputString);
}

TEST_CASE("Test query with BOOLEAN synonym evaluates as synonym output") {
  std::string program =
      "assign BOOLEAN; Select BOOLEAN";
  const char *expectedString = R""""(Parsed: [
Query(
  Output: BOOLEAN
  QuerySynonym(Assign, BOOLEAN)
)
])"""";
  auto queryParser = QueryParser(program);
  queryParser.parse();
  REQUIRE(queryParser.parsedQueryToString() == expectedString);
}

TEST_CASE("Test query with general stmt types as args for Affects SuchThatClause") {
  std::string program =
      "stmt s; prog_line n; Select s such that Affects(s, n)";
  const char *expectedString = R""""(Parsed: [
Query(
  Output: s
  QuerySynonym(Stmt, s)
  QuerySynonym(ProgLine, n)
  SuchThat Affects(s, n)
)
])"""";
  auto queryParser = QueryParser(program);
  queryParser.parse();
  REQUIRE(queryParser.parsedQueryToString() == expectedString);
}
