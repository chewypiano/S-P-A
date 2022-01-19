//
// Created by Junhua on 4/10/2021.
//

#include "QueryProjector.h"
#include "unordered_set"
#include "Row.h"
#include <string>

std::unordered_set<DesignEntityType> QueryProjector::readPrintType = {DesignEntityType::Read, DesignEntityType::Print};
std::string QueryProjector::falseResultString = "FALSE";
std::string QueryProjector::trueResultString = "TRUE";

std::string getProcNameAttr(QueryOutput *output, RowMap mp, PKB *pkb) {
  if (output->getSynonym()->getType() != DesignEntityType::Call) {
    throw std::invalid_argument("Invalid design entity in getProcNameAttr");
  }
  return pkb->getCalledProc(std::stoi(mp.at(*output->getSynonym())));
}

std::string unwrapMap(const std::unordered_set<std::string> &mp) {
  if (mp.size() != 1) {
    throw std::invalid_argument("Can only unwarp set with value 1");
  }
  return *mp.begin();
}

std::string getVarNameAttr(QueryOutput *output, RowMap mp, PKB *pkb) {
  std::string stmt = mp.at(*output->getSynonym());
  switch (output->getSynonym()->getType()) {
    case DesignEntityType::Print:
      // Find the variable the print statement is using
      return unwrapMap(pkb->getVarUsedByStmt(std::stoi(stmt)));
    case DesignEntityType::Read:
      // Find the variable the read statement the read statement is modifying
      return unwrapMap(pkb->getVarModifiedByStmt(std::stoi(stmt)));
    default:
      throw std::invalid_argument("Invalid attribute to getVarNameAttr");
  }
}

std::string QueryProjector::addStringBasedOnType(QueryOutput *singleOut, RowMap mp) {
  std::stringstream res;
  std::string result = mp.at(*singleOut->getSynonym());
  auto synonymType = singleOut->getSynonym()->getType();
  switch (singleOut->getType()) {
    case QueryOutputType::ProcNameAttribute:
      if (synonymType == DesignEntityType::Call)
        return getProcNameAttr(singleOut, mp, pkb) + ' ';
    case QueryOutputType::VarNameAttribute:
      if (readPrintType.count(synonymType) > 0)
        return getVarNameAttr(singleOut, mp, pkb) + ' ';
    default:
      break;
  }
  res << result << ' ';
  return res.str();
}

std::string QueryProjector::getLineFromRow(const RowMap &mp, const std::vector<QueryOutput *> &outputList) {
  std::stringstream res;
  for (const auto &singleOut: outputList) {
    if (!mp.count(*singleOut->getSynonym())) {
      // Output Syn not found
      return "";
    }
    res << addStringBasedOnType(singleOut, mp);
  }
  return res.str();
}

void QueryProjector::projectAnswer(ClauseResult *clauseResult,
                                   const std::vector<QueryOutput *> &outputList,
                                   std::list<std::string> &results) {
  // Check if the output is boolean
  if (outputList.size() == 1 && outputList[0]->getType() == QueryOutputType::Boolean) {
    results.emplace_back(clauseResult->empty() ? falseResultString : trueResultString);
    return;
  }

  // Removing duplicate answers
  std::unordered_set<std::string> out;
  std::string r;

  for (auto row: clauseResult->getRows()) {
    r = getLineFromRow(row.getMap(), outputList);
    if (r.empty()) {
      return;
    }
    out.insert(r.substr(0, r.size() - 1));
  }

  for (const auto &re: out) {
    results.emplace_back(re);
  }
}

QueryProjector::QueryProjector(PKB *pkb) {
  this->pkb = pkb;
}