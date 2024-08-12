#include "UtilFunctionTesters/RLCUtil/HasOnlyOneBalancedParenthesesTester.h"
#include "Utils.h"

HasOnlyOneBalancedParenthesesTester::HasOnlyOneBalancedParenthesesTester() {}

bool HasOnlyOneBalancedParenthesesTester::runTest() {
    return rlc_util::hasOnlyOneBalancedParentheses("()") == true &&
           rlc_util::hasOnlyOneBalancedParentheses("()()") == false &&
           rlc_util::hasOnlyOneBalancedParentheses("(())") == false &&
           rlc_util::hasOnlyOneBalancedParentheses("(test)") == true &&
           rlc_util::hasOnlyOneBalancedParentheses(")()") == false &&
           rlc_util::hasOnlyOneBalancedParentheses("(a)b") == true &&
           rlc_util::hasOnlyOneBalancedParentheses("()b") == true &&
           rlc_util::hasOnlyOneBalancedParentheses("") == false;
}