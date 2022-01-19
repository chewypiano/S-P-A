//
// Created by vaish on 6/11/2021.
//

#ifndef AUTOTESTER_SPA_TEAM07_CODE07_SRC_SPA_SRC_PQL_QUERYPARSER_IRRELEVANTCLAUSEPROCESSOR_H_
#define AUTOTESTER_SPA_TEAM07_CODE07_SRC_SPA_SRC_PQL_QUERYPARSER_IRRELEVANTCLAUSEPROCESSOR_H_

#include <string>
#include <unordered_set>
#include <unordered_map>
#include "Clause.h"
#include "Query.h"

class IrrelevantClauseProcessor {
  Query *query;
  std::unordered_map<std::string, bool> relevanceMap;
  std::unordered_map<std::string, std::string> irrelevantPairMap;
  ClauseList relevantClauses;

  void checkRelevanceOfAllClauses();

  void makePairedSynRelevant(const std::string &syn);

  void checkSuchThatClauseSynRelevance(SuchThatClause *clause);

  void checkWithClauseSynRelevance(WithClause *clause);

  void checkPatternClauseSynRelevance(PatternClause *clause);

  void modifyDataStructuresAccordingToRelevance(bool isRelevant,
                                                const std::pair<std::string, std::string> &pair,
                                                int numSynonyms);

  void convertIrrelevantClauses();

  void makeSuchThatClauseIrrelevantIfNeeded(SuchThatClause *clause);

  void makeWithClauseIrrelevantIfNeeded(WithClause *clause);

  void makePatternClauseIrrelevantIfNeeded(PatternClause *clause);

 public:
  explicit IrrelevantClauseProcessor(Query *query);

  void processIrrelevantClauses();
};

#endif //AUTOTESTER_SPA_TEAM07_CODE07_SRC_SPA_SRC_PQL_QUERYPARSER_IRRELEVANTCLAUSEPROCESSOR_H_
