//
// Created by Kamil Gimik on 12/9/21.
//

#include <queue>
#include "DesignAbstractor.h"


DesignAbstractor::DesignAbstractor(PKB *pkb) {
  this->pkb = pkb;
}

bool DesignAbstractor::process() {
  this->populateParentStar();
  this->populateFollowStar();
  this->populateCallStar();
  this->updateDesignAbstractionsForCall();
  this->populateModifies();
  this->populateUses();
  this->populateNext();
  this->populateCfg();

  return true;
}

bool DesignAbstractor::populateParentStar() {

  std::unordered_set<int> childList = this->pkb->getChildList();

  for (int child: childList) {
    int parent = this->pkb->getParent(child);
    this->pkb->addParentStar(parent, child);
    while (this->pkb->isChild(parent)) {
      parent = this->pkb->getParent(parent);
      this->pkb->addParentStar(parent, child);
    }
  }

  return true;
}

bool DesignAbstractor::populateFollowStar() {

  std::unordered_set<int> followList = this->pkb->getAllFollowers();

  for (int follower: followList) {
    int prev = this->pkb->getFollowedBy(follower);
    this->pkb->addFollowStar(prev, follower);
    while (this->pkb->isFollowing(prev)) { // pkb needs to change for this, should be isBeingFollowed() given the rest of his definitions
      prev = this->pkb->getFollowedBy(prev);
      //prev = this->pkb->getFollower(prev);
      this->pkb->addFollowStar(prev, follower);
    }
  }

  return true;
}

void DesignAbstractor::populateCallStar() {

  std::unordered_set<int> callList = this->pkb->getCallStmts();
  std::unordered_set<std::string> procList = this->pkb->getProcList();

  std::unordered_set<std::string> callerProcs;

  for (int stmt: callList) {
    std::string caller = this->pkb->getCallerProc(stmt);
    std::string calledProc = this->pkb->getCalledProc(stmt);
    if (procList.count(calledProc) == 0) {
      throw std::runtime_error("Call stmt calls Procedure that does not exist");
    }
    callerProcs.insert(caller);
  }

  for (std::string caller: callerProcs) {
    this->bfsCallStar(caller);
  }

}

void DesignAbstractor::bfsCallStar(std::string caller) {

  std::unordered_set<std::string> visited;
  visited.insert(caller);
//<<<<<<< HEAD   __----CHECK LATER
//
//  std::queue<std::string> bfsQ;
//  bfsQ.push(caller);
//
//  while (!bfsQ.empty()) {
//
//    std::string vertex = bfsQ.front();
//    bfsQ.pop();
//
//    std::unordered_set<std::string> calledProcs = this->pkb->getProcsCalledByProc(vertex);
//
//    for (std::string calledProc: calledProcs) {
//
//      if (calledProc == caller) {
//        throw std::runtime_error("Cyclic Call exists between procedures");
//      }
//
//      visited.insert(calledProc);
//      bfsQ.push(calledProc);
//
//=======

  std::queue<std::string> bfsQ;
  bfsQ.push(caller);

  while (!bfsQ.empty()) {

    std::string vertex = bfsQ.front();
    bfsQ.pop();

    std::unordered_set<std::string> calledProcs = this->pkb->getProcsCalledByProc(vertex);

    for (std::string calledProc: calledProcs) {
      if (calledProc == caller) {
        throw std::runtime_error("Cyclic Call exists between procedures");
      }
      visited.insert(calledProc);
      bfsQ.push(calledProc);
      this->pkb->addCallStar(caller, calledProc);
    }
  }
}

void DesignAbstractor::updateDesignAbstractionsForCall() {

  this->updateProcRelationsForCall();
  this->updateStmtRelationsForCall();

}

void DesignAbstractor::updateProcRelationsForCall() {
  std::unordered_set<std::string> procList = this->pkb->getProcList();

  for (std::string proc: procList) {

    std::unordered_set<std::string> procsCalledStar = this->pkb->getProcsCalledStarByProc(proc);

    for (std::string procCalled: procsCalledStar) {
      std::unordered_set<std::string> usedVars = this->pkb->getVarUsedByProc(procCalled);
      for (std::string var: usedVars) {
        this->pkb->addUseProc(proc, var);
      }
      std::unordered_set<std::string> modifiedVars = this->pkb->getVarModifiedByProc(procCalled);
      for (std::string var: modifiedVars) {
        this->pkb->addModifyProc(proc, var);
      }
    }
  }
}

void DesignAbstractor::populateCfg() {

  std::unordered_set<std::string> procList = this->pkb->getProcList();
  for (std::string proc: procList) {
    std::unordered_map<int, std::unordered_set<int>> nextList = this->pkb->getNextListForProc(proc);
    // cfg population
    CFG *cfg = new CFG(nextList);
    cfg->setAssignList(this->pkb->getAssignList());
    cfg->setModifiesList(this->pkb->getStmtListofVar2());

    // cfg bip population
    auto nextListBip = nextList;
    //this->pkb->getNextList();
    this->pkb->addCFG(proc, cfg);
  }
  //TODO:: add bip into cfg

  // TODO:: REFACTOR LOGIC INTO ONE BIG BIP
  CFGBip *cfgBip = new CFGBip(this->pkb->getCombinedNextList());
  cfgBip->setAssignList(this->pkb->getAssignList());
  cfgBip->setModifiesList(this->pkb->getStmtListofVar2());
  // call statements setting
  cfgBip->setProcedureList(this->pkb->getAllProcedureWithStartAndEnd());
  cfgBip->setCallList(this->pkb->getAllCallStmts());
  cfgBip->turnToBip();

  this->pkb->addCFGBip(cfgBip);

}

void DesignAbstractor::populateNext() {
  this->updateNextFromFollows();
  this->updateNextFromParent();
  this->updateNextFromWhile();
  this->updateNextFromIf();
}

void DesignAbstractor::updateNextFromFollows() {

  std::unordered_set<int> followedStmts = this->pkb->getAllFollowedBy();

  for (int followed: followedStmts) {
    if (this->pkb->getStmtType(followed) != ifStmt) {
      int follower = this->pkb->getFollower(followed);
      this->pkb->addNext(followed, follower);
    }
  }
}

void DesignAbstractor::updateNextFromParent() {

  std::unordered_set<int> parentList = this->pkb->getParentList();

  for (int parent: parentList) {
    this->pkb->addNext(parent, parent + 1);
    if (this->pkb->getStmtType(parent) == ifStmt) {
      int secondChild = this->pkb->getElseStmt(parent);
      this->pkb->addNext(parent, secondChild);
    }
  }
}

void DesignAbstractor::updateNextFromWhile() {

  std::unordered_set<int> whileStmts = this->pkb->getAllWhile();

  for (int whileStm: whileStmts) {

    int lastStmt = this->pkb->getWhileEndStmt(whileStm);
    if (this->pkb->isParentPair(whileStm, lastStmt)) {
      this->pkb->addNext(lastStmt, whileStm);
    }

    if (this->pkb->isBeingFollowed(whileStm)) {
      continue;
    }

    int currStmt = whileStm;
    while (this->pkb->isChild(currStmt)) {
      int parent = this->pkb->getParent(currStmt);

      if (this->pkb->getStmtType(parent) == whileStmt) {
        this->pkb->addNext(whileStm, parent);
        break;
      }

      if (this->pkb->isBeingFollowed(parent)) {
        int follower = this->pkb->getFollower(parent);
        this->pkb->addNext(whileStm, follower);
        break;
      }

      currStmt = parent;
    }
  }
}

void DesignAbstractor::updateNextFromIf() {

  std::unordered_set<int> ifStmts = this->pkb->getAllIf();

  for (int ifStm: ifStmts) {

    int ifLastStmt = this->pkb->getElseStmt(ifStm) - 1;
    int elseLastStmt = this->pkb->getIfEndStmt(ifStm);     // check if pkb impl correctly
    std::vector<int> lastStmtVector = {ifLastStmt, elseLastStmt};

    for (int lastStmt: lastStmtVector) {

      if (this->pkb->isParentPair(ifStm, lastStmt)) {

        if (this->pkb->isBeingFollowed(ifStm)) { // ---------
          int follower = this->pkb->getFollower(ifStm);
          this->pkb->addNext(lastStmt, follower);
          continue;
        }

        int currStmt = ifStm;
        while (this->pkb->isChild(currStmt)) {
          int parent = this->pkb->getParent(currStmt);

          if (this->pkb->getStmtType(parent) == whileStmt) {
            this->pkb->addNext(lastStmt, parent);
            break;
          }

          if (this->pkb->isBeingFollowed(parent)) {  // -------
            int follower = this->pkb->getFollower(parent);
            this->pkb->addNext(lastStmt, follower);
            break;
          }

          currStmt = parent;
        }
      }
    }
  }
}
void DesignAbstractor::updateStmtRelationsForCall() {
  std::unordered_set<int> callList = this->pkb->getCallStmts();

  for (int stmt: callList) {

    std::string calledProc = this->pkb->getCalledProc(stmt);
    std::unordered_set<std::string> usedVars = this->pkb->getVarUsedByProc(calledProc);
    for (std::string var: usedVars) {
      this->pkb->addUseStmt(stmt, var);
    }
    std::unordered_set<std::string> modifiedVars = this->pkb->getVarModifiedByProc(calledProc);
    for (std::string var: modifiedVars) {
      this->pkb->addModifyStmt(stmt, var);
    }

  }
}

bool DesignAbstractor::populateModifies() {

  std::unordered_set<int> modifiesList = this->pkb->getAllModifies();

  for (int stmt: modifiesList) {
    if (!this->pkb->isChild(stmt)) {
      continue;
    }
    std::unordered_set<std::string> vars = this->pkb->getVarModifiedByStmt(stmt);
    int parent = this->pkb->getParent(stmt);
    for (std::string var: vars) {
      this->pkb->addModifyStmt(parent, var);
    }
    while (this->pkb->isChild(parent)) {
      parent = this->pkb->getParent(parent);
      for (std::string var: vars) {
        this->pkb->addModifyStmt(parent, var);
      }
    }
  }

  return true;
}

bool DesignAbstractor::populateUses() {

  std::unordered_set<int> usesList = this->pkb->getAllUse();

  for (int stmt: usesList) {
    if (!this->pkb->isChild(stmt)) {
      continue;
    }
    std::unordered_set<std::string> vars = this->pkb->getVarUsedByStmt(stmt);
    int parent = this->pkb->getParent(stmt);
    for (std::string var: vars) {
      this->pkb->addUseStmt(parent, var);
    }
    while (this->pkb->isChild(parent)) {
      parent = this->pkb->getParent(parent);
      for (std::string var: vars) {
        this->pkb->addUseStmt(parent, var);
      }
    }
  }

  return true;
}


