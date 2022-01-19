//
// Created by Junhua on 1/9/2021.
//

#include "Tokenizer.h"
#include "Rules.h"
#include "Token.h"
#include "catch.hpp"
#include <string>

TEST_CASE("Tokenizer Parse PQL") {
  std::string program = "Hello world";
  auto tokenizer = createPQLTokenizer(program);
  Token *t[] = {
      new Token(Name, "Hello"),
      new Token(Name, "world"),
  };
  for (auto &expected: t) {
    auto result = tokenizer->getNext();
    REQUIRE(result != nullptr);
    REQUIRE(expected->getType() == result->getType());
    REQUIRE(expected->getValue() == result->getValue());
  }
  REQUIRE(tokenizer->getNext() == nullptr);
}

TEST_CASE("Basic Program with Select Clause") {
  std::string program = "assign a; Select a";
  Token *expected[] = {
      new Token(Assign, "assign"),
      new Token(Name, "a"),
      new Token(Semicolon, ";"),
      new Token(Select, "Select"),
      new Token(Name, "a"),
  };

  auto tokenizer = createPQLTokenizer(program);

  for (auto &expected_res: expected) {
    auto token = tokenizer->getNext();
    REQUIRE(token != nullptr);
    REQUIRE(token->getType() == expected_res->getType());
    REQUIRE(token->getValue() == expected_res->getValue());
  }
  REQUIRE(tokenizer->getNext() == nullptr);
  delete tokenizer;
}

TEST_CASE("Test Negative lookahead") {
  std::string program = "Uses(a,b)";
  Token *expected[] = {
      new Token(Uses, "Uses"),
      new Token(LBracket, "("),
      new Token(Name, "a"),
      new Token(Comma, ","),
      new Token(Name, "b"),
      new Token(RBracket, ")")
  };

  auto tokenizer = createPQLTokenizer(program);

  for (auto expected_res: expected) {
    auto result = tokenizer->getNext();
    REQUIRE(result != nullptr);
    REQUIRE(expected_res->getType() == result->getType());
    REQUIRE(expected_res->getValue() == result->getValue());
  }
  REQUIRE(tokenizer->getNext() == nullptr);
}

TEST_CASE("Tokenize Sample Program with Pattern") {
  std::string program = "assign a;\nSelect a pattern a ( _ , \"v + x * y + z * t\")";
  Token *expected[] = {
      new Token(Assign, "assign"),
      new Token(Name, "a"),
      new Token(Semicolon, ";"),
      new Token(Select, "Select"),
      new Token(Name, "a"),
      new Token(Pattern, "pattern"),
      new Token(Name, "a"),
      new Token(LBracket, "("),
      new Token(Wildcard, "_"),
      new Token(Comma, ","),
      new Token(String, "\"v + x * y + z * t\""),
      new Token(RBracket, ")")
  };

  auto tokenizer = createPQLTokenizer(program);

  for (auto &expected_res: expected) {
    auto result = tokenizer->getNext();
    REQUIRE(result != nullptr);
    REQUIRE(expected_res->getValue() == result->getValue());
    REQUIRE(expected_res->getType() == result->getType());
  }
  REQUIRE(tokenizer->getNext() == nullptr);
  delete tokenizer;
}

TEST_CASE("PQL Statement with such that clause") {
  std::string program = "assign a;variable v;\nSelect a such that Uses(a,v)";
  Token *expected[] = {
      new Token(Assign, "assign"),
      new Token(Name, "a"),
      new Token(Semicolon, ";"),
      new Token(Variable, "variable"),
      new Token(Name, "v"),
      new Token(Semicolon, ";"),
      new Token(Select, "Select"),
      new Token(Name, "a"),
      new Token(Such, "such"),
      new Token(That, "that"),
      new Token(Uses, "Uses"),
      new Token(LBracket, "("),
      new Token(Name, "a"),
      new Token(Comma, ","),
      new Token(Name, "v"),
      new Token(RBracket, ")"),
  };

  auto tokenizer = createPQLTokenizer(program);

  for (auto &expected_res: expected) {
    auto token = tokenizer->getNext();
    REQUIRE(token != nullptr);
    REQUIRE(token->getType() == expected_res->getType());
    REQUIRE(token->getValue() == expected_res->getValue());
  }
  REQUIRE(tokenizer->getNext() == nullptr);
  delete tokenizer;
}

TEST_CASE("Test simple tokenizer") {
  std::string program =
      "procedure Example {\nx = 2;\nz = 3;\ni = 5;\nwhile (i!=0) {\nx = x - 1;\nif (x==1) then {\nz = x + 1; }\nelse {\ny = z + x; }\n}}";
  auto tokenizer = createSimpleTokenizer(program);
  Token *expected[] = {
      new Token(Name, "procedure"),
      new Token(Name, "Example"),
      new Token(LBrace, "{"),
      new Token(Name, "x"),
      new Token(Equal, "="),
      new Token(Number, "2"),
      new Token(Semicolon, ";"),
      new Token(Name, "z"),
      new Token(Equal, "="),
      new Token(Number, "3"),
      new Token(Semicolon, ";"),
      new Token(Name, "i"),
      new Token(Equal, "="),
      new Token(Number, "5"),
      new Token(Semicolon, ";"),
      new Token(Name, "while"),
      new Token(LBracket, "("),
      new Token(Name, "i"),
      new Token(Relative, "!="),
      new Token(Number, "0"),
      new Token(RBracket, ")"),
      new Token(LBrace, "{"),
      new Token(Name, "x"),
      new Token(Equal, "="),
      new Token(Name, "x"),
      new Token(Arithmetic, "-"),
      new Token(Number, "1"),
      new Token(Semicolon, ";"),
      new Token(Name, "if"),
      new Token(LBracket, "("),
      new Token(Name, "x"),
      new Token(Relative, "=="),
      new Token(Number, "1"),
      new Token(RBracket, ")"),
      new Token(Name, "then"),
      new Token(LBrace, "{"),
      new Token(Name, "z"),
      new Token(Equal, "="),
      new Token(Name, "x"),
      new Token(Arithmetic, "+"),
      new Token(Number, "1"),
      new Token(Semicolon, ";"),
      new Token(RBrace, "}"),
      new Token(Name, "else"),
      new Token(LBrace, "{"),
      new Token(Name, "y"),
      new Token(Equal, "="),
      new Token(Name, "z"),
      new Token(Arithmetic, "+"),
      new Token(Name, "x"),
      new Token(Semicolon, ";"),
      new Token(RBrace, "}"),
      new Token(RBrace, "}"),
      new Token(RBrace, "}"),
  };

  for (auto &expected_res: expected) {
    auto token = tokenizer->getNext();
    REQUIRE(token != nullptr);
    REQUIRE(token->getValue() == expected_res->getValue());
    REQUIRE(token->getType() == expected_res->getType());

  }
  REQUIRE(tokenizer->getNext() == nullptr);
  delete tokenizer;
}

TEST_CASE("Invalid PQL symbols in Quotes") {
  std::string program = "\"Hello world ;~12`\"";
  auto tokenizer = createPQLTokenizer(program);
  auto token = tokenizer->getNext();
  REQUIRE(token != nullptr);
  REQUIRE(token->getType() == String);
  REQUIRE(token->getValue() == "\"Hello world ;~12`\"");
  delete token;
  delete tokenizer;
}

TEST_CASE("One letter program") {
  std::string program = ";";
  auto tokenizer = createPQLTokenizer(program);
  auto token = tokenizer->getNext();
  REQUIRE(token != nullptr);
  REQUIRE(token->getType() == Semicolon);
  REQUIRE(token->getValue() == ";");
  delete token;
  delete tokenizer;
}

TEST_CASE("Only Invalid PQL symbols") {
  std::string program = "::::::";
  auto tokenizer = createPQLTokenizer(program);
  auto token = tokenizer->getNext();
  REQUIRE(token != nullptr);
  REQUIRE(token->getType() == Error);
  REQUIRE(token->getValue() == program);
  delete token;
  delete tokenizer;
}

TEST_CASE("Unterminated quote string") {
  std::string program = "\" test";
  auto tokenizer = createPQLTokenizer(program);
  auto token = tokenizer->getNext();
  REQUIRE(token != nullptr);
  REQUIRE(token->getType() == Error);
  REQUIRE(token->getValue() == program);
  delete token;
  delete tokenizer;
}

TEST_CASE("Test hasNext") {
  std::string program = "procedure testToken {}";
  auto tokenizer = createSimpleTokenizer(program);
  Token *expected[] = {
      new Token(Name, "procedure"),
      new Token(Name, "testToken"),
      new Token(LBrace, "{"),
      new Token(RBrace, "}")
  };

  for (auto &token: expected) {
    REQUIRE(tokenizer->hasNext());
    auto actualToken = tokenizer->getNext();
    REQUIRE(token->getType() == actualToken->getType());
    REQUIRE(token->getValue() == actualToken->getValue());
    delete actualToken;
  }

  REQUIRE(!tokenizer->hasNext());
  delete tokenizer;
}

TEST_CASE("Test hasNext additional lines handled correctly") {
  std::string program = "procedure testToken {} \n \n \n \n";
  auto tokenizer = createSimpleTokenizer(program);
  Token *expected[] = {
      new Token(Name, "procedure"),
      new Token(Name, "testToken"),
      new Token(LBrace, "{"),
      new Token(RBrace, "}")
  };

  for (auto &token: expected) {
    REQUIRE(tokenizer->hasNext());
    auto actualToken = tokenizer->getNext();
    REQUIRE(token->getType() == actualToken->getType());
    REQUIRE(token->getValue() == actualToken->getValue());
    delete actualToken;
  }
  REQUIRE(!tokenizer->hasNext());
  REQUIRE(tokenizer->getNext() == nullptr);
  REQUIRE(!tokenizer->hasNext());

  delete tokenizer;
}

TEST_CASE("Test carriage return") {
  std::string program = "\r";
  auto parser = createSimpleTokenizer(program);
  REQUIRE(!parser->hasNext());
  REQUIRE(parser->getNext() == nullptr);
}

TEST_CASE("Test valid isIdentString") {
  std::string program = "\"a1\"";
  auto parser = createPQLTokenizer(program);
  REQUIRE(parser->isTokenIdentOrNameString(parser->getNext()));
}

TEST_CASE("Test invalid isIdentString when it is purely integer in string") {
  std::string program = "\"1\"";
  auto parser = createPQLTokenizer(program);
  REQUIRE(!parser->isTokenIdentOrNameString(parser->getNext()));
}

TEST_CASE("Test invalid isIdentString when it is not String") {
  std::string program = "1";
  auto parser = createPQLTokenizer(program);
  REQUIRE(!parser->isTokenIdentOrNameString(parser->getNext()));
}

TEST_CASE("Test invalid isIdentString when it is empty string") {
  std::string program = "\"\"";
  auto parser = createPQLTokenizer(program);
  REQUIRE(!parser->isTokenIdentOrNameString(parser->getNext()));
}

TEST_CASE("Test invalid isIdentString when it is not digit") {
  std::string program = "\"!\"";
  auto parser = createPQLTokenizer(program);
  REQUIRE(!parser->isTokenIdentOrNameString(parser->getNext()));
}

TEST_CASE("Test valid isIntegerString") {
  std::string program = "\"124\"";
  auto parser = createPQLTokenizer(program);
  REQUIRE(parser->isTokenIntegerString(parser->getNext()));
}

TEST_CASE("Test invalid isIntegerString when it contains a non-digit") {
  std::string program = "\"124a\"";
  auto parser = createPQLTokenizer(program);
  REQUIRE(!parser->isTokenIntegerString(parser->getNext()));
}

TEST_CASE("Test invalid isIntegerString when it is empty string") {
  std::string program = "\"\"";
  auto parser = createPQLTokenizer(program);
  REQUIRE(!parser->isTokenIntegerString(parser->getNext()));
}

TEST_CASE("Test invalid isIntegerString when it is not string") {
  std::string program = "13";
  auto parser = createPQLTokenizer(program);
  REQUIRE(!parser->isTokenIntegerString(parser->getNext()));
}

TEST_CASE("Test invalid isIntegerString when it is not digit") {
  std::string program = "\"!\"";
  auto parser = createPQLTokenizer(program);
  REQUIRE(!parser->isTokenIntegerString(parser->getNext()));
}

TEST_CASE("Test peek is working") {
  std::string program = "hello world hi hello";
  Token *t[] = {
      new Token(Name, "hello"),
      new Token(Name, "world"),
      new Token(Name, "hi"),
      new Token(Name, "hello"),
  };
  auto tokenizer = createSimpleTokenizer(program);
  for (auto &expected: t) {
    auto r1 = tokenizer->peek();
    REQUIRE(r1 != nullptr);
    REQUIRE(expected->getType() == r1->getType());
    REQUIRE(expected->getValue() == r1->getValue());
    auto result = tokenizer->getNext();
    REQUIRE(result != nullptr);
    REQUIRE(expected->getType() == result->getType());
    REQUIRE(expected->getValue() == result->getValue());
  }
  REQUIRE(tokenizer->getNext() == nullptr);
}

TEST_CASE("One &") {
  std::string program = "while(((a == b) && (a == b)))";
  Token *t[] = {
      new Token(Name, "while"),
      new Token(LBracket, "("),
      new Token(LBracket, "("),
      new Token(LBracket, "("),
      new Token(Name, "a"),
      new Token(Relative, "=="),
      new Token(Name, "b"),
      new Token(RBracket, ")"),
      new Token(Logical, "&&"),
      new Token(LBracket, "("),
      new Token(Name, "a"),
      new Token(Relative, "=="),
      new Token(Name, "b"),
      new Token(RBracket, ")"),
      new Token(RBracket, ")"),
      new Token(RBracket, ")")
  };
  auto tokenizer = createSimpleTokenizer(program);
  for (auto &expected: t) {
    auto r1 = tokenizer->peek();
    REQUIRE(r1 != nullptr);
    REQUIRE(expected->getType() == r1->getType());
    REQUIRE(expected->getValue() == r1->getValue());
    auto result = tokenizer->getNext();
    REQUIRE(result != nullptr);
    REQUIRE(expected->getType() == result->getType());
    REQUIRE(expected->getValue() == result->getValue());
  }
  REQUIRE(tokenizer->getNext() == nullptr);
}

TEST_CASE("Test whitespace single char") {
  std::string program = "  \"  a  \n \"";
  auto parser = createPQLTokenizer(program);
  REQUIRE(parser->extractWhitespace(program) == "\"a\"");
}

TEST_CASE("Test whitespace multiple chars spread out") {
  std::string program = "  \"1                + \r \n  y   \"";
  auto parser = createPQLTokenizer(program);
  REQUIRE(parser->extractWhitespace(program) == "\"1+y\"");
}

TEST_CASE("Test iteration 2 statements for PQL with statement") {
  std::string program = "assign a; procedure p; Select a with a.procName = p.procName";
  auto tokenizer = createPQLTokenizer(program);
  std::vector<Token *> expected = {
      new Token(Assign, "assign"),
      new Token(Name, "a"),
      new Token(Semicolon, ";"),
      new Token(Procedure, "procedure"),
      new Token(Name, "p"),
      new Token(Semicolon, ";"),
      new Token(Select, "Select"),
      new Token(Name, "a"),
      new Token(With, "with"),
      new Token(Name, "a"),
      new Token(ProcName, ".procName"),
      new Token(Equal, "="),
      new Token(Name, "p"),
      new Token(ProcName, ".procName"),
  };
  for (auto &value: expected) {
    auto token = tokenizer->getNext();
    INFO(TokenToName::tokenToName(token->getType()))
    INFO(TokenToName::tokenToName(value->getType()))
    INFO(token->getValue())
    INFO(value->getValue())
    REQUIRE(token->getType() == value->getType());
    INFO(token->getValue())
    INFO(value->getValue())
    REQUIRE(token->getValue() == value->getValue());
  }
  REQUIRE(tokenizer->getNext() == nullptr);
}

TEST_CASE("Test iteration 2 statements for PQL Select tuple statement") {
  std::string program = "assign a, b; Select <a,b>";
  auto tokenizer = createPQLTokenizer(program);
  std::vector<Token *> expected = {
      new Token(Assign, "assign"),
      new Token(Name, "a"),
      new Token(Comma, ","),
      new Token(Name, "b"),
      new Token(Semicolon, ";"),
      new Token(Select, "Select"),
      new Token(LAngleBracket, "<"),
      new Token(Name, "a"),
      new Token(Comma, ","),
      new Token(Name, "b"),
      new Token(RAngleBracket, ">")
  };
  for (auto &value: expected) {
    auto token = tokenizer->getNext();
    INFO(TokenToName::tokenToName(token->getType()))
    INFO(TokenToName::tokenToName(value->getType()))
    INFO(token->getValue())
    INFO(value->getValue())
    REQUIRE(token->getType() == value->getType());
    INFO(token->getValue())
    INFO(value->getValue())
    REQUIRE(token->getValue() == value->getValue());
  }
  REQUIRE(tokenizer->getNext() == nullptr);
}

TEST_CASE("Select Boolean") {
  std::string program = "Select BOOLEAN";
  auto tokenizer = createPQLTokenizer(program);
  std::vector<Token *> expected = {
      new Token(Select, "Select"),
      new Token(Boolean, "BOOLEAN")
  };
  for (auto &value: expected) {
    auto token = tokenizer->getNext();
    INFO(TokenToName::tokenToName(token->getType()))
    INFO(TokenToName::tokenToName(value->getType()))
    INFO(token->getValue())
    INFO(value->getValue())
    REQUIRE(token->getType() == value->getType());
    INFO(token->getValue())
    INFO(value->getValue())
    REQUIRE(token->getValue() == value->getValue());
  }
  REQUIRE(tokenizer->getNext() == nullptr);
}

TEST_CASE("Select Next clause") {
  std::string program = "Select BOOLEAN such that Next(1,2)";
  auto tokenizer = createPQLTokenizer(program);
  std::vector<Token *> expected = {
      new Token(Select, "Select"),
      new Token(Boolean, "BOOLEAN"),
      new Token(Such, "such"),
      new Token(That, "that"),
      new Token(Next, "Next"),
      new Token(LBracket, "("),
      new Token(Integer, "1"),
      new Token(Comma, ","),
      new Token(Integer, "2"),
      new Token(RBracket, ")")
  };
  for (auto &value: expected) {
    auto token = tokenizer->getNext();
    INFO(TokenToName::tokenToName(token->getType()))
    INFO(TokenToName::tokenToName(value->getType()))
    INFO(token->getValue())
    INFO(value->getValue())
    REQUIRE(token->getType() == value->getType());
    INFO(token->getValue())
    INFO(value->getValue())
    REQUIRE(token->getValue() == value->getValue());
  }
  REQUIRE(tokenizer->getNext() == nullptr);
}

TEST_CASE("Select Next* clause") {
  std::string program = "Select BOOLEAN such that Next*(1,2)";
  auto tokenizer = createPQLTokenizer(program);
  std::vector<Token *> expected = {
      new Token(Select, "Select"),
      new Token(Boolean, "BOOLEAN"),
      new Token(Such, "such"),
      new Token(That, "that"),
      new Token(NextT, "Next*"),
      new Token(LBracket, "("),
      new Token(Integer, "1"),
      new Token(Comma, ","),
      new Token(Integer, "2"),
      new Token(RBracket, ")")
  };
  for (auto &value: expected) {
    auto token = tokenizer->getNext();
    INFO(TokenToName::tokenToName(token->getType()))
    INFO(TokenToName::tokenToName(value->getType()))
    INFO(token->getValue())
    INFO(value->getValue())
    REQUIRE(token->getType() == value->getType());
    INFO(token->getValue())
    INFO(value->getValue())
    REQUIRE(token->getValue() == value->getValue());
  }
  REQUIRE(tokenizer->getNext() == nullptr);
}

TEST_CASE("With Attributes") {
  std::string program = "Select BOOLEAN with a.procName = b.value and c.varName = b.stmt#";
  auto tokenizer = createPQLTokenizer(program);
  std::vector<Token *> expected = {
      new Token(Select, "Select"),
      new Token(Boolean, "BOOLEAN"),
      new Token(With, "with"),
      new Token(Name, "a"),
      new Token(ProcName, ".procName"),
      new Token(Equal, "="),
      new Token(Name, "b"),
      new Token(Value, ".value"),
      new Token(And, "and"),
      new Token(Name, "c"),
      new Token(VarName, ".varName"),
      new Token(Equal, "="),
      new Token(Name, "b"),
      new Token(StmtNo, ".stmt#")
  };
  for (auto &value: expected) {
    auto token = tokenizer->getNext();
    INFO(TokenToName::tokenToName(token->getType()))
    INFO(TokenToName::tokenToName(value->getType()))
    INFO(token->getValue())
    INFO(value->getValue())
    REQUIRE(token->getType() == value->getType());
    INFO(token->getValue())
    INFO(value->getValue())
    REQUIRE(token->getValue() == value->getValue());
  }
  REQUIRE(tokenizer->getNext() == nullptr);
}

TEST_CASE("Progline Synonym") {
  std::string program = "prog_line a, b; Select BOOLEAN with a.procName = b.value and c.varName = b.stmt#";
  auto tokenizer = createPQLTokenizer(program);
  std::vector<Token *> expected = {
      new Token(ProgLine, "prog_line"),
      new Token(Name, "a"),
      new Token(Comma, ","),
      new Token(Name, "b"),
      new Token(Semicolon, ";"),
      new Token(Select, "Select"),
      new Token(Boolean, "BOOLEAN"),
      new Token(With, "with"),
      new Token(Name, "a"),
      new Token(ProcName, ".procName"),
      new Token(Equal, "="),
      new Token(Name, "b"),
      new Token(Value, ".value"),
      new Token(And, "and"),
      new Token(Name, "c"),
      new Token(VarName, ".varName"),
      new Token(Equal, "="),
      new Token(Name, "b"),
      new Token(StmtNo, ".stmt#")
  };
  for (auto &value: expected) {
    auto token = tokenizer->getNext();
    INFO(TokenToName::tokenToName(token->getType()))
    INFO(TokenToName::tokenToName(value->getType()))
    INFO(token->getValue())
    INFO(value->getValue())
    REQUIRE(token->getType() == value->getType());
    INFO(token->getValue())
    INFO(value->getValue())
    REQUIRE(token->getValue() == value->getValue());
  }
  REQUIRE(tokenizer->getNext() == nullptr);
}

TEST_CASE("Tokenize new syntax") {
  std::string program = "NextBip NextBip* AffectsBip Affects Affects* Affectsss";
  auto tokenizer = createPQLTokenizer(program);
  std::vector<Token *> expected = {
      new Token(NextBip, "NextBip"),
      new Token(NextBipT, "NextBip*"),
      new Token(AffectsBip, "AffectsBip"),
      new Token(Affects, "Affects"),
      new Token(AffectsT, "Affects*"),
      new Token(Name, "Affectsss"),
  };
  for (auto &value: expected) {
    auto token = tokenizer->getNext();
    INFO(TokenToName::tokenToName(token->getType()))
    INFO(TokenToName::tokenToName(value->getType()))
    INFO(token->getValue())
    INFO(value->getValue())
    REQUIRE(token->getType() == value->getType());
    INFO(token->getValue())
    INFO(value->getValue())
    REQUIRE(token->getValue() == value->getValue());
  }
  REQUIRE(tokenizer->getNext() == nullptr);
}

TEST_CASE("Do not accept leading 0s") {
  std::string program = "procedure a{ a = 001 }";
  auto tokenizer = createSimpleTokenizer(program);
  std::vector<Token *> expected = {
      new Token(Name, "procedure"),
      new Token(Name, "a"),
      new Token(LBrace, "{"),
      new Token(Name, "a"),
      new Token(Equal, "="),
      new Token(Error, "001"),
      new Token(RBrace, "}"),
  };
  for (auto &value: expected) {
    auto token = tokenizer->getNext();
    INFO(TokenToName::tokenToName(token->getType()))
    INFO(TokenToName::tokenToName(value->getType()))
    INFO(token->getValue())
    INFO(value->getValue())
    REQUIRE(token->getType() == value->getType());
    INFO(token->getValue())
    INFO(value->getValue())
    REQUIRE(token->getValue() == value->getValue());
  }
  REQUIRE(tokenizer->getNext() == nullptr);
}

TEST_CASE("0 by itself is valid") {
  std::string program = "procedure a{ a = 0 }";
  auto tokenizer = createSimpleTokenizer(program);
  std::vector<Token *> expected = {
      new Token(Name, "procedure"),
      new Token(Name, "a"),
      new Token(LBrace, "{"),
      new Token(Name, "a"),
      new Token(Equal, "="),
      new Token(Number, "0"),
      new Token(RBrace, "}"),
  };
  for (auto &value: expected) {
    auto token = tokenizer->getNext();
    INFO(TokenToName::tokenToName(token->getType()))
    INFO(TokenToName::tokenToName(value->getType()))
    INFO(token->getValue())
    INFO(value->getValue())
    REQUIRE(token->getType() == value->getType());
    INFO(token->getValue())
    INFO(value->getValue())
    REQUIRE(token->getValue() == value->getValue());
  }
  REQUIRE(tokenizer->getNext() == nullptr);
}

TEST_CASE("2 by itself is valid") {
  std::string program = "procedure a{ a = 2 }";
  auto tokenizer = createSimpleTokenizer(program);
  std::vector<Token *> expected = {
      new Token(Name, "procedure"),
      new Token(Name, "a"),
      new Token(LBrace, "{"),
      new Token(Name, "a"),
      new Token(Equal, "="),
      new Token(Number, "2"),
      new Token(RBrace, "}"),
  };
  for (auto &value: expected) {
    auto token = tokenizer->getNext();
    INFO(TokenToName::tokenToName(token->getType()))
    INFO(TokenToName::tokenToName(value->getType()))
    INFO(token->getValue())
    INFO(value->getValue())
    REQUIRE(token->getType() == value->getType());
    INFO(token->getValue())
    INFO(value->getValue())
    REQUIRE(token->getValue() == value->getValue());
  }
  REQUIRE(tokenizer->getNext() == nullptr);
}