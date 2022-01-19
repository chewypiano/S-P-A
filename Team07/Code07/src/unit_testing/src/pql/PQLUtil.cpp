//
// Created by Junhua on 4/10/2021.
//

#include "PQLUtil.h"
#include "Row.h"

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

ClauseResult *createClauseResult(const RowList &addList) {
  auto r = new ClauseResult();
  for (auto &row: addList) {
    r->addRow(row);
  }
  return r;
}