//
// Created by Junhua on 29/9/2021.
//

#include "Clause.h"

Clause::Clause(ClauseType type) {
  this->type = type;
}

ClauseType Clause::getType() {
  return this->type;
}
