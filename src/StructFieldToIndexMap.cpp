#include "StructFieldToIndexMap.h"
#include "Debug.h"
#include "Constants.h"

StructFieldToIndexMap::StructFieldToIndexMap() {

}

void StructFieldToIndexMap::buildMap(const TempFileManager& astInfoFile) {
    TempFileManager mapTempFile = TempFileManager("mapTempFile");

    std::string readASTCommand =
        "python3 " + AST_FIELD_TO_INDEX_MAP_GENERATOR_LOCATION + " " +
        astInfoFile.getFileName() + " " + mapTempFile.getFileName();

    int command_exit_status = system(readASTCommand.c_str());

    logout("ran command " << readASTCommand);

    if (command_exit_status != 0) {
        errs() << "error occured while building map; early exit\n";
        std::exit(EXIT_FAILURE);
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
                logout("unexpected error; line '" << line << "' does not look like key-pair map in the form 'a=b'; early exit");
                std::exit(EXIT_FAILURE);
            }

            auto structNameAndField = keyMapPair[0];
            auto structNameAndIndex = keyMapPair[1];

            this->fieldToIndexMap[structNameAndField] = structNameAndIndex;
        }
    }
}

std::string StructFieldToIndexMap::get(const std::string& structNameAndField) {
    return this->fieldToIndexMap[structNameAndField];
}

bool StructFieldToIndexMap::structNameAndFieldIsInMap(const std::string& structNameAndField) {
    return this->fieldToIndexMap.count(structNameAndField) && this->fieldToIndexMap[structNameAndField] != "";
}