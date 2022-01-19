//
// Created by Junhua on 11/9/2021.
//

#include "Query.h"
#include "catch.hpp"

TEST_CASE("Empty Query toString no Error") {
  Query q = Query();
  REQUIRE_NOTHROW(q.toString());
}

TEST_CASE("findSynonymByName Throws exception on not found") {
  Query q = Query();
  REQUIRE_THROWS(q.findSynonymByName("Not found"), Catch::Matchers::Contains("Cannot find QuerySynonym"));
}

TEST_CASE("getSynonymList returns empty vector") {
  Query q = Query();
  REQUIRE(q.getSynonymList().empty());
}

TEST_CASE("checkIfSynonymExists works for valid synonym name") {
  Query q = Query();
  q.insertSynonym(QuerySynonym(DesignEntityType::Assign, "a1"));
  q.insertSynonym(QuerySynonym(DesignEntityType::Assign, "a2"));
  q.insertSynonym(QuerySynonym(DesignEntityType::Stmt, "s1"));

  REQUIRE(q.checkIfSynonymExists("a1"));
}

TEST_CASE("checkIfSynonymExists works for invalid synonym name") {
  Query q = Query();
  q.insertSynonym(QuerySynonym(DesignEntityType::Assign, "a1"));
  q.insertSynonym(QuerySynonym(DesignEntityType::Assign, "a2"));
  q.insertSynonym(QuerySynonym(DesignEntityType::Stmt, "s1"));

  REQUIRE(!q.checkIfSynonymExists("a3"));
}

TEST_CASE("findSynonymByName works for valid synonym name") {
  Query q = Query();
  QuerySynonym querySynonym = QuerySynonym(DesignEntityType::Assign, "a1");
  q.insertSynonym(querySynonym);
  q.insertSynonym(QuerySynonym(DesignEntityType::Assign, "a2"));
  q.insertSynonym(QuerySynonym(DesignEntityType::Stmt, "s1"));

  REQUIRE(*q.findSynonymByName("a1") == querySynonym);
}

TEST_CASE("findSynonymByName works for invalid synonym name") {
  Query q = Query();
  QuerySynonym querySynonym = QuerySynonym(DesignEntityType::Assign, "a1");
  q.insertSynonym(querySynonym);
  q.insertSynonym(QuerySynonym(DesignEntityType::Assign, "a2"));
  q.insertSynonym(QuerySynonym(DesignEntityType::Stmt, "s1"));

  REQUIRE_THROWS(*q.findSynonymByName("a3"));
}
