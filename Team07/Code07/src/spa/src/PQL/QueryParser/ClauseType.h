//
// Created by Junhua on 29/9/2021.
//

#ifndef AUTOTESTER_SRC_SPA_SRC_PQL_QUERYPARSER_CLAUSETYPE_H_
#define AUTOTESTER_SRC_SPA_SRC_PQL_QUERYPARSER_CLAUSETYPE_H_

enum class ClauseType {
  With,
  Pattern,
  Parent,
  Follows,
  Calls,
  Next,
  ModifiesS,
  ModifiesP,
  UsesS,
  UsesP,
  CallsT,
  ParentT,
  FollowsT,
  NextT,
  Affects,
  AffectsT,
  NextBip,
  NextBipT,
  AffectsBip,
  UnidentifiedModifies,
  UnidentifiedUses, // the unidentified ClauseTypes stand as placeholder before identifying correct type
};

#endif //AUTOTESTER_SRC_SPA_SRC_PQL_QUERYPARSER_CLAUSETYPE_H_
