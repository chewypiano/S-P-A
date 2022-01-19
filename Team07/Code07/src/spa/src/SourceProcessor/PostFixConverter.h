//
// Created by Sean Chan  on 28/9/21.
//

#ifndef AUTOTESTER_POSTFIXCONVERTER_H
#define AUTOTESTER_POSTFIXCONVERTER_H

#include "Token.h"
#include "Tokenizer.h"

namespace PostFixConverter {
std::string toPostFix(std::vector<Token *> tokens);
std::string toPostFix(std::string expression);
};

#endif //AUTOTESTER_POSTFIXCONVERTER_H
