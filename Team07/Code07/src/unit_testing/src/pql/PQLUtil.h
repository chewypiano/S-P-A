//
// Created by Junhua on 4/10/2021.
//

#ifndef AUTOTESTER_SRC_UNIT_TESTING_SRC_PQL_PQLUTIL_H_
#define AUTOTESTER_SRC_UNIT_TESTING_SRC_PQL_PQLUTIL_H_

#include <vector>

#include "Row.h"
#include "ClauseResult.h"
#include "QuerySynonym.h"

Row createRow(const std::vector<std::pair<QuerySynonym, std::string>> &row);
RowList createRowList(const std::vector<std::vector<std::pair<QuerySynonym, std::string>>> &rows);
ClauseResult *createClauseResult(const RowList &addList);

#endif //AUTOTESTER_SRC_UNIT_TESTING_SRC_PQL_PQLUTIL_H_
