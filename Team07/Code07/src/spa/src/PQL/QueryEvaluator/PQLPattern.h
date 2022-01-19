//
// Created by Junhua on 14/9/2021.
//

#ifndef AUTOTESTER_SRC_SPA_SRC_PQL_PQLPATTERN_H_
#define AUTOTESTER_SRC_SPA_SRC_PQL_PQLPATTERN_H_

#include "PatternType.h"
#include <string>

class PQLPattern {
  PatternType type;
  std::string value;

 public:
  explicit PQLPattern(std::string patternValue, Location loc);
  PatternType getType();
  std::string getValue();
  std::string toString();
  std::string getInner();
};

#endif //AUTOTESTER_SRC_SPA_SRC_PQL_PQLPATTERN_H_
