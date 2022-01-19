//
// Created by Kamil Gimik on 29/9/21.
//

#include "PkbPopulator.h"

PkbPopulator::PkbPopulator(PKB *pkb) {
  this->pkb = pkb;
}

void PkbPopulator::populateVarTable(Token *varToken) {
  this->pkb->addVariable(varToken->getValue());
}

void PkbPopulator::populateConstTable(Token *varToken) {
  this->pkb->addConst(varToken->getValue());
}

void PkbPopulator::populateProcTable(Token *procToken) {
  if (this->pkb->getProcList().count(procToken->getValue()) != 0) {
    throw std::runtime_error("SimpleParser: Two Procedures with same name error");
  }
  this->pkb->addProc(procToken->getValue());
}

void PkbPopulator::populateStmtTable(stmtType stmtType, int stmtNum, std::string procedure) {
  this->pkb->addStatement(stmtNum, stmtType, procedure);
}

void PkbPopulator::populateModifiesTable(Token *varToken, int stmtNum, std::string procedure) {
  this->pkb->addModifyStmt(stmtNum, varToken->getValue());
  this->pkb->addModifyProc(procedure, varToken->getValue());
}

void PkbPopulator::populateUsesTable(Token *varToken, int stmtNum, int prevIf, int prevWhile, std::string procedure) {
  this->pkb->addUseStmt(stmtNum, varToken->getValue());
  this->pkb->addUseProc(procedure, varToken->getValue());

  if (stmtNum == prevIf) {
    this->pkb->addIfCtrlVar(stmtNum, varToken->getValue());
  }
  if (stmtNum == prevWhile) {
    this->pkb->addWhileCtrlVar(stmtNum, varToken->getValue());
  }
}

void PkbPopulator::populateCallTable(Token *callToken, int stmtNum, std::string procedure) {
  this->pkb->addCall(stmtNum, procedure, callToken->getValue());
  populateStmtTable(call, stmtNum, procedure);
}

void PkbPopulator::populateIfTable(int ifStmtNum, int endStmtNum, int elseStmtNum, std::string procedure) {
  this->pkb->addIfStmt(ifStmtNum, endStmtNum, elseStmtNum);
  this->pkb->addStatement(ifStmtNum, ifStmt, procedure);
}

void PkbPopulator::populateWhileTable(int whileStmtNum, int endStmtNum, std::string procedure) {
  this->pkb->addWhileStmt(whileStmtNum, endStmtNum);
  this->pkb->addStatement(whileStmtNum, whileStmt, procedure);
}

void PkbPopulator::populateAssignTable(Token *lhs, std::vector<Token *> rhs, int stmtNum, std::string procedure) {
  std::string rhsString;
  for (Token *token: rhs) {
    if (token->getType() == TokenType::Semicolon) continue;
    rhsString += token->getValue();
  }
  this->pkb->addAssignPattern(stmtNum, lhs->getValue(), rhsString);
  populateStmtTable(assign, stmtNum, procedure);
}

void PkbPopulator::populateParentTable(int parent, int child) {
  this->pkb->addParent(parent, child);
}

void PkbPopulator::populateFollowsTable(int first, int second) {
  this->pkb->addFollow(first, second);
}

void PkbPopulator::populateNextTable(int first, int second) {
  this->pkb->addNext(first, second);
}

void PkbPopulator::populatePostFix(int stmt, std::string postFix) {
  this->pkb->addSubString(stmt, postFix);
}
