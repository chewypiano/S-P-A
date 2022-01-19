//
// Created by vaish on 5/9/2021.
//

#include "DesignEntityType.h"

std::unordered_map<DesignEntityType, std::string> DesignEntityToName::convertTable = {
    {DesignEntityType::Stmt, "Stmt"},
    {DesignEntityType::Read, "Read"},
    {DesignEntityType::Print, "Print"},
    {DesignEntityType::Call, "Call"},
    {DesignEntityType::While, "While"},
    {DesignEntityType::If, "If"},
    {DesignEntityType::Assign, "Assign"},
    {DesignEntityType::Variable, "Variable"},
    {DesignEntityType::Constant, "Constant"},
    {DesignEntityType::ProgLine, "ProgLine"},
    {DesignEntityType::Procedure, "Procedure"}
};

std::string DesignEntityToName::designEntityToName(DesignEntityType entityType) {
  return DesignEntityToName::convertTable[entityType];
}
