//
// Created by Kamil Gimik on 29/9/21.
//

#include "SourceProcessorUtils.h"
#include <algorithm>
#include <stdexcept>
#include <iostream>

bool SourceProcessorUtils:: hasNoExtraBrackets(std::vector<Token *> expr){

  std::vector<int> vis = std::vector<int>(expr.size(),0);

  std::vector<int> negateIndexes = {};
  std::vector<int> logicalIndexes = {};

  for(int i = 0 ; i < expr.size(); i++){
    auto tok = expr[i];
    if(tok->getType()==Negate){
      negateIndexes.push_back(i);
    }
    if(tok->getType()==Logical){
      logicalIndexes.push_back(i);
    }
  }
  int buffer;
  for(int logicalIndex: logicalIndexes){
    // go right
    buffer = 0;
    vis[logicalIndex+1] = 1;
    for(int i = logicalIndex+1; i < expr.size();i++){
      auto currExpr = expr[i];
      if(currExpr->getType()==LBracket){
        buffer++;
      }
      else if (currExpr->getType()==RBracket){
        buffer--;
      }

      if(buffer==0){
        vis[i] = 1;
        break;
      }
    }
    // go left
    buffer = 0;
    vis[logicalIndex-1] = 1;
    for(int i = logicalIndex-1; i>=0; i--){
      auto currExpr = expr[i];
      if(currExpr->getType()==RBracket){
        buffer++;
      }
      else if (currExpr->getType()==LBracket){
        buffer--;
      }
      if(buffer==0){
        vis[i] = 1;
        break;
      }
    }
  }

  for(int negateIndex: negateIndexes){
    buffer = 0;
    vis[negateIndex+1] = 1;
    for(int i = negateIndex+1; i < expr.size();i++){
      auto currExpr = expr[i];
      if(currExpr->getType()==LBracket){
        buffer++;
      }
      else if (currExpr->getType()==RBracket){
        buffer--;
      }

      if(buffer==0){
        vis[i] = 1;
        break;
      }
    }
  }
  buffer = 0;
  for(int i  = 0 ; i < expr.size(); i ++){
    auto tok = expr[i];
    if(tok->getType()==LBracket && vis[i]==0){
      buffer++;
    }
    if(tok->getType()==RBracket && vis[i]==0 ){
      buffer--;
    }
    if(tok->getType() == Relative && buffer!=0 && vis[i]==0){
      return false;
    }
  }


  return true;
}

bool SourceProcessorUtils::isEnclosedProperly(std::vector<Token *> expr) {


  if(!hasNoExtraBrackets(expr)){
    return false;

  }

  // insert ( at front and ) at back
  auto tokenLeft = new Token(LBracket, "(");
  auto tokenRight = new Token(RBracket, ")");


  expr.emplace(expr.begin(), tokenLeft);
  expr.emplace_back(tokenRight);

  std::vector<int> visited = std::vector(expr.size(), -1);
  std::unordered_set<int> logicals;
  std::unordered_set<int> negates;
  for (int i = 0; i < expr.size(); i++) {
    auto tok = expr[i];
    if (tok->getType() == Logical) {
      logicals.insert(i);
    } else if (tok->getType() == Negate) {
      negates.insert(i);
    }
  }
  // make sure left and right are brackets
  for (auto negate: negates) {
    auto right = expr[negate + 1];
    if (right->getType() != LBracket) return false;
  }
  for (auto logical: logicals) {
    auto left = expr[logical - 1];
    auto right = expr[logical + 1];
    if (left->getType() != RBracket) return false;
    if (right->getType() != LBracket) return false;
  }
  while (logicals.size() > 0) {
    int prevSize = logicals.size();

    for (auto logical: logicals) {
      // go right
      int rightBuffer = (visited[logical + 1]) == -1 ? 2 : 1;
      int indexR = logical + 2;
      auto currToken = expr[indexR];
      bool rightable = false;
      while (rightBuffer != 0 && indexR < expr.size()) {
        currToken = expr[indexR];
        if (visited[indexR] != -1) {
          indexR++;
          currToken = expr[indexR];
          continue;
        }
        if (currToken->getType() == Negate) { // may have bug
          negates.erase(indexR);
        }
        if (currToken->getType() == LBracket) {
          rightBuffer++;
        }
        if (currToken->getType() == RBracket) {
          rightBuffer--;
        }
        if (currToken->getType() == Logical) {
          break;
        }
        if (rightBuffer == 0) {
          rightable = true;
        }
        indexR++;

      }
      // go left
      int leftBuffer = (visited[logical - 1]) == -1 ? 2 : 1;
      int indexL = logical - 2;
      currToken = expr[indexL];
      bool leftable = false;
      while (leftBuffer != 0 && indexL >= 0) {
        currToken = expr[indexL];
        if (visited[indexL] != -1) {
          indexL--;
          currToken = expr[indexL];
          continue;
        }
        if (currToken->getType() == Negate) { // may have bug
          negates.erase(indexL);
        }
        if (currToken->getType() == RBracket) {
          leftBuffer++;
        }
        if (currToken->getType() == LBracket) {
          leftBuffer--;
        }
        if (currToken->getType() == Logical) {
          break;
        }
        if (leftBuffer == 0) {
          leftable = true;
        }
        indexL--;

      }

      if (leftable && rightable) {

        int start = indexL + 1;
        int end = indexR - 1;
        int bracket = 0;

        int nestingLevel = 0;
        for (int i = start; i <= end; i++) {
          if (expr[i]->getType() == LBracket) nestingLevel++;
          if (expr[i]->getType() == RBracket) nestingLevel--;
          // add assert
        }

        for (int i = start; i <= end; i++) {
          if (expr[i]->getType() == LBracket) bracket++;
          if (expr[i]->getType() == RBracket) bracket--;

          visited[i] = bracket;
        }
        logicals.erase(logical);
        break;
      }
    }

    if (prevSize == logicals.size()) { // no more possible logicals to factor down
      return false;
    }

  }

  for (int i = 0; i < expr.size(); i++) {
    Token *tok = expr[i];
    if (tok->getType() == Negate) {
      int nIndex = i + 2;
      Token *nTok;
      int nCount = 1;
      while (nCount != 0) {
        nTok = expr[nIndex++];
        auto tokType = nTok->getType();
        if (tokType == LBracket) nCount++;
        if (tokType == RBracket) nCount--;
      }
      if (nIndex >= expr.size()) continue;
      nTok = expr[nIndex];
      if (nTok->getType() == Logical) return false;

    }
  }
  return true;
}

bool SourceProcessorUtils::expectType(Token *tokenToCheck, TokenType expectedType) {
  if (tokenToCheck->getType() != expectedType) {
    throw std::runtime_error("SimpleParser: Expected " + TokenToName::tokenToName(expectedType)
                                 + " token, not " + TokenToName::tokenToName(tokenToCheck->getType()));
  }
  return true;
}

bool SourceProcessorUtils::expectType(Token *tokenToCheck, std::unordered_set<TokenType> expectedTypes) {

  if (expectedTypes.count(tokenToCheck->getType()) == 0) {
    std::vector<std::string> expectedStrings = {};
    std::string stringOfExpected = "";
    for (TokenType type: expectedTypes) {
      expectedStrings.push_back(TokenToName::tokenToName(type));
    }
    std::sort(expectedStrings.begin(), expectedStrings.end());
    for (std::string type: expectedStrings) {
      stringOfExpected += type + " ";
    }
    stringOfExpected += "tokens";
    throw std::runtime_error("SimpleParser: Expected " + stringOfExpected
                                 + ", not " + TokenToName::tokenToName(tokenToCheck->getType()));
  }
  return true;
}
