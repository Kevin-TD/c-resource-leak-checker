#ifndef TEST_RUNNER_H
#define TEST_RUNNER_H

#include "PassType.h"

class TestRunner {
public:
    static bool runTests(MappedMethods expectedResult, MappedMethods receivedResult); 
};

#endif 