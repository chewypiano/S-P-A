//
// Created by Junhua on 4/11/2021.
//

#ifndef AUTOTESTER_SRC_INTEGRATION_TESTING_SRC_TESTUTILS_H_
#define AUTOTESTER_SRC_INTEGRATION_TESTING_SRC_TESTUTILS_H_

#include <string>
#include <vector>
#include "QueryEvaluator.h"

Row createRow(const std::vector<std::pair<QuerySynonym, std::string>> &row);
RowList createRowList(const std::vector<std::vector<std::pair<QuerySynonym, std::string>>> &rows);

PKB *generateAffectsPKB();
PKB *generateNoAffectsPKB();

PKB *generateBipPKB();
PKB *generateNoNextBipPKB();
PKB *generateNoAffectsBipPKB();

#endif //AUTOTESTER_SRC_INTEGRATION_TESTING_SRC_TESTUTILS_H_
