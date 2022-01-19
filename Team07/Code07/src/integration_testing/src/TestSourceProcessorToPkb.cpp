//
// Created by Kamil Gimik on 9/9/21.
//

#include "SimpleParser.h"
#include "catch.hpp"


// TODO:: test assign table and check that it stores vector of tokens


SimpleParser *createSimpleParser(PKB *pkb, std::string program) {
  return new SimpleParser(pkb, program);
}

void callDesignAbstractor(PKB *pkb) {
  auto da = new DesignAbstractor(pkb);
  da->process();
}

PKB *createPKB() {
  return new PKB();
}

TEST_CASE("SourceProcessor sanity check") {
  REQUIRE(1 == 1);
}

TEST_CASE("VarTable, ProcTable, ConstTable insertion works for assign, single procedure") {
  std::string program = "procedure Example { a = b; b = 3; }";
  auto *pkb = createPKB();
  auto simpleParser = createSimpleParser(pkb, program);

  REQUIRE(simpleParser->parse());
  callDesignAbstractor(pkb);

  auto expectedVarResults = {"a", "b"};
  auto actualVarResults = pkb->getVarList();

  REQUIRE(expectedVarResults.size() == actualVarResults.size());

  for (auto &result: expectedVarResults) {
    REQUIRE(actualVarResults.count(result) == 1);
  }

  auto expectedProcResults = {"Example"};
  auto actualProcResults = pkb->getProcList();

  REQUIRE(expectedProcResults.size() == actualProcResults.size());

  for (auto &result: expectedProcResults) {
    REQUIRE(actualProcResults.count(result) == 1);
  }

  auto expectedConstResults = {"3"};
  auto actualConstResults = pkb->getConstList();

  REQUIRE(expectedConstResults.size() == actualConstResults.size());

  for (auto &result: expectedConstResults) {
    REQUIRE(actualConstResults.count(result) == 1);
  }

  delete simpleParser;
  delete pkb;
}

TEST_CASE("VarTable insertion works for read, print") {
  std::string program = "procedure Example { read a; print b; }";
  auto *pkb = createPKB();
  auto simpleParser = createSimpleParser(pkb, program);

  REQUIRE(simpleParser->parse());
  callDesignAbstractor(pkb);

  auto expectedResults = {"a", "b"};
  auto actualResults = pkb->getVarList();

  REQUIRE(expectedResults.size() == actualResults.size());

  for (auto &result: expectedResults) {
    REQUIRE(actualResults.count(result) == 1);
  }

  delete simpleParser;
  delete pkb;
}

TEST_CASE("ProcTable insertion works for multiple procedures") {
  std::string program = "procedure Example { read a; print b; }"; // \n procedure Eggsample { } ";
  auto *pkb = createPKB();
  auto simpleParser = createSimpleParser(pkb, program);

  REQUIRE(simpleParser->parse());
  callDesignAbstractor(pkb);

  auto expectedResults = {"Example"};
  auto actualResults = pkb->getProcList();

  REQUIRE(expectedResults.size() == actualResults.size());

  for (auto &result: expectedResults) {
    REQUIRE(actualResults.count(result) == 1);
  }

  delete simpleParser;
  delete pkb;
}

TEST_CASE("ModifiesTable for proc and var insertion works for read") {
  std::string program = "procedure Example { read a; print b; read c; }"; // \n procedure Eggsample { read c; } ";
  auto *pkb = createPKB();
  auto simpleParser = createSimpleParser(pkb, program);

  REQUIRE(simpleParser->parse());
  callDesignAbstractor(pkb);

  auto expectedVarResults = {"a"};
  auto actualVarResults = pkb->getVarModifiedByStmt(1);

  REQUIRE(expectedVarResults.size() == actualVarResults.size());

  for (auto &result: expectedVarResults) {
    REQUIRE(actualVarResults.count(result) == 1);
  }

  auto expectedProcResults = {"a", "c"};
  auto actualProcResults = pkb->getVarModifiedByProc("Example");

  REQUIRE(expectedProcResults.size() == actualProcResults.size());

  for (auto &result: expectedProcResults) {
    REQUIRE(actualProcResults.count(result) == 1);
  }

  delete simpleParser;
  delete pkb;
}

TEST_CASE("Follows insertion test") {
  std::string program = "procedure Example { a = b; b = 3; }";
  auto *pkb = createPKB();
  auto simpleParser = createSimpleParser(pkb, program);

  REQUIRE(simpleParser->parse());
  callDesignAbstractor(pkb);

  auto expectedFollowerResults = {1};
  auto actualFollowerResults = pkb->getAllFollowedBy();

  REQUIRE(expectedFollowerResults.size() == actualFollowerResults.size());

  REQUIRE(pkb->getFollower(1) == 2);

  for (auto &result: expectedFollowerResults) {
    REQUIRE(actualFollowerResults.count(result) == 1);
  }

  delete simpleParser;
  delete pkb;
}

TEST_CASE("Follows insertion test 2") {
  std::string program = "procedure Example { a = b; b = 3; while (a==b) { c = 1; } print a; }";
  auto *pkb = createPKB();
  auto simpleParser = createSimpleParser(pkb, program);

  REQUIRE(simpleParser->parse());
  callDesignAbstractor(pkb);

  auto expectedFollowerResults = {1, 2, 3};
  auto actualFollowerResults = pkb->getAllFollowedBy();

  REQUIRE(expectedFollowerResults.size() == actualFollowerResults.size());

  for (auto &result: expectedFollowerResults) {
    REQUIRE(actualFollowerResults.count(result) == 1);
  }

  REQUIRE(pkb->getFollower(1) == 2);
  REQUIRE(pkb->getFollower(2) == 3);
  REQUIRE(pkb->getFollower(3) == 5);

  delete simpleParser;
  delete pkb;
}

TEST_CASE("Parent insertion test 1") {
  std::string program = "procedure Example { a = b; b = 3; while (a==b) { c = 1; } print a; }";
  auto *pkb = createPKB();
  auto simpleParser = createSimpleParser(pkb, program);

  REQUIRE(simpleParser->parse());
  callDesignAbstractor(pkb);

  auto expectedParentResults = {3};
  auto actualParentResults = pkb->getParentList();

  REQUIRE(expectedParentResults.size() == actualParentResults.size());

  for (auto &result: expectedParentResults) {
    REQUIRE(actualParentResults.count(result) == 1);
  }

  REQUIRE(pkb->getParent(4) == 3);

  delete simpleParser;
  delete pkb;
}

TEST_CASE("Parent (Nested) insertion test 1") {
  std::string program = "procedure Example {"
                        "a = b;"
                        "b = 3;"
                        "while (a==b) {"
                        "       c = 1;"
                        "       while (a < (b+c)) {"
                        "              print c;"
                        "              }"
                        "        }"
                        "print a;"
                        "}";
  auto *pkb = createPKB();
  auto simpleParser = createSimpleParser(pkb, program);

  REQUIRE(simpleParser->parse());
  callDesignAbstractor(pkb);

  auto expectedParentResults = {3, 5};
  auto actualParentResults = pkb->getParentList();

  REQUIRE(expectedParentResults.size() == actualParentResults.size());

  for (auto &result: expectedParentResults) {
    REQUIRE(actualParentResults.count(result) == 1);
  }

  auto expectedChildResults = {4, 5, 6};
  auto actualChildResults = pkb->getChildList();

  REQUIRE(expectedChildResults.size() == actualChildResults.size());

  for (auto &result: expectedChildResults) {
    REQUIRE(actualChildResults.count(result) == 1);
  }

  REQUIRE(pkb->getParent(4) == 3);
  REQUIRE(pkb->getParent(5) == 3);
  REQUIRE(pkb->getParent(6) == 5);

  delete simpleParser;
  delete pkb;
}

TEST_CASE("Parent (Nested) insertion test 2") {
  std::string program = "procedure Example {"
                        "a = b;"
                        "b = 3;"
                        "while (a==b) {"
                        "       c = 1;"
                        "       if (a < (b+c)) then {"
                        "              print c;"
                        "              } else {"
                        "              b = 3;"
                        "}"
                        "        }"
                        "print a;"
                        "}";
  auto *pkb = createPKB();
  auto simpleParser = createSimpleParser(pkb, program);

  REQUIRE(simpleParser->parse());
  callDesignAbstractor(pkb);

  auto expectedParentResults = {3, 5};
  auto actualParentResults = pkb->getParentList();

  REQUIRE(expectedParentResults.size() == actualParentResults.size());

  for (auto &result: expectedParentResults) {
    REQUIRE(actualParentResults.count(result) == 1);
  }

  auto expectedChildResults = {4, 5, 6, 7};
  auto actualChildResults = pkb->getChildList();

  REQUIRE(expectedChildResults.size() == actualChildResults.size());

  for (auto &result: expectedChildResults) {
    REQUIRE(actualChildResults.count(result) == 1);
  }

  REQUIRE(pkb->getParent(4) == 3);
  REQUIRE(pkb->getParent(5) == 3);
  REQUIRE(pkb->getParent(6) == 5);
  REQUIRE(pkb->getParent(7) == 5);

  delete simpleParser;
  delete pkb;
}

TEST_CASE("Parent (Nested) insertion test 3") {
  std::string program = "procedure Example {"
                        "    a = b;"//1
                        "    b = 3;"//2
                        "    while (a==b) {"//3
                        "       c = 1;"//4
                        "       if (a < (b+c)) then {"//5
                        "              print c;"//6
                        "       } else {"
                        "              while (a==b) { "
                        "                    print c; }"//7 8
                        "              b = 3;"//9
                        "       }"
                        "    }"
                        "    print a;"//10
                        "}";

  auto *pkb = createPKB();
  auto simpleParser = createSimpleParser(pkb, program);

  REQUIRE(simpleParser->parse());
  callDesignAbstractor(pkb);

  auto expectedParentResults = {3, 5, 7};
  auto actualParentResults = pkb->getParentList();

  REQUIRE(expectedParentResults.size() == actualParentResults.size());

  for (auto &result: expectedParentResults) {
    REQUIRE(actualParentResults.count(result) == 1);
  }

  auto expectedChildResults = {4, 5, 6, 7, 8, 9};
  auto actualChildResults = pkb->getChildList();

  REQUIRE(expectedChildResults.size() == actualChildResults.size());

  for (auto &result: expectedChildResults) {
    REQUIRE(actualChildResults.count(result) == 1);
  }

  REQUIRE(pkb->getParent(4) == 3);
  REQUIRE(pkb->getParent(5) == 3);
  REQUIRE(pkb->getParent(6) == 5);
  REQUIRE(pkb->getParent(7) == 5);
  REQUIRE(pkb->getParent(8) == 7);
  REQUIRE(pkb->getParent(9) == 5);

  auto expectedParentStar = {3, 5, 7}; // for stmt 8
  auto actualParentStar = pkb->getParentStar(8);

  REQUIRE(expectedParentStar.size() == actualParentStar.size());

  for (auto &result: expectedParentStar) {
    REQUIRE(actualParentStar.count(result) == 1);
  }

  delete simpleParser;
  delete pkb;
}

TEST_CASE("Follows (Nested) insertion test 1") {
  std::string program = "procedure Example {"
                        "a = b;"//1
                        "b = 3;"//2
                        "while (a==b) {"//3
                        "       c = 1;"//4
                        "       if (a < (b+c)) then {"//5
                        "              print c;"//6
                        "              } else {"
                        " while (a==b) { print c; }"//7 8
                        "              b = 3;"//9
                        "}"
                        "        }"
                        "print a;"//10
                        "}";
  auto *pkb = createPKB();
  auto simpleParser = createSimpleParser(pkb, program);

  REQUIRE(simpleParser->parse());
  callDesignAbstractor(pkb);

  auto expectedFollowResults = {2, 3, 5, 9, 10};
  auto actualFollowResults = pkb->getAllFollowers();

  REQUIRE(expectedFollowResults.size() == actualFollowResults.size());

  for (auto &result: expectedFollowResults) {
    REQUIRE(actualFollowResults.count(result) == 1);
  }

  auto expectedFollowedResults = {1, 2, 3, 4, 7};
  auto actualFollowedResults = pkb->getAllFollowedBy();

  REQUIRE(expectedFollowedResults.size() == actualFollowedResults.size());

  for (auto &result: expectedFollowedResults) {
    REQUIRE(actualFollowedResults.count(result) == 1);
  }

  REQUIRE(pkb->getFollower(1) == 2);
  REQUIRE(pkb->getFollower(2) == 3);
  REQUIRE(pkb->getFollower(3) == 10);
  REQUIRE(pkb->getFollower(4) == 5);
  REQUIRE(pkb->getFollower(7) == 9);

  auto expectedFollowerStar = {2, 3, 10}; // for stmt 1
  auto actualFollowerStar = pkb->getFollowerStar(1);

  REQUIRE(expectedFollowerStar.size() == actualFollowerStar.size());

  for (auto &result: expectedFollowerStar) {
    REQUIRE(actualFollowerStar.count(result) == 1);
  }

  auto expectedFollowedByStar = {1, 2, 3}; // for stmt 10
  auto actualFollowedByStar = pkb->getFollowedByStar(10);

  REQUIRE(expectedFollowedByStar.size() == actualFollowedByStar.size());

  for (auto &result: expectedFollowedByStar) {
    REQUIRE(actualFollowedByStar.count(result) == 1);
  }

  delete simpleParser;
  delete pkb;
}

TEST_CASE("While/If (Nested) insertion test 1") {
  std::string program = "procedure Example {"
                        "    a = b;"//1
                        "    b = 3;"//2
                        "    while (a==b) {"//3
                        "       c = 1;"//4
                        "       if (a < (b+c)) then {"//5
                        "              print c;"//6
                        "       } else {"
                        "              if (a==b) then {" //7
                        "                   while (a==d) { " //8
                        "                         print c; }"//9
                        "                    b = 3;"//10
                        "                    d = 8 + 3 + (b - 9 % y); " //11
                        "              } else {"
                        "                     a = b; }"  //12
                        "       }"
                        "    }"
                        "    print a;"//13
                        "}";

  auto *pkb = createPKB();
  auto simpleParser = createSimpleParser(pkb, program);

  REQUIRE(simpleParser->parse());
  callDesignAbstractor(pkb);

  auto expectedIfResults = {5, 7};
  auto actualIfResults = pkb->getAllIf();

  REQUIRE(expectedIfResults.size() == actualIfResults.size());

  for (auto &result: expectedIfResults) {
    REQUIRE(actualIfResults.count(result) == 1);
  }

  auto expectedIfVarResults = {5};
  auto actualIfVarResults = pkb->getIfCtrlVar("c");

  REQUIRE(expectedIfVarResults.size() == actualIfVarResults.size());

  for (auto &result: expectedIfVarResults) {
    REQUIRE(actualIfVarResults.count(result) == 1);
  }

  auto expectedIfVar2Results = {5, 7};
  auto actualIfVar2Results = pkb->getIfCtrlVar("b");

  REQUIRE(expectedIfVar2Results.size() == actualIfVar2Results.size());

  for (auto &result: expectedIfVar2Results) {
    REQUIRE(actualIfVar2Results.count(result) == 1);
  }

  auto expectedWhileResults = {3, 8};
  auto actualWhileResults = pkb->getAllWhile();

  REQUIRE(expectedWhileResults.size() == actualWhileResults.size());

  for (auto &result: expectedWhileResults) {
    REQUIRE(actualWhileResults.count(result) == 1);
  }

  auto expectedWhileVarResults = {8};
  auto actualWhileVarResults = pkb->getWhileCtrlVar("d");

  REQUIRE(expectedWhileVarResults.size() == actualWhileVarResults.size());

  for (auto &result: expectedWhileVarResults) {
    REQUIRE(actualWhileVarResults.count(result) == 1);
  }

  delete simpleParser;
  delete pkb;
}

TEST_CASE("Modifies/ Uses (Nested) insertion test 1") {
  std::string program = "procedure Example {"
                        "    a = b;"//1
                        "    b = 3;"//2
                        "    while (a==b) {"//3
                        "       c = 1;"//4
                        "       if (a < (b+c)) then {"//5
                        "              print c;"//6
                        "       } else {"
                        "              if (a==b) then {" //7
                        "                   while (a==d) { " //8
                        "                         print c; }"//9
                        "                    b = 3;"//10
                        "                    d = 8 + 3 + (b - 9 * y); " //11
                        "              } else {"
                        "                     a = b; }"  //12
                        "       }"
                        "    }"
                        "    print a;"//13
                        "}";

  auto *pkb = createPKB();
  auto simpleParser = createSimpleParser(pkb, program);

  REQUIRE(simpleParser->parse());
  callDesignAbstractor(pkb);

  auto expectedUseResults = {3, 5, 6, 7, 8, 9};
  auto actualUseResults = pkb->getStmtUsingVar("c");

  REQUIRE(expectedUseResults.size() == actualUseResults.size());

  for (auto &result: expectedUseResults) {
    REQUIRE(actualUseResults.count(result) == 1);
  }

  auto expectedModifiesResults = {3, 4};
  auto actualModifiesResults = pkb->getStmtModifyingVar("c");

  REQUIRE(expectedModifiesResults.size() == actualModifiesResults.size());

  for (auto &result: expectedModifiesResults) {
    REQUIRE(actualModifiesResults.count(result) == 1);
  }

  delete simpleParser;
  delete pkb;
}

TEST_CASE("Nested while statements") {
  std::string program = "procedure nestedWhileAsf{\n"
                        "    a = 2;\n"
                        "    b = 1 + 2;\n"
                        "    while (a >= 2){\n"
                        "        a = a - 1;\n"
                        "        while (b >= 3) {\n"
                        "            b = b - 1;\n"
                        "            while(c >= 4) {\n"
                        "                c = c - 1;\n"
                        "                while(d >= 5){\n"
                        "                    d = d - 1;\n"
                        "                    while(f >= 6){\n"
                        "                        f = f - 1;\n"
                        "                    }\n"
                        "                }\n"
                        "            }\n"
                        "        }\n"
                        "    }\n"
                        "    b = a;\n"
                        "}";
  auto pkb = createPKB();
  auto simpleParser = createSimpleParser(pkb, program);
  REQUIRE(simpleParser->parse());
  callDesignAbstractor(pkb);

  std::unordered_set<int> expectedParentResults = {3, 5, 7, 9, 11};
  auto actualParentResults = pkb->getParentList();
  REQUIRE(expectedParentResults == actualParentResults);

  std::vector<int> expectedFollows = {1, 2, 3, 4, 6, 8, 10};
  std::vector<int> expectedFollowing = {2, 3, 13, 5, 7, 9, 11};
  for (int i = 0; i < expectedFollows.size(); ++i) {
    INFO(std::to_string(expectedFollows[i]) + " " + std::to_string(expectedFollowing[i]));
    REQUIRE(pkb->isFollow(expectedFollows[i], expectedFollowing[i]));
  }

}

TEST_CASE("Assignments") {
  std::string program = "procedure Example {"
                        "    a = z+(9+a*(b+c))/2;"//1
                        "    b = (((c+d)*e)+f)*g+h/i;"//2
                        "}";

  auto *pkb = createPKB();
  auto simpleParser = createSimpleParser(pkb, program);

  REQUIRE(simpleParser->parse());
  callDesignAbstractor(pkb);

  // actual assignment pairs
  std::vector<std::pair<int, std::pair<std::string, std::string>>>
      assignmentPairs = {{1, {"a", "z+(9+a*(b+c))/2"}}, {2, {"b", "(((c+d)*e)+f)*g+h/i"}}};

  int expectedAssignPatternSize = assignmentPairs.size();
  int actualAssignPatternSize = pkb->getAssignStmts().size();

  REQUIRE(expectedAssignPatternSize == actualAssignPatternSize);

  for (const auto &assignment: assignmentPairs) {
    int stmtNum = assignment.first;

    std::string expectedLHS = assignment.second.first;
    std::string expectedRHS = assignment.second.second;
    std::string actualLHS = pkb->getLHSAssign(stmtNum);
    std::string actualRHS = pkb->getRHSAssign(stmtNum);

    REQUIRE(expectedLHS == actualLHS);
    REQUIRE(expectedRHS == actualRHS);
  }

  delete simpleParser;
  delete pkb;
}

TEST_CASE("Nested while Assignments") {
  std::string program = "procedure nestedWhileAsf {\n"
                        "    a = 2;\n" //1
                        "    b = 1 + 2;\n" //2
                        "    while (a >= 2){\n"
                        "        a = a - 1;\n" //4
                        "        while (b >= 3) {\n"
                        "            b = b - 1;\n" //6
                        "            while(c >= 4) {\n"
                        "                c = c - 1;\n" //8
                        "                while(d >= 5){\n"
                        "                    d = d - 1;\n" //10
                        "                    while(f >= 6){\n"
                        "                        f = f - 1;\n" //12
                        "                    }\n"
                        "                }\n"
                        "            }\n"
                        "        }\n"
                        "    }\n"
                        "    b = a;\n" //13
                        "}";

  auto *pkb = createPKB();
  auto simpleParser = createSimpleParser(pkb, program);

  REQUIRE(simpleParser->parse());
  callDesignAbstractor(pkb);

  // actual assignment pairs
  std::vector<std::pair<int, std::pair<std::string, std::string>>> assignmentPairs =
      {
          {1, {"a", "2"}},
          {2, {"b", "1+2"}},
          {4, {"a", "a-1"}},
          {6, {"b", "b-1"}},
          {8, {"c", "c-1"}},
          {10, {"d", "d-1"}},
          {12, {"f", "f-1"}},
          {13, {"b", "a"}},
      };

  int expectedAssignPatternSize = assignmentPairs.size();
  int actualAssignPatternSize = pkb->getAssignStmts().size();

  REQUIRE(expectedAssignPatternSize == actualAssignPatternSize);

  for (auto assignment: assignmentPairs) {
    int stmtNum = assignment.first;

    std::string expectedLHS = assignment.second.first;
    std::string expectedRHS = assignment.second.second;
    std::string actualLHS = pkb->getLHSAssign(stmtNum);
    std::string actualRHS = pkb->getRHSAssign(stmtNum);

    REQUIRE(expectedLHS == actualLHS);
    REQUIRE(expectedRHS == actualRHS);
  }

  std::unordered_set<int> expectedAssign = {1, 2, 4, 6, 8, 10, 12, 13};
  REQUIRE(pkb->getAssignStmts() == expectedAssign);

  std::unordered_set<int> expectedModifies = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13};
  REQUIRE(pkb->getAllModifies() == expectedModifies);

  std::unordered_set<int> expectedUses = {3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13};
  REQUIRE(pkb->getAllUse() == expectedUses);

  std::unordered_set<std::string> expected = {"nestedWhileAsf"};
  REQUIRE(pkb->getProcList() == expected);

  delete simpleParser;
  delete pkb;
}

TEST_CASE("Nested If Assignments") {
  std::string program = "procedure if {\n"
                        "    a = a + 1;\n" //1
                        "    if (a == 1) then {\n"
                        "        a = a + 1;\n" // 3
                        "        if (b == 1) then {\n"
                        "            b = b + 1;\n" // 5
                        "            if (c == 1) then {\n"
                        "                c = c + 1;\n" // 7
                        "                if (d == 1) then {\n"
                        "                    d = d + 1;\n" // 9
                        "                    if (e == 1) then {\n"
                        "                        e = e + 1;\n" // 11
                        "                        if (f == 1) then {\n"
                        "                            f = f + 1;\n" // 13
                        "                        } else {\n"
                        "                            f = f + 1;\n" //14
                        "                        }\n"
                        "                    } else {\n"
                        "                        f = f + 1;\n" // 15
                        "                    }\n"
                        "                } else {\n"
                        "                    f = f + 1;\n" // 16
                        "                }\n"
                        "            } else {\n"
                        "                f = f + 1;\n" // 17
                        "            }\n"
                        "        } else {\n"
                        "            f = f + 1;\n" // 18
                        "        }\n"
                        "    } else {\n"
                        "        f = f + 1;\n" // 19
                        "    }\n"
                        "}";

  auto *pkb = createPKB();
  auto simpleParser = createSimpleParser(pkb, program);

  REQUIRE(simpleParser->parse());
  callDesignAbstractor(pkb);

  // actual assignment pairs
  std::vector<std::pair<int, std::pair<std::string, std::string>>> assignmentPairs =
      {
          {1, {"a", "a+1"}},
          {3, {"a", "a+1"}},
          {5, {"b", "b+1"}},
          {7, {"c", "c+1"}},
          {9, {"d", "d+1"}},
          {11, {"e", "e+1"}},
          {13, {"f", "f+1"}},
          {14, {"f", "f+1"}},
          {15, {"f", "f+1"}},
          {16, {"f", "f+1"}},
          {17, {"f", "f+1"}},
          {18, {"f", "f+1"}},
          {19, {"f", "f+1"}},
      };

  int expectedAssignPatternSize = assignmentPairs.size();
  int actualAssignPatternSize = pkb->getAssignStmts().size();

  REQUIRE(expectedAssignPatternSize == actualAssignPatternSize);

  for (auto assignment: assignmentPairs) {
    int stmtNum = assignment.first;

    std::string expectedLHS = assignment.second.first;
    std::string expectedRHS = assignment.second.second;
    std::string actualLHS = pkb->getLHSAssign(stmtNum);
    std::string actualRHS = pkb->getRHSAssign(stmtNum);

    REQUIRE(expectedLHS == actualLHS);
    REQUIRE(expectedRHS == actualRHS);
  }

  delete simpleParser;
  delete pkb;
}

TEST_CASE("If without else") {
  std::string program = "procedure example{if(1 == b)then{ a = 1; }}";
  auto pkb = createPKB();
  auto simpleParser = createSimpleParser(pkb, program);
  REQUIRE_THROWS(simpleParser->parse());
}

TEST_CASE("Parent table correctly populated") {
  std::string program =
      "procedure Example {\n"
      "\ta = 1;\n"
      "\tb = 1;\n"
      "\tprint b;\n"
      "\tprint a;\n"
      "\tread procedure;\n"
      "\tprint procedure;\n"
      "\tread proc;\n"
      "\ta = b + c;\n"
      "\tb = c + d;\n"
      "\tif (a == 2) then {\n"
      "\t\twhile(a > 0){\n"
      "\t\t\tc = d + 1;\n"
      "\t\t\ta = a - 1;\n"
      "\t\t\tif (c <= d) then {\n"
      "\t\t\t\td = d - 1;\n"
      "\t\t\t\tc = c + 1;\n"
      "\t\t\t} else {\n"
      "\t\t\t\ta = a + 1;\n"
      "\t\t\t}\n"
      "\t\t}\n"
      "\t} else {\n"
      "\t\tc = c + 1;\n"
      "\t}\n"
      "\tc = 7 + e;\n"
      "}";
  auto pkb = createPKB();
  auto simpleParser = createSimpleParser(pkb, program);
  REQUIRE(simpleParser->parse());
  callDesignAbstractor(pkb);

  // expected
  std::vector<int> tenChild = {11, 18};
  std::vector<int> elevenChild = {12, 13, 14};
  std::vector<int> fourteenChild = {15, 16, 17};

  std::vector<std::pair<int, std::vector<int>>> parentChildren = {
      {10, {11, 18}},
      {11, {12, 13, 14}},
      {14, {15, 16, 17}},
  };

  for (auto pc: parentChildren) {
    int parent = pc.first;
    auto expectedChildren = pc.second;
    auto actualChildren = pkb->getChild(parent);
    // ensure same size. Make sure pkb dont have extra children
    REQUIRE(actualChildren.size() == expectedChildren.size());
    // all expected child in same pkb
    for (int expectedChild: expectedChildren) {
      REQUIRE(actualChildren.count(expectedChild) == 1);
    }

  }

}

TEST_CASE("Follows insertion test for multiple procs") {
  std::string program = "procedure Example {"
                        " a = b; "
                        "b = 3; "
                        "while (a==b) { "
                        "c = 1; "
                        "} "
                        "print a; "
                        "}"
                        ""
                        "procedure Eggsample {"
                        "a=b;"
                        "b=3;"
                        "}";
  auto *pkb = createPKB();
  auto simpleParser = createSimpleParser(pkb, program);

  REQUIRE(simpleParser->parse());
  callDesignAbstractor(pkb);

  auto expectedFollowedByResults = {1, 2, 3, 6};
  auto actualFollowedByResults = pkb->getAllFollowedBy();

  REQUIRE(expectedFollowedByResults.size() == actualFollowedByResults.size());

  for (auto &result: expectedFollowedByResults) {
    REQUIRE(actualFollowedByResults.count(result) == 1);
  }

  REQUIRE(pkb->getFollower(1) == 2);
  REQUIRE(pkb->getFollower(2) == 3);
  REQUIRE(pkb->getFollower(3) == 5);
  REQUIRE(pkb->getFollower(6) == 7);

  auto expectedFollowerStarResultsForStmt1 = {2, 3, 5};
  auto actualFollowerStarResultsForStmt1 = pkb->getFollowerStar(1);

  REQUIRE(expectedFollowerStarResultsForStmt1.size() == actualFollowerStarResultsForStmt1.size());

  for (auto &result: expectedFollowerStarResultsForStmt1) {
    REQUIRE(actualFollowerStarResultsForStmt1.count(result) == 1);
  }

  delete simpleParser;
  delete pkb;
}

TEST_CASE("CallStar test for multiple procs") {
  std::string program = "procedure C {"
                        "call B;"
                        "call = 2;"
                        "}"
                        ""
                        "procedure B {"
                        "a=b;"
                        "}";
  auto *pkb = createPKB();
  auto simpleParser = createSimpleParser(pkb, program);

  REQUIRE(simpleParser->parse());
  callDesignAbstractor(pkb);

  auto expectedCallResults = {1};
  auto actualCallResults = pkb->getCallStmts();

  REQUIRE(expectedCallResults.size() == actualCallResults.size());

  for (auto &result: expectedCallResults) {
    REQUIRE(actualCallResults.count(result) == 1);
  }

  REQUIRE(pkb->getCallerProc(1) == "C");
  REQUIRE(pkb->getCalledProc(1) == "B");
  REQUIRE(pkb->isCallRelation("C", "B"));
  REQUIRE(pkb->isCallStarRelation("C", "B"));

  delete simpleParser;
  delete pkb;
}

TEST_CASE("Cyclic call error") {
  std::string program = "procedure A {"
                        "call B;"
                        "call = 2;"
                        "}"
                        ""
                        "procedure B {"
                        "call C;"
                        "}"
                        "procedure C {"
                        "call A;"
                        "}";
  auto *pkb = createPKB();
  auto simpleParser = createSimpleParser(pkb, program);
  REQUIRE_NOTHROW(simpleParser->parse());

  REQUIRE_THROWS_WITH(callDesignAbstractor(pkb), Catch::Matchers::Contains(
      "Cyclic Call exists between procedures"));

  delete simpleParser;
  delete pkb;
}

TEST_CASE("Non existent Proc call error") {
  std::string program = "procedure A {"
                        "call B;"
                        "call = 2;"
                        "}"
                        ""
                        "procedure B {"
                        "call C;"
                        "}"
                        "procedure C {"
                        "call D;"
                        "}";
  auto *pkb = createPKB();
  auto simpleParser = createSimpleParser(pkb, program);
  REQUIRE_NOTHROW(simpleParser->parse());

  REQUIRE_THROWS_WITH(callDesignAbstractor(pkb), Catch::Matchers::Contains(
      "Call stmt calls Procedure that does not exist"));

  delete simpleParser;
  delete pkb;
}

TEST_CASE("Multi proc call star valid") {
  std::string program = "procedure A {"
                        "call B;"
                        "call C;"
                        "call = 2;"
                        "}"
                        ""
                        "procedure B {"
                        "call C;"
                        "}"
                        "procedure C {"
                        "call D;"
                        "}"
                        "procedure D {"
                        "call = A;"
                        "call = B;"
                        "}";
  auto *pkb = createPKB();
  auto simpleParser = createSimpleParser(pkb, program);

  REQUIRE(simpleParser->parse());
  callDesignAbstractor(pkb);

  auto expectedCallResults = {1, 2, 4, 5};
  auto actualCallResults = pkb->getCallStmts();

  REQUIRE(expectedCallResults.size() == actualCallResults.size());

  for (auto &result: expectedCallResults) {
    REQUIRE(actualCallResults.count(result) == 1);
  }

  REQUIRE(pkb->getCallerProc(1) == "A");
  REQUIRE(pkb->getCalledProc(1) == "B");
  REQUIRE(pkb->getCallerProc(2) == "A");
  REQUIRE(pkb->getCalledProc(2) == "C");
  REQUIRE(!pkb->isCallRelation("C", "B"));
  REQUIRE(!pkb->isCallStarRelation("C", "B"));
  REQUIRE(pkb->isCallRelation("B", "C"));
  REQUIRE(pkb->isCallStarRelation("B", "C"));
  REQUIRE(pkb->isCallStarRelation("B", "D"));
  REQUIRE(pkb->isCallRelation("C", "D"));
  REQUIRE(pkb->isCallStarRelation("A", "C"));
  REQUIRE(pkb->isCallStarRelation("A", "D"));
  REQUIRE(pkb->isCallStarRelation("A", "B"));

  delete simpleParser;
  delete pkb;
}

TEST_CASE("Cyclic call error many procs") {
  std::string program = "procedure A {"
                        "call B;"
                        "call C;"
                        "call = 2;"
                        "}"
                        ""
                        "procedure B {"
                        "call D;"
                        "call E;"
                        "}"
                        "procedure C {"
                        "call F;"
                        "call G;"
                        "}"
                        "procedure D {"
                        "call H;"
                        "call I;"
                        "}"
                        "procedure E {"
                        "call = I;"
                        "}"
                        "procedure F {"
                        "call = B;"
                        "}"
                        "procedure G {"
                        "call = C;"
                        "}"
                        "procedure H {"
                        "call = 1;"
                        "}"
                        "procedure I {"
                        "call A;"
                        "}";
  auto *pkb = createPKB();
  auto simpleParser = createSimpleParser(pkb, program);
  REQUIRE_NOTHROW(simpleParser->parse());
  REQUIRE_THROWS_WITH(callDesignAbstractor(pkb), Catch::Matchers::Contains(
      "Cyclic Call exists between procedures"));

  delete simpleParser;
  delete pkb;
}

TEST_CASE("Valid many procs no cyclic") {
  std::string program = "procedure A {"
                        "call B;"
                        "call C;"
                        "call = 2;"
                        "}"
                        ""
                        "procedure B {"
                        "call D;"
                        "call E;"
                        "}"
                        "procedure C {"
                        "call F;"
                        "call G;"
                        "}"
                        "procedure D {"
                        "call H;"
                        "call I;"
                        "}"
                        "procedure E {"
                        "call = I;"
                        "}"
                        "procedure F {"
                        "call = B;"
                        "}"
                        "procedure G {"
                        "call = C;"
                        "}"
                        "procedure H {"
                        "call = 1;"
                        "}"
                        "procedure I {"
                        "call C;"
                        "}";
  auto *pkb = createPKB();
  auto simpleParser = createSimpleParser(pkb, program);

  REQUIRE(simpleParser->parse());
  callDesignAbstractor(pkb);

  REQUIRE(pkb->isCallStarRelation("A", "B"));
  REQUIRE(pkb->isCallStarRelation("B", "I"));
  REQUIRE(pkb->isCallStarRelation("A", "I"));
  REQUIRE(pkb->isCallRelation("I", "C"));
  REQUIRE(pkb->isCallStarRelation("I", "G"));
  REQUIRE(pkb->isCallStarRelation("A", "D"));
  REQUIRE(pkb->isCallStarRelation("A", "E"));
  REQUIRE(pkb->isCallStarRelation("A", "H"));
  REQUIRE(pkb->isCallStarRelation("A", "F"));
  REQUIRE(!pkb->isCallStarRelation("I", "B"));
  REQUIRE(pkb->isCallStarRelation("B", "F"));

  delete simpleParser;
  delete pkb;
}

TEST_CASE("Next test 1") {
  std::string program = "procedure Example {"
                        "a = b;"//1
                        "while (a == b) {" //2
                        "  c = 2;" //3
                        "  d = 3;" //4
                        "}"
                        "if (a==b) then {" //5
                        "a = 2;" //6
                        "} else {"
                        "b = 3;" //7
                        "}"
                        "print a;"//8
                        "}";
  auto *pkb = createPKB();
  auto simpleParser = createSimpleParser(pkb, program);

  REQUIRE(simpleParser->parse());
  callDesignAbstractor(pkb);

  std::unordered_set<std::pair<int, int>, std::hash_pair> expectedNextResults = {
      {1, 2}, {2, 3}, {3, 4}, {4, 2}, {2, 5},
      {5, 6}, {5, 7}, {6, 8}, {7, 8}};

  auto actualNextResults = pkb->getAllNextPairs();

  REQUIRE(expectedNextResults.size() == actualNextResults.size());

  for (auto &result: expectedNextResults) {
    REQUIRE(actualNextResults.count(result) == 1);
  }

  delete simpleParser;
  delete pkb;
}

TEST_CASE("Next (Nested) test 1") {
  std::string program = "procedure Example {"
                        "a = b;"//1
                        "while (a == b) {" //2
                        "  c = 2;" //3
                        "  while (a==b) {" //4
                        "    c=1;" //5
                        "}"
                        "}"
                        "while (a==b) {" //6
                        "if (a==b) then {" //7
                        "a = 2;" //8
                        "} else {"
                        "b = 3;" //9
                        "}"
                        "}"
                        "print a;"//10
                        "}";
  auto *pkb = createPKB();
  auto simpleParser = createSimpleParser(pkb, program);

  REQUIRE(simpleParser->parse());
  callDesignAbstractor(pkb);

  std::unordered_set<std::pair<int, int>, std::hash_pair> expectedNextResults = {
      {1, 2}, {2, 3}, {3, 4}, {4, 2}, {4, 5},
      {5, 4}, {2, 6}, {6, 7}, {7, 8}, {7, 9},
      {8, 6}, {9, 6}, {6, 10}};

  auto actualNextResults = pkb->getAllNextPairs();

  REQUIRE(expectedNextResults.size() == actualNextResults.size());

  for (auto &result: expectedNextResults) {
    REQUIRE(actualNextResults.count(result) == 1);
  }

  delete simpleParser;
  delete pkb;
}

TEST_CASE("Next (Nested) test 2") {
  std::string program = "procedure Example {"
                        "a = b;"//1
                        "if (a == b) then {" //2
                        "  c = 2;" //3
                        "  while (a==b) {" //4
                        "    c=1;" //5
                        "}"
                        "} else {"
                        "if (a != b) then {" //6
                        "a = b;" //7
                        "} else {"
                        "a = 3;" //8
                        "}"
                        "}"
                        "print a;"//9
                        "}";
  auto *pkb = createPKB();
  auto simpleParser = createSimpleParser(pkb, program);

  REQUIRE(simpleParser->parse());
  callDesignAbstractor(pkb);

  std::unordered_set<std::pair<int, int>, std::hash_pair> expectedNextResults = {
      {1, 2}, {2, 3}, {3, 4}, {4, 5}, {5, 4},
      {4, 9}, {2, 6}, {6, 7}, {6, 8}, {7, 9},
      {8, 9}};

  auto actualNextResults = pkb->getAllNextPairs();

  REQUIRE(expectedNextResults.size() == actualNextResults.size());

  for (auto &result: expectedNextResults) {
    REQUIRE(actualNextResults.count(result) == 1);
  }

  delete simpleParser;
  delete pkb;
}

TEST_CASE("Next (Triple Nested) test 1") {
  std::string program = "procedure Example {"
                        "a = b;"//1
                        "if (a == b) then {" //2
                        "  c = 2;" //3
                        "if (a==b) then {" //4
                        "  while (a==b) {" //5
                        "    c=1;" //6
                        "}"
                        "} else {"
                        "a = 3;"//7
                        "}"
                        "c = a;"//8
                        "} else {"
                        "a = c;"//9
                        "}"
                        "}";
  auto *pkb = createPKB();
  auto simpleParser = createSimpleParser(pkb, program);

  REQUIRE(simpleParser->parse());
  callDesignAbstractor(pkb);

  std::unordered_set<std::pair<int, int>, std::hash_pair> expectedNextResults = {
      {1, 2}, {2, 3}, {3, 4}, {4, 5}, {5, 6},
      {6, 5}, {5, 8}, {4, 7}, {7, 8}, {2, 9}};

  auto actualNextResults = pkb->getAllNextPairs();

  REQUIRE(expectedNextResults.size() == actualNextResults.size());

  for (auto &result: expectedNextResults) {
    REQUIRE(actualNextResults.count(result) == 1);
  }

  delete simpleParser;
  delete pkb;
}

TEST_CASE("Next (Triple Nested) test 2") {
  std::string program = "procedure Example {"
                        "a = b;"//1
                        "if (a == b) then {" //2
                        "  c = 2;" //3
                        "if (a==b) then {" //4
                        "  while (a==b) {" //5
                        "    c=1;" //6
                        "}"
                        "} else {"
                        "a = 3;"//7
                        "}"
                        "} else {"
                        "a = c;"//8
                        "}"
                        "a = b;"//9
                        "}";
  auto *pkb = createPKB();
  auto simpleParser = createSimpleParser(pkb, program);

  REQUIRE(simpleParser->parse());
  callDesignAbstractor(pkb);

  std::unordered_set<std::pair<int, int>, std::hash_pair> expectedNextResults = {
      {1, 2}, {2, 3}, {3, 4}, {4, 5}, {5, 6}, {2, 8},
      {6, 5}, {5, 9}, {4, 7}, {7, 9}, {8, 9}};

  auto actualNextResults = pkb->getAllNextPairs();

  REQUIRE(expectedNextResults.size() == actualNextResults.size());

  for (auto &result: expectedNextResults) {
    REQUIRE(actualNextResults.count(result) == 1);
  }

  delete simpleParser;
  delete pkb;
}

TEST_CASE("Next (Super Nested) test 1") {
  std::string program = "procedure QUAAAD {\n"
                        "    a = 1;\n"                                   //1
                        "    while ( a != a) {\n"                        //2
                        "        if ( a == a ) then {\n"                 //3
                        "            if ( a != a ) then {\n"             //4
                        "                while ( a == a) {\n"            //5
                        "                    a = 1 + 1;\n"               //6
                        "                    a = 1 - 1;\n"               //7
                        "                }\n"
                        "            } else {\n"
                        "                while (b == b) {\n"             //8
                        "                    while (c == c) {\n"         //9
                        "                        a = 1;\n"               //10
                        "                    }\n"
                        "                a = 1;\n"                       //11
                        "                }\n"
                        "            }\n"
                        "        } else {\n"
                        "            a = 1;\n"                           //12
                        "        }\n"
                        "    }\n"
                        "}";
  auto *pkb = createPKB();
  auto simpleParser = createSimpleParser(pkb, program);

  REQUIRE(simpleParser->parse());
  callDesignAbstractor(pkb);

  std::unordered_set<std::pair<int, int>, std::hash_pair> expectedNextResults = {
      {1, 2}, {2, 3}, {3, 4}, {4, 5}, {5, 6}, {6, 7},
      {7, 5}, {5, 2}, {4, 8}, {8, 9}, {9, 10}, {10, 9},
      {11, 8}, {9, 11}, {8, 2}, {12, 2}, {3, 12}};

  auto actualNextResults = pkb->getAllNextPairs();

  REQUIRE(expectedNextResults.size() == actualNextResults.size());

  for (auto &result: expectedNextResults) {
    REQUIRE(actualNextResults.count(result) == 1);
  }

  delete simpleParser;
  delete pkb;
}

TEST_CASE("Next* (Super Nested) test 1") {
  std::string program = "procedure quad {\n"
                        "    a = 1;\n"                                   //1
                        "    while ( a != a) {\n"                        //2
                        "        if ( a == a ) then {\n"                 //3
                        "            if ( a != a ) then {\n"             //4
                        "                while ( a == a) {\n"            //5
                        "                    a = 1 + 1;\n"               //6
                        "                    a = 1 - 1;\n"               //7
                        "                }\n"
                        "            } else {\n"
                        "                while (b == b) {\n"             //8
                        "                    while (c == c) {\n"         //9
                        "                        a = 1;\n"               //10
                        "                    }\n"
                        "                a = 1;\n"                       //11
                        "                }\n"
                        "            }\n"
                        "        } else {\n"
                        "            a = 1;\n"                           //12
                        "        }\n"
                        "    }\n"
                        "}";
  auto *pkb = createPKB();
  auto simpleParser = createSimpleParser(pkb, program);

  REQUIRE(simpleParser->parse());
  callDesignAbstractor(pkb);

  auto cfg = pkb->getCFG("quad");

  REQUIRE(cfg->isNextStar(1, 3));
  REQUIRE(!cfg->isNextStar(3, 1));
  REQUIRE(cfg->isNextStar(9, 9));
  REQUIRE(cfg->isNextStar(8, 8));
  REQUIRE(cfg->isNextStar(1, 12));
  REQUIRE(cfg->isNextStar(6, 8));
  REQUIRE(cfg->isNextStar(6, 2));

  auto nextStarListForStmt4 = cfg->getNextStarList(6);
  std::unordered_set<int> expectedResultForStmt4 = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};

  REQUIRE(nextStarListForStmt4.size() == expectedResultForStmt4.size());

  for (auto &result: expectedResultForStmt4) {
    REQUIRE(nextStarListForStmt4.count(result) == 1);
  }

  cfg->clearCache();

  delete simpleParser;
  delete pkb;
}

TEST_CASE("Next* test 2") {
  std::string program = "procedure first {\n"
                        "  a = 1;\n" //1
                        "  if (a == b) then {\n" //2
                        "    a = 1;\n" //3
                        "    if (b == a) then {\n" //4
                        "      a = 1;\n" //5
                        "    } else {\n"
                        "      a = 1;\n" //6
                        "      a = 1;\n" //7
                        "    }\n"
                        "  } else {\n"
                        "    a = 1;\n" //8
                        "  }\n"
                        "  a = 1;" //9
                        "}\n"
                        "\n"
                        "procedure second {\n"
                        "  a = 1;\n" //10
                        "}";
  auto *pkb = createPKB();
  auto simpleParser = createSimpleParser(pkb, program);

  REQUIRE(simpleParser->parse());
  callDesignAbstractor(pkb);

  auto cfg = pkb->getCFG("first");

  auto nextStarListForStmt3 = cfg->getNextStarList(3);
  std::unordered_set<int> expectedResultForStmt3 = {4, 5, 6, 7, 9};

  REQUIRE(nextStarListForStmt3.size() == expectedResultForStmt3.size());

  for (auto &result: expectedResultForStmt3) {
    REQUIRE(nextStarListForStmt3.count(result) == 1);
  }

  cfg->clearCache();

  delete simpleParser;
  delete pkb;
}

TEST_CASE("Valid call procs with modifies properly updated") {
  std::string program = "procedure A {"
                        "call B;"
                        "call C;"
                        "Amod = Ause;"
                        "}"
                        "procedure B {"
                        "call D;"
                        "Bmod = Buse;"
                        "}"
                        "procedure C {"
                        "Cmod = Cuse;"
                        "}"
                        "procedure D {"
                        "Dmod = Duse;"
                        "}";
  auto *pkb = createPKB();
  auto simpleParser = createSimpleParser(pkb, program);

  REQUIRE(simpleParser->parse());
  callDesignAbstractor(pkb);

  REQUIRE(pkb->isCallStarRelation("A", "B"));
  REQUIRE(pkb->isCallStarRelation("A", "C"));
  REQUIRE(pkb->isCallStarRelation("A", "D"));

  REQUIRE(pkb->isProcModifyingVar("A", "Amod"));
  REQUIRE(pkb->isProcModifyingVar("A", "Bmod"));
  REQUIRE(pkb->isProcModifyingVar("A", "Cmod"));
  REQUIRE(pkb->isProcModifyingVar("A", "Dmod"));

  REQUIRE(pkb->isStmtModifyingVar(1, "Bmod"));
  REQUIRE(pkb->isStmtModifyingVar(1, "Dmod"));
  REQUIRE(pkb->isStmtModifyingVar(1, "Cmod") == false);
  REQUIRE(pkb->isStmtModifyingVar(1, "Amod") == false);

  REQUIRE(pkb->isStmtModifyingVar(2, "Bmod") == false);
  REQUIRE(pkb->isStmtModifyingVar(2, "Dmod") == false);
  REQUIRE(pkb->isStmtModifyingVar(2, "Cmod"));
  REQUIRE(pkb->isStmtModifyingVar(2, "Amod") == false);

  delete simpleParser;
  delete pkb;
}

TEST_CASE("Valid call procs with uses properly updated") {
  std::string program = "procedure A {"
                        "call B;"
                        "call C;"
                        "Amod = Ause;"
                        "}"
                        "procedure B {"
                        "call D;"
                        "Bmod = Buse;"
                        "}"
                        "procedure C {"
                        "Cmod = Cuse;"
                        "}"
                        "procedure D {"
                        "Dmod = Duse;"
                        "}";
  auto *pkb = createPKB();
  auto simpleParser = createSimpleParser(pkb, program);

  REQUIRE(simpleParser->parse());
  callDesignAbstractor(pkb);

  REQUIRE(pkb->isProcUsingVar("A", "Ause"));
  REQUIRE(pkb->isProcUsingVar("A", "Buse"));
  REQUIRE(pkb->isProcUsingVar("A", "Cuse"));
  REQUIRE(pkb->isProcUsingVar("A", "Duse"));

  REQUIRE(pkb->isStmtUsingVar(1, "Buse"));
  REQUIRE(pkb->isStmtUsingVar(1, "Duse"));
  REQUIRE(pkb->isStmtUsingVar(1, "Cuse") == false);
  REQUIRE(pkb->isStmtUsingVar(1, "Ause") == false);

  REQUIRE(pkb->isStmtUsingVar(2, "Buse") == false);
  REQUIRE(pkb->isStmtUsingVar(2, "Duse") == false);
  REQUIRE(pkb->isStmtUsingVar(2, "Cuse"));
  REQUIRE(pkb->isStmtUsingVar(2, "Ause") == false);

  delete simpleParser;
  delete pkb;
}

////TODO:: Update test when pkb method updated
TEST_CASE("Pattern correctly added") {
  std::string program = "procedure A {"
                        "a = b+c*d+(e/7);"
                        "while( a+c >= c*d+e ){"
                        " a= a+1;"
                        "}"
                        "}";
  auto *pkb = createPKB();
  auto simpleParser = createSimpleParser(pkb, program);

  REQUIRE(simpleParser->parse());
  callDesignAbstractor(pkb);

  auto actualPatterns = pkb->getAllIfWhilePatterns();

  std::vector<std::pair<int, std::string>> expectedPatterns = {
      {1, " b c d * + e 7 / + "},
      {3, " a 1 + "},
  };

  REQUIRE(actualPatterns.size() == expectedPatterns.size());

  for (auto p: expectedPatterns) {
    REQUIRE(actualPatterns.count(p.first) != 0);
    std::string expectedPostFix = p.second;
    REQUIRE(actualPatterns[p.first].size() == 1);
    REQUIRE(actualPatterns[p.first].count(expectedPostFix) == 1);
  }

  delete simpleParser;
  delete pkb;
}

TEST_CASE("Pattern correctly added 2") {
  std::string program = "procedure example {\n"
                        "        while(      !((x*9-9*7>y) && ((x!=0) && (y!=1)))       ){"
                        "            a = c*(d+e); "
                        "        }\n"
                        "}\n";
  auto *pkb = createPKB();
  auto simpleParser = createSimpleParser(pkb, program);

  REQUIRE(simpleParser->parse());
  callDesignAbstractor(pkb);

  auto actualPatterns = pkb->getAllIfWhilePatterns();

  std::vector<std::pair<int, std::string>> expectedPatterns = {
      {2, " c d e + * "},
  };

  REQUIRE(actualPatterns.size() == expectedPatterns.size());

  for (auto p: expectedPatterns) {
    REQUIRE(actualPatterns.count(p.first) != 0);
    std::string expectedPostFix = p.second;
    REQUIRE(actualPatterns[p.first].size() == 1);
    //for(auto str:actualPatterns[p.first]) std::cout << str << endl;
    REQUIRE(actualPatterns[p.first].count(expectedPostFix) == 1);
  }

  delete simpleParser;
  delete pkb;
}

TEST_CASE("Pattern correctly added 3") {
  std::string program = "procedure example {\n"
                        "        while(      !((x*9-9*7>y) && ((x!=0) && (y!=1)))       ){"
                        "            a = ((u1+1)*(u2+2))*(u3+3); "
                        "        }\n"
                        "}\n";
  auto *pkb = createPKB();
  auto simpleParser = createSimpleParser(pkb, program);

  REQUIRE(simpleParser->parse());
  callDesignAbstractor(pkb);

  auto actualPatterns = pkb->getAllIfWhilePatterns();

  std::vector<std::pair<int, std::string>> expectedPatterns = {
      {2, " u1 1 + u2 2 + * u3 3 + * "},

  };

  REQUIRE(actualPatterns.size() == expectedPatterns.size());

  for (auto p: expectedPatterns) {
    REQUIRE(actualPatterns.count(p.first) != 0);
    std::string expectedPostFix = p.second;
    REQUIRE(actualPatterns[p.first].size() == 1);
//    for (auto str: actualPatterns[p.first]) std::cout << str << endl;
    REQUIRE(actualPatterns[p.first].count(expectedPostFix) == 1);
  }

  delete simpleParser;
  delete pkb;
}

TEST_CASE("Pattern correctly added 4") {
  std::string program = "procedure example {\n"
                        "        while(      !((x*9-9*7>y) && ((x!=0) && (y!=1)))       ){"
                        "            a = a/1-3; "
                        "        }\n"
                        "}\n";
  auto *pkb = createPKB();
  auto simpleParser = createSimpleParser(pkb, program);

  REQUIRE(simpleParser->parse());
  callDesignAbstractor(pkb);

  auto actualPatterns = pkb->getAllIfWhilePatterns();

  std::vector<std::pair<int, std::string>> expectedPatterns = {
      {2, " a 1 / 3 - "},

  };

  REQUIRE(actualPatterns.size() == expectedPatterns.size());

  for (auto p: expectedPatterns) {
    REQUIRE(actualPatterns.count(p.first) != 0);
    std::string expectedPostFix = p.second;
    REQUIRE(actualPatterns[p.first].size() == 1);
//    for (auto str: actualPatterns[p.first]) std::cout << str << endl;
    REQUIRE(actualPatterns[p.first].count(expectedPostFix) == 1);
  }

  delete simpleParser;
  delete pkb;
}

TEST_CASE("Pattern correctly added 5") {
  std::string program = "procedure example {\n"
                        "        while(      !((x*9-9*7>y) && ((x!=0) && (y!=1)))       ){"
                        "            a=x;"
                        "        }\n"
                        "}\n";
  auto *pkb = createPKB();
  auto simpleParser = createSimpleParser(pkb, program);

  REQUIRE(simpleParser->parse());
  callDesignAbstractor(pkb);

  auto actualPatterns = pkb->getAllIfWhilePatterns();

  std::vector<std::pair<int, std::string>> expectedPatterns = {
      {2, " x "},

  };

  REQUIRE(actualPatterns.size() == expectedPatterns.size());

  for (auto p: expectedPatterns) {
    REQUIRE(actualPatterns.count(p.first) != 0);
    std::string expectedPostFix = p.second;
    REQUIRE(actualPatterns[p.first].size() == 1);
//    for (auto str: actualPatterns[p.first]) std::cout << str << endl;
    REQUIRE(actualPatterns[p.first].count(expectedPostFix) == 1);
  }

  delete simpleParser;
  delete pkb;
}

TEST_CASE("Pattern correctly added 6") {
  std::string program = "procedure example {\n"
                        "        while(      !((x*9-9*7>y) && ((x!=0) && (y!=1)))       ){"
                        "            a=(a+b)*cd*(e+f)/h*i*(j+k)-l*m/(n+o+p);"
                        "        }\n"
                        "}\n";
  auto *pkb = createPKB();
  auto simpleParser = createSimpleParser(pkb, program);

  REQUIRE(simpleParser->parse());
  callDesignAbstractor(pkb);

  auto actualPatterns = pkb->getAllIfWhilePatterns();

  std::vector<std::pair<int, std::string>> expectedPatterns = {
      {2, " a b + cd * e f + * h / i * j k + * l m * n o + p + / - "},

  };

  REQUIRE(actualPatterns.size() == expectedPatterns.size());

  for (auto p: expectedPatterns) {
    //REQUIRE(actualPatterns.count(p.first) != 0);
    std::string expectedPostFix = p.second;
    //REQUIRE(actualPatterns[p.first].size() == 1);
//    for (auto str: actualPatterns[p.first]) std::cout << str << endl;
    //REQUIRE(actualPatterns[p.first].count(expectedPostFix) == 1);
  }

  delete simpleParser;
  delete pkb;
}

TEST_CASE("Affects test 1 (lec notes pg 24) temp") {
  std::string program = "procedure p {\n"
                        "  x = 1;\n" //1
                        "  y = 1;\n" //2
                        "  z = y;\n" //3
                        "  call q;\n" //4
                        "  z = x + y + z;\n" //5
                        "}\n"
                        "\n"
                        "procedure q {\n"
                        "  x = 1;\n" //6
                        "  t = 1;\n" // 7
                        "  if ( z> 0 ) then {\n" //8
                        "    t = x+1;\n" //9
                        "  }\n"
                        "  else{\n"
                        "    y = z + x ;\n" //11
                        "  }\n"
                        "  x = t+1;\n" //12
                        "}";

  auto *pkb = createPKB();
  auto simpleParser = createSimpleParser(pkb, program);

  REQUIRE(simpleParser->parse());
  callDesignAbstractor(pkb);

  auto cfg1 = pkb->getCFG("p");
  auto cfg2 = pkb->getCFG("q");

  REQUIRE(!cfg1->isAffects(1, 5));
  REQUIRE(!cfg1->isAffects(2, 5));
  REQUIRE(cfg1->isAffects(3, 5));

  // TODO:: update modifies method after pkb implemented and make sure this passes

  REQUIRE(cfg2->isAffects(7, 11));

  delete pkb;
  delete simpleParser;

}

TEST_CASE("Affects test 2 nesting (lec notes ) page 27 temp") {
  std::string program = "procedure second {\n"
                        "    x = 0   ;\n" //1
                        "    i = 5  ;\n" //2
                        "    while( i!=0 ){\n" //3
                        "        x = x + 2*y;\n" //4
                        "        call third;\n" //5
                        "        i = i + 1;\n" // 6
                        "    }\n"
                        "    if ( x==1 ) then {\n"// 7
                        "        x = x+1;\n" // 8
                        "    }else{\n"
                        "        z = 1 ;\n" // 9
                        "    }\n"
                        "    z = z + x + i;\n" // 10
                        "    y = z + 2;\n" // 11
                        "    x= x*y+z;\n" // 12
                        "}\n"
                        "procedure third{\n"
                        "    z = 5  ;\n" // 13
                        "    v = z;\n" // 14
                        "    print v;\n" // 15
                        "}";

  auto *pkb = createPKB();
  auto simpleParser = createSimpleParser(pkb, program);

  REQUIRE(simpleParser->parse());
  callDesignAbstractor(pkb);

  auto cfg1 = pkb->getCFG("second");
  auto cfg2 = pkb->getCFG("third");

  REQUIRE(cfg1->isAffects(1, 4));
  // while loop
  REQUIRE(cfg1->isAffects(4, 4));
  REQUIRE(cfg1->isAffects(6, 6));
  REQUIRE(cfg1->isAffects(4, 10));
  REQUIRE(cfg1->isAffects(8, 10));
  REQUIRE(cfg1->isAffects(9, 10));

  REQUIRE(cfg1->isAffects(1, 10));
  REQUIRE(cfg1->isAffects(10, 11));
  REQUIRE(!cfg1->isAffects(1, 11)); // shud be affects *


  REQUIRE(cfg1->isAffectsStar(10, 11));
  REQUIRE(cfg1->isAffectsStar(1, 11));
  REQUIRE(cfg1->isAffectsStar(1, 4));
  REQUIRE(cfg1->isAffectsStar(1, 11));

  REQUIRE(cfg1->isAffectsStar(10, 11));
  REQUIRE(cfg1->isAffectsStar(1, 11));
  REQUIRE(cfg1->isAffectsStar(1, 4));
  REQUIRE(cfg1->isAffectsStar(1, 11));

  REQUIRE(!cfg1->isAffects(9, 12));
  REQUIRE(cfg1->isAffectsStar(9, 12));

  delete pkb;
  delete simpleParser;

}

TEST_CASE("Affects test 3 nesting  if inside while") {
  std::string program = "procedure second {\n"
                        "    x = u  ;\n" // 1
                        "    i = 5 ;\n" // 2
                        "    while( i!=0 ){\n" // 3
                        "    if ( x==1 ) then {\n" // 4
                        "        y = x+1;\n" // 5
                        "    }else{\n"
                        "        x = y+1;\n" // 6
                        "    }\n"
                        "    }\n"
                        "    if ( x==1 ) then {\n" // 7
                        "        y = x+1;\n" // 8
                        "    }else{\n"
                        "        x = y+1;\n" // 9
                        "    }\n"
                        "}\n";

  auto *pkb = createPKB();
  auto simpleParser = createSimpleParser(pkb, program);

  REQUIRE(simpleParser->parse());
  callDesignAbstractor(pkb);

  auto cfg1 = pkb->getCFG("second");

  REQUIRE(!cfg1->isAffects(8, 9));
  REQUIRE(cfg1->isAffects(6, 5));
  REQUIRE(cfg1->isAffects(5, 6));

  REQUIRE(cfg1->isAffects(6, 8));
  REQUIRE(cfg1->isAffects(5, 9));
  REQUIRE(cfg1->isAffects(1, 8));

  delete pkb;
  delete simpleParser;

}

TEST_CASE("Affects* test 1 lec notes page 28") {
  std::string program = "procedure second {\n"
                        "    x = u  ;\n" // 1
                        "    i = 5 ;\n" // 2
                        "    while( i!=0 ){\n" // 3
                        "    if ( x==1 ) then {\n" // 4
                        "        y = x+1;\n" // 5
                        "    }else{\n"
                        "        x = y+1;\n" // 6
                        "    }\n"
                        "    }\n"
                        "    if ( x==1 ) then {\n" // 7
                        "        y = x+1;\n" // 8
                        "    }else{\n"
                        "        x = y+1;\n" // 9
                        "    }\n"
                        "}\n";

  auto *pkb = createPKB();
  auto simpleParser = createSimpleParser(pkb, program);

  REQUIRE(simpleParser->parse());
  callDesignAbstractor(pkb);

  auto cfg1 = pkb->getCFG("second");

  REQUIRE(!cfg1->isAffects(8, 9));
  REQUIRE(cfg1->isAffects(6, 5));
  REQUIRE(cfg1->isAffects(5, 6));

  REQUIRE(cfg1->isAffects(6, 8));
  REQUIRE(cfg1->isAffects(5, 9));
  REQUIRE(cfg1->isAffects(1, 5));
  REQUIRE(cfg1->isAffects(1, 8));

  REQUIRE(!cfg1->hasNext(8));
  REQUIRE(!cfg1->hasNext(9));
  REQUIRE(cfg1->hasNext(7));

  REQUIRE(!cfg1->hasAffects(8));
  REQUIRE(!cfg1->hasAffects(9));
  REQUIRE(cfg1->hasAffects(5));
  REQUIRE(cfg1->hasAffects(6));

  delete pkb;
  delete simpleParser;

}

TEST_CASE("Affects* test 2 corner case") {
  std::string program = "procedure second {\n"
                        "    while( i!=0 ){\n" // 1
                        "        a = b;\n" // 2
                        "        b = c;\n" // 3
                        "        c = a;\n" // 4
                        "    }\n"
                        "}\n";

  auto *pkb = createPKB();
  auto simpleParser = createSimpleParser(pkb, program);

  REQUIRE(simpleParser->parse());
  callDesignAbstractor(pkb);

  auto cfg1 = pkb->getCFG("second");

  REQUIRE(cfg1->isAffects(2, 4));
  REQUIRE(cfg1->isAffects(3, 2));
  REQUIRE(cfg1->isAffects(4, 3));

  REQUIRE(!cfg1->isAffects(2, 3));
  REQUIRE(cfg1->isAffectsStar(2, 3));

  REQUIRE(!cfg1->isAffects(4, 2));
  REQUIRE(cfg1->isAffectsStar(4, 2));

  delete pkb;
  delete simpleParser;

}

TEST_CASE("Affects* test 3 corner case") {
  std::string program = "procedure second {\n"

                        "    while( i!=0 ){\n" // 1
                        "        a = b;\n" // 2
                        "        b = c;\n" // 3
                        "        c = a;\n" // 4
                        "    }\n"
                        "}\n";

  auto *pkb = createPKB();
  auto simpleParser = createSimpleParser(pkb, program);

  REQUIRE(simpleParser->parse());
  callDesignAbstractor(pkb);

  auto cfg1 = pkb->getCFG("second");

  REQUIRE(cfg1->isAffects(2, 4));
  REQUIRE(cfg1->isAffects(3, 2));
  REQUIRE(cfg1->isAffects(4, 3));

  REQUIRE(!cfg1->isAffects(2, 3));
  REQUIRE(cfg1->isAffectsStar(2, 3));

  REQUIRE(!cfg1->isAffects(4, 2));
  REQUIRE(cfg1->isAffectsStar(4, 2));

  delete pkb;
  delete simpleParser;

}

TEST_CASE("Affects* test loop") {
  std::string program = "procedure second {\n"
                        "    x = 1  ;\n" // 1
                        "    i = 5 ;\n" // 2
                        "    if ( x==1 ) then {\n" // 3
                        "        y = x+1;\n" // 4
                        "    }else{\n"
                        "        x = y+1;\n" // 5
                        "    }\n"
                        "    if ( x==1 ) then {\n" // 6
                        "        y = x+1;\n" // 7
                        "    }else{\n"
                        "        x = y+1;\n" // 8
                        "    }"
                        "    y = x;" // 9
                        " }\n";

  auto *pkb = createPKB();
  auto simpleParser = createSimpleParser(pkb, program);

  REQUIRE(simpleParser->parse());
  callDesignAbstractor(pkb);

  auto cfg1 = pkb->getCFG("second");

  REQUIRE(cfg1->isAffects(1, 4));
  REQUIRE(cfg1->isAffects(1, 7));
  REQUIRE(cfg1->isAffects(1, 9));

  REQUIRE(cfg1->isAffects(5, 7));
  REQUIRE(cfg1->isAffects(5, 9));

  REQUIRE(cfg1->isAffects(4, 8));
  REQUIRE(!cfg1->isAffects(4, 5));

  REQUIRE(!cfg1->isAffects(1, 8));
  REQUIRE(cfg1->isAffectsStar(1, 8));

  std::vector<int> expectedAffectList = {4, 7, 9};
  auto actualAffectList = cfg1->getAffectsList(1);

  REQUIRE(actualAffectList.size() == expectedAffectList.size());

  for (int expectedAffected: expectedAffectList) {
    REQUIRE(actualAffectList.count(expectedAffected) != 0);
  }

  std::vector<int> expectedAffectStarList = {4, 7, 8, 9};
  auto actualAffectStarList = cfg1->getAffectsStarList(1);

  REQUIRE(actualAffectStarList.size() == expectedAffectStarList.size());

  for (int expectedAffectStar: expectedAffectStarList) {
    REQUIRE(actualAffectStarList.count(expectedAffectStar) != 0);
  }

  delete pkb;
  delete simpleParser;

}

TEST_CASE("Affects* test 1 loop blocker") {
  std::string program = "procedure second {\n"
                        "    x = 1  ;\n" // 1
                        "    i = 5 ;\n" // 2
                        "    if ( x==1 ) then {\n" // 3
                        "      call secant;" //4
                        "        y = x+1;\n" // 5
                        "    }else{\n"
                        "        x = y+1;\n" // 6
                        "    }\n"
                        "    if ( x==1 ) then {\n" // 7
                        "        y = x+1;\n" // 8
                        "    }else{\n"
                        "        x = y+1;\n" // 9
                        "    }"
                        //   "    y = x;" // 10
                        " }\n"
                        "procedure secant {\n"
                        "    x = 1  ;\n" // 11
                        " }\n";

  auto *pkb = createPKB();
  auto simpleParser = createSimpleParser(pkb, program);

  REQUIRE(simpleParser->parse());
  callDesignAbstractor(pkb);

  auto cfg1 = pkb->getCFG("second");

  REQUIRE(!cfg1->isAffects(1, 5));

  delete pkb;
  delete simpleParser;

}

TEST_CASE("cfg bip") {
  std::string program = "procedure second {\n"
                        "    x = 1  ;\n" // 1
                        "    i = 5 ;\n" // 2
                        "    if ( x==1 ) then {\n" // 3
                        "      call secant;" //4
                        "        y = x+1;\n" // 5
                        "    }else{\n"
                        "        x = y+1;\n" // 6
                        "    }\n"
                        "    if ( x==1 ) then {\n" // 7
                        "        y = x+1;\n" // 8
                        "    }else{\n"
                        "        x = y+1;\n" // 9
                        "    }"
                        " }\n"
                        "procedure secant {\n"
                        "    y = 1 +x  ;\n" // 10
                        " }\n"
                        "procedure third {\n"
                        "    call second;\n" // 11
                        "    a = 1;           " //12
                        " }\n";

  auto *pkb = createPKB();
  auto simpleParser = createSimpleParser(pkb, program);

  REQUIRE(simpleParser->parse());
  callDesignAbstractor(pkb);
  //simpleParser->parse();

  auto cfgB = pkb->getCFGBip(); //This line mem err -> depends on uninitialized value / probably
  REQUIRE(cfgB->isNextStar(1, 10)); // Leads to this segfault
  REQUIRE(cfgB->isNextStar(11, 1));
  REQUIRE(cfgB->isNextStar(8, 12));
  REQUIRE(cfgB->isNextStar(9, 12));
  // should be true


  // TODO:: pass this
  REQUIRE(cfgB->isNextStar(1, 2));
  REQUIRE(cfgB->isNextStar(11, 10));
  REQUIRE(cfgB->isNextStar(10, 5));
  REQUIRE(cfgB->isNextStar(4, 10));
  REQUIRE(!cfgB->isNextStar(8, 10));

  REQUIRE(!cfgB->isNextBip(1, 3));
  REQUIRE(cfgB->isNextBip(1, 2));
  REQUIRE(cfgB->isNextBip(4, 10));
  REQUIRE(cfgB->isNextBip(10, 5));

  REQUIRE(cfgB->isNextBip(8, 12));
  REQUIRE(cfgB->isNextBip(9, 12));

  std::vector<int> expectedNextBipOf7 = {8, 9};
  auto actualNextBipOf7 = cfgB->getNextBipList(7);

  REQUIRE(expectedNextBipOf7.size() == actualNextBipOf7.size());

  for (int expected: expectedNextBipOf7) {
    REQUIRE(actualNextBipOf7.count(expected));
  }

  std::vector<int> expectedNextBipOf11 = {1};
  auto actualNextBipOf11 = cfgB->getNextBipList(11);

  REQUIRE(expectedNextBipOf11.size() == actualNextBipOf11.size());

  for (int expected: expectedNextBipOf11) {
    REQUIRE(actualNextBipOf11.count(expected));
  }

  auto actualNextBipOf12 = cfgB->getNextBipList(12);
  REQUIRE(actualNextBipOf12.size() == 0);

  delete simpleParser;
  delete pkb;
  delete cfgB;

}

TEST_CASE("cfg bip edge") {
  std::string program = "procedure fourth {\n"
                        "    a1 = 1;\n"
                        "    b1 = 1;\n"
                        "    c1 = 1;\n"
                        "    while (x==1){\n" //4
                        "        UnaffectedByInnerNest = 1;\n" //5
                        "        AffectedByInnerNest = 1 + AffectedByInnerNest;\n" //6
                        "        if (x == 2) then {\n" //7
                        "            d1 = a1;\n" // 8
                        "            while (x==4) {\n" //9
                        "                e1 = f1;\n" //10
                        "                f1 = 1;\n"  //11
                        "            }\n"
                        "        } else {\n"
                        "            e1 = d1;\n" //12
                        "            UnaffectedByInnerNest = 1;\n" //13
                        "            if ( x==5 ) then {\n" //14
                        "                AffectedByInnerNest = 1;\n" //15
                        "            } else{\n"
                        "                AffectedByInnerNest = 1;\n" //16
                        "            }\n"
                        "        }\n"
                        "    }\n"
                        "}";

  auto *pkb = createPKB();
  auto simpleParser = createSimpleParser(pkb, program);



  REQUIRE_NOTHROW(simpleParser->parse()); callDesignAbstractor(pkb);
  //simpleParser->parse();



  auto cfgB = pkb->getCFG("fourth"); //This line mem err -> depends on uninitialized value / probably
  auto cfgBB = pkb->getCFGBip(); //This line mem err -> depends on uninitialized value / probably


  REQUIRE(cfgB->isNextStar(6, 6));
  REQUIRE(cfgB->isAffects(6, 6));





  delete simpleParser;
  delete pkb;
  delete cfgB;

}

TEST_CASE("cfg bip edge simple") {
  std::string program = "procedure fourth {\n"

                        "    while (x==1){\n"
                        "        AffectedByInnerNest = 1 + AffectedByInnerNest;\n"
                        "}\n"
                        "}";

  auto *pkb = createPKB();
  auto simpleParser = createSimpleParser(pkb, program);

  REQUIRE_NOTHROW(simpleParser->parse()); callDesignAbstractor(pkb);
  //simpleParser->parse();

  auto cfgB = pkb->getCFG("fourth"); //This line mem err -> depends on uninitialized value / probably
  REQUIRE(cfgB->isNextStar(2, 2));
  REQUIRE(cfgB->isAffectsStar(2, 2));
  REQUIRE(cfgB->isAffects(2, 2));

  delete simpleParser;
  delete pkb;
  delete cfgB;

}

TEST_CASE("cfg bip edge simple debug") {
  std::string program = "procedure first{\n"
                        "    notAffecting = 5;\n"
                        "\n"
                        "    a = 1;\n"
                        "    b = 2;\n"
                        "    c = 3;\n"
                        "    d = 4;\n"
                        "    e = 5;\n"
                        "\n"
                        "    read d;\n"
                        "    while( x==1 ){\n"
                        "        if (x==2) then {\n"
                        "          read e;\n"
                        "          z = a;\n"
                        "          y = y;\n"
                        "          a = 1;\n"
                        "          call second;\n"
                        "          z = b;\n"
                        "          z = c;\n"
                        "        } else{\n"
                        "            z = e;\n"
                        "            a = 1;\n"
                        "            c = 1;\n"
                        "        }\n"
                        "    }\n"
                        "\n"
                        "    a = 1;\n"
                        "    b = 2;\n"
                        "    c = 3;\n"
                        "    d = 4;\n"
                        "    e = 5;\n"
                        "\n"
                        "    if (x==3) then {\n"
                        "         b = 1+c;\n"
                        "         c = 1+b;\n"
                        "         while(x==4){\n"
                        "            b = z;\n"
                        "            if (x==5) then {\n"
                        "                c = b+c;\n"
                        "            } else {\n"
                        "                c = b+c;\n"
                        "            }\n"
                        "         }\n"
                        "         z = a;\n"
                        "\n"
                        "    } else {\n"
                        "        b = 1;\n"
                        "        z = c;\n"
                        "    }\n"
                        "    z = b;\n"
                        "}\n"
                        "\n"
                        "procedure second{\n"
                        "    call third;\n"
                        "}\n"
                        "\n"
                        "procedure third{\n"
                        "    c = b;\n"
                        "    b = c;\n"
                        "}\n"
                        "\n"
                        "procedure fourth {\n"
                        "    a1 = 1;\n"
                        "    b1 = 1;\n"
                        "    c1 = 1;\n"
                        "    while (x==1){\n"
                        "        UnaffectedByInnerNest = 1;\n"
                        "        AffectedByInnerNest = 1 + AffectedByInnerNest;\n"
                        "        if (x == 2) then {\n"
                        "            d1 = a1;\n"
                        "            while (x==4) {\n"
                        "                e1 = f1;\n"
                        "                f1 = 1;\n"
                        "            }\n"
                        "        } else {\n"
                        "            e1 = d1;\n"
                        "            UnaffectedByInnerNest = 1;\n"
                        "            if ( x==5 ) then {\n"
                        "                AffectedByInnerNest = 1;\n"
                        "            } else{\n"
                        "                AffectedByInnerNest = 1;\n"
                        "            }\n"
                        "        }\n"
                        "    }\n"
                        "}";

  auto *pkb = createPKB();
  auto simpleParser = createSimpleParser(pkb, program);

  REQUIRE_NOTHROW(simpleParser->parse()); callDesignAbstractor(pkb);
  //simpleParser->parse();

  auto cfgB = pkb->getCFGBip(); //This line mem err -> depends on uninitialized value / probably
  REQUIRE(cfgB->getNextList()[45].count(46)==1);
  REQUIRE(cfgB->isNextStar(39, 36));
  REQUIRE(cfgB->isNextStar(37, 34));
  REQUIRE(cfgB->isNextBip(14, 37));
  REQUIRE(cfgB->isNextStar(37, 38));
  REQUIRE(cfgB->isNextStar(37, 38)); // should be true
  REQUIRE(cfgB->isAffects(3, 15)==false);
  REQUIRE(cfgB->isNextStar(37, 38)); // should be true
  REQUIRE(cfgB->isNextStar(3, 39));

  REQUIRE(cfgB->isNextBip(39, 15));
  REQUIRE(cfgB->getNextBipList(39).size()==1);
  REQUIRE(cfgB->getNextBipList(39).count(15));


  delete simpleParser;
  delete pkb;
  delete cfgB;

}

TEST_CASE("cfg bip edge simple debug 2") {
  std::string program = "procedure first{\n"
                        "    a = 1;\n"
                        "    call second;\n"
                        "    a = 1;\n"
                        "}\n"
                        "\n"
                        "procedure second{\n"
                        "    call third;\n"
                        "}\n"
                        "\n"
                        "procedure third{\n"
                        "    c = b;\n"
                        "    b = c;\n"
                        "}\n";


  auto *pkb = createPKB();
  auto simpleParser = createSimpleParser(pkb, program);

  REQUIRE_NOTHROW(simpleParser->parse()); callDesignAbstractor(pkb);
  //simpleParser->parse();

  auto cfgB = pkb->getCFGBip();
  auto cfg = pkb->getCFG("first");
  REQUIRE(cfgB->isNextStar(1, 3));

  REQUIRE(cfgB->isNextStar(1, 4));

  REQUIRE(cfgB->isNextStar(1, 2));

  REQUIRE(!cfgB->isAffects(1000000, 2000000));
  REQUIRE(!cfgB->isAffectsStar(1000000,200000));

  REQUIRE(!cfg->isAffects(1000000, 2000000));
  REQUIRE(!cfg->hasAffects(1000000));
  REQUIRE(cfg->getAffectsList(10000).size()==0);

  auto nextStarBipsOf1 = cfgB->getNextStarList(1);

  std::vector<int> expectedNextStar = {2,3,4,5,6};
  nextStarBipsOf1 = cfgB->getNextStarList(1);

  REQUIRE(expectedNextStar.size()==nextStarBipsOf1.size());
  for(int num:expectedNextStar){
    REQUIRE(nextStarBipsOf1.count(num)!=0);
  }



  delete simpleParser;
  delete pkb;
  delete cfgB;

}

TEST_CASE("cfg bip edge simple debug if else terminal last") {
  std::string program = "procedure first{\n"
                        "    c = 1;  "   //1
                        "    call second;\n" //2
                        "}\n"
                        "\n"
                        "procedure second{\n"
                        "    call third;\n" //3
                        "}\n"
                        "\n"
                        "procedure third{\n"
                        "    if (x==3) then {\n" //4
                        "         b = 1+c;\n" //5
                        "\n"
                        "    } else {\n"
                        "        b = 1+c;\n" //6
                        "    }\n"

                        "}\n";



  auto *pkb = createPKB();
  auto simpleParser = createSimpleParser(pkb, program);

  REQUIRE_NOTHROW(simpleParser->parse()); callDesignAbstractor(pkb);
  //simpleParser->parse();

  auto cfgB = pkb->getCFGBip(); //This line mem err -> depends on uninitialized value / probably
  REQUIRE(cfgB->isNextStar(1, 5));
  REQUIRE(cfgB->isAffects(1, 5));



  delete simpleParser;
  delete pkb;
  delete cfgB;

}

TEST_CASE("cfg bip edge random") {
  std::string program = "procedure first{\n"
                        "    c = 1;  "   //1
                        "}\n"
                        "\n"
                        "procedure second{\n"
                        "    c=1;\n" //3
                        "}\n";



  auto *pkb = createPKB();
  auto simpleParser = createSimpleParser(pkb, program);

  REQUIRE_NOTHROW(simpleParser->parse()); callDesignAbstractor(pkb);
  //simpleParser->parse();

  auto cfgB = pkb->getCFGBip(); //This line mem err -> depends on uninitialized value / probably



  delete simpleParser;
  delete pkb;
  delete cfgB;

}