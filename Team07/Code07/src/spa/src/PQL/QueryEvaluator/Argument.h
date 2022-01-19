//
// Created by Jazer on 15/9/2021.
//

#ifndef AUTOTESTER_TEAM07_CODE07_SRC_SPA_SRC_PQL_ARGUMENT_H_
#define AUTOTESTER_TEAM07_CODE07_SRC_SPA_SRC_PQL_ARGUMENT_H_

#include "PatternType.h"
#include "ArgumentType.h"
#include <string>

class Argument {
  ArgumentType type;
  std::string value;

 public:
  explicit Argument(std::string argumentValue);
  ArgumentType getType();
  std::string getValue();
  std::string toString();

};

#endif //AUTOTESTER_TEAM07_CODE07_SRC_SPA_SRC_PQL_ARGUMENT_H_
