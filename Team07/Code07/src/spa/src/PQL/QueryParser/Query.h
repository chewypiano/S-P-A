//
// Created by vaish on 4/9/2021.
//

#ifndef AUTOTESTER_QUERY_H
#define AUTOTESTER_QUERY_H

#include <vector>
#include <unordered_map>
#include "QuerySynonym.h"
#include "SuchThatClause.h"
#include "PatternClause.h"
#include "WithClause.h"
#include "ClauseList.h"
#include "QueryOutput.h"

class Query {
  std::vector<QuerySynonym> synonymList;
  std::unordered_map<std::string, QuerySynonym> synonymMap;
  ClauseList clauses;
  ClauseList irrelevantClauses;
  std::vector<QueryOutput *> outputList;
  bool isInvalid;

 public:
  Query();

  void insertOutput(QueryOutput *queryOutput);

  void insertOutputSynonym(QuerySynonym *synonym);

  QuerySynonym *findSynonymByName(std::string name);

  bool checkIfSynonymExists(const std::string &name);

  void insertSynonym(QuerySynonym synonym);

  void insertClause(Clause *clause);

  void insertIrrelevantClause(Clause *clause);

  std::vector<QuerySynonym> getSynonymList();

  ClauseList getClauses();

  ClauseList getIrrelevantClauses();

  void setClauses(ClauseList newClauses);

  std::vector<QueryOutput *> getOutputList();

  void setInvalidQuery();

  bool isInvalidQuery();

  std::string toString();

  void print();
};

#endif //AUTOTESTER_QUERY_H
