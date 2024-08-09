#include "UtilFunctionTesters/RLCDataflow/GetSuccessorsTester.h"
#include "Utils.h"

GetSuccessorsTester::GetSuccessorsTester() {}

bool GetSuccessorsTester::runTest() {
    llvm::LLVMContext context;

    // based on a version of test/if_lub_test/main.c
    std::string irCode = R"(
        ; ModuleID = '../test/if_lub_test/main.c'
        source_filename = "../test/if_lub_test/main.c"
        target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
        target triple = "x86_64-pc-linux-gnu"

        ; Function Attrs: noinline nounwind uwtable
        define dso_local i32 @main() #0 !dbg !10 {
        entry:
        %retval = alloca i32, align 4
        %str = alloca i8*, align 8
        %a = alloca i32, align 4
        store i32 0, i32* %retval, align 4
        call void @llvm.dbg.declare(metadata i8** %str, metadata !14, metadata !DIExpression()), !dbg !15
        %call = call noalias i8* @malloc(i64 15) #4, !dbg !16
        store i8* %call, i8** %str, align 8, !dbg !15
        call void @llvm.dbg.declare(metadata i32* %a, metadata !17, metadata !DIExpression()), !dbg !18
        %call1 = call i32 @getchar(), !dbg !19
        store i32 %call1, i32* %a, align 4, !dbg !18
        %0 = load i32, i32* %a, align 4, !dbg !20
        %cmp = icmp eq i32 %0, -15, !dbg !22
        br i1 %cmp, label %if.then, label %if.end, !dbg !23

        if.then:                                          ; preds = %entry
        %1 = load i8*, i8** %str, align 8, !dbg !24
        call void @free(i8* %1) #4, !dbg !26
        br label %if.end, !dbg !27

        if.end:                                           ; preds = %if.then, %entry
        %2 = load i32, i32* %retval, align 4, !dbg !28
        ret i32 %2, !dbg !28
        }

        ; Function Attrs: nounwind readnone speculatable
        declare void @llvm.dbg.declare(metadata, metadata, metadata) #1

        ; Function Attrs: nounwind
        declare dso_local noalias i8* @malloc(i64) #2

        declare dso_local i32 @getchar() #3

        ; Function Attrs: nounwind
        declare dso_local void @free(i8*) #2

        attributes #0 = { noinline nounwind uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
        attributes #1 = { nounwind readnone speculatable }
        attributes #2 = { nounwind "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
        attributes #3 = { "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
        attributes #4 = { nounwind }

        !llvm.dbg.cu = !{!0}
        !llvm.module.flags = !{!6, !7, !8}
        !llvm.ident = !{!9}

        !0 = distinct !DICompileUnit(language: DW_LANG_C99, file: !1, producer: "clang version 8.0.1- (branches/release_80)\", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, enums: !2, retainedTypes: !3, nameTableKind: None)
        !1 = !DIFile(filename: "../test/if_lub_test/main.c", directory: "/c-resource-leak-checker/build")
        !2 = !{}
        !3 = !{!4}
        !4 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !5, size: 64)
        !5 = !DIBasicType(name: "char", size: 8, encoding: DW_ATE_signed_char)
        !6 = !{i32 2, !"Dwarf Version", i32 4}
        !7 = !{i32 2, !"Debug Info Version", i32 3}
        !8 = !{i32 1, !"wchar_size", i32 4}
        !9 = !{!"clang version 8.0.1- (branches/release_80)\"}
        !10 = distinct !DISubprogram(name: "main", scope: !1, file: !1, line: 6, type: !11, scopeLine: 6, spFlags: DISPFlagDefinition, unit: !0, retainedNodes: !2)
        !11 = !DISubroutineType(types: !12)
        !12 = !{!13}
        !13 = !DIBasicType(name: "int", size: 32, encoding: DW_ATE_signed)
        !14 = !DILocalVariable(name: "str", scope: !10, file: !1, line: 8, type: !4)
        !15 = !DILocation(line: 8, column: 11, scope: !10)
        !16 = !DILocation(line: 8, column: 26, scope: !10)
        !17 = !DILocalVariable(name: "a", scope: !10, file: !1, line: 9, type: !13)
        !18 = !DILocation(line: 9, column: 9, scope: !10)
        !19 = !DILocation(line: 9, column: 13, scope: !10)
        !20 = !DILocation(line: 11, column: 9, scope: !21)
        !21 = distinct !DILexicalBlock(scope: !10, file: !1, line: 11, column: 9)
        !22 = !DILocation(line: 11, column: 11, scope: !21)
        !23 = !DILocation(line: 11, column: 9, scope: !10)
        !24 = !DILocation(line: 13, column: 14, scope: !25)
        !25 = distinct !DILexicalBlock(scope: !21, file: !1, line: 11, column: 19)
        !26 = !DILocation(line: 13, column: 9, scope: !25)
        !27 = !DILocation(line: 14, column: 5, scope: !25)
        !28 = !DILocation(line: 16, column: 1, scope: !10)
    )";
    std::map<unsigned, unsigned> instructionNumToExpectedNumberOfSuccsMap;
    instructionNumToExpectedNumberOfSuccsMap[0] = 1;
    instructionNumToExpectedNumberOfSuccsMap[1] = 1;
    instructionNumToExpectedNumberOfSuccsMap[2] = 1;
    instructionNumToExpectedNumberOfSuccsMap[3] = 1;
    instructionNumToExpectedNumberOfSuccsMap[4] = 1;
    instructionNumToExpectedNumberOfSuccsMap[5] = 1;
    instructionNumToExpectedNumberOfSuccsMap[6] = 1;
    instructionNumToExpectedNumberOfSuccsMap[7] = 1;
    instructionNumToExpectedNumberOfSuccsMap[8] = 1;
    instructionNumToExpectedNumberOfSuccsMap[9] = 1;
    instructionNumToExpectedNumberOfSuccsMap[10] = 1;
    instructionNumToExpectedNumberOfSuccsMap[11] = 1;
    instructionNumToExpectedNumberOfSuccsMap[12] = 2;
    instructionNumToExpectedNumberOfSuccsMap[13] = 1;
    instructionNumToExpectedNumberOfSuccsMap[14] = 1;
    instructionNumToExpectedNumberOfSuccsMap[15] = 1;
    instructionNumToExpectedNumberOfSuccsMap[16] = 1;
    instructionNumToExpectedNumberOfSuccsMap[17] = 0;

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
                bool expectedEqResult = instructionNumToExpectedNumberOfSuccsMap[instructionNumber] ==  rlc_dataflow::getSuccessors(&instruction).size();
                if (!expectedEqResult) {
                    return false;
                }

                instructionNumber++;
            }
        }
    }

    return true;
}