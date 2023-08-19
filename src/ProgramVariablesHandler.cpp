#include "ProgramVariablesHandler.h"
#include "Debug.h"

void ProgramVariablesHandler::addAlias(ProgramVariable receiving,
                                       ProgramVariable receiver) {
  for (ProgramVariable &pv : this->programVariables) {
    if (pv.getRawName() == receiving.getRawName()) {
      pv.addAlias(receiver);
      return;
    } else if (pv.getRawName() == receiver.getRawName()) {
      pv.addAlias(receiving);
      return;
    }

    for (std::string alias : pv.getAllAliases(false)) {
      if (alias == receiving.getRawName()) {
        pv.addAlias(receiver);
        return;
      } else if (alias == receiver.getRawName()) {
        pv.addAlias(receiving);
        return;
      }
    }
  }

  receiver.addAlias(receiving);
  this->programVariables.push_back(receiver);
}

void ProgramVariablesHandler::addVariable(ProgramVariable programVar) {
  this->programVariables.push_back(programVar);
}

std::list<ProgramVariable> ProgramVariablesHandler::getProgramVariables() {
  return this->programVariables;
}

std::set<std::string>
ProgramVariablesHandler::findVarAndNamedAliases(std::string cleanedName) {
  for (ProgramVariable &pv : this->programVariables) {
    if (pv.getCleanedName() == cleanedName) {
      return pv.getNamedAliases(true);
    }

    for (std::string alias : pv.getAllAliases(true)) {
      if (alias == cleanedName) {
        return pv.getNamedAliases(true);
      }
    }
  }

  return std::set<std::string>();
}