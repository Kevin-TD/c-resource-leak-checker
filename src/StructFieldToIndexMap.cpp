#include "StructFieldToIndexMap.h"
#include "Debug.h"

StructFieldToIndexMap::StructFieldToIndexMap() {

}

void StructFieldToIndexMap::buildMap(const TempFileManager& astFile) {
    TempFileManager mapTempFile = TempFileManager("mapTempFile");

    std::string readASTCommand =
        "python3 ../TestHelpers/struct_field_to_index_map_generator.py " +
        astFile.getFileName() + " " + mapTempFile.getFileName();

    system(readASTCommand.c_str());

    logout("to py run " << readASTCommand);

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
                std::exit(1);
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
    return this->fieldToIndexMap.count(structNameAndField); 
}