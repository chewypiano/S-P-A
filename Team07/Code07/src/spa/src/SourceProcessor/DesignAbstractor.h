//
// Created by Kamil Gimik on 12/9/21.
//

#include <SimpleParser.h>
#include <CFG.h>
#include <CFGBip.h>

#ifndef AUTOTESTER_DESIGNEXTRACTOR_H
#define AUTOTESTER_DESIGNEXTRACTOR_H

class DesignAbstractor {

  PKB *pkb;

 private:

  void updateNextFromFollows();
  void updateNextFromParent();
  void updateNextFromWhile();
  void updateNextFromIf();

  // perform bfs on call graph/table to populate call star table
  void bfsCallStar(std::string caller);

  // update modifies/uses for call stmt and procedure due to call/*
  void updateDesignAbstractionsForCall();
  void updateProcRelationsForCall();
  void updateStmtRelationsForCall();

  // core logic in populating relations in pkb
  bool populateParentStar();
  bool populateFollowStar();
  void populateCallStar();
  bool populateUses();
  bool populateModifies();
  void populateNext();
  void populateCfg();

 public:

  explicit DesignAbstractor(PKB *pkb);

  // adds all the complex relationships in the simple program to PKB
  bool process();

};

#endif //AUTOTESTER_DESIGNEXTRACTOR_H
