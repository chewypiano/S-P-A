//
// Created by Christopher Mervyn on 4/10/21.
//

#include "catch.hpp"
#include "PKB/PKB.h"

TEST_CASE("add next relation") {
  PKB storage1;
  storage1.addStatement(1, read, "ASample");
  storage1.addStatement(2, print, "ASample");
  storage1.addStatement(3, assign, "ASample");
  storage1.addStatement(4, call, "toyota");
  storage1.addNext(1, 2);
  storage1.addNext(2, 3);
  storage1.addNext(3, 4);
  REQUIRE(storage1.getAllNextPairs() == std::unordered_set<std::pair<int, int>, std::hash_pair>
      {std::pair<int, int>(1, 2), std::pair<int, int>(2, 3), std::pair<int, int>(3, 4)});
}

TEST_CASE("get next relation") {
  PKB storage1;
  storage1.addStatement(1, read, "ASample");
  storage1.addStatement(2, print, "ASample");
  storage1.addStatement(3, assign, "ASample");
  storage1.addStatement(4, call, "toyota");
  storage1.addNext(1, 2);
  storage1.addNext(2, 3);
  storage1.addNext(2, 4);
  storage1.addNext(3, 4);
  REQUIRE(storage1.getNextStatementsOf(2) == std::unordered_set<int>{3, 4});
}

TEST_CASE("negative get next relation") {
  PKB storage1;
  storage1.addStatement(1, read, "ASample");
  storage1.addStatement(2, print, "ASample");
  storage1.addStatement(3, assign, "ASample");
  storage1.addStatement(4, call, "toyota");
  storage1.addNext(1, 2);
  storage1.addNext(2, 3);
  storage1.addNext(3, 4);
  REQUIRE(storage1.getNextStatementsOf(4).empty());
}

TEST_CASE("get prev relation") {
  PKB storage1;
  storage1.addStatement(1, read, "ASample");
  storage1.addStatement(2, print, "ASample");
  storage1.addStatement(3, assign, "ASample");
  storage1.addStatement(4, call, "toyota");
  storage1.addNext(1, 2);
  storage1.addNext(2, 3);
  storage1.addNext(2, 4);
  storage1.addNext(3, 4);
  REQUIRE(storage1.getPrevStatementsOf(4) == std::unordered_set<int>{2, 3});
}

TEST_CASE("negative get prev relation") {
  PKB storage1;
  storage1.addStatement(1, read, "ASample");
  storage1.addStatement(2, print, "ASample");
  storage1.addStatement(3, assign, "ASample");
  storage1.addStatement(4, call, "toyota");
  storage1.addNext(1, 2);
  storage1.addNext(2, 3);
  storage1.addNext(2, 4);
  storage1.addNext(3, 4);
  REQUIRE(storage1.getPrevStatementsOf(1).empty());
}

TEST_CASE("get next list for proc") {
  PKB storage1;
  storage1.addStatement(1, read, "ASample");
  storage1.addStatement(2, print, "ASample");
  storage1.addStatement(3, assign, "ASample");
  storage1.addStatement(4, call, "ASample");
  storage1.addStatement(5, assign, "toyota");
  storage1.addNext(1, 2);
  storage1.addNext(2, 3);
  storage1.addNext(2, 4);

  std::unordered_map<int, std::unordered_set<int>> stubNextProc = {};
  stubNextProc.emplace(1, std::unordered_set<int>{2});
  stubNextProc.emplace(2, std::unordered_set<int>{3, 4});
  REQUIRE(storage1.getNextListForProc("ASample") == stubNextProc);
}

TEST_CASE("negative get next list for proc") {
  PKB storage1;
  storage1.addStatement(1, read, "ASample");
  storage1.addStatement(2, print, "ASample");
  storage1.addStatement(3, assign, "ASample");
  storage1.addStatement(4, call, "ASample");
  storage1.addStatement(5, assign, "toyota");
  storage1.addNext(1, 2);
  storage1.addNext(2, 3);
  storage1.addNext(2, 4);

  REQUIRE(storage1.getNextListForProc("toyota").empty());
}

TEST_CASE("get combined next list") {
  PKB storage1;
  storage1.addStatement(1, read, "ASample");
  storage1.addStatement(2, print, "ASample");
  storage1.addStatement(3, assign, "ASample");
  storage1.addStatement(4, call, "ASample");
  storage1.addStatement(5, assign, "toyota");
  storage1.addStatement(6, call, "toyota");
  storage1.addStatement(7, call, "porsche");
  storage1.addStatement(8, ifStmt, "porsche");
  storage1.addStatement(9, assign, "porsche");

  storage1.addProc("ASample");
  storage1.addProc("toyota");
  storage1.addProc("porsche");

  storage1.addNext(1, 2);
  storage1.addNext(2, 3);
  storage1.addNext(2, 4);

  storage1.addNext(5, 6);

  storage1.addNext(7, 8);
  storage1.addNext(8, 9);

  std::unordered_map<int, std::unordered_set<int>> stubNextProc = {};
  stubNextProc.emplace(1, std::unordered_set<int>{2});
  stubNextProc.emplace(2, std::unordered_set<int>{3, 4});
  stubNextProc.emplace(5, std::unordered_set<int>{6});
  stubNextProc.emplace(7, std::unordered_set<int>{8});
  stubNextProc.emplace(8, std::unordered_set<int>{9});

  REQUIRE(storage1.getCombinedNextList() == stubNextProc);
}

TEST_CASE("get cfg for proc") {
  PKB storage1;
  storage1.addStatement(1, read, "ASample");
  storage1.addStatement(2, print, "ASample");
  storage1.addStatement(3, assign, "ASample");
  storage1.addStatement(4, call, "ASample");
  storage1.addStatement(5, assign, "toyota");
  storage1.addNext(1, 2);
  storage1.addNext(2, 3);
  storage1.addNext(2, 4);

  std::unordered_map<int, std::unordered_set<int>> stubNextProc = {};
  stubNextProc.emplace(1, std::unordered_set<int>{2});
  stubNextProc.emplace(2, std::unordered_set<int>{3, 4});

  CFG *cfg = new CFG(stubNextProc);
  storage1.addCFG("ASample", cfg);

  REQUIRE(storage1.getCFG("ASample")->getNextList() == cfg->getNextList());
}

TEST_CASE("negative get cfg for proc") {
  PKB storage1;
  storage1.addStatement(1, read, "ASample");
  storage1.addStatement(2, print, "ASample");
  storage1.addStatement(3, assign, "ASample");
  storage1.addStatement(4, call, "ASample");
  storage1.addStatement(5, assign, "toyota");
  storage1.addNext(1, 2);
  storage1.addNext(2, 3);
  storage1.addNext(2, 4);

  std::unordered_map<int, std::unordered_set<int>> stubNextProc = {};
  stubNextProc.emplace(1, std::unordered_set<int>{2});
  stubNextProc.emplace(2, std::unordered_set<int>{3, 4});

  CFG *cfg = new CFG(stubNextProc);
  CFG *cfgEmpty = new CFG({});
  storage1.addCFG("ASample", cfg);

  REQUIRE(storage1.getCFG("mitsubishi") == nullptr);
}

TEST_CASE("next* test") {
  PKB storage1;
  storage1.addStatement(1, read, "ASample");
  storage1.addStatement(2, print, "ASample");
  storage1.addStatement(3, assign, "ASample");
  storage1.addStatement(4, call, "ASample");
  storage1.addStatement(5, assign, "toyota");
  storage1.addNext(1, 2);
  storage1.addNext(2, 3);
  storage1.addNext(2, 4);

  std::unordered_map<int, std::unordered_set<int>> stubNextProc = {};
  stubNextProc.emplace(1, std::unordered_set<int>{2});
  stubNextProc.emplace(2, std::unordered_set<int>{3, 4});

  CFG *cfg = new CFG(stubNextProc);
  storage1.addCFG("ASample", cfg);

  REQUIRE(storage1.getCFG("ASample")->isNextStar(2, 4) == true);
}

TEST_CASE("negative next* test") {
  PKB storage1;
  storage1.addStatement(1, read, "ASample");
  storage1.addStatement(2, print, "ASample");
  storage1.addStatement(3, assign, "ASample");
  storage1.addStatement(4, call, "ASample");
  storage1.addStatement(5, assign, "toyota");
  storage1.addNext(1, 2);
  storage1.addNext(2, 3);
  storage1.addNext(2, 4);

  std::unordered_map<int, std::unordered_set<int>> stubNextProc = {};
  stubNextProc.emplace(1, std::unordered_set<int>{2});
  stubNextProc.emplace(2, std::unordered_set<int>{3, 4});

  CFG *cfg = new CFG(stubNextProc);
  storage1.addCFG("ASample", cfg);

  REQUIRE(storage1.getCFG("ASample")->isNextStar(1, 5) == false);
}

TEST_CASE("next* test caching") {
  PKB storage1;
  storage1.addStatement(1, read, "ASample");
  storage1.addStatement(2, print, "ASample");
  storage1.addStatement(3, assign, "ASample");
  storage1.addStatement(4, call, "ASample");
  storage1.addStatement(5, assign, "toyota");
  storage1.addNext(1, 2);
  storage1.addNext(2, 3);
  storage1.addNext(2, 4);

  std::unordered_map<int, std::unordered_set<int>> stubNextProc = {};
  stubNextProc.emplace(1, std::unordered_set<int>{2});
  stubNextProc.emplace(2, std::unordered_set<int>{3, 4});

  CFG *cfg = new CFG(stubNextProc);
  storage1.addCFG("ASample", cfg);
  storage1.getCFG("ASample")->isNextStar(1, 3);

  REQUIRE(storage1.getCFG("ASample")->hasBeenCached == true);
}

TEST_CASE("negative next* test caching") {
  PKB storage1;
  storage1.addStatement(1, read, "ASample");
  storage1.addStatement(2, print, "ASample");
  storage1.addStatement(3, assign, "ASample");
  storage1.addStatement(4, call, "ASample");
  storage1.addStatement(5, assign, "toyota");
  storage1.addStatement(6, print, "toyota");
  storage1.addStatement(7, call, "toyota");

  storage1.addNext(1, 2);
  storage1.addNext(2, 3);
  storage1.addNext(2, 4);

  storage1.addNext(5, 6);
  storage1.addNext(6, 7);

  std::unordered_map<int, std::unordered_set<int>> stubNextProc = {};
  stubNextProc.emplace(1, std::unordered_set<int>{2});
  stubNextProc.emplace(2, std::unordered_set<int>{3, 4});

  CFG *cfg = new CFG(stubNextProc);
  storage1.addCFG("ASample", cfg);

  REQUIRE(storage1.getCFG("ASample")->hasBeenCached == false);
}

TEST_CASE("clear all cache") {
  PKB storage1;
  storage1.addStatement(1, read, "ASample");
  storage1.addStatement(2, print, "ASample");
  storage1.addStatement(3, assign, "ASample");
  storage1.addStatement(4, call, "ASample");
  storage1.addStatement(5, assign, "toyota");
  storage1.addStatement(6, print, "toyota");
  storage1.addStatement(7, call, "toyota");

  storage1.addNext(1, 2);
  storage1.addNext(2, 3);
  storage1.addNext(2, 4);

  storage1.addNext(5, 6);
  storage1.addNext(6, 7);
  storage1.addProc("ASample");
  storage1.addProc("toyota");

  std::unordered_map<int, std::unordered_set<int>> stubNextProc = {};
  stubNextProc.emplace(1, std::unordered_set<int>{2});
  stubNextProc.emplace(2, std::unordered_set<int>{3, 4});

  std::unordered_map<int, std::unordered_set<int>> stubNextProc2 = {};
  stubNextProc2.emplace(5, std::unordered_set<int>{6});
  stubNextProc2.emplace(6, std::unordered_set<int>{7});

  CFG *cfg = new CFG(stubNextProc);
  storage1.addCFG("ASample", cfg);
  storage1.getCFG("ASample")->isNextStar(1, 3);

  CFG *cfg2 = new CFG(stubNextProc2);
  storage1.addCFG("toyota", cfg2);
  storage1.getCFG("toyota")->isNextStar(5, 7);

  storage1.clearCacheCFG();

  REQUIRE(storage1.getCFG("toyota")->hasBeenCached == false);
}