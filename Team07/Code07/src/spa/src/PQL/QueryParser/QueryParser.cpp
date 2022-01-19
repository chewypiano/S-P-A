//
// Created by vaish on 5/9/2021.
//

#include "TokenType.h"
#include "QueryParser.h"
#include <string>
#include <sstream>

static std::unordered_set<TokenType> acceptableNameTypes({
                                                             TokenType::Variable, TokenType::Assign, TokenType::Such,
                                                             TokenType::That, TokenType::Select, TokenType::Follow,
                                                             TokenType::Parent, TokenType::Uses, TokenType::Modifies,
                                                             TokenType::Pattern, TokenType::Statement,
                                                             TokenType::Procedure,
                                                             TokenType::Read, TokenType::Print, TokenType::Call,
                                                             TokenType::While, TokenType::If, TokenType::Constant,
                                                             TokenType::Name, TokenType::With, TokenType::And,
                                                             TokenType::Boolean, TokenType::Calls, TokenType::Next,
                                                             TokenType::Affects, TokenType::NextBip,
                                                             TokenType::AffectsBip
                                                         });
static std::unordered_set<DesignEntityType> stmtAndLineTypes({
                                                                 DesignEntityType::Stmt, DesignEntityType::Assign,
                                                                 DesignEntityType::If, DesignEntityType::While,
                                                                 DesignEntityType::Print, DesignEntityType::Read,
                                                                 DesignEntityType::Call, DesignEntityType::ProgLine
                                                             });
static std::unordered_set<DesignEntityType> assignContainingStmtTypes({
                                                                          DesignEntityType::Stmt,
                                                                          DesignEntityType::Assign,
                                                                          DesignEntityType::ProgLine
                                                                      });
static std::unordered_set<ClauseType> stmtOrLineTypeClauses({
                                                                ClauseType::Parent, ClauseType::ParentT,
                                                                ClauseType::Follows,
                                                                ClauseType::FollowsT, ClauseType::Next,
                                                                ClauseType::NextT,
                                                                ClauseType::NextBip, ClauseType::NextBipT
                                                            });
static std::unordered_set<ClauseType> procTypeClauses({
                                                          ClauseType::Calls, ClauseType::CallsT
                                                      });
static std::unordered_set<ClauseType> affectClauses({
                                                        ClauseType::Affects, ClauseType::AffectsT,
                                                        ClauseType::AffectsBip
                                                    });
static std::unordered_set<TokenType> querySynonymTypes({
                                                           TokenType::Statement, TokenType::Read, TokenType::Print,
                                                           TokenType::Call, TokenType::While, TokenType::If,
                                                           TokenType::Assign, TokenType::Variable, TokenType::Constant,
                                                           TokenType::ProgLine, TokenType::Procedure
                                                       });
static std::unordered_set<DesignEntityType> procNameTypes({DesignEntityType::Procedure, DesignEntityType::Call});
static std::unordered_set<DesignEntityType>
    varNameTypes({DesignEntityType::Variable, DesignEntityType::Read, DesignEntityType::Print});
static std::unordered_set<TokenType> baseFactors({TokenType::Name, TokenType::Number});
static std::unordered_set<WithType> intWithTypes({
                                                     WithType::Synonym, WithType::Integer
                                                 });
static std::unordered_map<TokenType, DesignEntityType> tokenTypeToEntityMap = {
    {TokenType::Statement, DesignEntityType::Stmt},
    {TokenType::Read, DesignEntityType::Read},
    {TokenType::Print, DesignEntityType::Print},
    {TokenType::Call, DesignEntityType::Call},
    {TokenType::While, DesignEntityType::While},
    {TokenType::If, DesignEntityType::If},
    {TokenType::Assign, DesignEntityType::Assign},
    {TokenType::Variable, DesignEntityType::Variable},
    {TokenType::Constant, DesignEntityType::Constant},
    {TokenType::ProgLine, DesignEntityType::ProgLine},
    {TokenType::Procedure, DesignEntityType::Procedure}
};
static std::unordered_map<TokenType, QueryOutputType> tokenTypeToQueryOutputMap = {
    {TokenType::StmtNo, QueryOutputType::StmtNoAttribute},
    {TokenType::ProcName, QueryOutputType::ProcNameAttribute},
    {TokenType::VarName, QueryOutputType::VarNameAttribute},
    {TokenType::Value, QueryOutputType::ValueAttribute},
};
static std::unordered_map<TokenType, AttributeType> tokenTypeToAttributeTypeMap = {
    {TokenType::ProcName, AttributeType::ProcName},
    {TokenType::VarName, AttributeType::VarName},
    {TokenType::Value, AttributeType::Value},
    {TokenType::StmtNo, AttributeType::StmtNo}
};
static std::unordered_map<TokenType, ClauseType> tokenTypeToRelRefMap = {
    // Modifies and Uses will be assigned to correct type later
    {TokenType::Modifies, ClauseType::UnidentifiedModifies},
    {TokenType::Uses, ClauseType::UnidentifiedUses},
    {TokenType::Calls, ClauseType::Calls},
    {TokenType::CallsT, ClauseType::CallsT},
    {TokenType::Parent, ClauseType::Parent},
    {TokenType::ParentT, ClauseType::ParentT},
    {TokenType::Follow, ClauseType::Follows},
    {TokenType::FollowT, ClauseType::FollowsT},
    {TokenType::Next, ClauseType::Next},
    {TokenType::NextT, ClauseType::NextT},
    {TokenType::Affects, ClauseType::Affects},
    {TokenType::AffectsT, ClauseType::AffectsT},
    {TokenType::NextBip, ClauseType::NextBip},
    {TokenType::NextBipT, ClauseType::NextBipT},
    {TokenType::AffectsBip, ClauseType::AffectsBip},
};
static std::unordered_map<TokenType, AttributeType> attributeTypeMap = {
    {TokenType::ProcName, AttributeType::ProcName},
    {TokenType::VarName, AttributeType::VarName},
    {TokenType::Value, AttributeType::Value},
    {TokenType::StmtNo, AttributeType::StmtNo}
};
static std::unordered_map<AttributeType, WithType> attributeToWithTypeMap = {
    {AttributeType::ProcName, WithType::String},
    {AttributeType::VarName, WithType::String},
    {AttributeType::Value, WithType::Integer},
    {AttributeType::StmtNo, WithType::Integer},
};

bool isValidToken(Token *token) {
  return token != nullptr && token->getType() != TokenType::Error && token->getType() != TokenType::Ignore;
}

void assertValidToken(Token *token) {
  if (isValidToken(token)) {
    return;
  }
  if (token == nullptr) {
    throw std::runtime_error("Expected Token but got EOF");
  }
  throw std::runtime_error("Expected Token but got " + token->getValue());
}

void checkIfAcceptableSynonymName(Token *token) {
  assertValidToken(token);
  // synonym : LETTER ( LETTER | DIGIT )*
  // No Wildcard, no pure numbers, no *,"
  if (acceptableNameTypes.count(token->getType()) == 0) {
    throw std::runtime_error(
        "Unexpected token in QueryParser. Expected acceptable name for synonym but received " +
            token->getValue());
  }
}

bool isSynonymStmtOrLineType(DesignEntityType type) {
  return stmtAndLineTypes.count(type) > 0;
}

bool checkIfStmtOrLineRef(Query *query, Token *token) {
  assertValidToken(token);
  return (query->checkIfSynonymExists(token->getValue()) &&
      isSynonymStmtOrLineType(query->findSynonymByName(token->getValue())->getType())) ||
      token->getType() == TokenType::Wildcard ||
      token->getType() == TokenType::Integer;
}

bool checkIfAssignOrStmtRef(Query *query, Token *token) {
  assertValidToken(token);
  return (query->checkIfSynonymExists(token->getValue()) &&
      assignContainingStmtTypes.count(query->findSynonymByName(token->getValue())->getType()) > 0) ||
      token->getType() == TokenType::Wildcard ||
      token->getType() == TokenType::Integer;
}

// if synonym exists, restricts synonym type to just input type (e.g. Variable)
bool checkIfEntRef(Tokenizer *tokenizer, Query *query, Token *token, DesignEntityType type) {
  assertValidToken(token);
  return (query->checkIfSynonymExists(token->getValue()) &&
      query->findSynonymByName(token->getValue())->getType() == type) ||
      token->getType() == TokenType::Wildcard ||
      Tokenizer::isTokenIdentOrNameString(token);
}

bool checkIfValidAttributeTypeForSynonym(Query *query, AttributeType type, std::string synonymName) {
  QuerySynonym *synonym = query->findSynonymByName(std::move(synonymName));
  DesignEntityType synonymType = synonym->getType();
  switch (type) {
    case AttributeType::ProcName: {
      // procedure.procName, call.procName
      return procNameTypes.count(synonymType) > 0;
    }
    case AttributeType::VarName: {
      // variable.varName, read.varName, print.varName : NAME
      return varNameTypes.count(synonymType) > 0;
    }
    case AttributeType::Value: {
      // constant.value : INTEGER
      return synonymType == DesignEntityType::Constant;
    }
    case AttributeType::StmtNo: {
      // stmt.stmt#, read.stmt#, print.stmt#, call.stmt#, while.stmt#, if.stmt#, assign.stmt#: INTEGER
      return stmtAndLineTypes.count(synonymType) > 0 && synonymType != DesignEntityType::ProgLine;
    }
    default:
      throw std::runtime_error("Unexpected AttributeType in QueryParser");
  }
  return true;
}

bool checkIfLeftTypeMatchesRightType(WithArgument *left, WithArgument *right) {
  WithType leftType = left->getType();
  AttributeType leftAttribute = left->getAttribute();
  WithType rightType = right->getType();
  AttributeType rightAttribute = right->getAttribute();
  switch (leftType) {
    case WithType::String:
      return rightType == WithType::String || (rightType == WithType::Attribute
          && (rightAttribute == AttributeType::ProcName || rightAttribute == AttributeType::VarName));
    case WithType::Integer:
      return intWithTypes.count(rightType) > 0 || rightType == WithType::Attribute
          && (rightAttribute == AttributeType::Value || rightAttribute == AttributeType::StmtNo);
    case WithType::Attribute: {
      WithType type = attributeToWithTypeMap.at(leftAttribute);
      return checkIfLeftTypeMatchesRightType(new WithArgument(type, left->getValue(), leftAttribute), right);
    }
    case WithType::Synonym:
      // ProgLine -> Integer
      return checkIfLeftTypeMatchesRightType(new WithArgument(WithType::Integer, left->getValue(), leftAttribute),
                                             right);
    default:
      throw std::runtime_error("Unexpected WithType in QueryParser");
  }
  return true;
}

QueryParser::QueryParser(std::string &program) {
  this->tokenizer = createPQLTokenizer(program);
  this->query = new Query();
}

void QueryParser::insertQuery(Query *queryToInsert) {
  this->query = queryToInsert;
}

void QueryParser::parse() {
  // At initial call, if no token available, then the program input is invalid -> Invalid Query generated
  if (!this->tokenizer->hasNext()) {
    this->query->setInvalidQuery();
    return;
  }

  try {
    this->parseQuery();
    auto processor = new IrrelevantClauseProcessor(query);
    processor->processIrrelevantClauses();
    this->insertQuery(query);
  } catch (std::runtime_error &error) {
    query->setInvalidQuery();
    return;
  }
}

void QueryParser::parseQuery() {
  Token *currentToken;

  if (tokenizer->hasNext()) {
    currentToken = this->tokenizer->getNext();
    assertValidToken(currentToken);
  } else if (query->getOutputList().empty() || query->getSynonymList().empty()) {
    throw std::runtime_error("Ran out of tokens before parsing all synonyms in QueryParser.");
  } else {
    // No more tokens
    return;
  }

  TokenType currentTokenType = currentToken->getType();

  if (querySynonymTypes.count(currentTokenType) > 0) {
    this->parseQuerySynonym(currentToken);
    // handles till semicolon.
    return parseQuery();
  } else if (currentTokenType == TokenType::Select) {
    parseSelectStatement();
  } else {
    throw std::runtime_error("Unexpected Token in QueryParser. Received " + currentToken->getValue() +
        " when expecting QuerySynonym or Select.");
  }

  parseClause();
}

// Parses synonym declaration, e.g.: assign a1;
void QueryParser::parseQuerySynonym(Token *currentToken) {
  DesignEntityType entityType = tokenTypeToEntityMap[currentToken->getType()];

  bool hasSynOfSameTypeToParse = true;
  Token *tokenAfterName;
  while (hasSynOfSameTypeToParse) {
    Token *nameToken = tokenizer->getNext();
    assertValidToken(nameToken);
    checkIfAcceptableSynonymName(nameToken);
    std::string name = nameToken->getValue();

    if (query->checkIfSynonymExists(name)) {
      throw std::runtime_error("Received synonym declaration with a name that has been previously declared.");
    }
    QuerySynonym newSynonym = QuerySynonym(entityType, name);
    query->insertSynonym(newSynonym);

    tokenAfterName = tokenizer->getNext();
    assertValidToken(tokenAfterName);
    hasSynOfSameTypeToParse = tokenAfterName->getType() == TokenType::Comma;
  }

  if (tokenAfterName->getType() != TokenType::Semicolon) {
    throw std::runtime_error("Unexpected token in QueryParser. Expected end of QuerySynonym declaration.");
  }
}

void QueryParser::parseSelectStatement() {
  Token *token = tokenizer->peek();
  assertValidToken(token);

  switch (token->getType()) {
    case TokenType::LAngleBracket: {
      parseOutputTuple();
      break;
    }
    case TokenType::Boolean: {
      parseOutputBoolean(token);
      break;
    }
    default:
      parseOutputSynonym();
  }

  if (token->getType() != TokenType::Boolean && query->getSynonymList().empty()) {
    throw std::runtime_error("Parsing Error in QueryParser. Reached query output without parsing any QuerySynonym.");
  }
}

void QueryParser::parseOutputTuple() {
  tokenizer->getNext(); // clear peeked token
  bool hasElemToParse = true;
  Token *commaOrEndToken;
  while (hasElemToParse) {
    QueryOutput *queryOutput = this->extractElement();
    query->insertOutput(queryOutput);
    commaOrEndToken = tokenizer->getNext();
    assertValidToken(commaOrEndToken);
    hasElemToParse = commaOrEndToken->getType() == TokenType::Comma;
  }

  if (commaOrEndToken->getType() != TokenType::RAngleBracket) {
    throw std::runtime_error("Unexpected token in select statement, expected RAngleBracket");
  }
}

void QueryParser::parseOutputBoolean(Token *token) {
  if (!query->checkIfSynonymExists(token->getValue())) {
    tokenizer->getNext(); // clear peeked token
    auto *queryOutput = new QueryOutput(QueryOutputType::Boolean);
    query->insertOutput(queryOutput);
  } else {
    parseOutputSynonym();
  }
}

void QueryParser::parseOutputSynonym() {
  QueryOutput *queryOutput = this->extractElement();
  query->insertOutput(queryOutput);
}

QueryOutput *QueryParser::extractElement() {
  Token *token = this->tokenizer->getNext();
  assertValidToken(token);
  std::string synonymName = token->getValue();
  QuerySynonym *outputSynonym = query->findSynonymByName(synonymName);

  Token *potentialAttributeToken = this->tokenizer->peek();

  QueryOutput *queryOutput;
  if (potentialAttributeToken != nullptr && tokenTypeToQueryOutputMap.count(potentialAttributeToken->getType()) > 0) {
    this->tokenizer->getNext(); // clear peeked token

    if (!checkIfValidAttributeTypeForSynonym(query,
                                             tokenTypeToAttributeTypeMap[potentialAttributeToken->getType()],
                                             synonymName)) {
      throw std::runtime_error("Invalid synonym type for the referencing attribute type");
    }
    QueryOutputType attributeType = tokenTypeToQueryOutputMap[potentialAttributeToken->getType()];
    queryOutput = new QueryOutput(attributeType, outputSynonym);
  } else {
    // normal synonym
    queryOutput = new QueryOutput(QueryOutputType::Synonym, outputSynonym);
  }

  return queryOutput;
}

// Checks if a clause exists, and if so directs to appropriate parser
void QueryParser::parseClause() {
  if (query->getOutputList().empty()) {
    throw std::runtime_error(
        "Parsing Error in QueryParser. Reached clause parsing without parsing any output QuerySynonym.");
  }
  if (!tokenizer->hasNext()) {
    // reached end of eval, end
    return;
  }
  Token *afterSelectToken = tokenizer->getNext();
  assertValidToken(afterSelectToken);
  TokenType nextTokenType = afterSelectToken->getType();

  switch (nextTokenType) {
    case TokenType::Pattern:
      parsePatternClause();
      break;
    case TokenType::Such: {
      this->extractSingleTokenOfParticularType(TokenType::That);
      parseSuchThatClause();
      break;
    }
    case TokenType::With:
      parseWithClause();
      break;
    default:
      throw std::runtime_error("Unexpected token in QueryParser. Expected clause token but received " +
          afterSelectToken->getValue());
  }

  // After managing current clause, recurse until end of query
  parseClause();
}

void QueryParser::parseSuchThatClause() {
  // "such that" already extracted, next will be ClauseType(left, right)
  Token *relRefToken = tokenizer->getNext();
  assertValidToken(relRefToken);
  TokenType relRefTokenType = relRefToken->getType();

  if (tokenTypeToRelRefMap.count(relRefTokenType) == 0) {
    throw std::runtime_error(
        "Unexpected token in QueryParser. Expected ClauseType token but received " + relRefToken->getValue());
  }
  ClauseType type = tokenTypeToRelRefMap[relRefTokenType];

  TokenPair bracketPair = this->extractBracketedContentsForSuchThat();

  if (type == ClauseType::UnidentifiedModifies || type == ClauseType::UnidentifiedUses) {
    // Modifies and Uses will be type checked to see which type
    type = getModifiesOrUsesType(type, bracketPair);
  } else if (!isValidSuchThatClauseSyntax(type, bracketPair)) {
    throw std::runtime_error("Invalid SuchThatClause syntax");
  }

  auto clause = new SuchThatClause(type, bracketPair.first->getValue(), bracketPair.second->getValue());
  query->insertClause(clause);

  // Check for 'and'
  Token *peekedToken = this->tokenizer->peek();
  if (peekedToken != nullptr && peekedToken->getType() == TokenType::And) {
    this->extractSingleTokenOfParticularType(TokenType::And);
    this->parseSuchThatClause();
  }
}

ClauseType QueryParser::getModifiesOrUsesType(ClauseType suchThatClauseType, TokenPair pair) {
  Token *left = pair.first;
  Token *right = pair.second;
  bool rhsIsValid = checkIfEntRef(tokenizer, query, right, DesignEntityType::Variable);
  if (!rhsIsValid) {
    throw std::runtime_error("Invalid Modifies or Uses rhs syntax in QueryParser");
  }

  bool lhsIsNotWildcard = left->getType() != TokenType::Wildcard;
  bool lhsIsProcType = lhsIsNotWildcard &&
      (query->checkIfSynonymExists(left->getValue()) &&
          query->findSynonymByName(left->getValue())->getType() == DesignEntityType::Procedure) ||
      Tokenizer::isTokenIdentOrNameString(left);

  if (lhsIsProcType) {
    switch (suchThatClauseType) {
      case ClauseType::UnidentifiedModifies:
        return ClauseType::ModifiesP;
      case ClauseType::UnidentifiedUses:
        return ClauseType::UsesP;
      default:
        throw std::runtime_error("Unidentified ClauseType in getModifiesOrUsesType in QueryParser");
    }
  }

  bool lhsIsEntRef = lhsIsNotWildcard && checkIfStmtOrLineRef(query, left);
  if (!lhsIsEntRef) {
    throw std::runtime_error("Unidentified syntax of Modifies/Uses Clause lhs, neither proc nor Ent type");
  }

  switch (suchThatClauseType) {
    case ClauseType::UnidentifiedModifies:
      return ClauseType::ModifiesS;
    case ClauseType::UnidentifiedUses:
      return ClauseType::UsesS;
    default:
      throw std::runtime_error("Unidentified ClauseType in getModifiesOrUsesType in QueryParser");
  }
}

bool QueryParser::isValidSuchThatClauseSyntax(ClauseType suchThatClauseType, TokenPair pair) {
  Token *left = pair.first;
  Token *right = pair.second;
  assertValidToken(left);
  assertValidToken(right);

  if (stmtOrLineTypeClauses.count(suchThatClauseType) > 0) {
    return checkIfStmtOrLineRef(query, left) && checkIfStmtOrLineRef(query, right);
  } else if (procTypeClauses.count(suchThatClauseType) > 0) {
    return checkIfEntRef(tokenizer, query, left, DesignEntityType::Procedure)
        && checkIfEntRef(tokenizer, query, right, DesignEntityType::Procedure);
  } else if (affectClauses.count(suchThatClauseType) > 0) {
    return checkIfAssignOrStmtRef(query, left) && checkIfAssignOrStmtRef(query, right);
  } else {
    throw std::runtime_error("Unexpected ClauseType suchThatClauseType in SuchThatClause in QueryParser");
  }
}

void QueryParser::parsePatternClause() {
  // "pattern" already extracted, next will be synonymName(left, right)
  Token *synonymNameToken = tokenizer->getNext();
  assertValidToken(synonymNameToken);

  std::string synonymName = synonymNameToken->getValue();
  QuerySynonym *patternSynonym = query->findSynonymByName(synonymName);

  switch (patternSynonym->getType()) {
    case DesignEntityType::Assign:
      this->parseAssignPatternClause(patternSynonym);
      break;
    case DesignEntityType::If:
      this->parseIfPatternClause(patternSynonym);
      break;
    case DesignEntityType::While:
      this->parseWhilePatternClause(patternSynonym);
      break;
    default:
      throw std::runtime_error("Invalid synonym type for pattern clause in QueryParser");
  }

  // Check for 'and'
  Token *peekedToken = this->tokenizer->peek();
  if (peekedToken != nullptr && peekedToken->getType() == TokenType::And) {
    this->extractSingleTokenOfParticularType(TokenType::And);
    this->parsePatternClause();
  }
}

void QueryParser::parseAssignPatternClause(QuerySynonym *patternSynonym) {
  // syn-assign must be declared as synonym of assignment (design entity ‘assign’).
  if (patternSynonym->getType() != DesignEntityType::Assign) {
    throw std::runtime_error("Invalid synonym for pattern clause in QueryParser");
  }

  std::string synonymName = patternSynonym->getName();
  std::pair<TokenPair, std::string> bracketPair = this->extractBracketedContentsForAssignPattern();
  Token *leftArgToken = bracketPair.first.first;
  Token *rightArgToken = bracketPair.first.second;
  std::string rightArgValue = bracketPair.second;

  if (!checkIfEntRef(tokenizer, query, leftArgToken, DesignEntityType::Variable)) {
    throw std::runtime_error("Invalid left side argument type for pattern clause in QueryParser");
  }

  if (rightArgToken->getType() != TokenType::Wildcard) {
    std::string valWithoutExtraChars;
    bool hasDoubleWildcard = false;
    if (rightArgValue.at(0) == '_') {
      hasDoubleWildcard = true;
      valWithoutExtraChars = rightArgValue.substr(2, rightArgValue.size() - 4);
    } else {
      valWithoutExtraChars = rightArgValue.substr(1, rightArgValue.size() - 2);
    }
    Tokenizer *simpleTokenizer = createSimpleTokenizer(valWithoutExtraChars);
    if (!this->isValidAssignExpr(simpleTokenizer, 0)) {
      throw std::runtime_error("Invalid right side argument type for pattern clause in QueryParser");
    }
    std::stringstream stringStream;
    std::string postFixValWithoutChars = PostFixConverter::toPostFix(valWithoutExtraChars);

    if (hasDoubleWildcard) {
      stringStream << "_\"" << postFixValWithoutChars << "\"_";
    } else {
      stringStream << "\"" << postFixValWithoutChars << "\"";
    }

    rightArgValue = stringStream.str();
  }

  auto *clause = new PatternClause(
      PatternSynonymType::Assign,
      leftArgToken->getValue(),
      rightArgValue, synonymName);
  query->insertClause(clause);
}

void QueryParser::parseIfPatternClause(QuerySynonym *patternSynonym) {
  if (patternSynonym->getType() != DesignEntityType::If) {
    throw std::runtime_error("Invalid synonym for pattern clause in QueryParser");
  }

  std::string synonymName = patternSynonym->getName();

  auto[left, centre, right] = this->extractBracketedContentsForIfPattern();

  if (!checkIfEntRef(tokenizer, query, left, DesignEntityType::Variable)) {
    throw std::runtime_error("Invalid left side argument type for pattern clause in QueryParser");
  }

  if (centre->getType() != TokenType::Wildcard || right->getType() != TokenType::Wildcard) {
    throw std::runtime_error("Invalid centre or argument type for pattern clause in QueryParser");
  }

  auto *clause =
      new PatternClause(PatternSynonymType::If, left->getValue(), centre->getValue(), right->getValue(), synonymName);
  query->insertClause(clause);
}

void QueryParser::parseWhilePatternClause(QuerySynonym *patternSynonym) {
  if (patternSynonym->getType() != DesignEntityType::While) {
    throw std::runtime_error("Invalid synonym for pattern clause in QueryParser");
  }

  std::string synonymName = patternSynonym->getName();
  auto[left, right] = this->extractBracketedContentsForWhilePattern();

  if (!checkIfEntRef(tokenizer, query, left, DesignEntityType::Variable)) {
    throw std::runtime_error("Invalid left side argument type for pattern clause in QueryParser");
  }

  if (right->getType() != TokenType::Wildcard) {
    throw std::runtime_error("Invalid centre or argument type for pattern clause in QueryParser");
  }

  auto
      *clause = new PatternClause(PatternSynonymType::While, left->getValue(), right->getValue(), synonymName);
  query->insertClause(clause);
}

void QueryParser::parseWithClause() {
  // 'with' already extracted
  auto leftArg = extractArgumentForWith();
  this->extractSingleTokenOfParticularType(TokenType::Equal);
  auto rightArg = extractArgumentForWith();

  // Check that left type matches right type
  if (!checkIfLeftTypeMatchesRightType(leftArg, rightArg)) {
    throw std::runtime_error("Left type does not match right type in WithClause for QueryParser");
  }

  auto *clause = new WithClause(leftArg, rightArg);
  query->insertClause(clause);

  // Check for 'and'
  Token *peekedToken = this->tokenizer->peek();
  if (peekedToken != nullptr && peekedToken->getType() == TokenType::And) {
    this->extractSingleTokenOfParticularType(TokenType::And);
    this->parseWithClause();
  }
}

// For Such that: left|right = Name|String?
TokenPair QueryParser::extractBracketedContentsForSuchThat() {
  this->extractSingleTokenOfParticularType(TokenType::LBracket);

  Token *left = tokenizer->getNext();
  assertValidToken(left);

  this->extractSingleTokenOfParticularType(TokenType::Comma);

  Token *right = tokenizer->getNext();
  assertValidToken(right);

  this->extractSingleTokenOfParticularType(TokenType::RBracket);

  return {left, right};
}

// For Pattern: left|right = _ | String
std::pair<TokenPair, std::string> QueryParser::extractBracketedContentsForAssignPattern() {
  this->extractSingleTokenOfParticularType(TokenType::LBracket);

  Token *left = tokenizer->getNext();
  assertValidToken(left);

  this->extractSingleTokenOfParticularType(TokenType::Comma);

  Token *tokenAfterComma = tokenizer->getNext();
  assertValidToken(tokenAfterComma);

  Token *right;
  std::stringstream rightVal;
  switch (tokenAfterComma->getType()) {
    case TokenType::Wildcard:
      right = this->extractWildcardRightArgInAssignPattern(&rightVal, tokenAfterComma);
      break;
    case TokenType::String: {
      right = this->extractStringRightArgInAssignPattern(&rightVal, tokenAfterComma);
      break;
    }
    default:
      throw std::runtime_error("Unexpected token on right side of pattern clause in QueryParser. Received " +
          tokenAfterComma->getValue());
  }

  return {{left, right}, rightVal.str()};
}

Token *QueryParser::extractWildcardRightArgInAssignPattern(std::stringstream *rightVal, Token *tokenAfterComma) {
  Token *right;
  *rightVal << "_";
  Token *tokenAfterWildcard = tokenizer->getNext();
  assertValidToken(tokenAfterWildcard);
  switch (tokenAfterWildcard->getType()) {
    case TokenType::String: {
      right = tokenAfterWildcard;
      right->setValue(tokenAfterWildcard->getValue());
      *rightVal << tokenAfterWildcard->getValue() << "_";
      // Current implementation for right: "\"string_val\"" (so inner "" chars are included)
      this->extractSingleTokenOfParticularType(TokenType::Wildcard);
      this->extractSingleTokenOfParticularType(TokenType::RBracket);
      break;
    }
    case TokenType::RBracket:
      // arg only had wildcard
      right = tokenAfterComma;
      break;
    default:
      throw std::runtime_error(
          "Unexpected token after wildcard on right side of pattern clause in QueryParser. Received "
              + tokenAfterComma->getValue());
  }

  return right;
}

Token *QueryParser::extractStringRightArgInAssignPattern(std::stringstream *rightVal, Token *tokenAfterComma) {
  Token *right = tokenAfterComma;
  std::string whitespaceExtractedRightVal = Tokenizer::extractWhitespace(right->getValue());
  *rightVal << whitespaceExtractedRightVal;
  this->extractSingleTokenOfParticularType(TokenType::RBracket);
  return right;
}

std::tuple<Token *, Token *, Token *> QueryParser::extractBracketedContentsForIfPattern() {
  this->extractSingleTokenOfParticularType(TokenType::LBracket);

  Token *left = tokenizer->getNext();
  assertValidToken(left);

  this->extractSingleTokenOfParticularType(TokenType::Comma);

  Token *centre = tokenizer->getNext();
  assertValidToken(centre);

  this->extractSingleTokenOfParticularType(TokenType::Comma);

  Token *right = tokenizer->getNext();
  assertValidToken(right);

  this->extractSingleTokenOfParticularType(TokenType::RBracket);

  return std::make_tuple(left, centre, right);
}

TokenPair QueryParser::extractBracketedContentsForWhilePattern() {
  this->extractSingleTokenOfParticularType(TokenType::LBracket);

  Token *left = tokenizer->getNext();
  assertValidToken(left);

  this->extractSingleTokenOfParticularType(TokenType::Comma);

  Token *right = tokenizer->getNext();
  assertValidToken(right);

  this->extractSingleTokenOfParticularType(TokenType::RBracket);

  return {left, right};
}

WithArgument *QueryParser::extractArgumentForWith() {
  Token *firstToken = tokenizer->getNext();
  assertValidToken(firstToken);

  WithType type;
  std::string value;
  AttributeType attributeType;

  switch (firstToken->getType()) {
    case TokenType::String:
      type = WithType::String;
      if (!Tokenizer::isTokenIdentOrNameString(firstToken)) {
        throw std::runtime_error("Unexpected token in QueryParser, did not get valid string token");
      }
      value = firstToken->getValue();
      attributeType = AttributeType::NotApplicable;
      break;
    case TokenType::Integer:
      type = WithType::Integer;
      value = firstToken->getValue();
      attributeType = AttributeType::NotApplicable;
      break;
    default:
      // either synonym or attRef
      return this->extractSynOrAttributeArgumentForWith(firstToken);
  }

  return new WithArgument(type, value, attributeType);
}

WithArgument *QueryParser::extractSynOrAttributeArgumentForWith(Token *token) {
  Token *synonym = token;

  WithType type;
  std::string value;
  AttributeType attributeType;

  if (!query->checkIfSynonymExists(synonym->getValue())) {
    throw std::runtime_error("Got undeclared synonym in with clause");
  }
  Token *peekedToken = tokenizer->peek();

  if (peekedToken == nullptr
      && query->findSynonymByName(synonym->getValue())->getType() != DesignEntityType::ProgLine) {
    throw std::runtime_error("Received synonym token in WithClause, that is not ProgLine.");
  } else if (peekedToken == nullptr || attributeTypeMap.count(peekedToken->getType()) == 0) {
    // No more token -> no more attributes
    type = WithType::Synonym;
    value = synonym->getValue();
    attributeType = AttributeType::NotApplicable;
  } else {
    assertValidToken(peekedToken);
    TokenType peekedTokenType = peekedToken->getType();
    tokenizer->getNext(); // Clear peeked token
    type = WithType::Attribute;
    value = synonym->getValue();
    attributeType = attributeTypeMap[peekedTokenType];

    if (!checkIfValidAttributeTypeForSynonym(query, attributeType, value)) {
      throw std::runtime_error("Invalid synonym type for the referencing attributeType type");
    }
  }

  return new WithArgument(type, value, attributeType);
}

bool QueryParser::isValidAssignExpr(Tokenizer *simpleTokenizer, int bracketLevel) {
  Token *token = simpleTokenizer->getNext();
  assertValidToken(token);

  if (baseFactors.count(token->getType()) > 0) {
    // factor: var_name | const_value
    return this->isValidExprAfterLhsExpr(simpleTokenizer, bracketLevel);
  } else if (token->getType() == TokenType::LBracket) {
    // factor: ‘(’ expr ‘)’
    bool innerBracketedExprIsValid =
        this->isValidAssignExpr(simpleTokenizer, bracketLevel + 1) && simpleTokenizer->hasNext();
    if (!innerBracketedExprIsValid) {
      return false;
    }

    Token *rhsBracketToken = simpleTokenizer->getNext();
    assertValidToken(rhsBracketToken);
    if (rhsBracketToken->getType() != TokenType::RBracket) {
      return false;
    }

    return this->isValidExprAfterLhsExpr(simpleTokenizer, bracketLevel);
  } else {
    // Unidentified Token
    return false;
  }
}

bool QueryParser::isValidExprAfterLhsExpr(Tokenizer *simpleTokenizer, int bracketLevel) {
  if (!simpleTokenizer->hasNext()) {
    // end of expr/factor/term
    return true;
  }

  Token *tokenAfterFactor = simpleTokenizer->peek();
  assertValidToken(tokenAfterFactor);

  if (tokenAfterFactor->getType() == TokenType::Arithmetic) {
    // evaluate expr after operator
    simpleTokenizer->getNext(); // clear peeked token
    return this->isValidAssignExpr(simpleTokenizer, bracketLevel);
  }

  return tokenAfterFactor->getType() == TokenType::RBracket && bracketLevel > 0;
}

void QueryParser::extractSingleTokenOfParticularType(TokenType requiredType) {
  Token *expectedToken = tokenizer->getNext();
  assertValidToken(expectedToken);
  if (expectedToken->getType() != requiredType) {
    throw std::runtime_error("Unexpected token when extracting single token in QueryParser. Received " +
        expectedToken->getValue());
  }
}

std::string QueryParser::parsedQueryToString() {
  std::stringstream stringStream;
  stringStream << "Parsed: [\n" << this->query->toString() << "\n]";
  return stringStream.str();
}

Query QueryParser::getQuery() {
  return *this->query;
}
