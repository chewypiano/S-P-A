//
// Created by Jazer on 30/9/2021.
//

#include "ClauseResult.h"
#include "QueryOutput.h"
#include <algorithm>

ClauseResult::ClauseResult(bool isIdent) {
  this->isIdent = isIdent;
  synonyms = SynonymSet();
  rows = {};
  output = {};
}

void ClauseResult::setOutputSyn(std::vector<QueryOutput *> outSyn) {
  output = std::move(outSyn);
}

std::vector<QueryOutput *> ClauseResult::getOutput() {
  return output;
}

size_t ClauseResult::size() {
  return rows.size();
}

bool ClauseResult::empty() {
  return rows.empty() && !isIdent;
}

void ClauseResult::addRow(Row row) {
  if (std::find(rows.begin(), rows.end(), row) != rows.end()) {
    return;
  }
  rows.emplace_back(row);
  for (auto &item: row.getMap()) {
    synonyms.insert(item.first);
  }
}

void ClauseResult::mergeSynonyms(const SynonymSet &syn) {
  for (auto &s: syn) {
    this->synonyms.insert(s);
  }
}

void ClauseResult::mergeRowIfConsistent(Row row1, Row row2, RowList &result) {
  if (!row1.isConsistentWith(row2)) return;
  row1.merge(row2);
  result.emplace_back(row1);
}

RowList ClauseResult::cartesianProduct(RowList &rowList) {
  RowList results = RowList();
  for (const Row &i: rows) {
    for (const Row &j: rowList) {
      mergeRowIfConsistent(i, j, results);
    }
  }
  return results;
}

RowList ClauseResult::HashJoin(const RowList &rowList, SynonymSet sharedSyn) {
  if (sharedSyn.empty()) {
    throw std::runtime_error("Do not use HashJoin for no shared syns");
  }

  RowList results = RowList();
  QuerySynonym syn = *sharedSyn.begin();
  std::unordered_multimap<SynonymValue, Row> commonHashMap;

  // Hash the first table.
  for (Row r: rows) {
    commonHashMap.insert({r.getMap().at(syn), r});
  }

  // Merge the table
  for (Row r: rowList) {
    auto range = commonHashMap.equal_range(r.getMap().at(syn));
    for (auto it = range.first; it != range.second; ++it) {
      mergeRowIfConsistent(r, it->second, results);
    }
  }

  // Return the results
  return results;
}

void ClauseResult::mergeClause(ClauseResult *other) {

  // Update rows with merged rows
  if (other->isIdent) {
    if (!isIdent)
      this->isIdent = false;
    return;
  }
  if (isIdent) {
    this->rows = other->getRows();
    this->isIdent = false;
    return;
  }

  auto shared = getSharedSynonyms(other);

  // Add synonyms to current pool of synonyms
  mergeSynonyms(other->getSynonyms());

  // Cartesian product
  if (!shared.empty()) {
    //Hash join
    this->rows = HashJoin(other->getRows(), shared);
    return;
  }

  RowList lst = other->getRows();
  this->rows = cartesianProduct(lst);
}

SynonymSet ClauseResult::getSharedSynonyms(ClauseResult *other) {
  if (other->isIdent) {
    return synonyms;
  }
  SynonymSet result;
  for (const auto &s: other->getSynonyms()) {
    if (synonyms.find(s) != synonyms.end()) {
      result.insert(s);
    }
  }
  return result;
}

size_t ClauseResult::numSharedSynonyms(ClauseResult *other) {
  if (isIdent || other->isIdent) {
    return std::max({this->getSynonyms().size(), other->getSynonyms().size()});
  }
  int result = 0;
  for (const auto &s: other->getSynonyms()) {
    if (synonyms.find(s) != synonyms.end()) {
      ++result;
    }
  }
  return result;
}

SynonymSet ClauseResult::getSynonyms() {
  return synonyms;
}
RowList ClauseResult::getRows() {
  return rows;
}
bool ClauseResult::getIdent() const {
  return isIdent;
}

void ClauseResult::addIntRow(QuerySynonym *syn, int num, const std::pair<QuerySynonym *, std::string> &synPair) {
  Row row = Row();
  row.addSynonym(*syn, std::to_string(num));
  if (synPair.first != nullptr) {
    row.addSynonym(*synPair.first, synPair.second);
  }
  this->addRow(row);
}
