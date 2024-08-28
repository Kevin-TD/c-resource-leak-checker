#include "LineNumberToLValueMap.h"
#include "Debug.h"
#include "Constants.h"

LineNumberToLValueMap::LineNumberToLValueMap() {

}

void LineNumberToLValueMap::buildMap(const TempFileManager& astInfoFile) {
    TempFileManager mapTempFile = TempFileManager("mapTempFile");

    std::string readASTCommand = "python3 " + AST_L_VALUE_PASS_LOCATION + " " + astInfoFile.getFileName() + " " + mapTempFile.getFileName();

    int command_exit_status = system(readASTCommand.c_str());

    logout("ran command " << readASTCommand);

    if (command_exit_status != 0) {
        errs() << "error occurred while building l-value map; early exit\n";
        std::exit(1);
    }

    std::ifstream mapFile = mapTempFile.getFileStream();

    std::string line;
    if (mapFile.is_open()) {
        while (std::getline(mapFile, line)) {
            if (line == "") {
                continue;
            }

            auto keyMapPair = rlc_util::splitString(line, '=');
            if (keyMapPair.size() != 2) {
                logout("unexpected error; line '" << line << "' does not look like key-pair map in the form 'a=b' for l-value map; early exit");
                std::exit(1);
            }

            unsigned lineNumber = std::stoi(keyMapPair[0]);
            std::string lvalueName = keyMapPair[1];

            this->lineNumberToLValueMap[lineNumber] = lvalueName;
        }
    }
}

std::string LineNumberToLValueMap::get(unsigned lineNumber) {
    return this->lineNumberToLValueMap[lineNumber];
}

bool LineNumberToLValueMap::lineNumberIsInMap(unsigned lineNumber) {
    return this->lineNumberToLValueMap.count(lineNumber) && this->lineNumberToLValueMap[lineNumber] != "";
}