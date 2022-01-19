//
// Created by Junhua on 28/8/2021.
//

#include "Token.h"
#include <string>
#include <iostream>
#include <utility>

Token::Token(TokenType type, std::string value) {
  this->value = std::move(value);
  this->type = type;
}

std::string Token::getValue() {
  return this->value;
}

TokenType Token::getType() {
  return this->type;
}

size_t Token::getLength() {
  return this->value.length();
}

/// This is used by QueryParser.
void Token::setValue(std::string newValue) {
  this->value = std::move(newValue);
}
