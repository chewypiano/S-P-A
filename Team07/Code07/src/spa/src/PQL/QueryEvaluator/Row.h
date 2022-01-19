//
// Created by Jazer on 30/9/2021.
//

#ifndef AUTOTESTER_TEAM07_CODE07_SRC_SPA_SRC_PQL_NEWQUERYEVALUATOR_ROW_H_
#define AUTOTESTER_TEAM07_CODE07_SRC_SPA_SRC_PQL_NEWQUERYEVALUATOR_ROW_H_
#include <unordered_map>
#include <QuerySynonym.h>
#include <vector>
#include <sstream>

typedef std::string SynonymValue;
typedef std::unordered_map<QuerySynonym, SynonymValue, QuerySynonym::HashFunction> RowMap;
typedef std::unordered_map<SynonymValue, std::vector<QuerySynonym>> rRowMap;

class Row {
  RowMap map;
  rRowMap rmap;
  bool isMismatchedVariable(const std::pair<QuerySynonym, SynonymValue> &item);

 public:
  Row();

  void addSynonym(QuerySynonym &synonym, const SynonymValue &value);

  /**
   * Adds the name-value pairs from rhs into this Row's map if all overlapping names have matching values.
   * Throws an exception if an overlapping name has mismatched values.
   * @param rhs
   */
  void merge(Row &rhs);

  /**
   * Checks if the names in rhs that are present in this map have the same values.
   * @param rhs The Row to match against
   * @return true if there are no conflicting mappings, false otherwise
   */
  bool isConsistentWith(Row &rhs);

  size_t getSize();

  RowMap getMap();

  bool operator==(const Row &p) const {
    return map == p.map;
  }

};

#endif //AUTOTESTER_TEAM07_CODE07_SRC_SPA_SRC_PQL_NEWQUERYEVALUATOR_ROW_H_
