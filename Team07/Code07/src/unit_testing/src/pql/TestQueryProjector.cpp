//
// Created by Junhua on 4/10/2021.
//

#include "catch.hpp"
#include "PQLUtil.h"
#include "QueryProjector.h"
#include <algorithm>

QueryProjector *createQueryProjector(PKB *pkb) {
  return new QueryProjector(pkb);
}

TEST_CASE("Test empty Query Projector") {
  auto clauseResult = new ClauseResult();
  std::vector<QueryOutput *> outputSyn = {
      new QueryOutput(QueryOutputType::Synonym, new QuerySynonym(DesignEntityType::Assign, "x"))
  };
  std::list<std::string> result;
  auto queryProjector = createQueryProjector(new PKB());

  queryProjector->projectAnswer(clauseResult, outputSyn, result);

  REQUIRE(result.empty());
}

TEST_CASE("Test Query Projector with 1 output synonym") {
  auto clauseResult = new ClauseResult();
  auto syn = new QuerySynonym(DesignEntityType::Assign, "x");

  Row r = createRow({{*syn, "2"}});
  clauseResult->addRow(r);

  std::vector<QueryOutput *> outputSyn = {new QueryOutput(QueryOutputType::Synonym, syn)};

  std::list<std::string> result;

  auto queryProjector = createQueryProjector(new PKB());
  queryProjector->projectAnswer(clauseResult, outputSyn, result);

  REQUIRE(result.size() == 1);
  REQUIRE(result == std::list{std::string{"2"}});
}

TEST_CASE("Test Query Projector with 2 output synonym") {
  auto syn = new QuerySynonym(DesignEntityType::Assign, "x");
  auto syn2 = new QuerySynonym(DesignEntityType::Assign, "y");

  auto r = createRowList(
      {
          {
              {*syn, "2"},
              {*syn2, "3"}
          }, {
              {*syn, "3"},
              {*syn2, "4"}
          }
      }
  );
  auto clauseResult = createClauseResult(r);

  std::vector<QueryOutput *>
      outputSyn = {new QueryOutput(QueryOutputType::Synonym, syn), new QueryOutput(QueryOutputType::Synonym, syn2)};
  std::list<std::string> result;

  auto queryProjector = createQueryProjector(new PKB());
  queryProjector->projectAnswer(clauseResult, outputSyn, result);

  std::vector<std::string> expected = {
      "2 3",
      "3 4"
  };

  REQUIRE(result.size() == 2);
  for (auto &i: expected) {
    REQUIRE(std::find(result.begin(), result.end(), i) != result.end());
  }

}

TEST_CASE("Test query projector with boolean true") {
  auto syn = new QuerySynonym(DesignEntityType::Assign, "x");
  auto syn2 = new QuerySynonym(DesignEntityType::Assign, "y");

  auto r = createRowList(
      {
          {
              {*syn, "2"},
              {*syn2, "3"}
          }, {
              {*syn, "3"},
              {*syn2, "4"}
          }
      }
  );
  auto clauseResult = createClauseResult(r);

  std::vector<QueryOutput *>
      outputSyn = {new QueryOutput(QueryOutputType::Boolean)};
  std::list<std::string> result;

  auto queryProjector = createQueryProjector(new PKB());
  queryProjector->projectAnswer(clauseResult, outputSyn, result);

  REQUIRE(result.size() == 1);
  REQUIRE(std::find(result.begin(), result.end(), "TRUE") != result.end());
}

TEST_CASE("Test query projector with boolean false") {
  auto r = createRowList({});
  auto clauseResult = createClauseResult(r);

  std::vector<QueryOutput *>
      outputSyn = {new QueryOutput(QueryOutputType::Boolean)};
  std::list<std::string> result;

  auto queryProjector = createQueryProjector(new PKB());
  queryProjector->projectAnswer(clauseResult, outputSyn, result);

  REQUIRE(result.size() == 1);
  REQUIRE(std::find(result.begin(), result.end(), "FALSE") != result.end());
}

TEST_CASE("Test query projector with boolean true with ident") {
  auto clauseResult = new ClauseResult(true);

  std::vector<QueryOutput *>
      outputSyn = {new QueryOutput(QueryOutputType::Boolean)};
  std::list<std::string> result;

  auto queryProjector = createQueryProjector(new PKB());
  queryProjector->projectAnswer(clauseResult, outputSyn, result);

  REQUIRE(result.size() == 1);
  REQUIRE(std::find(result.begin(), result.end(), "TRUE") != result.end());
}

TEST_CASE("Test query projector with print varname attribute success") {
  auto synonym = new QuerySynonym(DesignEntityType::Print, "p");
  auto r = createRowList(
      {
          {
              {*synonym, "1"},
          },
      }
  );
  auto clauseResult = createClauseResult(r);
  auto pkb = new PKB();

  pkb->addProc("Test");

  // print a;
  pkb->addStatement(1, print, "eg");
  pkb->addUseStmt(1, "a");

  std::vector<QueryOutput *>
      outputSyn = {new QueryOutput(QueryOutputType::VarNameAttribute, synonym)};
  std::list<std::string> result;

  auto queryProjector = createQueryProjector(pkb);
  queryProjector->projectAnswer(clauseResult, outputSyn, result);

  REQUIRE(result.size() == 1);
  REQUIRE(std::find(result.begin(), result.end(), "a") != result.end());
}

TEST_CASE("Test query projector with print varname attribute fail") {
  auto synonym = new QuerySynonym(DesignEntityType::Print, "p");
  auto r = createRowList({});
  auto clauseResult = createClauseResult(r);
  auto pkb = new PKB();

  pkb->addProc("Test");

  std::vector<QueryOutput *>
      outputSyn = {new QueryOutput(QueryOutputType::VarNameAttribute, synonym)};
  std::list<std::string> result;

  auto queryProjector = createQueryProjector(pkb);
  queryProjector->projectAnswer(clauseResult, outputSyn, result);

  REQUIRE(result.empty());
}

TEST_CASE("Test query projector with read varname attribute success") {
  auto synonym = new QuerySynonym(DesignEntityType::Read, "p");
  auto r = createRowList(
      {
          {
              {*synonym, "1"},
          },
      }
  );
  auto clauseResult = createClauseResult(r);
  auto pkb = new PKB();

  pkb->addProc("Test");

  // print a;
  pkb->addStatement(1, read, "eg");
  pkb->addModifyStmt(1, "a");

  std::vector<QueryOutput *>
      outputSyn = {new QueryOutput(QueryOutputType::VarNameAttribute, synonym)};
  std::list<std::string> result;

  auto queryProjector = createQueryProjector(pkb);
  queryProjector->projectAnswer(clauseResult, outputSyn, result);

  REQUIRE(result.size() == 1);
  REQUIRE(std::find(result.begin(), result.end(), "a") != result.end());
}

TEST_CASE("Test query projector with read varname attribute fail") {
  auto synonym = new QuerySynonym(DesignEntityType::Read, "p");
  auto r = createRowList({});
  auto clauseResult = createClauseResult(r);
  auto pkb = new PKB();

  pkb->addProc("Test");

  // print a;
  pkb->addStatement(1, read, "eg");
  pkb->addModifyStmt(1, "a");

  std::vector<QueryOutput *>
      outputSyn = {new QueryOutput(QueryOutputType::VarNameAttribute, synonym)};
  std::list<std::string> result;

  auto queryProjector = createQueryProjector(pkb);
  queryProjector->projectAnswer(clauseResult, outputSyn, result);

  REQUIRE(result.empty());
}

TEST_CASE("Test query projector with variable varname attribute") {
  auto synonym = new QuerySynonym(DesignEntityType::Variable, "p");
  auto r = createRowList(
      {
          {
              {*synonym, "a"}
          }
      }
  );
  auto clauseResult = createClauseResult(r);
  auto pkb = new PKB();

  pkb->addProc("Test");

  // print a;
  pkb->addStatement(1, read, "eg");
  pkb->addModifyStmt(1, "a");

  std::vector<QueryOutput *>
      outputSyn = {new QueryOutput(QueryOutputType::VarNameAttribute, synonym)};
  std::list<std::string> result;

  auto queryProjector = createQueryProjector(pkb);
  queryProjector->projectAnswer(clauseResult, outputSyn, result);

  REQUIRE(result.size() == 1);
  REQUIRE(std::find(result.begin(), result.end(), "a") != result.end());
}

TEST_CASE("Test query projector with procedure prodName attribute") {
  auto synonym = new QuerySynonym(DesignEntityType::Procedure, "p");
  auto r = createRowList(
      {
          {
              {*synonym, "a"}
          }
      }
  );
  auto clauseResult = createClauseResult(r);
  auto pkb = new PKB();

  pkb->addProc("a");

  std::vector<QueryOutput *>
      outputSyn = {new QueryOutput(QueryOutputType::ProcNameAttribute, synonym)};
  std::list<std::string> result;

  auto queryProjector = createQueryProjector(pkb);
  queryProjector->projectAnswer(clauseResult, outputSyn, result);

  REQUIRE(result.size() == 1);
  REQUIRE(std::find(result.begin(), result.end(), "a") != result.end());
}

TEST_CASE("Test query projector with call prodName attribute success") {
  auto synonym = new QuerySynonym(DesignEntityType::Call, "p");
  auto r = createRowList(
      {
          {
              {*synonym, "1"}
          }
      }
  );
  auto clauseResult = createClauseResult(r);
  auto pkb = new PKB();

  pkb->addProc("test");
  pkb->addProc("a");
  pkb->addCall(1, "test", "a");

  std::vector<QueryOutput *>
      outputSyn = {new QueryOutput(QueryOutputType::ProcNameAttribute, synonym)};
  std::list<std::string> result;

  auto queryProjector = createQueryProjector(pkb);
  queryProjector->projectAnswer(clauseResult, outputSyn, result);

  REQUIRE(result.size() == 1);
  REQUIRE(std::find(result.begin(), result.end(), "a") != result.end());
}

TEST_CASE("Test query projector with call prodName attribute fail") {
  auto synonym = new QuerySynonym(DesignEntityType::Call, "p");
  auto r = createRowList({});
  auto clauseResult = createClauseResult(r);
  auto pkb = new PKB();

  pkb->addProc("test");
  pkb->addProc("a");
  pkb->addCall(1, "test", "a");

  std::vector<QueryOutput *>
      outputSyn = {new QueryOutput(QueryOutputType::ProcNameAttribute, synonym)};
  std::list<std::string> result;

  auto queryProjector = createQueryProjector(pkb);
  queryProjector->projectAnswer(clauseResult, outputSyn, result);

  REQUIRE(result.empty());
}

