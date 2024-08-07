#include "FunctionInfosManager.h";
#include "Debug.h"
#include "Constants.h"

void FunctionInfosManager::buildFunctionInfo(const TempFileManager& astInfoFile) {
    TempFileManager functionInfoTempFile = TempFileManager("functionInfoTempFile");

    std::string readASTCommand =
        "python3 " + AST_FUNCTION_INFO_PASS_LOCATION + " " +
        astInfoFile.getFileName() + " " + functionInfoTempFile.getFileName();

    int command_exit_status = system(readASTCommand.c_str());

    logout("ran command " << readASTCommand);

    if (command_exit_status != 0) {
        errs() << "error occurred while building map; early exit\n";
        std::exit(1);
    }

    std::ifstream fiFile = functionInfoTempFile.getFileStream();

    std::string line;
    if (fiFile.is_open()) {
        while (std::getline(fiFile, line)) {
            if (line == "") {
                continue;
            }

            // example line
            // FUNCTION=_IO_vfscanf|RETURNS=int|PARAMS|0|_IO_FILErestrict|1|constcharrestrict|2|struct __va_list_tag|3|intrestrict

            auto fiChunks = rlc_util::splitString(line, '|');

            if (fiChunks.size() < 2) {
                errs() << "error 0 occurred while reading function info: malformed '" << line << "'. early exit\n";
                std::exit(1);
            }

            std::string functionName, returnType;

            // FUNCTION=_IO_vfscanf
            auto functionChunks = rlc_util::splitString(fiChunks[0], '=');
            if (functionChunks.size() != 2) {
                errs() << "error 1 occurred while reading function info: bad function declaration '" << line << "'. early exit\n";
                std::exit(1);
            }
            if (functionChunks[0] != "FUNCTION") {
                errs() << "error 2 occurred while reading function info: function declaration missing \"FUNCTION\" '" << line << "'. early exit\n";
                std::exit(1);
            }
            functionName = functionChunks[1];


            // RETURNS=int
            auto returnTypeChunks = rlc_util::splitString(fiChunks[1], '=');
            if (returnTypeChunks.size() != 2) {
                errs() << "error 3 occurred while reading function info: bad return declaration '" << line << "'. early exit\n";
                std::exit(1);
            }
            if (returnTypeChunks[0] != "RETURNS") {
                errs() << "error 4 occurred while reading function info: function declaration missing \"RETURNS\" '" << line << "'. early exit\n";
                std::exit(1);
            }
            returnType = returnTypeChunks[1];


            // (optional) PARAMS
            FunctionInfo fiCreated = FunctionInfo(functionName, returnType);

            // case where there are no parameters
            if (fiChunks.size() == 2) {
                this->functionInfos.push_back(fiCreated);
                continue;
            }

            if (fiChunks[2] != "PARAMS") {
                errs() << "error 5 occurred while reading function info: PARAMS missing at '" << line << "'. early exit\n";
                std::exit(1);
            }

            // 0|_IO_FILErestrict|1|constcharrestrict|2|struct __va_list_tag|3|intrestrict
            unsigned paramIterator = 3;
            while (paramIterator < fiChunks.size()) {
                unsigned paramIndex = std::stoi(fiChunks[paramIterator]);
                if (paramIndex + 1 >= fiChunks.size()) {
                    errs() << "error 6 occurred while reading function info: param index missing type declaration at '" << line << "'. early exit\n";
                    std::exit(1);
                }

                std::string paramType = fiChunks[paramIterator + 1];

                fiCreated.addParameter(paramIndex, paramType);

                paramIterator += 2;
            }

            this->functionInfos.push_back(fiCreated);
        }
    }
}

FunctionInfo* FunctionInfosManager::getFunction(const std::string& functionName) {
    for (auto& fi : this->functionInfos) {
        if (fi.getFunctionName() == functionName) {
            return &fi;
        }
    }

    return NULL;
}