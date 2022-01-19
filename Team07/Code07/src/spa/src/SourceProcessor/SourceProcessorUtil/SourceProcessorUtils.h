//
// Created by Kamil Gimik on 29/9/21.
//

#ifndef TEAM_07_SOURCEPROCESSORUTILS_H
#define TEAM_07_SOURCEPROCESSORUTILS_H

#include "Token.h"
#include <vector>
#include <unordered_set>
#include <string>

namespace SourceProcessorUtils {
bool hasNoExtraBrackets(std::vector<Token *> expr);
bool isEnclosedProperly(std::vector<Token *> expr);
bool expectType(Token *tokenToCheck, TokenType expectedType);
bool expectType(Token *tokenToCheck, std::unordered_set<TokenType> expectedTypes);

};

#endif //TEAM_07_SOURCEPROCESSORUTILS_H
