//
// Created by Jazer on 30/9/2021.
//

#include "catch.hpp"
#include "Row.h"

TEST_CASE("Check equality method") {
  Row a = Row();
  Row b = Row();
  QuerySynonym x = QuerySynonym(DesignEntityType::Stmt, "x");
  QuerySynonym y = QuerySynonym(DesignEntityType::Stmt, "y");
  QuerySynonym z = QuerySynonym(DesignEntityType::Stmt, "z");

  a.addSynonym(x, "1");
  a.addSynonym(y, "1");
  a.addSynonym(z, "1");

  b.addSynonym(x, "1");
  b.addSynonym(y, "1");
  b.addSynonym(z, "1");

  REQUIRE(a == b);
  REQUIRE(a.getSize() == b.getSize());
}

TEST_CASE("Check equality method differentiates different Rows") {
  Row a = Row();
  Row b = Row();
  QuerySynonym x = QuerySynonym(DesignEntityType::Stmt, "x");
  QuerySynonym y = QuerySynonym(DesignEntityType::Stmt, "y");
  QuerySynonym z = QuerySynonym(DesignEntityType::Stmt, "z");

  a.addSynonym(x, "1");
  a.addSynonym(y, "1");
  a.addSynonym(z, "1");

  b.addSynonym(x, "2");
  b.addSynonym(y, "1");
  b.addSynonym(z, "1");

  REQUIRE_FALSE(a == b);
}

TEST_CASE("Consistency check should pass for consistent Rows") {
  Row a = Row();
  Row b = Row();
  QuerySynonym x = QuerySynonym(DesignEntityType::Stmt, "x");
  QuerySynonym y = QuerySynonym(DesignEntityType::Stmt, "y");
  QuerySynonym z = QuerySynonym(DesignEntityType::Stmt, "z");

  a.addSynonym(x, "1");
  a.addSynonym(y, "2");

  b.addSynonym(y, "2");
  b.addSynonym(z, "3");

  REQUIRE(a.isConsistentWith(b));
}

TEST_CASE("Consistency check should fail for inconsistent Rows") {
  Row a = Row();
  Row b = Row();
  QuerySynonym x = QuerySynonym(DesignEntityType::Stmt, "x");
  QuerySynonym y = QuerySynonym(DesignEntityType::Stmt, "y");
  QuerySynonym z = QuerySynonym(DesignEntityType::Stmt, "z");

  a.addSynonym(x, "1");
  a.addSynonym(y, "1");

  b.addSynonym(y, "2");
  b.addSynonym(z, "1");

  REQUIRE_FALSE(a.isConsistentWith(b));
}

TEST_CASE("Merge with consistent Rows") {
  Row a = Row();
  Row b = Row();
  QuerySynonym x = QuerySynonym(DesignEntityType::Stmt, "x");
  QuerySynonym y = QuerySynonym(DesignEntityType::Stmt, "y");
  QuerySynonym z = QuerySynonym(DesignEntityType::Stmt, "z");

  a.addSynonym(x, "1");
  a.addSynonym(y, "2");

  b.addSynonym(y, "2");
  b.addSynonym(z, "3");

  Row expected = Row();
  expected.addSynonym(x, "1");
  expected.addSynonym(y, "2");
  expected.addSynonym(z, "3");

  a.merge(b);
  REQUIRE(expected.getSize() == a.getSize());
  REQUIRE(expected == a);
}

TEST_CASE("Merge with inconsistent Rows should throw exception") {
  Row a = Row();
  Row b = Row();
  QuerySynonym x = QuerySynonym(DesignEntityType::Stmt, "x");
  QuerySynonym y = QuerySynonym(DesignEntityType::Stmt, "y");
  QuerySynonym z = QuerySynonym(DesignEntityType::Stmt, "z");

  a.addSynonym(x, "1");
  a.addSynonym(y, "1");

  b.addSynonym(y, "2");
  b.addSynonym(z, "1");

  REQUIRE_THROWS(a.merge(b));
}

TEST_CASE("Check Row addSynonym is working") {
  Row a = Row();
  QuerySynonym s = QuerySynonym(DesignEntityType::Stmt, "x");
  a.addSynonym(s, "1");
  RowMap map = a.getMap();
  REQUIRE(map.size() == 1);
  REQUIRE(map.at(s) == "1");
}

TEST_CASE("Check Row size is working") {
  Row a = Row();
  REQUIRE(a.getSize() == 0);
  QuerySynonym s = QuerySynonym(DesignEntityType::Stmt, "x");
  a.addSynonym(s, "1");
  REQUIRE(a.getSize() == 1);
}

TEST_CASE("Check Row getMap is working") {
  Row a = Row();
  REQUIRE(a.getMap().empty());
  QuerySynonym s = QuerySynonym(DesignEntityType::Stmt, "x");
  a.addSynonym(s, "1");
  RowMap expected = RowMap();
  expected[s] = "1";
  REQUIRE(a.getMap() == expected);
}