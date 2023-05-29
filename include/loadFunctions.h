#ifndef LOAD_FUNCTIONS_H
#define LOAD_FUNCTIONS_H

#include "RunAnalysis.h"
#include "Utils.h"
#include "PointerAnalysis.h"
#include "Domain.h"

#include <list> 
#include <string>
#include <fstream>
#include <vector> 


// std::map<std::string, std::map<std::string, dataflow::CalledMethodsEstimate>> CallMethodsMap; // left = branch name. inner left = var name

namespace dataflow {
    
    void loadFunctions();

    // consider adding a "is defined/is trackable/is mem alloc var" member
    struct CalledMethodsEstimate {
        std::string allocationFunction; 
        std::string deallocationFunction; 
        Domain calledMethodsState; 
    };

    

}

#endif // LOAD_FUNCTIONS_H