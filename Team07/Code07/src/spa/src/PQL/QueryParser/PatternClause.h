//
// Created by vaish on 3/9/2021.
//

#ifndef AUTOTESTER_PATTERNCLAUSE_H
#define AUTOTESTER_PATTERNCLAUSE_H

#include <string>
#include "Clause.h"
#include "PQL/QueryEvaluator/PQLPattern.h"

class PatternClause : public Clause {
  PatternSynonymType synonymType;
  std::string synonymName;
  PQLPattern *left;
  PQLPattern *right;
  PQLPattern *centre;
 public:
  PatternClause(PatternSynonymType synonymType,
                std::string left,
                std::string right,
                std::string synonymName);
  PatternClause(PatternSynonymType synonymType,
                std::string synonymName,
                std::string left,
                std::string centre,
                std::string right);
  std::string getSynonymName();
  PQLPattern *getLeft();
  PQLPattern *getRight();
  PQLPattern *getCentre();
  std::string toString() override;
};

#endif //AUTOTESTER_PATTERNCLAUSE_H
