//
// Created by Junhua on 14/9/2021.
//

#ifndef AUTOTESTER_SRC_SPA_SRC_PQL_PATTERNTYPE_H_
#define AUTOTESTER_SRC_SPA_SRC_PQL_PATTERNTYPE_H_

enum class PatternSynonymType {
  Assign,
  If,
  While
};

enum class PatternType {
  Wildcard,
  DoubleWildcard,
  String,
  Synonym
};

enum class Location {
  Left,
  Right,
  Centre
};

#endif //AUTOTESTER_SRC_SPA_SRC_PQL_PATTERNTYPE_H_
