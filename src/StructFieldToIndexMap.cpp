#include "StructFieldToIndexMap.h"
#include "Debug.h"

StructFieldToIndexMap::StructFieldToIndexMap() {

}

void StructFieldToIndexMap::buildMap(const std::string& optLoadFileName) {
    char astTempTextFile[] = "/tmp/astTempTextFileXXXXXX";
    int astFD = mkstemp(astTempTextFile);

    if (astFD == -1) {
        logout("failed to create temp ast text file at buildMap");
        perror("mkstemp");
        exit(1);
    }

    std::string dumpASTCommand =
        "clang -Xclang -ast-dump -fsyntax-only -fno-color-diagnostics " +
        optLoadFileName + "> " + astTempTextFile;
    system(dumpASTCommand.c_str());

    char mapTempTextFile[] = "/tmp/mapFileXXXXXX";
    int mapFD = mkstemp(mapTempTextFile);

    if (mapFD == -1) {
        logout("failed to create temp annotations text file at buildMap");
        perror("mkstemp");
        exit(1);
    }

    std::string readASTCommand =
        "python3 ../TestHelpers/struct_field_to_index_map_generator.py " +
        std::string(astTempTextFile) + " " + std::string(mapTempTextFile);

    system(readASTCommand.c_str());

    logout("dump command " << dumpASTCommand);
    logout("to py run " << readASTCommand);

    std::ifstream mapFile(mapTempTextFile);

    std::string line;
    if (mapFile.is_open()) {
        while (std::getline(mapFile, line)) {
            if (line == "") {
                continue; 
            }

            auto keyMapPair = rlc_util::splitString(line, '='); 
            if (keyMapPair.size() != 2) {
                logout("unexpected error; key map pair not equal two for line '" << line << "'; early exit");
                std::exit(1);
            }

            auto structNameAndField = keyMapPair[0];
            auto structNameAndIndex = keyMapPair[1]; 

            this->fieldToIndexMap[structNameAndField] = structNameAndIndex; 
        }
    }

    close(astFD);
    close(mapFD);
    unlink(astTempTextFile);
    unlink(mapTempTextFile);
}

std::string StructFieldToIndexMap::get(const std::string& structNameAndField) {
    return this->fieldToIndexMap[structNameAndField];
}

bool StructFieldToIndexMap::structNameAndFieldIsInMap(const std::string& structNameAndField) {
    return this->fieldToIndexMap.count(structNameAndField); 
}