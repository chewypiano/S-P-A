//
// Created by Junhua on 28/8/2021.
//

#ifndef TEAM07_TOKEN_H
#define TEAM07_TOKEN_H

#include <string>
#include "TokenType.h"

class Token {
  std::string value;
  TokenType type;
 public:
  Token(TokenType type, std::string value);
  std::string getValue();
  void setValue(std::string newValue);
  TokenType getType();
  size_t getLength();
};

#endif //TEAM07_TOKEN_H
