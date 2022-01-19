//
// Created by Junhua on 30/9/2021.
//

#include "QueryEvaluator.h"
#include "Sort.h"
#include <unordered_set>
#include <unordered_map>
#include "SuchEvaluator.h"
#include <algorithm>

QueryEvaluator::QueryEvaluator(QueryParser *parser, PKB *pkb) {
  this->parser = parser;
  this->pkb = pkb;
  this->collector = new ClauseResult(true);
  this->valueStore = new ValueStore(pkb);
  this->patternEvaluator = new PatternEvaluator(this->pkb, this->valueStore);
  this->suchEvaluator = new SuchEvaluator(this->pkb, this->valueStore);
  this->withEvaluator = new WithEvaluator(this->pkb);
  this->typeSort = [](Clause *c) {
    return (size_t) c->getType();
  };
  this->sizeSort = [this](ClauseResult *a) {
    return -(a->size() - this->collector->numSharedSynonyms(a) * 1000);
  };
}

ClauseResult *QueryEvaluator::evaluate() {
  try {
    auto query = parser->getQuery();
    return evaluateQuery(query);
  } catch (const std::runtime_error &e) {
    return new ClauseResult();
  }
}

ClauseResult *QueryEvaluator::evaluateQuery(Query &query) {
  // Clear Cache before everything
  pkb->clearCacheCFG();
  pkb->clearCacheCFGBip();
  valueStore->clearResults();

  if (query.isInvalidQuery())
    return new ClauseResult();

  // handle irrelevant clauses first
  for (auto clause: query.getIrrelevantClauses()) {
    ClauseResult *result = evaluateClause(clause, &query);
    if (result->empty()) {
      auto r = new ClauseResult();
      r->setOutputSyn(query.getOutputList());
      return r;
    }
  }

  collector = new ClauseResult(true);
  auto out = query.getOutputList();
  collector->setOutputSyn(out);
  valueStore->setStoredResults(collector);

  std::vector<ClauseResult *> clauseResults;

  auto allClauses = query.getClauses();
  counting_sort(allClauses, this->typeSort);

  ClauseResult *result;
  for (auto clause: query.getClauses()) {
    if (collector->empty()) {
      return collector;
    }
    result = evaluateClause(clause, &query);
    if (result->numSharedSynonyms(collector) > 0) {
      collector->mergeClause(result);
      continue;
    }
    clauseResults.emplace_back(result);
  }

  // Sort the queries to be similar to the number of shared synonyms O(n)
  counting_sort<ClauseResult *>(
      clauseResults,
      this->sizeSort
  );
  while (!clauseResults.empty()) {
    auto res = clauseResults.back();
    clauseResults.pop_back();
    collector->mergeClause(res);
    if (collector->empty()) {
      return collector;
    }
  }
  for (auto &output: out) {
    if (output->getType() == QueryOutputType::Boolean
        || collector->getSynonyms().count(*output->getSynonym()) > 0) {
      // If is boolean, then no rows
      continue;
    }
    auto syn = output->getSynonym();
    auto tempClause = new ClauseResult();
    for (auto &res: getValuesFor(syn)) {
      Row r = Row();
      r.addSynonym(*syn, res);
      tempClause->addRow(r);
    }
    collector->mergeClause(tempClause);
  }
  return collector;
}

std::unordered_set<std::string> QueryEvaluator::getValuesFor(QuerySynonym *synonym) {
  auto synCount = collector->getSynonyms().count(*synonym);
  if (synCount == 0) {
    return valueStore->valuesInType(synonym->getType());
  }
  std::unordered_set<std::string> result;
  for (auto row: collector->getRows()) {
    result.insert(row.getMap().at(*synonym));
  }
  return result;
}

bool checkIfStmtTypeMatchesSynType(QuerySynonym *syn, stmtType receivedType) {
  DesignEntityType requiredType = syn->getType();
  std::unordered_map<DesignEntityType, stmtType> map = {
      {DesignEntityType::Print, stmtType::print},
      {DesignEntityType::Read, stmtType::read},
      {DesignEntityType::Assign, stmtType::assign},
      {DesignEntityType::Call, stmtType::call},
      {DesignEntityType::If, stmtType::ifStmt},
      {DesignEntityType::While, stmtType::whileStmt}
  };

  return requiredType == DesignEntityType::ProgLine || requiredType == DesignEntityType::Stmt
      || map[requiredType] == receivedType;
}


ClauseResult *QueryEvaluator::evaluateClause(Clause *clause, Query *query) {
  ClauseResult *result;
  switch (clause->getType()) {
    case ClauseType::Pattern: {
      result = this->patternEvaluator->evaluatePatternClause(dynamic_cast<PatternClause *>(clause), query);
      break;
    }
    case ClauseType::With: {
      result = this->withEvaluator->evaluateWithClause(dynamic_cast<WithClause *>(clause), query);
      break;
    }
    default: {
      // such that clause
      result = this->suchEvaluator->evaluateClause(dynamic_cast<SuchThatClause *>(clause), query);
    }
  }
  return result;
}


