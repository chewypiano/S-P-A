#include "TestWrapper.h"
#include "SimpleParser.h"
#include "QueryParser.h"
#include "QueryEvaluator.h"
#include "QueryProjector.h"
#include <fstream>
#include <sstream>
#include <string>

// implementation code of WrapperFactory - do NOT modify the next 5 lines
AbstractWrapper *WrapperFactory::wrapper = 0;
AbstractWrapper *WrapperFactory::createWrapper() {
  if (wrapper == 0) wrapper = new TestWrapper;
  return wrapper;
}
// Do not modify the following line
volatile bool AbstractWrapper::GlobalStop = false;

// a default constructor
TestWrapper::TestWrapper() {
  // create any objects here as instance variables of this class
  // as well as any initialization required for your spa program
  this->pkb = new PKB();
}

// method for parsing the SIMPLE source
void TestWrapper::parse(std::string filename) {
  // call your parser to do the parsing
  std::string line;

  std::ifstream file;
  file.open(filename);
  std::basic_stringstream<char> program; // Read file here
  if (!file.is_open()) {
    std::cout << "File cannot be opened: " << filename << std::endl;
    return;
  }
  while (std::getline(file, line)) {
    program << line << "\n";
  }
  file.close();
  std::string programStr = program.str();
  auto parser = new SimpleParser(pkb, programStr);
  parser->parse();
  auto dAbstractor = new DesignAbstractor(pkb);
  dAbstractor->process();
}

// method to evaluating a query
void TestWrapper::evaluate(std::string query, std::list<std::string> &results) {
  // call your evaluator to evaluate the query here
  auto parser = new QueryParser(query);
  parser->parse();
  auto result = (new QueryEvaluator(parser, this->pkb))->evaluate();
  (new QueryProjector(pkb))->projectAnswer(result, result->getOutput(), results);
}
