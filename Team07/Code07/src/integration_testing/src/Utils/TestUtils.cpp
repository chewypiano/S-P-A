//
// Created by Junhua on 4/11/2021.
//

#include "TestUtils.h"

Row createRow(const std::vector<std::pair<QuerySynonym, std::string>> &row) {
  Row r = Row();
  for (auto p: row) {
    r.addSynonym(p.first, p.second);
  }
  return r;
}

RowList createRowList(const std::vector<std::vector<std::pair<QuerySynonym, std::string>>> &rows) {
  RowList r = RowList();
  for (auto &row: rows) {
    r.emplace_back(createRow(row));
  }
  return r;
}

PKB *generateAffectsPKB() {
  auto pkb = new PKB();
  pkb->addProc("eg");

  // a = a
  pkb->addVariable("a");
  pkb->addStatement(1, assign, "eg");
  pkb->addModifyStmt(1, "a");
  pkb->addUseStmt(1, "a");

  // b = a
  pkb->addVariable("b");
  pkb->addStatement(2, assign, "eg");
  pkb->addModifyStmt(2, "b");
  pkb->addUseStmt(2, "a");

  // c = b + a
  pkb->addVariable("c");
  pkb->addStatement(3, assign, "eg");
  pkb->addModifyStmt(3, "c");
  pkb->addUseStmt(3, "b");
  pkb->addUseStmt(3, "a");

  pkb->addNext(1, 2);
  pkb->addNext(2, 3);
  auto cfg = new CFG(pkb->getNextListForProc("eg"));
  cfg->setAssignList(pkb->getAssignList());
  cfg->setModifiesList(pkb->getStmtListofVar2());
  pkb->addCFG("eg", cfg);
  return pkb;
}

PKB *generateNoAffectsPKB() {
  auto pkb = new PKB();
  pkb->addProc("eg");

  // a = a
  pkb->addVariable("a");
  pkb->addStatement(1, assign, "eg");
  pkb->addModifyStmt(1, "a");
  pkb->addUseStmt(1, "a");

  // b = a
  pkb->addVariable("b");
  pkb->addStatement(2, assign, "eg");
  pkb->addModifyStmt(2, "b");
  pkb->addUseStmt(2, "b");

  // c = b + a
  pkb->addVariable("c");
  pkb->addStatement(3, assign, "eg");
  pkb->addModifyStmt(3, "c");
  pkb->addUseStmt(3, "c");

  pkb->addNext(1, 2);
  pkb->addNext(2, 3);
  auto cfg = new CFG(pkb->getNextListForProc("eg"));
  cfg->setAssignList(pkb->getAssignList());
  cfg->setModifiesList(pkb->getStmtListofVar2());
  pkb->addCFG("eg", cfg);
  return pkb;
}

PKB *generateBipPKB() {
  auto pkb = new PKB();
  pkb->addProc("eg");
  pkb->addProc("eg2");

  // a = a
  pkb->addVariable("a");
  pkb->addStatement(1, assign, "eg");
  pkb->addModifyStmt(1, "a");
  pkb->addUseStmt(1, "a");
  pkb->addAssignPattern(1, "a", "a");

  // calls eg2;
  pkb->addStatement(2, call, "eg");
  pkb->addCall(2, "eg", "eg2");
  pkb->addModifyStmt(2, "b");
  pkb->addModifyStmt(2, "c");
  pkb->addUseStmt(2, "a");
  pkb->addUseStmt(2, "b");

  // b = a
  pkb->addVariable("b");
  pkb->addStatement(3, assign, "eg2");
  pkb->addModifyStmt(3, "b");
  pkb->addUseStmt(3, "a");
  pkb->addAssignPattern(3, "b", "a");

  // c = b + a
  pkb->addVariable("c");
  pkb->addStatement(4, assign, "eg2");
  pkb->addModifyStmt(4, "c");
  pkb->addUseStmt(4, "b");
  pkb->addUseStmt(4, "a");
  pkb->addAssignPattern(4, "a", "b a +");

  pkb->addNext(1, 2);
  pkb->addNext(3, 4);

  auto cfg = new CFGBip(pkb->getCombinedNextList());
  cfg->setAssignList(pkb->getAssignList());
  cfg->setModifiesList(pkb->getStmtListofVar2());
  cfg->setCallList(pkb->getAllCallStmts());
  cfg->setProcedureList(pkb->getAllProcedureWithStartAndEnd());
  cfg->turnToBip();

  pkb->addCFGBip(cfg);
  return pkb;
}

PKB *generateNoNextBipPKB() {
  auto pkb = new PKB();
  pkb->addProc("eg");
  pkb->addProc("eg2");

  // a = a
  pkb->addVariable("a");
  pkb->addStatement(1, assign, "eg");
  pkb->addModifyStmt(1, "a");
  pkb->addUseStmt(1, "a");

  // b = a
  pkb->addVariable("b");
  pkb->addStatement(2, assign, "eg2");
  pkb->addModifyStmt(2, "b");
  pkb->addUseStmt(2, "a");

  auto cfg = new CFGBip(pkb->getCombinedNextList());
  cfg->setAssignList(pkb->getAssignList());
  cfg->setModifiesList(pkb->getStmtListofVar2());
  cfg->setCallList(pkb->getAllCallStmts());
  cfg->setProcedureList(pkb->getAllProcedureWithStartAndEnd());
  cfg->turnToBip();
  pkb->addCFGBip(cfg);
  return pkb;
}

PKB *generateNoAffectsBipPKB() {
  auto pkb = new PKB();
  pkb->addProc("eg");
  pkb->addProc("eg2");

  // a = 1
  pkb->addVariable("a");
  pkb->addStatement(1, assign, "eg");
  pkb->addModifyStmt(1, "a");

  // b = 2
  pkb->addVariable("b");
  pkb->addStatement(2, assign, "eg2");
  pkb->addModifyStmt(2, "b");

  auto cfg = new CFGBip(pkb->getCombinedNextList());
  cfg->setAssignList(pkb->getAssignList());
  cfg->setModifiesList(pkb->getStmtListofVar2());
  cfg->setCallList(pkb->getAllCallStmts());
  cfg->setProcedureList(pkb->getAllProcedureWithStartAndEnd());
  cfg->turnToBip();
  pkb->addCFGBip(cfg);
  return pkb;
}
