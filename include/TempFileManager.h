#ifndef TMP_FILE_MANAGER_H
#define TMP_FILE_MANAGER_H

#include "Utils.h"

// handles creating a temp file and unlinking it upon destruction
class TempFileManager {
  private:
    std::string tempFileName;
    int tempFileFD;

  public:
    // creates a temp file and directory based off fileName.
    // the internal temp file name will differ from fileName.
    TempFileManager(const std::string& fileName);

    ~TempFileManager();

    // returns the generated file name
    std::string getFileName() const;

    // generates a new file stream for the temp file
    std::ifstream getFileStream() const;
};

namespace tempfile_util {

// generates the AST of the C program referenced by optLoadFileName and dumps
// its content into tmp
void dumpASTIntoTempFile(const std::string& optLoadFileName, TempFileManager& tmp);

// generates the AST and gets the AST Info of it then dumps it into tmp
void dumpASTInfoIntoTempFile(const std::string& optLoadFileName, TempFileManager& tmp);

}

#endif