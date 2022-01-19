//
// Created by Junhua on 28/8/2021.
//

#include "Tokenizer.h"
#include "Rules.h"

Tokenizer *createSimpleTokenizer(std::string program) {
  auto engine = new RuleEngine(Rules::getSimpleRules());
  return new Tokenizer(engine, program);
}

Tokenizer *createPQLTokenizer(std::string program) {
  auto engine = new RuleEngine(Rules::getPQLRules());
  return new Tokenizer(engine, program);
}

std::unordered_set<char> Tokenizer::whitespaces = {
    '\t', '\n', '\r', ' ',
};

Tokenizer::Tokenizer(RuleEngine *engine, std::string &program) {
  this->program = std::string(program);
  ruleEngine = engine;
  currentIndex = 0;
  current = fetch();
}

bool Tokenizer::hasNext() {
  return this->current != nullptr;
}

Token *Tokenizer::peek() {
  return this->current;
}

bool Tokenizer::hasFinished() {
  return this->currentIndex >= this->program.length();
}

bool Tokenizer::isWhitespace(char str) {
  return whitespaces.count(str) > 0;
}

Token *Tokenizer::replaceCurrent(Token *token) {
  auto temp = this->current;
  this->current = token;
  return temp;
}

bool Tokenizer::terminationCheck() {
  return !hasFinished() && 1 + currentIndex <= program.length();
}

bool Tokenizer::quoteCheck(size_t length, bool hasDoubleQuotes) {
  return (
      !isWhitespace(program[currentIndex + length])
          || hasDoubleQuotes
  )
      && currentIndex + length < program.length();
}

size_t Tokenizer::processQuotes() {
  bool hasDoubleQuotes = program[this->currentIndex] == '"';
  size_t length = 1;

  while (quoteCheck(length, hasDoubleQuotes)) {
    if (program[currentIndex + length] != '"') {
      ++length;
      continue;
    }
    hasDoubleQuotes = !hasDoubleQuotes;
    ++length;
    if (!hasDoubleQuotes) {
      break;
    }
  }
  return length;
}

Token *Tokenizer::fetch() {
  while (terminationCheck()) {
    size_t length = processQuotes();
    std::string curr_str = program.substr(currentIndex, length);

    // Parse the string here
    Token *new_token = ruleEngine->stringToToken(curr_str);
    currentIndex += new_token->getLength();
    if (new_token->getType() == Ignore) {
      continue;
    }
    return new_token;
  }
  return nullptr;
}

Token *Tokenizer::getNext() {
  return replaceCurrent(fetch());
}

bool Tokenizer::isTokenIdentOrNameString(Token *token) {
  if (token == nullptr ||
      token->getType() != TokenType::String ||
      token->getLength() <= 2 ||
      !isalpha(token->getValue().at(1))) {
    return false;
  }
  // check all inner values to ensure they are either digit or alphabet, outer values are '"'
  for (std::string::size_type i = 1; i < token->getLength() - 1; ++i) {
    int curr_char = token->getValue().at(i);
    if (isdigit(curr_char) || isalpha(curr_char)) {
      continue;
    }
    return false;
  }
  return true;
}

bool Tokenizer::isTokenIntegerString(Token *token) {
  if (token == nullptr
      || token->getType() != TokenType::String
      || token->getLength() <= 2) {
    return false;
  }

  // check all inner values to ensure they are digits, outer values are '"'
  for (std::string::size_type i = 1; i < token->getLength() - 1; ++i) {
    if (isdigit(token->getValue().at(i))) {
      continue;
    }
    return false;
  }
  return true;
}

std::string Tokenizer::extractWhitespace(std::string value) {
  value.erase(std::remove_if(value.begin(), value.end(), ::isspace), value.end());
  return value;
}
