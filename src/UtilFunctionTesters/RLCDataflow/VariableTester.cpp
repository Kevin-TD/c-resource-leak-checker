#include "UtilFunctionTesters/RLCDataflow/VariableTester.h"

VariableTester::VariableTester() {}

bool VariableTester::runTest() {
    llvm::LLVMContext context;

    std::string irCode = R"(
        ; ModuleID = 'example_module'
        source_filename = "example.ll"

        define i32 @example_function(i32 %a, i32 %b) {
        entry:
        ; Arithmetic Operations
        %add = add i32 %a, %b
        %sub = sub i32 %a, %b
        %mul = mul i32 %a, %b
        %div = sdiv i32 %a, %b
        %rem = srem i32 %a, %b

        ; Bitwise Operations
        %and = and i32 %a, %b
        %or = or i32 %a, %b
        %xor = xor i32 %a, %b
        %shl = shl i32 %a, 1
        %shr = ashr i32 %a, 1

        ; Memory Operations
        %ptr = alloca i32, align 4
        store i32 %a, i32* %ptr, align 4
        %load = load i32, i32* %ptr, align 4

        ; Control Flow
        %cmp = icmp eq i32 %a, %b
        br i1 %cmp, label %if_true, label %if_false

        if_true:
        ; Function Call
        %call = call i32 @another_function(i32 %a)
        br label %end

        if_false:
        br label %end

        end:
        ; Phi Node
        %phi = phi i32 [%add, %if_true], [%sub, %if_false]
        
        ; Type Conversion
        %conv = zext i32 %a to i64

        ; Return
        ret i32 %phi
        }

        define i32 @another_function(i32 %x) {
        entry:
        %result = mul i32 %x, 2
        ret i32 %result
        }

    )";

    std::map<unsigned, std::string> instructionNumToExpectedVariableMap;
    instructionNumToExpectedVariableMap[0] = "%add";
    instructionNumToExpectedVariableMap[1] = "%sub";
    instructionNumToExpectedVariableMap[2] = "%mul";
    instructionNumToExpectedVariableMap[3] = "%div";
    instructionNumToExpectedVariableMap[4] = "%rem";
    instructionNumToExpectedVariableMap[5] = "%and";
    instructionNumToExpectedVariableMap[6] = "%or";
    instructionNumToExpectedVariableMap[7] = "%xor";
    instructionNumToExpectedVariableMap[8] = "%shl";
    instructionNumToExpectedVariableMap[9] = "%shr";
    instructionNumToExpectedVariableMap[10] = "%ptr";
    instructionNumToExpectedVariableMap[11] = " store i32 %a, i32* %ptr, align 4";
    instructionNumToExpectedVariableMap[12] = "%load";
    instructionNumToExpectedVariableMap[13] = "%cmp";
    instructionNumToExpectedVariableMap[14] = " br i1 %cmp, label %if_true, label %if_false";
    instructionNumToExpectedVariableMap[15] = "%call";
    instructionNumToExpectedVariableMap[16] = " br label %end";
    instructionNumToExpectedVariableMap[17] = " br label %end";
    instructionNumToExpectedVariableMap[18] = "%phi";
    instructionNumToExpectedVariableMap[19] = "%conv";
    instructionNumToExpectedVariableMap[20] = " ret i32 %phi";
    instructionNumToExpectedVariableMap[21] = "%result";
    instructionNumToExpectedVariableMap[22] = " ret i32 %result";

    auto memBuffer = llvm::MemoryBuffer::getMemBuffer(irCode);
    llvm::SMDiagnostic error;
    std::unique_ptr<llvm::Module> parsedModule = llvm::parseAssembly(*memBuffer, error, context);

    if (!parsedModule) {
        error.print("IR Parsing Error", llvm::errs());
    }

    unsigned instructionNumber = 0;
    for (auto &F : *parsedModule) {
        for (auto &BB : F) {
            for (auto &instruction : BB) {
                bool expectedEqResult = instructionNumToExpectedVariableMap[instructionNumber] == rlc_dataflow::variable(&instruction);
                if (!expectedEqResult) {
                    return false;
                }
                instructionNumber++;
            }
        }
    }

    return true;
}