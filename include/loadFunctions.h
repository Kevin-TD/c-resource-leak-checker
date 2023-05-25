#ifndef LOAD_FUNCTIONS_H
#define LOAD_FUNCTIONS_H

#include "RunAnalysis.h"
#include "Utils.h"
#include "PointerAnalysis.h"

#include <list> 
#include <string>
#include <fstream>
#include <vector> 



namespace dataflow {
    // for some expression e we are tracking the methods call on, a safe function means that the function will not modify e. unsafe functions might modify e.


    void loadFunctions();

    struct CalledMethodsEstimate {
        std::string allocationFunction; 
        std::string deallocationFunction; 
        std::map<std::string, bool> estimates; // left = branch name, right = if must call is satisfied  
        bool finalMustCallIsSatisfied; 
    };

    

}

#endif // LOAD_FUNCTIONS_H