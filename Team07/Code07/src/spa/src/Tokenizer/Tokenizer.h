//
// Created by Junhua on 28/8/2021.
//

#ifndef TEAM07_TOKENIZER_H
#define TEAM07_TOKENIZER_H

#include "Token.h"
#include "RuleEngine.h"
#include <unordered_set>

class Tokenizer {
 private:
  static std::unordered_set<char> whitespaces;

  size_t currentIndex;
  std::string program;
  RuleEngine *ruleEngine;
  Token *current;

  static bool isWhitespace(char str);

  /// Replaces current token with arg and returns previous token.
  Token *replaceCurrent(Token *token);

  Token *fetch();
  bool hasFinished();
  bool terminationCheck();
  bool quoteCheck(size_t length, bool hasDoubleQuotes);
  size_t processQuotes();

 public:
  static bool isTokenIdentOrNameString(Token *token);
  static bool isTokenIntegerString(Token *token);
  static std::string extractWhitespace(std::string value);

  Tokenizer(RuleEngine *engine, std::string &program);
  bool hasNext();
  Token *peek();
  Token *getNext();
};

Tokenizer *createSimpleTokenizer(std::string program);
Tokenizer *createPQLTokenizer(std::string program);

#endif //TEAM07_TOKENIZER_H
