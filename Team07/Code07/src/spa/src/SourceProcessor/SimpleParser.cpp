//
// Created by Kamil Gimik on 2/9/21.
//

#include "SimpleParser.h"

SimpleParser::SimpleParser(PKB *pkb, std::string &program) {
  this->tokenizer = createSimpleTokenizer(program);
  this->currentStmtNum = 0;
  this->startingStmtNum = 0;
  this->currentToken = nullptr;
  this->pkb = pkb;
  this->pkbPopulator = new PkbPopulator(this->pkb);
  this->prevTokens = std::vector<Token *>();
  this->parenthesisStack = std::stack<Token *>();
  this->parentStack = std::stack<int>();
  this->isFollowable = false;
  this->isFollowsAfterNesting = false;
  this->followStack = std::stack<int>();
  this->prevIf = 0;
  this->prevWhile = 0;
  this->prevAssign = 0;
  this->prevClosingRBrace = nullptr;
  this->currentProcedure = "";
}

bool SimpleParser::parse() {

  while (this->tokenizer->hasNext()) {
    if (this->parenthesisStack.empty() && !this->hasCurrentProcedure()) {
      this->startingStmtNum = this->currentStmtNum;
      this->parseProcedure();
    } else {
      this->parseLine();
    }
  }

  bool programNotClosedProperly = !this->parenthesisStack.empty() || this->hasCurrentProcedure();

  if (programNotClosedProperly) {
    throw std::runtime_error("SimpleParser: Procedure not closed properly");
  }

  return true;
}

bool SimpleParser::hasCurrentProcedure() {
  return !this->currentProcedure.empty();
}

bool SimpleParser::isNested() {
  return !this->parentStack.empty();
}

// gets next token, throws error if nullptr i.e. EOF
Token *SimpleParser::populateToken() {
  this->currentToken = this->tokenizer->getNext();
  this->prevTokens.emplace_back(this->currentToken);
  if (this->currentToken == nullptr) {
    throw std::runtime_error("Current token is nullptr");
  }
  if (this->currentToken->getType() == Error) { // second expression may not be necessary
    throw std::runtime_error("SimpleParser: Error Token surfaced");
  }
  return this->currentToken;
}

// gets next token of expected type, throws error if token is ill-typed
Token *SimpleParser::populateToken(TokenType expectedType) {
  this->populateToken();
  SourceProcessorUtils::expectType(this->currentToken, expectedType);
  return this->currentToken;
}

// gets next token of expected type, throws error if token is ill-typed
Token *SimpleParser::populateToken(const std::unordered_set<TokenType> &expectedTypes) {
  this->populateToken();
  TokenType currTokenType = this->currentToken->getType();
  SourceProcessorUtils::expectType(this->currentToken, expectedTypes);
  return this->currentToken;
}

// ( Name Number
Token *SimpleParser::populateToken(TokenType expectedType, const std::string &expectedValue) {
  this->populateToken(expectedType);
  if (this->currentToken->getValue() != expectedValue) {
    throw std::runtime_error(
        "SimpleParser: Expected " + expectedValue + " but received " + this->currentToken->getValue());
  }
  return this->currentToken;
}

bool SimpleParser::parseProcedure() {
  populateToken(Name, "procedure");
  this->currentProcedure = populateToken(Name)->getValue();

  // TODO:: if current procedure in procTable, throw error term
  this->pkbPopulator->populateProcTable(this->currentToken);

  populateToken(LBrace);
  this->parenthesisStack.push(this->currentToken);

  return true;
}

bool SimpleParser::parseLine() {
  Token *firstToken = this->populateToken();
  TokenType type1 = firstToken->getType();
  if (type1 == Name) {
    parseStmt();
  } else if (type1 == RBrace) {
    parseSymbol();
  } else {
    throw std::runtime_error("SimpleParser: Line begins with invalid TokenType: " + TokenToName::tokenToName(type1));
  }
  return false;
}

bool SimpleParser::parseStmt() {
  // will call parseAssign()/ parseWhile()/ parseIf() etc. based on currToken and nextToken
  Token *firstToken = prevTokens.back();
  Token *secondToken = populateToken();
  TokenType type2 = secondToken->getType();
  std::string firstTokenValue = firstToken->getValue();
  this->currentStmtNum++;

  this->updateParentsAndFollowsRelations();

  switch (type2) {
    case Equal:
      this->parseAssign();
      break;
    case LBracket:
      this->parenthesisStack.push(this->currentToken);
      if (firstTokenValue == "if") {
        this->parseIf();
        break;
      }
      if (firstTokenValue == "while") {
        this->parseWhile();
        break;
      } else {
        throw std::runtime_error("SimpleParser: Expected while or if before LBracket, not: " + firstTokenValue);
      }
    case Name:
      if (firstTokenValue == "call") {
        this->parseCall();
        break;
      }
      if (firstTokenValue == "read") {
        this->parseRead();
        break;
      }
      if (firstTokenValue == "print") {
        this->parsePrint();
        break;
      } else {
        throw std::runtime_error("SimpleParser: Expected read, print or call before Variable Name, not: "
                                     + firstTokenValue);
      }
    default:
      throw std::runtime_error("SimpleParser: Unexpected tokenType");
  }

  return true;
}

bool SimpleParser::parseAssign() {
  this->prevAssign = this->currentStmtNum;
  Token *variableToken = prevTokens[prevTokens.size() - 2];
  this->pkbPopulator->populateVarTable(variableToken);
  this->pkbPopulator->populateModifiesTable(variableToken, this->currentStmtNum, this->currentProcedure);
  std::vector<Token *> expressionVector = parseExpression();
  SourceProcessorUtils::expectType(this->currentToken, Semicolon);
  this->pkbPopulator->populateAssignTable(variableToken, expressionVector,
                                          this->currentStmtNum, this->currentProcedure);
  this->pkbPopulator->populatePostFix(this->currentStmtNum, PostFixConverter::toPostFix(expressionVector));
  return true;
}

// TODO: Add test after error handling finalised
bool SimpleParser::parseCall() {

  Token *prevToken = this->prevTokens.back();
  this->pkbPopulator->populateCallTable(prevToken, this->currentStmtNum, this->currentProcedure);
  this->populateToken(Semicolon);
  return true;
}

// TODO: Add test after error handling finalised
bool SimpleParser::parseRead() {

  Token *var = this->currentToken;

  this->pkbPopulator->populateVarTable(var);

  this->populateToken(Semicolon);
  this->pkbPopulator->populateStmtTable(read, this->currentStmtNum, this->currentProcedure);
  this->pkbPopulator->populateModifiesTable(var, this->currentStmtNum, this->currentProcedure);

  return true;
}

// TODO: Add test after error handling finalised
bool SimpleParser::parsePrint() {

  Token *var = this->currentToken;

  this->pkbPopulator->populateVarTable(var);

  this->populateToken(Semicolon);
  this->pkbPopulator->populateStmtTable(print, this->currentStmtNum, this->currentProcedure);
  this->pkbPopulator->populateUsesTable(var, this->currentStmtNum,
                                        prevIf, prevWhile, this->currentProcedure);

  return true;
}

// TODO: Add tests
bool SimpleParser::parseSymbol() {
  int size = this->parenthesisStack.size();
  switch (size) {
    case (0):
      throw std::runtime_error("SimpleParser: Procedure not properly enclosed");
    case (1):
      SourceProcessorUtils::expectType(this->parenthesisStack.top(), LBrace);
      this->parenthesisStack.pop();

      if (!this->parenthesisStack.empty() || this->startingStmtNum == this->currentStmtNum) {
        throw std::runtime_error("SimpleParser: Procedure not closed properly");
      }

      this->resetVariables();
      break;
    default:
      SourceProcessorUtils::expectType(this->parenthesisStack.top(), LBrace);
      this->parenthesisStack.pop();
  }
  return true;
}

void SimpleParser::resetVariables() {
  this->currentProcedure = "";
  this->parentStack = std::stack<int>();
  this->isFollowable = false;
  this->isFollowsAfterNesting = false;
  this->followStack = std::stack<int>();
  this->prevIf = 0;
  this->prevWhile = 0;
  this->prevAssign = 0;
  this->prevClosingRBrace = nullptr;
  this->prevTokens = std::vector<Token *>();
}

// TODO: maybe add expr stack for parentheses within an expr
std::vector<Token *> SimpleParser::parseExpression() {
  // at this point:: current is either ( =
  std::stack<Token *> localBracketStack =
      std::stack<Token *>(); // size ==0 and global is not ( , throw error , else end if statement and expect then
  std::vector<Token *> expressionVector = std::vector<Token *>();
  expressionVector.push_back(this->populateToken({Name, Number, LBracket, Logical, Negate}));

  std::stack<int> extraBrackets = std::stack<int>(); // for negate validity check

  TokenType prevType;

  bool isEndOfExpression = false;

  while (!isEndOfExpression) {
    prevType = this->currentToken->getType();
    switch (prevType) {
      case (Negate):
        if (this->prevTokens[prevTokens.size() - 2]->getType() == Relative
            || this->prevTokens[prevTokens.size() - 2]->getType() == Equals) {
          throw std::runtime_error("SimpleParser: Negation Error. Can't have Equals , Relative before Negate");
        }
        if (prevAssign == this->currentStmtNum) {
          throw std::runtime_error("SimpleParser: Assign cannot have negate");
        }
        extraBrackets.push(localBracketStack.size());
        expressionVector.push_back(this->populateToken(LBracket));
        break;
      case (LBracket):
        localBracketStack.push(this->currentToken);
        expressionVector.push_back(this->populateToken({Negate, Name, Number, LBracket}));
        break;
      case (Name):
        this->pkbPopulator->populateVarTable(this->currentToken);
        this->pkbPopulator->populateUsesTable(this->currentToken, this->currentStmtNum,
                                              prevIf, prevWhile, this->currentProcedure);
        expressionVector.push_back(this->populateToken({Arithmetic, RBracket, Semicolon, Relative}));
        break;
      case (Number):
        this->pkbPopulator->populateConstTable(this->currentToken);
        expressionVector.push_back(this->populateToken({Arithmetic, RBracket, Semicolon, Relative}));
        break;
      case (Arithmetic):
        expressionVector.push_back(this->populateToken({Name, Number, LBracket}));
        break;
      case (RBracket):
        if (localBracketStack.empty()) {
          if (this->parenthesisStack.top()->getType() != LBracket) {
            throw std::runtime_error("SimpleParser: Bracket error, did not expect RBracket");
          }
          SourceProcessorUtils::expectType(this->parenthesisStack.top(), LBracket);
          this->parenthesisStack.pop();   ///// NEW CHANGE <- for parseCondExpr
          isEndOfExpression = true;
          break;
        }
        localBracketStack.pop();
        // should be able to end   // end at ; ) ==
        expressionVector.push_back(this->populateToken({Arithmetic, Semicolon, RBracket, Relative}));
        break;
      case (Semicolon):
      case (Relative):
        while (!extraBrackets.empty()) {
          int currTally = extraBrackets.top();
          extraBrackets.pop();
          int negateDiff = localBracketStack.size() - currTally;
          if (negateDiff <= 0) {
            throw std::runtime_error("SimpleParser: Negate declared wrongly");
          }

        }
        while (!localBracketStack.empty()) {
          this->parenthesisStack.push(localBracketStack.top());
          localBracketStack.pop();
        }
        isEndOfExpression = true;
        break;
      case (Logical):
        throw std::runtime_error("SimpleParser: Logical cannot appear mid expression");
      default:
        throw std::runtime_error("SimpleParser: parseExpression unexpected tokenType");
    }
  }

  return expressionVector;

}

std::vector<Token *> SimpleParser::parseCondExpr(int size = 0) {
  int sizeBefore = size;
  if (size == 0) {
    sizeBefore = this->parenthesisStack.size();
  }
  std::vector<Token *> leftExpression = this->parseExpression();
  SourceProcessorUtils::expectType(this->currentToken, Relative);
  std::vector<Token *> rightExpression = this->parseExpression();
  std::vector<Token *> fullExpression = leftExpression;
  fullExpression.insert(fullExpression.end(), rightExpression.begin(), rightExpression.end());
  while ((this->tokenizer->peek()->getType() == RBracket) && (sizeBefore < this->parenthesisStack.size())) {
    fullExpression.push_back(this->populateToken());
    SourceProcessorUtils::expectType(this->parenthesisStack.top(), LBracket);
    this->parenthesisStack.pop();
  }
  return fullExpression;
}

void SimpleParser::parseCondExprs() {
  int numCalls = 1;
  int sizeBefore = this->parenthesisStack.size(); // currsize of parenthesis stack
  std::vector<Token *> currExpr = this->parseCondExpr();
  SourceProcessorUtils::expectType(this->currentToken, RBracket);
  int diff = this->parenthesisStack.size() - sizeBefore;
  std::vector<Token *> newExpr;
  while (diff >= 0) {
    Token *next = this->tokenizer->peek();
    switch (next->getType()) {
      case RBracket:
        this->populateToken(RBracket);
        SourceProcessorUtils::expectType(this->parenthesisStack.top(), LBracket);
        this->parenthesisStack.pop();
        diff--;
        break;
      case Logical:
        numCalls++;
        currExpr.push_back(this->populateToken(Logical));
        SourceProcessorUtils::expectType(this->tokenizer->peek(), LBracket);
        newExpr = this->parseCondExpr(sizeBefore);
        currExpr.insert(currExpr.end(), newExpr.begin(), newExpr.end());
        diff = this->parenthesisStack.size() - sizeBefore;
        break;
      default:
        throw std::runtime_error("SimpleParser: CondExpr invalid token " + TokenToName::tokenToName(next->getType()));
    }
  }
  if (diff < -1) {
    throw std::runtime_error("SimpleParser: CondExpr too many brackets");
  }
//    bool isNotEnclosed = numCalls > 1 && !isEnclosedProperly(currExpr);
  bool isNotEnclosed = !SourceProcessorUtils::isEnclosedProperly(currExpr);
  //bool isNotEnclosed = numCalls > 1 && !SourceProcessorUtils::isEnclosedProperly(currExpr);
  if (isNotEnclosed) {
    throw std::runtime_error("SimpleParser: Logical token must come between enclosed cond exprs");
  }
}

bool SimpleParser::parseIf() {
  int stmtNum = this->currentStmtNum;
  this->prevIf = stmtNum;
  this->parseCondExprs();
  SourceProcessorUtils::expectType(this->currentToken, RBracket);

  this->populateToken(Name, "then");
  this->parentStack.push(this->currentStmtNum);
  this->parseLinesInNesting();

  if (this->currentStmtNum == stmtNum) {
    throw std::runtime_error("SimpleParser: If must contain at least 1 stmt in its block");
  }

  int elseBeginStmt = this->currentStmtNum;

  this->populateToken(Name, "else");
  this->parseLinesInNesting();

  if (this->currentStmtNum == elseBeginStmt) {
    throw std::runtime_error("SimpleParser: Else must contain at least 1 stmt in its block");
  }

  this->pkbPopulator->populateIfTable(stmtNum, this->currentStmtNum, elseBeginStmt + 1, this->currentProcedure);

  this->updateLocalVariablesAfterNestingEnds();
  return true;
}

bool SimpleParser::parseWhile() {
  int stmtNum = this->currentStmtNum;
  this->prevWhile = stmtNum;
  this->parseCondExprs();
  SourceProcessorUtils::expectType(this->currentToken, RBracket);
  this->parentStack.push(this->currentStmtNum);
  this->parseLinesInNesting();

  if (this->currentStmtNum == stmtNum) {
    throw std::runtime_error("SimpleParser: While must contain at least 1 stmt in its block");
  }

  this->pkbPopulator->populateWhileTable(stmtNum, this->currentStmtNum, this->currentProcedure);

  this->updateLocalVariablesAfterNestingEnds();

  return true;
}

void SimpleParser::updateLocalVariablesAfterNestingEnds() {
  if (this->tokenizer->peek()->getType() != RBrace) {
    this->followStack.push(this->parentStack.top());
  }
  this->parentStack.pop();
  this->isFollowsAfterNesting = true;
}

int SimpleParser::getCurrentParent() {
  return this->parentStack.top();
}

void SimpleParser::parseLinesInBlock() {

  while (true) {

    bool isRbrace = this->currentToken->getType() == RBrace;
    bool isNotPrevClosingRbrace = this->prevClosingRBrace != this->currentToken;

    if (isRbrace && isNotPrevClosingRbrace) {
      this->prevClosingRBrace = this->currentToken;
      break;
    }
    this->parseLine();
  }
}

void SimpleParser::parseLinesInNesting() {
  this->parenthesisStack.push(populateToken(LBrace));
  this->isFollowable = false;

  this->parseLinesInBlock();
}

void SimpleParser::updateParentsAndFollowsRelations() {
  if (this->isNested()) {
    this->pkbPopulator->populateParentTable(this->getCurrentParent(), this->currentStmtNum);
  }
  if (this->isFollowable) {
    if (this->followStack.empty() || !this->isFollowsAfterNesting) {
      this->pkbPopulator->populateFollowsTable(this->currentStmtNum - 1, this->currentStmtNum);
    } else {
      int prev = this->followStack.top();
      this->pkbPopulator->populateFollowsTable(prev, this->currentStmtNum);
      this->followStack.pop();
      this->isFollowsAfterNesting = false;
    }
  } else {
    this->isFollowable = true;
  }
}
