//
// Created by Jazer on 30/9/2021.
//

#include "catch.hpp"
#include "ClauseResult.h"

TEST_CASE("Test counting overlapping synonyms") {
  auto a = new ClauseResult();
  auto b = new ClauseResult();

  QuerySynonym w = QuerySynonym(DesignEntityType::Stmt, "w");
  QuerySynonym x = QuerySynonym(DesignEntityType::Stmt, "x");
  QuerySynonym y = QuerySynonym(DesignEntityType::Stmt, "y");
  QuerySynonym z = QuerySynonym(DesignEntityType::Stmt, "z");

  Row r1 = Row();
  Row r2 = Row();

  r1.addSynonym(w, "1");
  r1.addSynonym(x, "1");
  r1.addSynonym(y, "1");

  r2.addSynonym(y, "1");
  r2.addSynonym(z, "1");

  a->addRow(r1);
  b->addRow(r2);

  REQUIRE(a->numSharedSynonyms(b) == 1);
  REQUIRE_FALSE(a->getIdent());
  REQUIRE_FALSE(b->getIdent());
}

TEST_CASE("Test no overlapping synonym count") {
  auto a = new ClauseResult();
  auto b = new ClauseResult();
  REQUIRE(a->numSharedSynonyms(b) == 0);

  QuerySynonym w = QuerySynonym(DesignEntityType::Stmt, "a");
  Row r1 = Row();
  r1.addSynonym(w, "1");

  QuerySynonym x = QuerySynonym(DesignEntityType::Stmt, "a");
  Row r2 = Row();
  r1.addSynonym(x, "1");

  REQUIRE(a->numSharedSynonyms(b) == 0);
  REQUIRE_FALSE(a->getIdent());
  REQUIRE_FALSE(b->getIdent());
}

TEST_CASE("Test size") {
  auto a = new ClauseResult();
  REQUIRE(a->size() == 0);
  REQUIRE(a->empty());

  QuerySynonym w = QuerySynonym(DesignEntityType::Stmt, "w");
  Row r1 = Row();
  r1.addSynonym(w, "1");

  a->addRow(r1);
  REQUIRE(a->size() == 1);
  REQUIRE_FALSE(a->empty());
  REQUIRE_FALSE(a->getIdent());
}

TEST_CASE("Test add duplicate rows") {
  auto a = new ClauseResult();
  QuerySynonym w = QuerySynonym(DesignEntityType::Stmt, "w");
  Row r1 = Row();
  r1.addSynonym(w, "1");

  a->addRow(r1);
  REQUIRE(a->size() == 1);
  REQUIRE_FALSE(a->empty());
  REQUIRE_FALSE(a->getIdent());

  a->addRow(r1);
  REQUIRE(a->size() == 1);
  REQUIRE_FALSE(a->empty());
  REQUIRE_FALSE(a->getIdent());
}

TEST_CASE("Test merging consistent overlapping clauses") {
  auto a = new ClauseResult();
  auto b = new ClauseResult();

  QuerySynonym w = QuerySynonym(DesignEntityType::Stmt, "w");
  QuerySynonym x = QuerySynonym(DesignEntityType::Stmt, "x");
  QuerySynonym y = QuerySynonym(DesignEntityType::Stmt, "y");
  QuerySynonym z = QuerySynonym(DesignEntityType::Stmt, "z");

  Row r1 = Row();
  Row r2 = Row();
  Row r3 = Row();

  r1.addSynonym(w, "1");
  r1.addSynonym(x, "2");
  r1.addSynonym(y, "3");

  r2.addSynonym(y, "3");
  r2.addSynonym(z, "4");

  r3.addSynonym(y, "3");
  r3.addSynonym(z, "5");

  a->addRow(r1);
  b->addRow(r2);
  b->addRow(r3);

  Row r12 = Row();
  r12.addSynonym(w, "1");
  r12.addSynonym(x, "2");
  r12.addSynonym(y, "3");
  r12.addSynonym(z, "4");

  Row r13 = Row();
  r13.addSynonym(w, "1");
  r13.addSynonym(x, "2");
  r13.addSynonym(y, "3");
  r13.addSynonym(z, "4");

  a->mergeClause(b);
  RowList result = a->getRows();

  REQUIRE_FALSE(a->getIdent());
  REQUIRE_FALSE(b->getIdent());
  REQUIRE(std::find(result.begin(), result.end(), r12) != result.end());
  REQUIRE(std::find(result.begin(), result.end(), r13) != result.end());
  REQUIRE(a->size() == 2);
}

TEST_CASE("Test merging inconsistent overlapping clauses") {
  auto a = new ClauseResult();
  auto b = new ClauseResult();

  QuerySynonym w = QuerySynonym(DesignEntityType::Stmt, "w");
  QuerySynonym x = QuerySynonym(DesignEntityType::Stmt, "x");
  QuerySynonym y = QuerySynonym(DesignEntityType::Stmt, "y");
  QuerySynonym z = QuerySynonym(DesignEntityType::Stmt, "z");

  Row r1 = Row();
  Row r2 = Row();
  Row r3 = Row();

  r1.addSynonym(w, "1");
  r1.addSynonym(x, "1");
  r1.addSynonym(y, "1");

  r2.addSynonym(y, "2");
  r2.addSynonym(z, "1");

  r3.addSynonym(y, "2");
  r3.addSynonym(z, "2");

  a->addRow(r1);
  b->addRow(r2);
  b->addRow(r3);

  a->mergeClause(b);
  RowList result = a->getRows();
  REQUIRE_FALSE(a->getIdent());
  REQUIRE_FALSE(b->getIdent());
  REQUIRE(a->size() == 0);
}

TEST_CASE("Test merging non-overlapping clauses") {
  auto a = new ClauseResult();
  auto b = new ClauseResult();

  QuerySynonym w = QuerySynonym(DesignEntityType::Stmt, "w");
  QuerySynonym x = QuerySynonym(DesignEntityType::Stmt, "x");
  QuerySynonym y = QuerySynonym(DesignEntityType::Stmt, "y");
  QuerySynonym z = QuerySynonym(DesignEntityType::Stmt, "z");

  Row r1 = Row();
  Row r2 = Row();
  Row r3 = Row();

  r1.addSynonym(w, "1");
  r1.addSynonym(x, "2");
  r1.addSynonym(y, "3");

  r2.addSynonym(z, "4");

  r3.addSynonym(z, "5");

  a->addRow(r1);
  b->addRow(r2);
  b->addRow(r3);

  Row r12 = Row();
  r12.addSynonym(w, "1");
  r12.addSynonym(x, "2");
  r12.addSynonym(y, "3");
  r12.addSynonym(z, "4");

  Row r13 = Row();
  r13.addSynonym(w, "1");
  r13.addSynonym(x, "2");
  r13.addSynonym(y, "3");
  r13.addSynonym(z, "5");

  a->mergeClause(b);
  RowList result = a->getRows();

  REQUIRE_FALSE(a->getIdent());
  REQUIRE_FALSE(b->getIdent());
  REQUIRE(std::find(result.begin(), result.end(), r12) != result.end());
  REQUIRE(std::find(result.begin(), result.end(), r13) != result.end());
  REQUIRE(a->size() == 2);

}

TEST_CASE("Test merging with empty clause") {
  auto a = new ClauseResult();
  auto b = new ClauseResult();

  QuerySynonym x = QuerySynonym(DesignEntityType::Stmt, "x");
  QuerySynonym y = QuerySynonym(DesignEntityType::Stmt, "y");
  QuerySynonym z = QuerySynonym(DesignEntityType::Stmt, "z");

  Row r1 = Row();

  r1.addSynonym(x, "1");
  r1.addSynonym(y, "1");

  a->addRow(r1);

  a->mergeClause(b);
  RowList result = a->getRows();
  REQUIRE_FALSE(a->getIdent());
  REQUIRE_FALSE(b->getIdent());
  REQUIRE(a->size() == 0);
}

TEST_CASE("Test identity merge identity is merged in") {
  auto a = new ClauseResult(true);
  auto b = new ClauseResult();

  QuerySynonym x = QuerySynonym(DesignEntityType::Stmt, "x");
  QuerySynonym y = QuerySynonym(DesignEntityType::Stmt, "y");
  QuerySynonym z = QuerySynonym(DesignEntityType::Stmt, "z");

  Row r1 = Row();

  r1.addSynonym(x, "1");
  r1.addSynonym(y, "1");

  b->addRow(r1);

  b->mergeClause(a);
  RowList result = b->getRows();
  REQUIRE_FALSE(b->getIdent());
  REQUIRE(result.size() == 1);
}

TEST_CASE("Test identity merge identity is merging") {
  auto a = new ClauseResult(true);
  auto b = new ClauseResult();

  QuerySynonym x = QuerySynonym(DesignEntityType::Stmt, "x");
  QuerySynonym y = QuerySynonym(DesignEntityType::Stmt, "y");
  QuerySynonym z = QuerySynonym(DesignEntityType::Stmt, "z");

  Row r1 = Row();

  r1.addSynonym(x, "1");
  r1.addSynonym(y, "1");

  b->addRow(r1);

  a->mergeClause(b);
  RowList result = a->getRows();
  REQUIRE_FALSE(a->getIdent());
  REQUIRE(result.size() == 1);
}

TEST_CASE("Test identity num share syns") {
  auto a = new ClauseResult(true);
  auto b = new ClauseResult();

  QuerySynonym x = QuerySynonym(DesignEntityType::Stmt, "x");
  QuerySynonym y = QuerySynonym(DesignEntityType::Stmt, "y");
  QuerySynonym z = QuerySynonym(DesignEntityType::Stmt, "z");

  Row r1 = Row();

  r1.addSynonym(x, "1");
  r1.addSynonym(y, "1");

  b->addRow(r1);
  REQUIRE(a->getIdent());
  REQUIRE_FALSE(b->getIdent());
  REQUIRE(b->numSharedSynonyms(a) == b->getSynonyms().size());
  REQUIRE(a->numSharedSynonyms(b) == b->getSynonyms().size());
}

TEST_CASE("Test both ident syns") {
  auto a = new ClauseResult(true);
  auto b = new ClauseResult(true);

  a->mergeClause(b);
  REQUIRE(a->getIdent());
}