//
// Created by Kamil Gimik on 2/9/21.
//

#ifndef TEAM_07_SIMPLEPARSER_H
#define TEAM_07_SIMPLEPARSER_H

#include "Tokenizer.h"
#include "PKB/PKB.h"
#include "DesignAbstractor.h"
#include <stack>
#include <vector>
#include <optional>
#include <unordered_set>
#include "PkbPopulator.h"
#include <string>
#include "SourceProcessorUtils.h"
#include "PostFixConverter.h"

class SimpleParser {

  //enum class StmtType {Assign, Call, Print, Read, While, If};

  int currentStmtNum;
  int startingStmtNum;
  Tokenizer *tokenizer;
  PKB *pkb;
  DesignAbstractor *designAbstractor;
  PkbPopulator *pkbPopulator;

  // data structures for token backtracking
  std::string currentProcedure;
  std::stack<Token *> parenthesisStack;
  std::vector<Token *> prevTokens;
  std::stack<int> parentStack;
  std::stack<int> followStack;
  Token *currentToken;
  bool isFollowable;
  bool isFollowsAfterNesting;

  Token *prevClosingRBrace;

  int prevIf;
  int prevWhile;
  int prevAssign;

 private:

  int getCurrentParent();
  void parseLinesInBlock();

  // helper method to reset local variables after finishing parsing a procedure
  void resetVariables();
  void updateLocalVariablesAfterNestingEnds();
  void parseLinesInNesting();

  // updates pkb with parents and follows relations
  void updateParentsAndFollowsRelations();

  // checks whether parsing is occuring within a current procedure
  bool hasCurrentProcedure();

  // checks whether current stmt is nested
  bool isNested();

  // parses a procedure declaration, throws std::runtime exception if ill-defined
  bool parseProcedure();

  // parses a line, calls parseStmt for stmts and parseSymbol for end of procedures
  bool parseLine();

  // parses stmts, calls parseAssign, parseIf, parseWhile, etc. depending on token sequence
  bool parseStmt();

  // parsing stmts, pushes data to pkb
  bool parseAssign();
  bool parseIf();
  bool parseWhile();
  bool parseCall();
  bool parseRead();
  bool parsePrint();
  bool parseSymbol();
  std::vector<Token *> parseExpression();
  std::vector<Token *> parseCondExpr(int size);
  void parseCondExprs();

  // populate current token with new token from tokenizer, overridden to expect certain conditions
  Token *populateToken();
  Token *populateToken(TokenType expectedType);
  Token *populateToken(const std::unordered_set<TokenType> &expectedTypes);
  Token *populateToken(TokenType expectedType, const std::string &expectedValue);


 public:

  // Constructors
  SimpleParser(PKB *pkb, std::string &program);

  // Main method of SimpleParser, responsible for parsing the whole program
  bool parse();

};

#endif //TEAM_07_SIMPLEPARSER_H
