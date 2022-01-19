//
// Created by Sean Chan  on 1/11/21.
//

#include "CFGBip.h"
#include <unordered_set>

int generateTerminalInt(int num){
  return -1*(num+1000000);
}

int recursivelyGetNextofTerminal(std::unordered_map<int,std::unordered_set<int>> &nextList,int terminal){
  if(nextList.count(terminal)==0 && terminal<0){
    throw std::runtime_error("CFG: negative terminal node should exist in list");
  }
  while(terminal<0 && nextList[terminal].size()!=0 ){
    terminal = *nextList[terminal].begin();
  }
  return terminal;
}

CFGBip::CFGBip(std::unordered_map<int, std::unordered_set<int>> nextList): CFG(nextList) {
  this->nextList = nextList;
  this->nextBipList = {};
  this-> assignList = {};
  this-> modifiesList = {};
  this-> procedureList = {};
  this-> callList = {};
  this->procedureTerminals ={};
  this->terminals = {};
}

void CFGBip::turnToBip(){
  for(auto p: callList){
    int callStmt = p.first;
    modifiesList.erase(callStmt);
  }
  for(auto p:procedureList){
    auto endingStmts = p.second.second;
    int terminal = generateTerminalInt(p.second.first);
    procedureTerminals[p.first] = terminal;
    nextList[terminal] ={};
    for(int end: endingStmts){
      if(nextList.count(end)==0){
        nextList[end] ={};
      }
      nextList[end].insert(terminal);
      terminals.insert(terminal);
    }
  }
  for(auto p: callList){
    int callStmt = p.first;
    std:: string calledProcedure = p.second;
    int nextOfCall = *nextList[callStmt].begin();
    nextList[callStmt].erase(nextOfCall);
    nextList[callStmt].insert(procedureList[calledProcedure].first);
    nextList[procedureTerminals[calledProcedure]].insert(nextOfCall);
  }

  std::unordered_map<int,std::unordered_set<int>> temp = {};
  for(auto p: nextList){

    if(p.first<=0) continue;
    temp[p.first] = {};
    for(int con: p.second){
      int recursiveNext = recursivelyGetNextofTerminal(nextList,con);
      if(recursiveNext>0){
        temp[p.first].insert(recursivelyGetNextofTerminal(nextList,con));
      }
    }

  }
  for(auto p:temp){
    if(p.second.size()!=0){
     nextBipList[p.first] = p.second;
    }
  }
  nextList = nextBipList;
}

void CFGBip::setProcedureList(std::unordered_map<std::string, std::pair<int, std::unordered_set<int>>> procedureList) {
  this->procedureList = procedureList;
}

void CFGBip::setCallList(std::unordered_map<int, std::string> callList) {
  this->callList = callList;
}

bool CFGBip::isNextBip(int s1, int s2)  {

  if(this->nextList.count(s1)!=0 && this->nextList[s1].count(s2)){
    return true;
  }

  if(nextList.count(s1)==0) {
    return {};
  }

  for(int neighbour: nextList[s1]){
    if (neighbour<0){
      return isNextBip(neighbour,s2);
    }
  }

  return false;
}

std::unordered_set<int> CFGBip::getNextBipList(int stmt) {
  if(nextBipList.count(stmt)==0) return {};
  return nextBipList[stmt];
}

std::unordered_map<int, std::unordered_set<int>> CFGBip::getNextList(){
  return this->nextBipList;
}

std::unordered_map<int, std::unordered_set<int>> CFGBip::getNextBipList(){
  return this->nextBipList;
}

bool CFGBip::hasNextBip(int stmt){
  return this->nextBipList.count(stmt) != 0 && this->nextBipList[stmt].size() != 0;
};