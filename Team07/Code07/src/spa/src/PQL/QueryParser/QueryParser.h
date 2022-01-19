//
// Created by vaish on 5/9/2021.
//

#ifndef AUTOTESTER_QUERYPARSER_H
#define AUTOTESTER_QUERYPARSER_H

#include <utility>
#include <vector>
#include <unordered_set>
#include "Query.h"
#include "Tokenizer/RuleEngine.h"
#include "Tokenizer/Tokenizer.h"
#include "Tokenizer/Rules.h"
#include "SourceProcessor/PostFixConverter.h"
#include "IrrelevantClauseProcessor.h"

typedef std::pair<Token *, Token *> TokenPair;

class QueryParser {
  Query *query;
  Tokenizer *tokenizer;

  void parseQuery();

  void parseQuerySynonym(Token *currentToken);

  void parseSelectStatement();

  ClauseType getModifiesOrUsesType(ClauseType suchThatClauseType, TokenPair pair);

  bool isValidSuchThatClauseSyntax(ClauseType suchThatClauseType, TokenPair pair);

  void parseOutputTuple();

  void parseOutputBoolean(Token *token);

  void parseOutputSynonym();

  void parseClause();

  void parseSuchThatClause();

  void parsePatternClause();

  void parseAssignPatternClause(QuerySynonym *patternSynonym);

  void parseIfPatternClause(QuerySynonym *patternSynonym);

  void parseWhilePatternClause(QuerySynonym *patternSynonym);

  void parseWithClause();

  QueryOutput *extractElement();

  TokenPair extractBracketedContentsForSuchThat();

  std::pair<TokenPair, std::string> extractBracketedContentsForAssignPattern();

  Token *extractWildcardRightArgInAssignPattern(std::stringstream *rightVal, Token *tokenAfterComma);

  Token *extractStringRightArgInAssignPattern(std::stringstream *rightVal, Token *tokenAfterComma);

  std::tuple<Token *, Token *, Token *> extractBracketedContentsForIfPattern();

  TokenPair extractBracketedContentsForWhilePattern();

  WithArgument *extractArgumentForWith();

  WithArgument *extractSynOrAttributeArgumentForWith(Token *token);

  bool isValidAssignExpr(Tokenizer *simpleTokenizer, int bracketLevel);

  bool isValidExprAfterLhsExpr(Tokenizer *simpleTokenizer, int bracketLevel);

  void extractSingleTokenOfParticularType(TokenType requiredType);

 public:
  explicit QueryParser(std::string &program);

  void insertQuery(Query *queryToInsert);

  void parse();

  Query getQuery();

  std::string parsedQueryToString();
};

#endif //AUTOTESTER_QUERYPARSER_H
