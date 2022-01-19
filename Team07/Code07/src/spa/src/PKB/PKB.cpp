

#include "PKB.h"

PKB::PKB() {}


/*
 Adder Methods
 */

//Stores all the statements that belong to a procedure in a list
void PKB::addStatement(int stmtNum, stmtType type, std::string procedure) {
  stmtProcList.emplace(stmtNum, procedure);
  stmtTypeList.emplace(stmtNum, type);
  //Check if the procedure already exists in the stmtMap, if the procedure exists, adds the statement
  //number to the end of the list
  if (!stmtMap.emplace(procedure, std::unordered_set<int>{stmtNum}).second) {
    stmtMap.find(procedure)->second.emplace(stmtNum);
  }

  switch (type) {
    case print:
      printStmtList.emplace(stmtNum);
      break;
    case read:
      readStmtList.emplace(stmtNum);
      break;
    case assign:
      assignStmtList.emplace(stmtNum);
      break;
    case call:
      callStmtList.emplace(stmtNum);
      break;
    case ifStmt:
      ifStmtList.emplace(stmtNum);
      break;
    case whileStmt:
      whileStmtList.emplace(stmtNum);
      break;
    case nullValue:
      break;
  }
}

void PKB::addProc(std::string procName) {
  procList.emplace(procName);
}

void PKB::addVariable(std::string varName) {
  varList.emplace(varName);
}

void PKB::addConst(std::string constName) {
  constList.emplace(constName);
}

/*
 Getter Methods
 */
std::unordered_set<std::string> PKB::getVarList() {
  return varList;
}

std::unordered_set<std::string> PKB::getConstList() {
  return constList;
}

std::unordered_set<std::string> PKB::getProcList() {
  return procList;
}

std::unordered_set<int> PKB::getReadStmts() {
  return readStmtList;
}

std::unordered_set<int> PKB::getPrintStmts() {
  return printStmtList;
}

std::unordered_set<int> PKB::getAssignStmts() {
  return assignStmtList;
}

std::unordered_set<int> PKB::getCallStmts() {
  return callStmtList;
}

std::unordered_set<int> PKB::getAllStmt() {
  std::unordered_set<int> allStmt;
  for (auto kv: stmtTypeList) {
    allStmt.emplace(kv.first);
  }
  return allStmt;
}

//Returns all the statement in a specified procedure
std::unordered_set<int> PKB::getStmtList(std::string procedure) {
  if (stmtMap.find(procedure) != stmtMap.end()) {
    return stmtMap.at(procedure);
  }
  return {};
}

//Returns the procedure that the statement is nested in, else returns empty string
std::string PKB::getStmtProc(int stmtNum) {
  if (stmtProcList.find(stmtNum) != stmtProcList.end()) {
    return stmtProcList.at(stmtNum);
  }
  return "";
}

//Returns the statement type
stmtType PKB::getStmtType(int stmtNum) {
  if (stmtTypeList.find(stmtNum) != stmtTypeList.end()) {
    return stmtTypeList.at(stmtNum);
  }
  return stmtType::nullValue;
}

//Returns the total number of statements in the SIMPLE program
int PKB::getTotalStmtNum() {
  return stmtTypeList.size();
}

//Returns a list containing pairs of <stmtNum, Var>
std::unordered_map<int, std::string> PKB::getReadList() {
  return readPairList;
}

//Returns a list containing pairs of <stmtNum, Var>
std::unordered_map<int, std::string> PKB::getPrintList() {
  return printPairList;
}

std::unordered_map<int, std::pair<std::string, std::unordered_set<std::string>>> PKB::getAssignList() {
  this->getAssignStmts();
  for (auto ev: this->getAssignStmts()) {
    if (!this->getVarUsedByStmt(ev).empty()) {
      std::pair<std::string, std::unordered_set<std::string>> res;
      res = {(*this->getVarModifiedByStmt(ev).begin()), this->getVarUsedByStmt(ev)};
      this->affectsList.emplace(ev, res);
    } else {
      std::pair<std::string, std::unordered_set<std::string>> res;
      res = {(*this->getVarModifiedByStmt(ev).begin()), {}};
      this->affectsList.emplace(ev, res);
    }
  }
  return this->affectsList;
}

std::unordered_map<std::string, std::pair<int, std::unordered_set<int>>> PKB::getAllProcedureWithStartAndEnd() {
  std::unordered_map<std::string, std::pair<int, std::unordered_set<int>>> resMap1;
  std::unordered_set<std::string> localProcList = this->getProcList();
  for (const auto &kv: localProcList) {
    int min = INT32_MAX, max = 0;
    std::unordered_set<int> maxList;
    for (auto stmt: getStmtList(kv)) {
      if (stmt < min) {
        min = stmt;
      }
      if (getNextStatementsOf(stmt).empty()) {
        maxList.emplace(stmt);
      }
    }
    resMap1.emplace(kv, std::pair(min, maxList));
  }
  return resMap1;
}

/*
 * Pattern adders and getters
 */

bool PKB::addAssignPattern(int stmtNum, std::string LHS, std::string RHS) {
  return patternComponent.addAssignPattern(stmtNum, LHS, RHS);
}

std::string PKB::getLHSAssign(int stmtNum) {
  return patternComponent.getLHSAssign(stmtNum);
}

std::string PKB::getRHSAssign(int stmtNum) {
  return patternComponent.getRHSAssign(stmtNum);
}

void PKB::addSubString(int stmtNum, std::string subString) {
  patternComponent.addSubString(stmtNum, subString);
}

std::unordered_set<std::string> PKB::subStringsAt(int stmtNum) {
  return patternComponent.subStringsAt(stmtNum);
}

std::unordered_map<int, std::pair<std::string, std::string>> PKB::getAllPatternsAssignStmts() {
  return patternComponent.getAllPatternsAssignStmts();
}

std::unordered_map<int, std::unordered_set<std::string>> PKB::getAllIfWhilePatterns() {
  return patternComponent.getAllIfWhilePatterns();
}

/*
 * Block Control for both adders and getters
 */

bool PKB::addIfStmt(int startStmtNum, int lastStmtNum, int elseStmtNum) {
  return blockStorage.addIfStmt(startStmtNum, lastStmtNum, elseStmtNum);
}

bool PKB::addWhileStmt(int startStmtNum, int lastStmtNum) {
  return blockStorage.addWhileStmt(startStmtNum, lastStmtNum);
}

void PKB::addIfCtrlVar(int startStmtNum, std::string ctrlVar) {
  blockStorage.addIfBlock(startStmtNum, ctrlVar);
}

void PKB::addWhileCtrlVar(int startStmtNum, std::string ctrlVar) {
  blockStorage.addWhileBlock(startStmtNum, ctrlVar);
}

int PKB::getWhileEndStmt(int stmtNum) {
  return blockStorage.getWhileEndStmt(stmtNum);
}

int PKB::getElseStmt(int stmtNum) {
  return blockStorage.getElseStmt(stmtNum);
}

int PKB::getIfEndStmt(int stmtNum) {
  return blockStorage.getIfEndStmt(stmtNum);
}

std::unordered_set<int> PKB::getAllIf() {
  return blockStorage.getAllIfBlock();
}

std::unordered_set<int> PKB::getIfStmts() {
  return blockStorage.getAllIfStmts();
}

std::unordered_set<int> PKB::getAllWhile() {
  return blockStorage.getAllWhileBlock();
}

std::unordered_set<int> PKB::getWhileStmts() {
  return blockStorage.getAllWhileStmts();
}

std::unordered_set<int> PKB::getIfCtrlVar(std::string ctrlVar) {
  return blockStorage.getAllIfCtrlVar(ctrlVar);
}

std::unordered_set<int> PKB::getWhileCtrlVar(std::string ctrlVar) {
  return blockStorage.getAllWhileCtrlVar(ctrlVar);
}

/*
 * Call Relations for both adders and getters
 */

bool PKB::addCall(int stmtNum, std::string callerProc, std::string calledProc) {
  return callComponent.addCall(stmtNum, callerProc, calledProc);
}

bool PKB::addCallStar(std::string callerProc, std::string calledProc) {
  return callComponent.addCallStar(callerProc, calledProc);
}

std::string PKB::getCallerProc(int stmtNum) {
  return callComponent.getCallerProc(stmtNum);
}

std::string PKB::getCalledProc(int stmtNum) {
  return callComponent.getCalledProc(stmtNum);
}

std::unordered_set<std::string> PKB::getProcsCalledFromProc(std::string procName) {
  return callComponent.getProcsCalledFromProc(procName);
}

std::unordered_set<std::string> PKB::getProcsCalledStarFromProc(std::string procName) {
  return callComponent.getProcsCalledStarFromProc(procName);
}

std::unordered_set<std::string> PKB::getProcsCalledByProc(std::string procName) {
  return callComponent.getProcsCalledByProc(procName);
}

std::unordered_set<std::string> PKB::getProcsCalledStarByProc(std::string procName) {
  return callComponent.getProcsCalledStarByProc(procName);
}

std::unordered_map<int, std::string> PKB::getAllCallStmts() {
  return callComponent.getAllCallStmts();
}

bool PKB::isCallRelation(std::string callerProc, std::string calledProc) {
  return callComponent.isCallRelation(callerProc, calledProc);
}

bool PKB::isCallStarRelation(std::string callerProc, std::string calledProc) {
  return callComponent.isCallStarRelation(callerProc, calledProc);
}

/*
 * Modify relations for both adders and getters
 */

bool PKB::addModifyStmt(int stmtNum, std::string var) {
  //Checks if inputs are invalid
  if (var.length() == 0 || stmtNum <= 0) {
    return false;
  }
  //Checks if the modifying stmt is a read statement
  if (readStmtList.find(stmtNum) != readStmtList.end()) {
    readPairList.emplace(stmtNum, var);
  }
  return modifyComponent.addModifyStmt(stmtNum, var);
}

bool PKB::addModifyProc(std::string procName, std::string var) {
  if (var.length() == 0 || procName.length() == 0) {
    return false;
  }
  return modifyComponent.addModifyProc(procName, var);
}

bool PKB::isStmtModifyingVar(int stmtNum, std::string var) {
  return modifyComponent.isStmtVarPair(std::pair<int, std::string>(stmtNum, var));
}

bool PKB::isProcModifyingVar(std::string procName, std::string var) {
  return modifyComponent.isProcVarPair(std::pair<std::string, std::string>(procName, var));
}

std::unordered_set<std::string> PKB::getVarModifiedByStmt(int stmtNum) {
  return modifyComponent.getVarModifiedByStmt(stmtNum);
}

std::unordered_set<std::string> PKB::getVarModifiedByProc(std::string procName) {
  return modifyComponent.getVarModifiedByProc(procName);
}

std::unordered_set<int> PKB::getStmtModifyingVar(std::string var) {
  return modifyComponent.getStmtModifyingVar(var);
}

std::unordered_set<std::string> PKB::getProcModifyingVar(std::string var) {
  return modifyComponent.getProcModifyingVar(var);
}

std::unordered_set<int> PKB::getAllModifies() {
  return modifyComponent.getAllModifies();
}

std::unordered_set<std::string> PKB::getAllModifiesProc() {
  return modifyComponent.getAllModifiesProc();
}

std::unordered_map<int, std::unordered_set<std::string>> PKB::getStmtListofVar() {
  return modifyComponent.getStmtListofVar();
}

std::unordered_map<int, std::unordered_set<std::string>> PKB::getStmtListofVar2() {
  std::unordered_map<int, std::unordered_set<std::string>> res = this->getStmtListofVar();
  std::unordered_set<int> keys;
  for (auto ev: this->getStmtListofVar()) {
    keys.emplace(ev.first);
  }
  for (auto ev: this->getIfStmts()) {
    if (keys.find(ev) != keys.end()) {
      res.erase(ev);
    }
  }
  for (auto ev: this->getWhileStmts()) {
    if (keys.find(ev) != keys.end()) {
      res.erase(ev);
    }
  }
  return res;
}

/*
 * Uses relations for both adders and getters
 */

bool PKB::addUseStmt(int stmtNum, std::string var) {
  //Checks if inputs are invalid
  if (var.length() == 0 || stmtNum <= 0) {
    return false;
  }
  //Checks if the modifying stmt is a read statement
  if (printStmtList.find(stmtNum) != printStmtList.end()) {
    printPairList.emplace(stmtNum, var);
  }
  return useComponent.addUseStmt(stmtNum, var);
}

bool PKB::addUseProc(std::string procName, std::string var) {
  if (var.length() == 0 || procName.length() == 0) {
    return false;
  }
  return useComponent.addUseProc(procName, var);
}

bool PKB::isStmtUsingVar(int stmtNum, std::string var) {
  return useComponent.isStmtVarPair(std::pair<int, std::string>(stmtNum, var));
}

bool PKB::isProcUsingVar(std::string procName, std::string var) {
  return useComponent.isProcVarPair(std::pair<std::string, std::string>(procName, var));
}

std::unordered_set<std::string> PKB::getVarUsedByStmt(int stmtNum) {
  return useComponent.getVarUsedByStmt(stmtNum);
}

std::unordered_set<std::string> PKB::getVarUsedByProc(std::string procName) {
  return useComponent.getVarUsedByProc(procName);
}

std::unordered_set<int> PKB::getStmtUsingVar(std::string var) {
  return useComponent.getStmtUsingVar(var);
}

std::unordered_set<std::string> PKB::getProcUsingVar(std::string var) {
  return useComponent.getProcUsingVar(var);
}

std::unordered_set<int> PKB::getAllUse() {
  return useComponent.getAllUseStmt();
}

/*
 * Parent relations for both adders and getters
 */

bool PKB::addParent(int stmtParent, int stmtChild) {
  if (stmtParent >= stmtChild || (stmtParent <= 0) || (stmtChild <= 0)) {
    return false;
  }
  return parentComponent.addParent(stmtParent, stmtChild);
}

bool PKB::addParentStar(int stmtParent, int stmtChild) {
  if (stmtParent >= stmtChild || (stmtParent <= 0) || (stmtChild <= 0)) {
    return false;
  }
  return parentComponent.addParentStar(stmtParent, stmtChild);
}

bool PKB::isParent(int stmtNum) {
  return parentComponent.isParent(stmtNum);
}

bool PKB::isChild(int stmtNum) {
  return parentComponent.isChild(stmtNum);
}

int PKB::getParent(int stmtNum) {
  return parentComponent.getParent(stmtNum);
}

std::unordered_set<int> PKB::getParentStar(int stmtNum) {
  return parentComponent.getParentStar(stmtNum);
}

std::unordered_set<int> PKB::getChild(int stmtNum) {
  return parentComponent.getChild(stmtNum);
}

std::unordered_set<int> PKB::getChildStar(int stmtNum) {
  return parentComponent.getChildStar(stmtNum);
}

std::unordered_set<int> PKB::getParentList() {
  return parentComponent.getParentList();
}

std::unordered_set<int> PKB::getChildList() {
  return parentComponent.getChildList();
}

bool PKB::isParentPair(int s1, int s2) {
  return parentComponent.isParentPair(s1, s2);
}

bool PKB::isParentStarPair(int s1, int s2) {
  return parentComponent.isParentStarPair(s1, s2);
}

/*
 * Follow relations for both adders and getters
 */

bool PKB::addFollow(int stmt1, int stmt2) {
  if (stmt1 >= stmt2 || (stmt1 <= 0) || (stmt2 <= 0)) {
    return false;
  }
  return followComponent.addFollow(stmt1, stmt2);
}

bool PKB::addFollowStar(int stmt1, int stmt2) {
  if (stmt1 >= stmt2 || (stmt1 <= 0) || (stmt2 <= 0)) {
    return false;
  }
  return followComponent.addFollowStar(stmt1, stmt2);
}

bool PKB::isFollowing(int stmtNum) {
  return followComponent.isBeingFollowed(stmtNum);
}

bool PKB::isBeingFollowed(int stmtNum) {
  return followComponent.isFollowing(stmtNum);
}

int PKB::getFollowedBy(int s2) {
  return followComponent.getFollowedBy(s2);
}

int PKB::getFollower(int s1) {
  return followComponent.getFollower(s1);
}

std::unordered_set<int> PKB::getFollowerStar(int s1) {
  return followComponent.getFollowerStar(s1);
}

std::unordered_set<int> PKB::getFollowedByStar(int s2) {
  return followComponent.getFollowedByStar(s2);
}

std::unordered_set<int> PKB::getAllFollowers() {
  return followComponent.getAllFollowers();
}

std::unordered_set<int> PKB::getAllFollowedBy() {
  return followComponent.getAllFollowedBy();
}

bool PKB::isFollow(int s1, int s2) {
  return followComponent.isFollow(s1, s2);
}

bool PKB::isFollowStar(int s1, int s2) {
  return followComponent.isFollowStar(s1, s2);
}

/*
 * Next relations for both adders and getters
 */

bool PKB::addNext(int stmt1, int stmt2) {
  return nextComponent.addNext(stmt1, stmt2);
}

std::unordered_set<int> PKB::getNextStatementsOf(int stmtNum) {
  return nextComponent.getNextStatementsOf(stmtNum);
}

std::unordered_set<int> PKB::getPrevStatementsOf(int stmtNum) {
  return nextComponent.getPrevStatementsOf(stmtNum);
}

std::unordered_set<std::pair<int, int>, std::hash_pair> PKB::getAllNextPairs() {
  return nextComponent.getAllStatementsPairs();
}

std::unordered_map<int, std::unordered_set<int>> PKB::getNextListForProc(std::string procName) {
  std::unordered_map<int, std::unordered_set<int>> resultList;
  for (auto e: this->getStmtList(procName)) {
    if (!nextComponent.getNextStatementsOf(e).empty()) {
      resultList.emplace(e, nextComponent.getNextStatementsOf(e));
    }
  }
  return resultList;
}

std::unordered_map<int, std::unordered_set<int>> PKB::getCombinedNextList() {
  std::unordered_map<int, std::unordered_set<int>> resMap;
  std::unordered_set<std::string> procList = this->getProcList();
  for(auto ev: procList) {
    std::unordered_map<int, std::unordered_set<int>> nextList = this->getNextListForProc(ev);
    resMap.insert(nextList.begin(), nextList.end());
  }
  return resMap;
}




/*
 * Adders and Getters for CFG Component for Next*
 */

bool PKB::addCFG(std::string procName, CFG *cfg) {
  return cfgComponent.addCFG(procName, cfg);
}

CFG *PKB::getCFG(std::string procName) {
  return cfgComponent.getCFG(procName);
}

void PKB::clearCacheCFG() {
  std::unordered_set<std::string> procList = this->getProcList();
  for (auto e: procList) {
    auto cfg = this->getCFG(e);
    if (cfg != nullptr) {
      cfg->resetCacheVar();
    }
  }
}

std::unordered_set<CFG *> PKB::getCFGList() {
  return cfgComponent.getCFGList();
}

/*
 * Adders and Getters for CFGBip Component for Extension
 */

bool PKB::addCFGBip(CFGBip *cfgBip) {
  return cfgBipComponent.addCFGBip(cfgBip);
}

CFGBip *PKB::getCFGBip() {
  return cfgBipComponent.getCFGBip();
}

void PKB::clearCacheCFGBip() {
  cfgBipComponent.clearCFGBipCache();
}


