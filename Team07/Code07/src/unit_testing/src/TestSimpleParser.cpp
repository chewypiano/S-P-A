//
// Created by Kamil Gimik on 3/9/21.
//

#include "Rules.h"
#include "catch.hpp"
#include "SimpleParser.h"

SimpleParser *createSimpleParser(std::string program) {
  auto pkb = new PKB();
  return new SimpleParser(pkb, program);
}

 //Newer test cases with Exception handling

TEST_CASE("PopulateToken Throws Exception for Incorrect TokenType") {
  std::string program = "= procedure Example {";
  SimpleParser *simpleParser = createSimpleParser(program);
  REQUIRE_THROWS_WITH(simpleParser->parse(), Catch::Matchers::Contains(
      "SimpleParser: Expected Name token, not Equal"));
  delete simpleParser;
}

TEST_CASE("Parse Does Not Throw Exception for Correct TokenTypes") {
  std::string program = "procedure Example {"
                        "a = 1;"
                        "}";
  SimpleParser *simpleParser = createSimpleParser(program);
  REQUIRE_NOTHROW(simpleParser->parse());
  delete simpleParser;
}

TEST_CASE("PopulateToken for ParseProcedure Throws Exception if not in allowed set") {
  std::string program = "9 procedure Example {";
  SimpleParser *simpleParser = createSimpleParser(program);
  REQUIRE_THROWS_WITH(simpleParser->parse(), Catch::Matchers::Contains(
      "SimpleParser: Expected Name token, not Number"));
  delete simpleParser;
}

TEST_CASE("ParseLine Throws Exception if not in allowed set") {
  std::string program = "procedure Example {"
                        "9 = 5;"
                        "}";
  SimpleParser *simpleParser = createSimpleParser(program);
  REQUIRE_THROWS_WITH(simpleParser->parse(), Catch::Matchers::Contains(
      "SimpleParser: Line begins with invalid TokenType: Number"));
  delete simpleParser;
}

TEST_CASE("PopulateToken Throws Exception for Incorrect TokenValue") {
  std::string program = "procedur Example {";
  SimpleParser *simpleParser = createSimpleParser(program);
  REQUIRE_THROWS_WITH(simpleParser->parse(), Catch::Matchers::Contains(
      "SimpleParser: Expected procedure but received procedur"));
  delete simpleParser;
}

TEST_CASE("PopulateToken Does Not Throw Exception for Correct TokenValues and Types") {
  std::string program = "procedure Example {"
                        "a = 5;"
                        "b = 2;"
                        "}";
  SimpleParser *simpleParser = createSimpleParser(program);
  REQUIRE_NOTHROW(simpleParser->parse());
  delete simpleParser;
}

//:: TODO: Probably need segregate some of these tests as integration tests
TEST_CASE("ParseProcedure Throws Exception for Inc sorrect Syntax") {
  std::string program = "procedure {";
  SimpleParser *simpleParser = createSimpleParser(program);
  REQUIRE_THROWS_WITH(simpleParser->parse(), Catch::Matchers::Contains(
      "SimpleParser: Expected Name token, not LBrace"));
  delete simpleParser;
}

TEST_CASE("ParseProcedure Throws for Zero Stmts") {
  std::string program = "procedure Example {"
                        "}";
  SimpleParser *simpleParser = createSimpleParser(program);
  REQUIRE_THROWS_WITH(simpleParser->parse(), Catch::Matchers::Contains(
      "SimpleParser: Procedure not closed properly"));
  delete simpleParser;
}

TEST_CASE("ParseProcedure Throws for Zero Stmts in another procedure") {
  std::string program = "procedure Example {"
                        "a = 1;"
                        "}"
                        "procedure two {"
                        "}";
  SimpleParser *simpleParser = createSimpleParser(program);
  REQUIRE_THROWS_WITH(simpleParser->parse(), Catch::Matchers::Contains(
      "SimpleParser: Procedure not closed properly"));
  delete simpleParser;
}

TEST_CASE("ParseProcedure Throws for Zero Stmts in middle procedure") {
  std::string program = "procedure Example {"
                        "a = 1;"
                        "}"
                        "procedure two {"
                        "}"
                        "procedure C{"
                        "a = 1;"
                        "}";
  SimpleParser *simpleParser = createSimpleParser(program);
  REQUIRE_THROWS_WITH(simpleParser->parse(), Catch::Matchers::Contains(
      "SimpleParser: Procedure not closed properly"));
  delete simpleParser;
}

TEST_CASE("ParseProcedure Throws for 2 identically named procs") {
  std::string program = "procedure Example {"
                        "a = 1;"
                        "}"
                        "procedure Example{"
                        "a = b;"
                        "}"
                        "procedure C{"
                        "a = 1;"
                        "}";
  SimpleParser *simpleParser = createSimpleParser(program);
  REQUIRE_THROWS_WITH(simpleParser->parse(), Catch::Matchers::Contains(
      "SimpleParser: Two Procedures with same name error"));
  delete simpleParser;
}

TEST_CASE("ParseLine Throws Error for Invalid Start of Line") {
  std::string program = "procedure A {"
                        "5 = a + b;"
                        "}";
  SimpleParser *simpleParser = createSimpleParser(program);
  REQUIRE_THROWS_WITH(simpleParser->parse(), Catch::Matchers::Contains(
      "SimpleParser: Line begins with invalid TokenType: Number"));
  delete simpleParser;
}

TEST_CASE("ParseStmt Throws Error for Invalid Block Stmt Declaration") {
  // testing parseStmt for ability to throw error for if, while stmts
  std::string program = "procedure A {"
                        "While(a==b){"
                        "a = 1;"
                        "}"
                        "}";
  SimpleParser *simpleParser = createSimpleParser(program);
  REQUIRE_THROWS_WITH(simpleParser->parse(), Catch::Matchers::Contains(
      "SimpleParser: Expected while or if before LBracket, not: While"));
  delete simpleParser;
}

TEST_CASE("ParseWhile Throws Error for Invalid Block Stmt Declaration: no stmts in block") {
  // testing parseStmt for ability to throw error for if, while stmts
  std::string program = "procedure A {"
                        "while(a==b){"
                        "}"
                        "}";
  SimpleParser *simpleParser = createSimpleParser(program);
  REQUIRE_THROWS_WITH(simpleParser->parse(), Catch::Matchers::Contains(
      "SimpleParser: While must contain at least 1 stmt in its block"));
  delete simpleParser;
}

TEST_CASE("ParseStmt Throws Error for Invalid Complex Stmt Declaration") {
  // testing parseStmt for ability to throw error for Call, Print, Read stmts
  std::string program = "procedure B {"
                        "Call PROCEDURE;"
                        "}";
  SimpleParser *simpleParser = createSimpleParser(program);
  REQUIRE_THROWS_WITH(simpleParser->parse(), Catch::Matchers::Contains(
      "SimpleParser: Expected read, print or call before Variable Name, not: Call"));
  delete simpleParser;
}

TEST_CASE("Testing SimpleParser::parse") {
  std::string program = "procedure Example { print a; read abc;}";
  SimpleParser *simpleParser = createSimpleParser(program);
  REQUIRE_NOTHROW(simpleParser->parse());
  delete simpleParser;
}

TEST_CASE("Testing SimpleParser::parseExpression throws no error on valid expression") {
  std::string program = "procedure A {"
                        "a = a+b;"
                        "}";
  SimpleParser *simpleParser = createSimpleParser(program);
  REQUIRE_NOTHROW(simpleParser->parse());
  delete simpleParser;
}

TEST_CASE("Testing SimpleParser::parseExpression throws no error on complex valid expression") {
  std::string program = "procedure A {"
                        "a = (9+78*(a+b*(y+x)));"
                        "}";
  SimpleParser *simpleParser = createSimpleParser(program);
  REQUIRE_NOTHROW(simpleParser->parse());
  delete simpleParser;
}

TEST_CASE("ParseExpression invalid") {
  std::string program = "procedure A {"
                        "a = b + ;"
                        "}";
  SimpleParser *simpleParser = createSimpleParser(program);
  REQUIRE_THROWS_WITH(simpleParser->parse(), Catch::Matchers::Contains(
      "SimpleParser: Expected LBracket Name Number tokens, not Semicolon"));
  delete simpleParser;
}

TEST_CASE("ParseAssign invalid") {
  std::string program = "procedure A {"
                        "b + a;"
                        "}";
  SimpleParser *simpleParser = createSimpleParser(program);
  REQUIRE_THROWS_WITH(simpleParser->parse(), Catch::Matchers::Contains(
      "SimpleParser: Unexpected tokenType"));
  delete simpleParser;
}

TEST_CASE("Testing Example program") {
  std::string program = "procedure example {\n"
                        "        print b;\n"
                        "        print a;\n"
                        "        a=b*c-e;\n"
                        "}\n";
  auto simpleParser = createSimpleParser(program);
  REQUIRE(simpleParser->parse());
  delete simpleParser;
}

TEST_CASE("ParseConditionalExpressions valid") {
  std::string program = "procedure example {\n"
                        "        while(a==b){"
                        "            a = c*(d+e); "
                        "        }\n"
                        "}\n";
  auto simpleParser = createSimpleParser(program);
  REQUIRE_NOTHROW(simpleParser->parse());
  delete simpleParser;
}

TEST_CASE("ParseConditionalExpressions valid 2") {
  std::string program = "procedure example {\n"
                        "        while( (( a==b ) && (a==b )) || (a==b) ){"
                        "            a = c*(d+e); "
                        "        }\n"
                        "}\n";
  auto simpleParser = createSimpleParser(program);
  REQUIRE_NOTHROW(simpleParser->parse());
  delete simpleParser;
}

TEST_CASE("ParseConditionalExpressions valid 3") {
  std::string program = "procedure example {\n"
                        "        while( (a==b) && (c==d)  ){"
                        "            a = c*(d+e); "
                        "        }\n"
                        "}\n";
  auto simpleParser = createSimpleParser(program);
  REQUIRE_NOTHROW(simpleParser->parse());
  delete simpleParser;
}

TEST_CASE("ParseExpressions throws error for incorrect brackets 1") {
  std::string program = "procedure example {\n"
                        "         a = (d*c*(d+e); " // bracket not closed
                        "}\n";
  auto simpleParser = createSimpleParser(program);
  REQUIRE_THROWS_WITH(simpleParser->parse(), "SimpleParser: Expected LBrace token, not LBracket"); // thrown from stack
  delete simpleParser;
}

TEST_CASE("ParseExpressions throws error for incorrect brackets 2") {
  std::string program = "procedure example {\n"
                        "         a = c)*(d+e); " // bracket closed when not opened
                        "}\n";
  auto simpleParser = createSimpleParser(program);
  REQUIRE_THROWS_WITH(simpleParser->parse(), "SimpleParser: Bracket error, did not expect RBracket");
  delete simpleParser;
}

TEST_CASE("ParseExpressions throws error for incorrect brackets 3") {
  std::string program = "procedure example {\n"
                        "         a = z*(d/(c)*(d+e); " //  bracket not closed
                        "}\n";
  auto simpleParser = createSimpleParser(program);
  REQUIRE_THROWS_WITH(simpleParser->parse(), "SimpleParser: Expected LBrace token, not LBracket"); // thrown from stack
  delete simpleParser;
}

TEST_CASE("ParseExpressions throws error for incorrect brackets 4") {
  std::string program = "procedure example {\n"
                        "         a = (c)*(d)+e); " // bracket closed when not opened
                        "}\n";
  auto simpleParser = createSimpleParser(program);
  REQUIRE_THROWS_WITH(simpleParser->parse(), "SimpleParser: Bracket error, did not expect RBracket");
  delete simpleParser;
}

//// TODO:: (condExprs) && (condExprs) should not throw error
TEST_CASE("ParseConditionalExpressions valid 4") {
  std::string program = "procedure example {\n"
                        "        while(  ( (a==b) && (b==c) )  || (d==e)  ){"
                        "            a = c*(d+e); "
                        "        }\n"
                        "}\n";
  auto simpleParser = createSimpleParser(program);
  REQUIRE_NOTHROW(simpleParser->parse());
  delete simpleParser;
}

//// TODO:: ! (condExpr) && (condExpr) should not throw error
TEST_CASE("ParseConditionalExpressions valid 5") {
  std::string program = "procedure example {\n"
                        "        while(   !(!(a==b) && (b==c) ) ){"
                        "            a = c*(d+e); "
                        "        }\n"
                        "}\n";
  auto simpleParser = createSimpleParser(program);
  REQUIRE_THROWS_WITH(simpleParser->parse(), Catch::Matchers::Contains(
      "SimpleParser: Logical token must come between enclosed cond exprs"));
  delete simpleParser;
}

TEST_CASE("ParseConditionalExpression valid 3") {
  std::string program = "procedure example {\n"
                        "        while(  ( ( i ) ) < 2 - 2  + ((2) * 2 )  ){"
                        "            a = c*(d+e); "
                        "        }\n"
                        "}\n";
  auto simpleParser = createSimpleParser(program);
  REQUIRE_NOTHROW(simpleParser->parse());
  delete simpleParser;
}

TEST_CASE("ParseConditionalExpressions valid with ! 1") {
  std::string program = "procedure example {\n"
                        "        while( !(a==b) ){"
                        "            a = c*(d+e); "
                        "        }\n"
                        "}\n";
  auto simpleParser = createSimpleParser(program);
  REQUIRE_NOTHROW(simpleParser->parse());
  delete simpleParser;
}

TEST_CASE("ParseConditionalExpressions valid with ! 2") {
  std::string program = "procedure example {\n"
                        "        while( !(!(a==b)) ){"
                        "            a = c*(d+e); "
                        "        }\n"
                        "}\n";
  auto simpleParser = createSimpleParser(program);
  REQUIRE_NOTHROW(simpleParser->parse());
  delete simpleParser;
}

TEST_CASE("ParseConditionalExpressions valid with ! 4") {
  std::string program = "procedure example {\n"
                        "        while( ( (!(a==b))   || (!(b==c)) ) && (c==d) ){"
                        "            a = c*(d+e); "
                        "        }\n"
                        "}\n";
  auto simpleParser = createSimpleParser(program);
  REQUIRE_NOTHROW(simpleParser->parse());
  delete simpleParser;
}

TEST_CASE("ParseConditionalExpressions valid with ! 5") {
  std::string program = "procedure example {\n"
                        "        while( ! (! (! (a==b) ) )  ){"
                        "            a = c*(d+e); "
                        "        }\n"
                        "}\n";
  auto simpleParser = createSimpleParser(program);
  REQUIRE_NOTHROW(simpleParser->parse());
  delete simpleParser;
}

TEST_CASE("ParseConditionalExpressions invalid with wrongly placed ! 1") {
  std::string program = "procedure example {\n"
                        "        while( !(!(a==!b)) && !(c==d)  ){"
                        "            a = c*(d+e); "
                        "        }\n"
                        "}\n";
  auto simpleParser = createSimpleParser(program);
  REQUIRE_THROWS_WITH(simpleParser->parse(), Catch::Matchers::Contains(
      "SimpleParser: Negation Error. Can't have Equals , Relative before Negate"));
  delete simpleParser;
}
//
TEST_CASE("ParseConditionalExpressions invalid with wrongly placed ! 2") {
  std::string program = "procedure example {\n"
                        "        while( !((a==!(b))) && !(c==d)  ){"
                        "            a = !c*(d+e); "
                        "        }\n"
                        "}\n";
  auto simpleParser = createSimpleParser(program);
  REQUIRE_THROWS_WITH(simpleParser->parse(), Catch::Matchers::Contains(
      "SimpleParser: Negation Error. Can't have Equals , Relative before Negate"));
  delete simpleParser;
}
//
//// this should fail
TEST_CASE("ParseConditionalExpressions invalid with wrongly placed ! 3") {
  std::string program = "procedure example {\n"
                        "        while( !(a)==b  ){"
                        "            a = c*(d+e); "
                        "        }\n"
                        "}\n";
  auto simpleParser = createSimpleParser(program);
  REQUIRE_THROWS_WITH(simpleParser->parse(), Catch::Matchers::Contains(
      "SimpleParser: Negate declared wrongly"));
  delete simpleParser;
}

TEST_CASE("ParseConditionalExpressions invalid with wrongly placed ! 4") {
  std::string program = "procedure example {\n"
                        "        while( (!(!(a==b))) && !(c==d)  ){"
                        "            a = c*(d+e); "
                        "        }\n"
                        "}\n";
  auto simpleParser = createSimpleParser(program);
  REQUIRE_THROWS_WITH(simpleParser->parse(), Catch::Matchers::Contains(
      "SimpleParser: Expected LBracket token, not Negate"));
  delete simpleParser;
}

TEST_CASE("ParseConditionalExpressions invalid with wrongly placed ! 5") {
  std::string program = "procedure example {\n"
                        "        while(  ! (! (! (a==b) ) )  && (a>=b) ){"
                        "            a = c*(d+e); "
                        "        }\n"
                        "}\n";
  auto simpleParser = createSimpleParser(program);
  REQUIRE_THROWS_WITH(simpleParser->parse(), Catch::Matchers::Contains(
      "SimpleParser: Logical token must come between enclosed cond exprs"));
  delete simpleParser;
}

TEST_CASE("testing123") {
  std::string program2 = "procedure example {\n"
                         "        while( ((a==b)) && (c==d)  ){"
                         "            a = c*(d+e);"
                         "            b = 3 + a;"
                         "        }\n"
                         "        print c;"
                         "}\n";
  std::string program = "procedure Example { a = b; b = 3; }";
  auto simpleParser = createSimpleParser(program);
  REQUIRE_NOTHROW(simpleParser->parse());
  delete simpleParser;
}

TEST_CASE("ParseIf valid 1") {
  std::string program = "procedure example {\n"
                        "        if ( (a==b) && (c==d)  ) then {"
                        "            a = c*(d+e); "
                        "        }\n"
                        "        else {"
                        "            a = c*(d+e); "
                        "            d= c*(d+e); "
                        "        }\n"
                        "}\n";
  auto simpleParser = createSimpleParser(program);
  REQUIRE_NOTHROW(simpleParser->parse());
  delete simpleParser;
}

TEST_CASE("ParseIf valid 2") {
  std::string program = "procedure example {\n"
                        "        if ( (a==b) && (c==d)  ) then {"
                        "            a = c*(d+e); "
                        "            if ( (a==b) && (c==d)  ) then {"
                        "                  a = c*(d+e); "
                        "            }\n"
                        "            else {"
                        "              while (a==b) {"
                        "                     b = c;"
                        "              } "
                        "              a = c*(d+e); "
                        "              d= c*(d+e); "
                        "            } \n"
                        "            a = c*(d+e); "
                        " } \n"


                        "        else {"
                        "            a = c*(d+e); "
                        "            d= c*(d+e); "
                        "        }\n"
                        "}\n";
  auto simpleParser = createSimpleParser(program);
  REQUIRE_NOTHROW(simpleParser->parse());
  delete simpleParser;
}

TEST_CASE("ParseIf valid 3") {
  std::string program = "procedure example {\n"
                        "        if ( (a==b) && (( (c==d)||(a==b) ) && (a==b)) ) then  {"
                        "            a = c*(d+e); "
                        "        }\n"
                        "        else {"
                        "            a = c*(d+e); "
                        "            d= c*(d+e); "
                        "        }\n"
                        "}\n";
  auto simpleParser = createSimpleParser(program);
  REQUIRE_NOTHROW(simpleParser->parse());
  delete simpleParser;
}

TEST_CASE("Test nesting") {
  std::string program = "procedure nestedWhileAsf{\n"
                        "    a = 2;\n"
                        "    b = 1 + 2;\n"
                        "    while ( (a==b) ||  ( (  ((a==b) && ((a==b) || (b==a))  )  && ((a==b) && ((a==b) || (b==a))  )  )   && ((a==b) && ((a==b) || (b==a))  )  )         ){\n"
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
                        "}";
  auto simpleParser = createSimpleParser(program);
  REQUIRE_NOTHROW(simpleParser->parse());
}

TEST_CASE("Test if") {
  std::string program = "procedure if {\n"
                        "    a = a + 1;\n"
                        "    if (a == 1) then {\n"
                        "        a = a + 1;\n"
                        "        if (b == 1) then {\n"
                        "            b = b + 1;\n"
                        "            if (c == 1) then {\n"
                        "                c = c + 1;\n"
                        "                if (d == 1) then {\n"
                        "                    d = d + 1;\n"
                        "                    if (e == 1) then {\n"
                        "                        e = e + 1;\n"
                        "                        if (f == 1) then {\n"
                        "                            f = f + 1;\n"
                        "                        } else {\n"
                        "                            f = f + 1;\n"
                        "                        }\n"
                        "                    } else {\n"
                        "                        f = f + 1;\n"
                        "                    }\n"
                        "                } else {\n"
                        "                    f = f + 1;\n"
                        "                }\n"
                        "            } else {\n"
                        "                f = f + 1;\n"
                        "            }\n"
                        "        } else {\n"
                        "            f = f + 1;\n"
                        "        }\n"
                        "    } else {\n"
                        "        f = f + 1;\n"
                        "    }\n"
                        "}";
  auto simpleParser = createSimpleParser(program);
  REQUIRE_NOTHROW(simpleParser->parse());
}

// TODO::    FIX THIS
TEST_CASE("ParseConditionalExpressions invalid with wrongly placed ! 6") {
  std::string program = "procedure example {\n"
                        "        while(   (a==b) && (b==a) || (a==b) ) ){"
                        "            a = c*(d+e); "
                        "        }\n"
                        "}\n";
  auto simpleParser = createSimpleParser(program);
  REQUIRE_THROWS_WITH(simpleParser->parse(), Catch::Matchers::Contains(
      "SimpleParser: Logical token must come between enclosed cond exprs"));
  delete simpleParser;
}

TEST_CASE("ParseConditionalExpressions invalid with wrongly placed ! 7") {
  std::string program = "procedure example {\n"
                        "        while(       ( ! ( (a==b)  && (b==a))      ||      (a==b))        ){"
                        "            a = c*(d+e); "
                        "        }\n"
                        "}\n";
  auto simpleParser = createSimpleParser(program);
  REQUIRE_THROWS_WITH(simpleParser->parse(), Catch::Matchers::Contains(
      "SimpleParser: Logical token must come between enclosed cond exprs"));
  delete simpleParser;
}

TEST_CASE("Test nesting 1") {
  std::string program = "procedure nestedWhileAsf{\n"
                        "    a = 2;\n"
                        "    b = 1 + 2;\n"
                        "    while ( (a==b) ||  ( (  ((a==b) && ((a==b) || (b==a))  )  && ((a==b) && ((a==b) || (b==a))  )  )   && ((a==b) && ((a==b) || (b==a))  )  )         ){\n"
                        "        a = a - 1;\n"
                        "        while ((( u12 ==  mu11 ) && (!( 123 == 321 ))) || ( u13 == 1 + 1)) {\n"
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
                        "}";
  auto simpleParser = createSimpleParser(program);
  REQUIRE_NOTHROW(simpleParser->parse());
}

TEST_CASE("Test nesting 2") {
  std::string program = "procedure nestedWhileAsf{\n"
                        "    a = 2;\n"
                        "    b = 1 + 2;\n"
                        "    while (  !((x>y) && ((x!=0) && (y!=0)))  ){\n"
                        "        a = a - 1;\n"
                        "        while ( ( (a>b) || (a < b) ) || ( ( (a>b) && (a<b) ) && ( (a) != (a * (a) - (a)/2 + a % div ) ) ) ) {\n"
                        "            b = b - 1;\n"
                        "            while(  (( (a>b) || (a < b) ) || ( ( (a>b) && (a<b) ) && ( (a) != (a * (a) - (a)/2 + a % div ) ) )) && ( (( (a>b) || (a < b) ) || ( ( (a>b) && (a<b) ) && ( (a) != (a * (a) - (a)/2 + a % div ) ) )) ||  (( (a>b) || (a < b) ) || ( ( (a>b) && (a<b) ) && ( (a) != (a * (a) - (a)/2 + a % div ) ) )) )    ) {\n"
                        "                c = c - 1;\n"
                        "    while (  !((x>y) && (  (!(x!=0))   && (y!=0)))  ){\n"
                        "                    d = d - 1;\n"
                        "                    while(  (    (   (  ( ( ( (   ((a < b) || (b != 1999999)) && ((((((((a))))) + b) == 111111) || (1000000 == 10000002)))))))))    ){\n"
                        "                        f = f - 1;\n"
                        "                    }\n"
                        "                }\n"
                        "            }\n"
                        "        }\n"
                        "    }\n"
                        "}";
  auto simpleParser = createSimpleParser(program);
  REQUIRE_THROWS(simpleParser->parse());
}

TEST_CASE("Test nesting 3") {
  std::string program = "procedure example {\n"
                        "        while(  (x==y)  ){"
                        "            a = c*(d+e); "
                        "        }\n"
                        "}\n";
  auto simpleParser = createSimpleParser(program);
  REQUIRE_THROWS(simpleParser->parse());
  delete simpleParser;
}

TEST_CASE("Test nesting 4") {
  std::string program = "procedure example {\n"
                        "        while(  (!(x==y))  ){"
                        "            a = c*(d+e); "
                        "        }\n"
                        "}\n";
  auto simpleParser = createSimpleParser(program);
  REQUIRE_THROWS(simpleParser->parse());
  delete simpleParser;
}

TEST_CASE("Test nesting 5") {
  std::string program = "procedure example {\n"
                        "        while(   (  (!(x==y))  && (x==y)    )  ){"
                        "            a = c*(d+e); "
                        "        }\n"
                        "}\n";
  auto simpleParser = createSimpleParser(program);
  REQUIRE_THROWS(simpleParser->parse());
  delete simpleParser;
}

