//
// Created by vaish on 6/11/2021.
//

#include "catch.hpp"
#include "IrrelevantClauseProcessor.h"

TEST_CASE("Test irrelevant SuchThatClause") {
  const char *expectedString = R""""(Query(
  Output: a1
  QuerySynonym(Stmt, s)
  QuerySynonym(Assign, a1)
  QuerySynonym(Stmt, s1)
  -----
  SuchThat Follows(_, _)
))"""";

  auto s = new QuerySynonym(DesignEntityType::Stmt, "s");
  auto a1 = new QuerySynonym(DesignEntityType::Assign, "a1");
  auto s1 = new QuerySynonym(DesignEntityType::Stmt, "s1");
  auto query = new Query();
  query->insertSynonym(*s);
  query->insertSynonym(*a1);
  query->insertSynonym(*s1);
  query->insertOutputSynonym(a1);
  query->insertClause(new SuchThatClause(ClauseType::Follows, "s", "s1"));
  auto processor = new IrrelevantClauseProcessor(query);
  processor->processIrrelevantClauses();
  REQUIRE(query->toString() == expectedString);
}

TEST_CASE("Test irrelevant SuchThatClause but same argument") {
  const char *expectedString = R""""(Query(
  Output: a1
  QuerySynonym(Assign, a)
  QuerySynonym(Assign, a1)
  -----
  SuchThat Follows(a, a)
))"""";
  auto a = new QuerySynonym(DesignEntityType::Assign, "a");
  auto a1 = new QuerySynonym(DesignEntityType::Assign, "a1");
  auto query = new Query();
  query->insertSynonym(*a);
  query->insertSynonym(*a1);
  query->insertOutputSynonym(a1);
  query->insertClause(new SuchThatClause(ClauseType::Follows, "a", "a"));
  auto processor = new IrrelevantClauseProcessor(query);
  processor->processIrrelevantClauses();
  REQUIRE(query->toString() == expectedString);
}

TEST_CASE("Test irrelevant WithClause") {
  const char *expectedString = R""""(Query(
  Output: a1
  QuerySynonym(Assign, a)
  QuerySynonym(Assign, a1)
  QuerySynonym(Assign, a2)
  -----
  With(a.stmt#, a2.stmt#)
))"""";
  auto a = new QuerySynonym(DesignEntityType::Assign, "a");
  auto a1 = new QuerySynonym(DesignEntityType::Assign, "a1");
  auto a2 = new QuerySynonym(DesignEntityType::Assign, "a2");
  auto query = new Query();
  query->insertSynonym(*a);
  query->insertSynonym(*a1);
  query->insertSynonym(*a2);
  query->insertOutputSynonym(a1);
  query->insertClause(new WithClause(new WithArgument(WithType::Attribute,
                                                      "a",
                                                      AttributeType::StmtNo),
                                     new WithArgument(WithType::Attribute,
                                                      "a2",
                                                      AttributeType::StmtNo)));
  auto processor = new IrrelevantClauseProcessor(query);
  processor->processIrrelevantClauses();
  REQUIRE(query->toString() == expectedString);
}

TEST_CASE("Test irrelevant assign PatternClause") {
  const char *expectedString = R""""(Query(
  Output: a1
  QuerySynonym(Assign, a)
  QuerySynonym(Assign, a1)
  QuerySynonym(Variable, v)
  -----
  Pattern a(v, "2")
))"""";
  auto a = new QuerySynonym(DesignEntityType::Assign, "a");
  auto a1 = new QuerySynonym(DesignEntityType::Assign, "a1");
  auto v = new QuerySynonym(DesignEntityType::Variable, "v");
  auto query = new Query();
  query->insertSynonym(*a);
  query->insertSynonym(*a1);
  query->insertSynonym(*v);
  query->insertOutputSynonym(a1);
  query->insertClause(new PatternClause(PatternSynonymType::Assign, "v", "\"2\"", "a"));
  auto processor = new IrrelevantClauseProcessor(query);
  processor->processIrrelevantClauses();
  REQUIRE(query->toString() == expectedString);
}

TEST_CASE("Test irrelevant Modifies SuchThatClause") {
  const char *expectedString = R""""(Query(
  Output: a
  QuerySynonym(Assign, a)
  QuerySynonym(Stmt, s)
  QuerySynonym(Variable, v)
  -----
  SuchThat ModifiesS(s, v)
))"""";
  auto a = new QuerySynonym(DesignEntityType::Assign, "a");
  auto v = new QuerySynonym(DesignEntityType::Variable, "v");
  auto s = new QuerySynonym(DesignEntityType::Stmt, "s");
  auto query = new Query();
  query->insertSynonym(*a);
  query->insertSynonym(*s);
  query->insertSynonym(*v);
  query->insertOutputSynonym(a);
  query->insertClause(new SuchThatClause(ClauseType::ModifiesS, "s", "v"));
  auto processor = new IrrelevantClauseProcessor(query);
  processor->processIrrelevantClauses();
  REQUIRE(query->toString() == expectedString);
}

TEST_CASE("Test complicated query with irrelevant clause") {
  const char *expectedString = R""""(Query(
  Output: a1
  QuerySynonym(Assign, a)
  QuerySynonym(Assign, a1)
  QuerySynonym(Assign, a2)
  QuerySynonym(Assign, a3)
  QuerySynonym(Assign, a4)
  QuerySynonym(Assign, a5)
  QuerySynonym(Variable, v)
  SuchThat Follows*(a, a2)
  SuchThat Affects(a3, a4)
  With(a2.stmt#, a4.stmt#)
  -----
  Pattern a5(v, _)
))"""";
  auto a = new QuerySynonym(DesignEntityType::Assign, "a");
  auto a1 = new QuerySynonym(DesignEntityType::Assign, "a1");
  auto a2 = new QuerySynonym(DesignEntityType::Assign, "a2");
  auto a3 = new QuerySynonym(DesignEntityType::Assign, "a3");
  auto a4 = new QuerySynonym(DesignEntityType::Assign, "a4");
  auto a5 = new QuerySynonym(DesignEntityType::Assign, "a5");
  auto v = new QuerySynonym(DesignEntityType::Variable, "v");
  auto query = new Query();
  query->insertSynonym(*a);
  query->insertSynonym(*a1);
  query->insertSynonym(*a2);
  query->insertSynonym(*a3);
  query->insertSynonym(*a4);
  query->insertSynonym(*a5);
  query->insertSynonym(*v);
  query->insertOutputSynonym(a1);
  query->insertClause(new SuchThatClause(ClauseType::FollowsT, "a", "a2"));
  query->insertClause(new SuchThatClause(ClauseType::Affects, "a3", "a4"));
  query->insertClause(new WithClause(new WithArgument(WithType::Attribute,
                                                      "a2",
                                                      AttributeType::StmtNo),
                                     new WithArgument(WithType::Attribute,
                                                      "a4",
                                                      AttributeType::StmtNo)));
  query->insertClause(new PatternClause(PatternSynonymType::Assign, "v", "_", "a5"));
  auto processor = new IrrelevantClauseProcessor(query);
  processor->processIrrelevantClauses();
  REQUIRE(query->toString() == expectedString);
}
