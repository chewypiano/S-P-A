//
// Created by Junhua on 28/8/2021.
//

#include "TokenType.h"
#include <string>

std::unordered_map<TokenType, std::string> TokenToName::convertTable = {
    {LBrace, "LBrace"},
    {RBrace, "RBrace"},
    {LBracket, "LBracket"},
    {RBracket, "RBracket"},
    {Number, "Number"},
    {Space, "Space"},
    {Variable, "Variable"},
    {Semicolon, "Semicolon"},
    {Assign, "Assign"},
    {Wildcard, "Wildcard"},
    {Such, "Such"},
    {That, "That"},
    {Select, "Select"},
    {Follow, "Follow"},
    {Parent, "Parent"},
    {FollowT, "Follow*"},
    {ParentT, "Parent*"},
    {Uses, "Uses"},
    {Modifies, "Modifies"},
    {Pattern, "Pattern"},
    {Ignore, "Ignore"},
    {Comma, "Comma"},
    {String, "String"},
    {Statement, "Statement"},
    {Procedure, "Procedure"},
    {Read, "Read"},
    {Print, "Print"},
    {Call, "Call"},
    {While, "While"},
    {If, "If"},
    {Constant, "Constant"},
    {Integer, "Integer"},
    {Name, "Name"},
    {Arithmetic, "Arithmetic"},
    {Logical, "Logical"},
    {Relative, "Relative"},
    {Equal, "Equal"},
    {Error, "Error"},
    {Equals, "Equals"},
    {Negate, "Negate"},
    {With, "With"},
    {And, "And"},
    {Boolean, "Boolean"},
    {LAngleBracket, "<"},
    {RAngleBracket, ">"},
    {Calls, "Calls"},
    {CallsT, "Calls*"},
    {Next, "Next"},
    {NextT, "Next*"},
    {VarName, "VarName"},
    {ProcName, "ProcName"},
    {StmtNo, "StmtNo"},
    {Value, "Value"},
    {ProgLine, "ProgLine"},
    {Affects, "Affects"},
    {AffectsT, "Affects*"},
    {NextBip, "NextBip"},
    {NextBipT, "NextBip*"},
    {AffectsBip, "AffectsBip"},
};

std::string TokenToName::tokenToName(TokenType token) {
  return TokenToName::convertTable[token];
}