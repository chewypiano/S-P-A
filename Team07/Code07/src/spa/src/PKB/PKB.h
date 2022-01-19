#ifndef AUTOTESTER_PKB_H
#define AUTOTESTER_PKB_H

#include <string>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include "ModifyComponent.h"
#include "UseComponent.h"
#include "ParentComponent.h"
#include "FollowComponent.h"
#include "BlockComponent.h"
#include "CallComponent.h"
#include "NextComponent.h"
#include "PatternComponent.h"
#include "CFGComponent.h"
#include "CFGBipComponent.h"

enum stmtType { print, read, assign, call, ifStmt, whileStmt, nullValue };

class PKB {
 public:
  PKB();

  /*
      General adders
  */
  void addStatement(int stmtNum, stmtType type, std::string procedure);

  //Adds a proc as a string to the procList
  void addProc(std::string procName);

  //Adds a var as a string to the varList
  void addVariable(std::string varName);

  //Adds a const as a string to the constList
  void addConst(std::string constName);


  /*
      General getters
  */

  //Returns the list of constants
  std::unordered_set<std::string> getConstList();

  //Returns the list of variables
  std::unordered_set<std::string> getVarList();

  //Returns the list of procedures
  std::unordered_set<std::string> getProcList();

  //Returns the list of read statements number
  std::unordered_set<int> getReadStmts();

  //Returns the list of print statements number
  std::unordered_set<int> getPrintStmts();

  //Returns the list of assign statements number
  std::unordered_set<int> getAssignStmts();

  //Returns the list of call statements number
  std::unordered_set<int> getCallStmts();

  //Returns a list containing pairs of <stmtNum, Var>
  std::unordered_map<int, std::string> getReadList();

  //Returns a list containing pairs of <stmtNum, Var>
  std::unordered_map<int, std::string> getPrintList();

  //Returns the list of statements that are in the specified procedure
  std::unordered_set<int> getStmtList(std::string procedure);

  //Returns the statement type of the specified statement
  stmtType getStmtType(int stmtNum);

  //Returns the total number of statements in the SIMPLE program
  int getTotalStmtNum();

  //Returns the Procedure which the statement is nested in
  std::string getStmtProc(int stmtNum);

  //Returns all the stmt number in the SIMPLE program
  std::unordered_set<int> getAllStmt();


  //Returns a table of assign statements with the variables stored in the pair object
  std::unordered_map<int, std::pair<std::string,std::unordered_set<std::string>>> getAssignList();

  //Returns a table of procedures with the start and end stmt nums
  std::unordered_map<std::string, std::pair<int, std::unordered_set<int>>> getAllProcedureWithStartAndEnd();



  /*
   * Adders and Getters for Block Component
   */

  //Add if stmt and all the statements in a block into the table
  bool addIfStmt(int startStmtNum, int lastStmtNum, int elseStmtNum);

  //Add while stmt and all the statements in a block into the table
  bool addWhileStmt(int startStmtNum, int lastStmtNum);

  //Add if stmt and the control variables into the Block Storage
  void addIfCtrlVar(int startStmtNum, std::string ctrlVar);

  //Add while stmt and the control variables into the Block Storage
  void addWhileCtrlVar(int startStmtNum, std::string ctrlVar);

  //Returns the last stmt num of the specified while block
  int getWhileEndStmt(int stmtNum);

  //Returns the else stmt num that belongs to the specified if block
  int getElseStmt(int stmtNum);

  //Returns the last statement of the specified if block
  int getIfEndStmt(int stmtNum);

  //Returns all if statements that modifies a var
  std::unordered_set<int> getAllIf();

  //Returns all if statements regardless if it modifies a var
  std::unordered_set<int> getIfStmts();

  //Returns all while statements that modifies a var
  std::unordered_set<int> getAllWhile();

  //Returns all while statements regardless if it modifies a var
  std::unordered_set<int> getWhileStmts();

  //Returns a list of all if statements containing the specified variable
  std::unordered_set<int> getIfCtrlVar(std::string ctrlVar);

  //Returns a list of all while statements containing the specified variable
  std::unordered_set<int> getWhileCtrlVar(std::string ctrlVar);







  /*
   * Adders and Getters for Pattern
   */

  //Adds the stmtNum, LHS and RHS of an assign statement to the Pattern component
  bool addAssignPattern(int stmtNum, std::string LHS, std::string RHS);

  //Returns the LHS of the specified assign statement
  std::string getLHSAssign(int stmtNum);

  //Returns the RHS of the specified assign statement
  std::string getRHSAssign(int stmtNum);

  //Adds possible subStrings for the assign statement into the Pattern component
  void addSubString(int stmtNum, std::string subString);

  //Returns a list of possible subStrings at the assign statement stmtNum
  std::unordered_set<std::string> subStringsAt(int stmtNum);

  //Returns the table containing all the patterns for assign statements
  std::unordered_map<int, std::pair<std::string, std::string>> getAllPatternsAssignStmts();

  //Returns the table containing all the patterns for if and while cond expr
  std::unordered_map<int, std::unordered_set<std::string>> getAllIfWhilePatterns();

  /*
   * Adders and Getters for Call Relations
   */

  //Adds the stmtNum, proc that the call statement belongs to, proc that is being called to the callList
  bool addCall(int stmtNum, std::string callerProc, std::string calledProc);

  //Adds the callerProc and calledProc to the callStarList
  bool addCallStar(std::string callerProc, std::string calledProc);

  //Returns the proc that the call stmt is under
  std::string getCallerProc(int stmtNum);

  //Returns the proc that the call stmt is calling
  std::string getCalledProc(int stmtNum);

  //Returns the set of procedures that calls the input procName
  std::unordered_set<std::string> getProcsCalledFromProc(std::string procName);

  //Returns the set of procedures that calls the input procName directly/indirectly
  std::unordered_set<std::string> getProcsCalledStarFromProc(std::string procName);

  //Returns the set of procedures that the input procName calls
  std::unordered_set<std::string> getProcsCalledByProc(std::string procName);

  //Returns the set of procedures that the input procName calls directly/indirectly
  std::unordered_set<std::string> getProcsCalledStarByProc(std::string procName);

  //Returns the table of call stmts
  std::unordered_map<int, std::string> getAllCallStmts();

  //Checks if the call relation exists
  bool isCallRelation(std::string callerProc, std::string calledProc);

  //Checks if the call star relation exists
  bool isCallStarRelation(std::string callerProc, std::string calledProc);







  /*
   * Adders and Getters for Modify Relations
   */

  //Adds the modifies relation for a (statement, variable) in the ModifyComponent
  //Returns true if successful, else false if the stmtNum is negative or the string is invalid
  bool addModifyStmt(int stmtNum, std::string var);

  //Adds the modifies relation for a (procedure, variable) in the ModifyComponent
  //Returns true if successful, else false if the stmtNum is negative or the string is invalid
  bool addModifyProc(std::string procName, std::string var);

  //Checks if the Modifies(stmtNum, var) relations is true
  bool isStmtModifyingVar(int stmtNum, std::string var);

  //Checks if the Modifies(procName, var) relations is true
  bool isProcModifyingVar(std::string procName, std::string var);

  //Returns a list of var modified by specified statement
  std::unordered_set<std::string> getVarModifiedByStmt(int stmtNum);

  //Returns a list of var modified by specified procedure
  std::unordered_set<std::string> getVarModifiedByProc(std::string procName);

  //Returns a list of statements that modifies the var
  std::unordered_set<int> getStmtModifyingVar(std::string var);

  //Returns a list of procedures that modifies the var
  std::unordered_set<std::string> getProcModifyingVar(std::string procName);

  //Returns all modifies statements
  std::unordered_set<int> getAllModifies();

  //Returns all modifies procedure
  std::unordered_set<std::string> getAllModifiesProc();

  //Returns a table of statements and variables that have been modified
  std::unordered_map<int, std::unordered_set<std::string>> getStmtListofVar();

  //Returns a table of statements and variables that have been modified excluding if and while stmts
  std::unordered_map<int, std::unordered_set<std::string>> getStmtListofVar2();


  /*
   * Adders and Getters for Uses Relations
  */

  //Adds the use relation for a (statement, variable) in the UseComponent
  //Returns true if successful, else false if the stmtNum is negative or the string is invalid
  bool addUseStmt(int stmtNum, std::string var);

  //Adds the use relation for a (procedure, variable) in the UseComponent
  //Returns true if successful, else false if the stmtNum is negative or the string is invalid
  bool addUseProc(std::string procName, std::string var);

  //Checks if the Use(stmtNum, var) relations is true
  bool isStmtUsingVar(int stmtNum, std::string var);

  //Checks if the Use(procName, var) relations is true
  bool isProcUsingVar(std::string procName, std::string var);

  //Returns a list of var used by specified statement
  std::unordered_set<std::string> getVarUsedByStmt(int stmtNum);

  //Returns a list of var used by specified procedure
  std::unordered_set<std::string> getVarUsedByProc(std::string procName);

  //Returns a list of statements that uses the var
  std::unordered_set<int> getStmtUsingVar(std::string var);

  //Returns a list of procedures that uses the var
  std::unordered_set<std::string> getProcUsingVar(std::string var);

  //Returns all uses statements
  std::unordered_set<int> getAllUse();


  /*
  * Adders and Getters for Parent Relations
  */

  //Add Parent relation
  bool addParent(int stmtParent, int stmtChild);

  //Add Parent* relation
  bool addParentStar(int stmtParent, int stmtChild);

  //Checks if specified statement is a Parent
  bool isParent(int stmtNum);

  //Checks if specified statement is a Child
  bool isChild(int stmtNum);

  //Getters for Parent relations

  //For Parent(s1, s2) relation, return the parent statement s1 of the specified statement
  //Returns -1 for invalid stmtNum
  int getParent(int stmtNum);

  //For Parent*(s1, s2), return the list of parent statements {s1} of the specified statement
  std::unordered_set<int> getParentStar(int stmtNum);

  //For Parent(s1, s2), return the child statement s2 of the specified statement
  std::unordered_set<int> getChild(int stmtNum);

  //For Parent*(s1, s2), return the child statement s2 of the specified statement
  std::unordered_set<int> getChildStar(int stmtNum);

  //Returns all parent statements
  std::unordered_set<int> getParentList();

  //Returns all child statements
  std::unordered_set<int> getChildList();

  //Returns true if the parentPair exists
  bool isParentPair(int s1, int s2);

  //Returns true if the parentStarPair exists
  bool isParentStarPair(int s1, int s2);





  /*
  * Adders and Getters for Follow Relations
  */

  //Add Follow relations into the followComponent, returns false if the relation already exists
  bool addFollow(int stmt1, int stmt2);

  //Add Follow* relations into the followComponent, returns false if the relation already exists
  bool addFollowStar(int stmt1, int stmt2);

  //Checks if the stmtNum directly follows another statement, follows(stmtNum, s2) checks if there exists s2
  bool isFollowing(int stmtNum);

  //Checks if the stmtNum is being directly followed, follows(s1,stmtNum) checks if there exists s1
  bool isBeingFollowed(int stmtNum);

  //For Follow(s1, s2) relation, return the followed by statement s1 of the specified statement
  int getFollowedBy(int s2);

  //For Follow(s1, s2) relation, return the followed statement s2 of the specified statement
  int getFollower(int s1);

  //For Follow*(s1, s2), return the list of following statements {s2} of the specified statement
  std::unordered_set<int> getFollowerStar(int s1);

  //For Follow*(s1, s2), return the followed by statement s1 of the specified statement
  std::unordered_set<int> getFollowedByStar(int s2);

  //Returns a list of all statements that follows another statement
  //For Follow(s1, s2) relation, return a list of statements s2
  std::unordered_set<int> getAllFollowers();

  //Returns a list of all statements that is followed by another statement
  //For Follow(s1, s2) relation, return a list of statements s1
  std::unordered_set<int> getAllFollowedBy();

  //Returns true if the follow pair exists in the follow storage
  bool isFollow(int s1, int s2);

  //Returns true if the follow star pair exists in the follow storage
  bool isFollowStar(int s1, int s2);

  /*
  * Adders and Getters for Next Relations
  */

  //Add Next relations into the nextComponent, returns false if the relation already exists
  bool addNext(int stmt1, int stmt2);

  //Returns the list of stmtNums that are next of stmtNum
  std::unordered_set<int> getNextStatementsOf(int stmtNum);

  //Returns the list of stmtNums that are previous of stmtNum
  std::unordered_set<int> getPrevStatementsOf(int stmtNum);

  //Returns a list of all the pairs of next relations
  std::unordered_set<std::pair<int, int>, std::hash_pair> getAllNextPairs();

  //Returns the Next adjacency list for the given procedure input
  std::unordered_map<int, std::unordered_set<int>> getNextListForProc(std::string procName);

  //Returns the Combined Next List of the source program
  std::unordered_map<int, std::unordered_set<int>> getCombinedNextList();



  /*
  * Adders and Getters for CFG Component for Next*
  */

  //Returns true if addition of Next CFG is successful, else returns false
  bool addCFG(std::string procName, CFG *cfg);

  //Returns the Next CFG for the specified procedure
  CFG *getCFG(std::string procName);

  //Clears the cache for all the Next cfg objects
  void clearCacheCFG();

  //Returns the list of CFGs
  std::unordered_set<CFG *> getCFGList();

  /*
   * Adders and Getters for CFGBip Component for Extension
   */

  //Returns true if addition of CFGBip is successful, else returns false
  bool addCFGBip(CFGBip *cfgBip);

  //Returns the CFGBip object
  CFGBip *getCFGBip();

  //Clears the cache for the CFGBip object
  void clearCacheCFGBip();




  //Tables/lists of the various types of statements and procedure to be stored.
 private:
  std::unordered_map<int, stmtType> stmtTypeList;
  std::unordered_map<int, std::string> stmtProcList;
  std::unordered_set<std::string> varList;
  std::unordered_set<std::string> constList;
  std::unordered_set<std::string> procList;
  std::unordered_map<std::string, std::unordered_set<int>> stmtMap;
  std::unordered_set<int> assignStmtList;
  std::unordered_set<int> readStmtList;
  std::unordered_set<int> printStmtList;
  std::unordered_set<int> callStmtList;
  std::unordered_set<int> ifStmtList;
  std::unordered_set<int> whileStmtList;
  std::unordered_map<int, std::string> readPairList;
  std::unordered_map<int, std::string> printPairList;
  std::unordered_map<int, std::pair<std::string,std::unordered_set<std::string>>> affectsList;

  BlockComponent blockStorage;
  ModifyComponent modifyComponent;
  UseComponent useComponent;
  ParentComponent parentComponent;
  FollowComponent followComponent;
  CallComponent callComponent;
  NextComponent nextComponent;
  PatternComponent patternComponent;
  CFGComponent cfgComponent;
  CFGBipComponent cfgBipComponent;

};

#endif //AUTOTESTER_PKB_H
