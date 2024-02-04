#include "Debug/BranchLister/ProgramLinesBranchInfo.h"
#include "Debug.h"

ProgramLinesBranchInfo::ProgramLinesBranchInfo() {

}

void ProgramLinesBranchInfo::add(Function& function) {
    std::string functionName = function.getName().str();

    int branchCount = 0;
    for (inst_iterator I = inst_begin(function), E = inst_end(function); I != E; ++I) {
        Instruction* instruction = &(*I);
        if (isa<BranchInst>(instruction)) {
            branchCount++;
        }
    }

    if (branchCount <= 1) {
        return;
    }


    for (inst_iterator I = inst_begin(function), E = inst_end(function); I != E; ++I) {
        Instruction* instruction = &(*I);

        DebugLoc instDebugInfo = instruction->getDebugLoc();

        if (!instDebugInfo) {
            continue;
        }

        unsigned lineNumber = instDebugInfo->getLine();
        auto preds = rlc_dataflow::getPredecessors(instruction);
        std::string branchName = instruction->getParent()->getName().str();

        LineBranchInfo lbi = LineBranchInfo(lineNumber, branchName, preds, functionName);

        if (this->exists(lbi)) {
            continue;
        }

        this->lineBranches.push_back(lbi);
    }

}

bool ProgramLinesBranchInfo::exists(const LineBranchInfo otherLBI) const {
    for (auto lbi : this->lineBranches) {
        if (lbi.getBranchName() == otherLBI.getBranchName() && lbi.getFunctionName() == otherLBI.getFunctionName()) {
            return true;
        }
    }

    return false;
}

LineBranchInfo* ProgramLinesBranchInfo::getLBI(unsigned lineNumber) {
    for (LineBranchInfo& lbi : this->lineBranches) {
        if (lbi.getLineNumber() == lineNumber) {
            return &lbi;
        }
    }

    return NULL;
}

void ProgramLinesBranchInfo::generate(std::string cFileName, bool overwrite) {
    std::ifstream cFile(cFileName);
    std::string cLine;
    int cLineNumber = 1;
    std::vector<std::string> overWrittenFile;

    if (!cFile.is_open()) {
        logout("Could not open file");
        std::exit(1);
    }

    std::ifstream cFileForLineCounting(cFileName);
    int lineCount = std::count(std::istreambuf_iterator<char>(cFileForLineCounting),
                               std::istreambuf_iterator<char>(), '\n');

    while (std::getline(cFile, cLine)) {
        if (auto lbi = this->getLBI(cLineNumber)) {
            std::string whiteSpaceOfCurrentLine;
            for (int i = 0; i < cLine.length(); i++) {
                if (isspace(cLine[i])) {
                    whiteSpaceOfCurrentLine += cLine[i];
                }
            }
            std::string LBICommentString = whiteSpaceOfCurrentLine + "// " + lbi->toString();
            logout(LBICommentString);
            overWrittenFile.push_back(LBICommentString + "\n");
        }

        logout(cLine);

        if (cLineNumber - 1 != lineCount) {
            overWrittenFile.push_back(cLine + "\n");
        } else {
            overWrittenFile.push_back(cLine);
        }

        cLineNumber++;
    }

    if (overwrite) {
        std::ofstream cFileToOverwrite(cFileName, std::ios::trunc);

        if (!cFileToOverwrite.is_open()) {
            logout("Could not open file");
            std::exit(1);
        }

        for (auto& fileLine : overWrittenFile) {
            cFileToOverwrite << fileLine;
        }

    }
}