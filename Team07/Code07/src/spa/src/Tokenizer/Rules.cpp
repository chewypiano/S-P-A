//
// Created by Junhua on 31/8/2021.
//

#include <regex>
#include <vector>
#include "TokenType.h"
#include "Rules.h"

std::vector<Rule> Rules::pqlRules = {
    Rule(new std::regex("(^\\s+)"), Ignore),
    Rule(new std::regex("(^\\{)"), LBrace),
    Rule(new std::regex("(^\\})"), RBrace),
    Rule(new std::regex("(^\\()"), LBracket),
    Rule(new std::regex("(^\\))"), RBracket),
    Rule(new std::regex("(^=)"), Equal),
    Rule(new std::regex("(^;)"), Semicolon),
    Rule(new std::regex("(^_)"), Wildcard),
    Rule(new std::regex("(^,)"), Comma),
    Rule(new std::regex("(^<)"), LAngleBracket),
    Rule(new std::regex("(^>)"), RAngleBracket),
    Rule(new std::regex("(^Select$)"), Select),
    Rule(new std::regex("(^with$)"), With),
    Rule(new std::regex("(^and$)"), And),
    Rule(new std::regex("(^\\.procName(?![A-Za-z0-9]))"), ProcName),
    Rule(new std::regex("(^\\.varName(?![A-Za-z0-9]))"), VarName),
    Rule(new std::regex("(^\\.value(?![A-Za-z0-9]))"), Value),
    Rule(new std::regex("(^\\.stmt#(?![A-Za-z0-9]))"), StmtNo),
    Rule(new std::regex("(^constant$)"), Constant),
    Rule(new std::regex("(^prog_line)"), ProgLine),
    Rule(new std::regex("(^stmt$)"), Statement),
    Rule(new std::regex("(^assign$)"), Assign),
    Rule(new std::regex("(^such$)"), Such),
    Rule(new std::regex("(^that$)"), That),
    Rule(new std::regex("(^pattern$)"), Pattern),
    Rule(new std::regex("(^variable$)"), Variable),
    Rule(new std::regex("(^BOOLEAN$)"), Boolean),
    Rule(new std::regex("(^Parent\\*(?![A-Za-z0-9]))"), ParentT),
    Rule(new std::regex("(^Parent(?![A-Za-z0-9]))"), Parent),
    Rule(new std::regex("(^Calls\\*(?![A-Za-z0-9]))"), CallsT),
    Rule(new std::regex("(^Calls(?![A-Za-z0-9]))"), Calls),
    Rule(new std::regex("(^Next\\*(?![A-Za-z0-9]))"), NextT),
    Rule(new std::regex("(^Next(?![A-Za-z0-9]))"), Next),
    Rule(new std::regex("(^Follows\\*(?![A-Za-z0-9]))"), FollowT),
    Rule(new std::regex("(^Follows(?![A-Za-z0-9]))"), Follow),
    Rule(new std::regex("(^Affects\\*(?![A-Za-z0-9]))"), AffectsT),
    Rule(new std::regex("(^Affects(?![A-Za-z0-9]))"), Affects),
    Rule(new std::regex("(^NextBip\\*(?![A-Za-z0-9]))"), NextBipT),
    Rule(new std::regex("(^NextBip(?![A-Za-z0-9]))"), NextBip),
    Rule(new std::regex("(^AffectsBip(?![A-Za-z0-9]))"), AffectsBip),
    Rule(new std::regex("(^Uses(?![A-Za-z0-9]))"), Uses),
    Rule(new std::regex("(^Modifies(?![A-Za-z0-9]))"), Modifies),
    Rule(new std::regex("(^procedure$)"), Procedure),
    Rule(new std::regex("(^read$)"), Read),
    Rule(new std::regex("(^print$)"), Print),
    Rule(new std::regex("(^call$)"), Call),
    Rule(new std::regex("(^while$)"), While),
    Rule(new std::regex("(^if$)"), If),
    Rule(new std::regex("^([1-9][0-9]*)"), Integer),
    Rule(new std::regex("^(0)(?![0-9])"), Integer),
    Rule(new std::regex(R"(^("[^"]+")$)"), String),
    Rule(new std::regex("(^[A-Za-z][A-Za-z0-9]*)"), Name),
};

std::vector<Rule> Rules::simpleRules = {
    Rule(new std::regex("(^\\s+)"), Ignore),
    Rule(new std::regex("(^\\{)"), LBrace),
    Rule(new std::regex("(^\\})"), RBrace),
    Rule(new std::regex("(^\\()"), LBracket),
    Rule(new std::regex("(^\\))"), RBracket),
    Rule(new std::regex("^(;)"), Semicolon),
    Rule(new std::regex("^(\\&\\&)"), Logical),
    Rule(new std::regex("^(\\|\\|)"), Logical),
    Rule(new std::regex("^(>=)"), Relative),
    Rule(new std::regex("^(<=)"), Relative),
    Rule(new std::regex("^(==)"), Relative),
    Rule(new std::regex("^(\\!\\=)"), Relative),
    Rule(new std::regex("^(>)"), Relative),
    Rule(new std::regex("^(<)"), Relative),
    Rule(new std::regex(R"(^([\+\-\*\/\%]))"), Arithmetic),
    Rule(new std::regex("^(=)"), Equal),
    Rule(new std::regex("^([1-9][0-9]*)"), Number),
    Rule(new std::regex("^(0)(?![0-9])"), Number),
    Rule(new std::regex("^([A-Za-z][A-Za-z0-9]*)"), Name),
    Rule(new std::regex("^(!)"), Negate),

};

std::vector<Rule> Rules::getSimpleRules() {
  return simpleRules;
}

std::vector<Rule> Rules::getPQLRules() {
  return pqlRules;
}