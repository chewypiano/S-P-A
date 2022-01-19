//
// Created by Kamil Gimik on 29/9/21.
//

#ifndef TEAM_07_PKBPOPULATOR_H
#define TEAM_07_PKBPOPULATOR_H

#include <Token.h>
#include "PKB/PKB.h"

class PkbPopulator {

  PKB *pkb;

 public:

  PkbPopulator(PKB *pkb);

  void populateVarTable(Token *varToken);
  void populateConstTable(Token *varToken);
  void populateProcTable(Token *procToken);
  void populateStmtTable(stmtType stmtType, int stmtNum, std::string procedure);
  void populateModifiesTable(Token *varToken, int stmtNum, std::string procedure);
  void populateUsesTable(Token *varToken, int stmtNum, int prefIf, int prevWhile, std::string procedure);
  void populateCallTable(Token *callToken, int stmtNum, std::string procedure);
  void populateIfTable(int ifStmtNum, int endStmtNum, int elseStmtNum, std::string procedure);
  void populateWhileTable(int whileStmtNum, int endStmtNum, std::string procedure);
  void populateAssignTable(Token *lhs, std::vector<Token *> rhs, int stmtNum, std::string procedure);
  void populateParentTable(int parent, int child);
  void populateFollowsTable(int first, int second);
  void populateNextTable(int first, int second);
  void populatePostFix(int stmt, std::string postFix);

};

#endif //TEAM_07_PKBPOPULATOR_H
