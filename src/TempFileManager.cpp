#include "TempFileManager.h"
#include "Debug.h"

TempFileManager::TempFileManager(const std::string& fileName) {
    tempFileName = "/tmp/" + std::string(fileName) + "XXXXXX";

    char *tempFileFDChar = new char[tempFileName.length() + 1];
    strcpy(tempFileFDChar, tempFileName.c_str());

    tempFileFD = mkstemp(tempFileFDChar);

    tempFileName = std::string(tempFileFDChar);

    delete[] tempFileFDChar;

    if (tempFileFD == -1) {
        logout("failed to create temp file '" << fileName << "'");
        perror("mkstemp");
        exit(1);
    }   
}

std::string TempFileManager::getFileName() const {
    return tempFileName;
}

std::ifstream TempFileManager::getFileStream() const {
    return std::ifstream(tempFileName); 
}

TempFileManager::~TempFileManager() {
    close(tempFileFD);
    unlink(tempFileName.c_str());
}

namespace tempfile_util {

void dumpASTIntoTempFile(const std::string& optLoadFileName, TempFileManager& tmp) {
    std::string dumpASTCommand =
        "clang -Xclang -ast-dump -fsyntax-only -fno-color-diagnostics " +
        optLoadFileName + "> " + tmp.getFileName();
    system(dumpASTCommand.c_str());
}

}