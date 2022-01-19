//
// Created by Junhua on 28/8/2021.
//

#ifndef TEAM07_TOKENTYPE_H
#define TEAM07_TOKENTYPE_H

#include <string>
#include <unordered_map>

enum TokenType {
  LBrace,
  RBrace,
  LBracket,
  RBracket,
  Number,
  Space,
  Variable,
  Semicolon,
  Assign,
  Wildcard,
  Such,
  That,
  Select,
  Follow,
  Parent,
  FollowT,
  ParentT,
  Uses,
  Modifies,
  Pattern,
  Affects,
  AffectsT,
  NextBip,
  NextBipT,
  AffectsBip,
  Ignore,
  Comma,
  String,
  Statement,
  Procedure,
  Read,
  Print,
  Call,
  While,
  If,
  Constant,
  Integer,
  Name,
  Error,
  With,
  And,
  Boolean,
  LAngleBracket,
  RAngleBracket,
  Calls,
  CallsT,
  Next,
  NextT,
  ProcName,
  VarName,
  Value,
  StmtNo,
  ProgLine,

  // For SIMPLE
  Equals,
  Equal,
  Logical,
  Arithmetic,
  Relative,
  Negate
};

class TokenToName {
  static std::unordered_map<TokenType, std::string> convertTable;
 public:
  static std::string tokenToName(TokenType token);
};

#endif //TEAM07_TOKENTYPE_H
