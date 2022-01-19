//
// Created by vaish on 3/9/2021.
//

#ifndef TEAM07_DESIGNENTITYTYPE_H
#define TEAM07_DESIGNENTITYTYPE_H

#include <unordered_map>
#include <string>

enum class DesignEntityType {
  Stmt,
  Read,
  Print,
  Call,
  While,
  If,
  Assign,
  Variable,
  Constant,
  ProgLine,
  Procedure,
};

class DesignEntityToName {
  static std::unordered_map<DesignEntityType, std::string> convertTable;
 public:
  static std::string designEntityToName(DesignEntityType entityType);
};

#endif //TEAM07_DESIGNENTITYTYPE_H
