//
// Created by Sean Chan  on 28/9/21.
//
#include <iostream>
#include "PostFixConverter.h"
#include <vector>
#include <stack>

std::string PostFixConverter::toPostFix(std::vector<Token *> tokens) {
  std::stack<Token *> stk;
  std::string output = " ";

  if (tokens.back()->getType() == Semicolon) {
    tokens.pop_back();
  }

  // algo starts here
  for (auto token: tokens) {
    // if name or number
    if (token->getType() == Name || token->getType() == Number) {
      output += token->getValue() + " ";
    } else if (stk.empty()) {
      if (token->getType() == RBracket) {
        throw std::runtime_error("PostFixConverter: ) encountered before (");
      } else { // push ( and operators
        stk.push(token);
      }
    }

      // bracket
    else if (token->getType() == RBracket || token->getType() == LBracket) {
      if (token->getType() == LBracket) {
        stk.push(token);
      } else if (token->getType() == RBracket) {
        Token *topStk;
        while (true) {
          topStk = stk.top();
          stk.pop();
          if (topStk->getType() == LBracket) {
            break;
          }
          output += topStk->getValue() + " ";

          if (stk.empty())
            throw std::runtime_error("PostFixConverter: ) cant find ( down the stack");
        }
      }
    }

      // operator
    else if (token->getType() == Arithmetic) {
      Token *topStk = stk.top();
      // top stack non operator
      if (topStk->getType() == LBracket) {
        stk.push(token);
      }
        //operator
      else {
        // precedence stacking
        if (token->getValue() == "*" || token->getValue() == "/" || token->getValue() == "%") {
          // TODO::
          if (topStk->getValue() == "+" || topStk->getValue() == "-") {
            stk.push(token);
          } else {
            while (topStk->getType() == Arithmetic && !stk.empty()) {
              output += topStk->getValue() + " ";
              stk.pop();
              if (!stk.empty())topStk = stk.top();
            }
            stk.push(token);
          }
        } else if (token->getValue() == "+" || token->getValue() == "-") {
          //  while top stack is operator, pop all operators
          while (topStk->getType() == Arithmetic && !stk.empty()) {
            output += topStk->getValue() + " ";
            stk.pop();
            if (!stk.empty()) {
              topStk = stk.top();
            }
          }
          stk.push(token);
        }
      }

    }
  }
  while (!stk.empty()) {
    if (stk.top()->getValue() != "(" && stk.top()->getValue() != "!") {
      output += stk.top()->getValue() + " ";
    }
    stk.pop();
  }
  return output;
}

std::string PostFixConverter::toPostFix(std::string expression) {
  auto tokenizer = createSimpleTokenizer(expression);
  std::vector<Token *> tokens = {};
  while (tokenizer->hasNext()) {
    tokens.push_back(tokenizer->getNext());
  }
  return toPostFix(tokens);
}