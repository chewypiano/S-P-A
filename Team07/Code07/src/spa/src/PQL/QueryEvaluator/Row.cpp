//
// Created by Jazer on 30/9/2021.
//

#include "Row.h"
#include <algorithm>
#include <stdexcept>

void insertRMap(const SynonymValue &value, const QuerySynonym &syn, rRowMap &map) {
  if (map.find(value) == map.end()) {
    map[value] = std::vector<QuerySynonym>();
  }
  map[value].emplace_back(syn);
}

Row::Row() {
  this->map = RowMap();
  this->rmap = rRowMap();
}
void Row::addSynonym(QuerySynonym &synonym, const SynonymValue &value) {
  this->map.insert_or_assign(synonym, value);
  insertRMap(value, synonym, this->rmap);
}

bool Row::isMismatchedVariable(const std::pair<QuerySynonym, SynonymValue> &item) {
  // x:1 y:2, y:2 z:3
  return map.find(item.first) != map.end() && map.at(item.first) != item.second;
}

void Row::merge(Row &rhs) {
  if (!isConsistentWith(rhs))
    throw std::invalid_argument("Inconsistent values");

  for (const auto &item: rhs.getMap()) {
    this->map.insert(item);
    insertRMap(item.second, item.first, this->rmap);
  }
}

bool Row::isConsistentWith(Row &rhs) {
  RowMap rowMap = rhs.getMap();
  return std::all_of(
      rowMap.begin(),
      rowMap.end(),

      // [references to refer to] (params for map) { function body }
      [this](const std::pair<QuerySynonym, SynonymValue> &item) {
        // 1 var is not found in another && var cannot be same value
        return !this->isMismatchedVariable(item);
      });
}

size_t Row::getSize() {
  return map.size();
}
RowMap Row::getMap() {
  return map;
}

