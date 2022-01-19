//
// Created by Jazer on 30/9/2021.
//

#ifndef AUTOTESTER_TEAM07_CODE07_SRC_SPA_SRC_PQL_NEWQUERYEVALUATOR_CLAUSERESULT_H_
#define AUTOTESTER_TEAM07_CODE07_SRC_SPA_SRC_PQL_NEWQUERYEVALUATOR_CLAUSERESULT_H_

#include <vector>
#include <unordered_set>
#include <QuerySynonym.h>
#include "QueryOutput.h"
#include "Row.h"

typedef std::unordered_set<QuerySynonym, QuerySynonym::HashFunction> SynonymSet;
typedef std::vector<Row> RowList;

class ClauseResult {
  RowList rows;
  bool isIdent;
  SynonymSet synonyms;
  std::vector<QueryOutput *> output;

  void mergeSynonyms(const SynonymSet &syn);
  RowList cartesianProduct(RowList &rowList);
  static void mergeRowIfConsistent(Row row1, Row row2, RowList &result);
  RowList HashJoin(const RowList &rowList, SynonymSet sharedSyn);
  SynonymSet getSharedSynonyms(ClauseResult *other);

 public:
  explicit ClauseResult(bool isIdent = false);

  bool getIdent() const;

  /**
   * @return the number of rows in the result clause.
   */
  size_t size();

  /**
   * Checks if the ClauseResult is empty.
   * If the clause is an identity element it will always be false.
   * @return boolean representing if the list is empty
   */
  bool empty();

  void addRow(Row row);

  /**
   * Merge another different clauses together into the current one.
   * @param other ClauseResult to be merged.
   */
  void mergeClause(ClauseResult *other);

  size_t numSharedSynonyms(ClauseResult *other);

  SynonymSet getSynonyms();

  RowList getRows();

  void setOutputSyn(std::vector<QueryOutput *> outSyn);

  std::vector<QueryOutput *> getOutput();

  void addIntRow(QuerySynonym *syn, int num, const std::pair<QuerySynonym*, std::string> &synPair);
};

#endif //AUTOTESTER_TEAM07_CODE07_SRC_SPA_SRC_PQL_NEWQUERYEVALUATOR_CLAUSERESULT_H_
