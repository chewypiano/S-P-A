//
// Created by vaish on 3/9/2021.
//

#ifndef TEAM07_QUERYSYNONYM_H
#define TEAM07_QUERYSYNONYM_H

#include "DesignEntityType.h"
#include <string>

class QuerySynonym {
  DesignEntityType type;
  std::string name;

 public:
  QuerySynonym(DesignEntityType type, std::string name);

  DesignEntityType getType();

  std::string getName();

  std::string toString();

  void print();

  bool operator==(const QuerySynonym &p) const {
    return type == p.type && name == p.name;
  }

  struct HashFunction {
    size_t operator()(const QuerySynonym &x) const {
      return std::hash<std::string>()(x.name);
    }
  };
};

#endif //TEAM07_QUERYSYNONYM_H
