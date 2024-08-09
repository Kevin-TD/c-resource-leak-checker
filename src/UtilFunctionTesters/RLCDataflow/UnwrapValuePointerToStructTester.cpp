#include "UtilFunctionTesters/RLCDataflow/UnwrapValuePointerToStructTester.h"

UnwrapValuePointerToStructTester::UnwrapValuePointerToStructTester() {}

bool UnwrapValuePointerToStructTester::runTest() {
    llvm::LLVMContext context;
    
    // based on a version of the IR for test/long_struct/index.c
    std::string irCode = R"(
        ; ModuleID = '../test/long_struct/index.c'
        source_filename = "../test/long_struct/index.c"
        target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
        target triple = "x86_64-pc-linux-gnu"

        %struct.my_struct = type { i8*, i8*, i8*, i8*, i8*, i8*, i8*, i8*, i8*, i8*, i8*, i8*, i8*, i8*, i8*, i8*, i8*, i8* }

        @.str = private unnamed_addr constant [41 x i8] c"Calls target = _.FIELD(a) methods = free\00", section "llvm.metadata"
        @.str.1 = private unnamed_addr constant [28 x i8] c"../test/long_struct/index.c\00", section "llvm.metadata"
        @.str.2 = private unnamed_addr constant [35 x i8] c"MustCall target = _ methods = free\00", section "llvm.metadata"
        @.str.3 = private unnamed_addr constant [41 x i8] c"Calls target = _.FIELD(b) methods = free\00", section "llvm.metadata"
        @.str.4 = private unnamed_addr constant [41 x i8] c"Calls target = _.FIELD(c) methods = free\00", section "llvm.metadata"
        @.str.5 = private unnamed_addr constant [41 x i8] c"Calls target = _.FIELD(d) methods = free\00", section "llvm.metadata"
        @.str.6 = private unnamed_addr constant [32 x i8] c"Calls target = _ methods = free\00", section "llvm.metadata"
        @llvm.global.annotations = appending global [4 x { i8*, i8*, i8*, i32 }] [{ i8*, i8*, i8*, i32 } { i8* bitcast (void (%struct.my_struct*, %struct.my_struct*)* @does_something_a to i8*), i8* getelementptr inbounds ([41 x i8], [41 x i8]* @.str, i32 0, i32 0), i8* getelementptr inbounds ([28 x i8], [28 x i8]* @.str.1, i32 0, i32 0), i32 28 }, { i8*, i8*, i8*, i32 } { i8* bitcast (void (%struct.my_struct*, %struct.my_struct*)* @does_something_b to i8*), i8* getelementptr inbounds ([41 x i8], [41 x i8]* @.str.3, i32 0, i32 0), i8* getelementptr inbounds ([28 x i8], [28 x i8]* @.str.1, i32 0, i32 0), i32 33 }, { i8*, i8*, i8*, i32 } { i8* bitcast (void (%struct.my_struct*, %struct.my_struct*)* @does_something_c to i8*), i8* getelementptr inbounds ([41 x i8], [41 x i8]* @.str.4, i32 0, i32 0), i8* getelementptr inbounds ([28 x i8], [28 x i8]* @.str.1, i32 0, i32 0), i32 38 }, { i8*, i8*, i8*, i32 } { i8* bitcast (void (%struct.my_struct*, i32, %struct.my_struct*, %struct.my_struct*, i32, i8*)* @does_something_d to i8*), i8* getelementptr inbounds ([41 x i8], [41 x i8]* @.str.5, i32 0, i32 0), i8* getelementptr inbounds ([28 x i8], [28 x i8]* @.str.1, i32 0, i32 0), i32 43 }], section "llvm.metadata"

        ; Function Attrs: noinline nounwind uwtable
        define dso_local void @does_something_a(%struct.my_struct* noalias sret %agg.result, %struct.my_struct* byval align 8 %S) #0 !dbg !10 {
        entry:
        call void @llvm.dbg.declare(metadata %struct.my_struct* %S, metadata !34, metadata !DIExpression()), !dbg !35
        %S1 = bitcast %struct.my_struct* %S to i8*
        call void @llvm.var.annotation(i8* %S1, i8* getelementptr inbounds ([41 x i8], [41 x i8]* @.str, i32 0, i32 0), i8* getelementptr inbounds ([28 x i8], [28 x i8]* @.str.1, i32 0, i32 0), i32 28)
        %a = getelementptr inbounds %struct.my_struct, %struct.my_struct* %S, i32 0, i32 0, !dbg !36
        %0 = bitcast i8** %a to i8*, !dbg !36
        %1 = call i8* @llvm.ptr.annotation.p0i8(i8* %0, i8* getelementptr inbounds ([35 x i8], [35 x i8]* @.str.2, i32 0, i32 0), i8* getelementptr inbounds ([28 x i8], [28 x i8]* @.str.1, i32 0, i32 0), i32 7), !dbg !36
        %2 = bitcast i8* %1 to i8**, !dbg !36
        %3 = load i8*, i8** %2, align 8, !dbg !36
        call void @free(i8* %3) #2, !dbg !37
        %4 = bitcast %struct.my_struct* %agg.result to i8*, !dbg !38
        %5 = bitcast %struct.my_struct* %S to i8*, !dbg !38
        call void @llvm.memcpy.p0i8.p0i8.i64(i8* align 8 %4, i8* align 8 %5, i64 144, i1 false), !dbg !38
        ret void, !dbg !39
        }

        ; Function Attrs: nounwind readnone speculatable
        declare void @llvm.dbg.declare(metadata, metadata, metadata) #1

        ; Function Attrs: nounwind
        declare void @llvm.var.annotation(i8*, i8*, i8*, i32) #2

        ; Function Attrs: nounwind
        declare dso_local void @free(i8*) #3

        ; Function Attrs: nounwind
        declare i8* @llvm.ptr.annotation.p0i8(i8*, i8*, i8*, i32) #2

        ; Function Attrs: argmemonly nounwind
        declare void @llvm.memcpy.p0i8.p0i8.i64(i8* nocapture writeonly, i8* nocapture readonly, i64, i1) #4

        ; Function Attrs: noinline nounwind uwtable
        define dso_local void @does_something_b(%struct.my_struct* noalias sret %agg.result, %struct.my_struct* byval align 8 %S) #0 !dbg !40 {
        entry:
        call void @llvm.dbg.declare(metadata %struct.my_struct* %S, metadata !41, metadata !DIExpression()), !dbg !42
        %S1 = bitcast %struct.my_struct* %S to i8*
        call void @llvm.var.annotation(i8* %S1, i8* getelementptr inbounds ([41 x i8], [41 x i8]* @.str.3, i32 0, i32 0), i8* getelementptr inbounds ([28 x i8], [28 x i8]* @.str.1, i32 0, i32 0), i32 33)
        %b = getelementptr inbounds %struct.my_struct, %struct.my_struct* %S, i32 0, i32 1, !dbg !43
        %0 = bitcast i8** %b to i8*, !dbg !43
        %1 = call i8* @llvm.ptr.annotation.p0i8(i8* %0, i8* getelementptr inbounds ([35 x i8], [35 x i8]* @.str.2, i32 0, i32 0), i8* getelementptr inbounds ([28 x i8], [28 x i8]* @.str.1, i32 0, i32 0), i32 8), !dbg !43
        %2 = bitcast i8* %1 to i8**, !dbg !43
        %3 = load i8*, i8** %2, align 8, !dbg !43
        call void @free(i8* %3) #2, !dbg !44
        %4 = bitcast %struct.my_struct* %agg.result to i8*, !dbg !45
        %5 = bitcast %struct.my_struct* %S to i8*, !dbg !45
        call void @llvm.memcpy.p0i8.p0i8.i64(i8* align 8 %4, i8* align 8 %5, i64 144, i1 false), !dbg !45
        ret void, !dbg !46
        }

        ; Function Attrs: noinline nounwind uwtable
        define dso_local void @does_something_c(%struct.my_struct* noalias sret %agg.result, %struct.my_struct* byval align 8 %S) #0 !dbg !47 {
        entry:
        call void @llvm.dbg.declare(metadata %struct.my_struct* %S, metadata !48, metadata !DIExpression()), !dbg !49
        %S1 = bitcast %struct.my_struct* %S to i8*
        call void @llvm.var.annotation(i8* %S1, i8* getelementptr inbounds ([41 x i8], [41 x i8]* @.str.4, i32 0, i32 0), i8* getelementptr inbounds ([28 x i8], [28 x i8]* @.str.1, i32 0, i32 0), i32 38)
        %c = getelementptr inbounds %struct.my_struct, %struct.my_struct* %S, i32 0, i32 2, !dbg !50
        %0 = bitcast i8** %c to i8*, !dbg !50
        %1 = call i8* @llvm.ptr.annotation.p0i8(i8* %0, i8* getelementptr inbounds ([35 x i8], [35 x i8]* @.str.2, i32 0, i32 0), i8* getelementptr inbounds ([28 x i8], [28 x i8]* @.str.1, i32 0, i32 0), i32 9), !dbg !50
        %2 = bitcast i8* %1 to i8**, !dbg !50
        %3 = load i8*, i8** %2, align 8, !dbg !50
        call void @free(i8* %3) #2, !dbg !51
        %4 = bitcast %struct.my_struct* %agg.result to i8*, !dbg !52
        %5 = bitcast %struct.my_struct* %S to i8*, !dbg !52
        call void @llvm.memcpy.p0i8.p0i8.i64(i8* align 8 %4, i8* align 8 %5, i64 144, i1 false), !dbg !52
        ret void, !dbg !53
        }

        ; Function Attrs: noinline nounwind uwtable
        define dso_local void @does_something_d(%struct.my_struct* noalias sret %agg.result, i32 %a, %struct.my_struct* byval align 8 %S, %struct.my_struct* byval align 8 %S12, i32 %c, i8* %m) #0 !dbg !54 {
        entry:
        %a.addr = alloca i32, align 4
        %c.addr = alloca i32, align 4
        %m.addr = alloca i8*, align 8
        store i32 %a, i32* %a.addr, align 4
        call void @llvm.dbg.declare(metadata i32* %a.addr, metadata !58, metadata !DIExpression()), !dbg !59
        call void @llvm.dbg.declare(metadata %struct.my_struct* %S, metadata !60, metadata !DIExpression()), !dbg !61
        %S1 = bitcast %struct.my_struct* %S to i8*
        call void @llvm.var.annotation(i8* %S1, i8* getelementptr inbounds ([41 x i8], [41 x i8]* @.str.5, i32 0, i32 0), i8* getelementptr inbounds ([28 x i8], [28 x i8]* @.str.1, i32 0, i32 0), i32 43)
        call void @llvm.dbg.declare(metadata %struct.my_struct* %S12, metadata !62, metadata !DIExpression()), !dbg !63
        %S123 = bitcast %struct.my_struct* %S12 to i8*
        call void @llvm.var.annotation(i8* %S123, i8* getelementptr inbounds ([41 x i8], [41 x i8]* @.str.5, i32 0, i32 0), i8* getelementptr inbounds ([28 x i8], [28 x i8]* @.str.1, i32 0, i32 0), i32 43)
        store i32 %c, i32* %c.addr, align 4
        call void @llvm.dbg.declare(metadata i32* %c.addr, metadata !64, metadata !DIExpression()), !dbg !65
        store i8* %m, i8** %m.addr, align 8
        call void @llvm.dbg.declare(metadata i8** %m.addr, metadata !66, metadata !DIExpression()), !dbg !67
        %m.addr4 = bitcast i8** %m.addr to i8*
        call void @llvm.var.annotation(i8* %m.addr4, i8* getelementptr inbounds ([32 x i8], [32 x i8]* @.str.6, i32 0, i32 0), i8* getelementptr inbounds ([28 x i8], [28 x i8]* @.str.1, i32 0, i32 0), i32 43)
        %d = getelementptr inbounds %struct.my_struct, %struct.my_struct* %S, i32 0, i32 3, !dbg !68
        %0 = bitcast i8** %d to i8*, !dbg !68
        %1 = call i8* @llvm.ptr.annotation.p0i8(i8* %0, i8* getelementptr inbounds ([35 x i8], [35 x i8]* @.str.2, i32 0, i32 0), i8* getelementptr inbounds ([28 x i8], [28 x i8]* @.str.1, i32 0, i32 0), i32 10), !dbg !68
        %2 = bitcast i8* %1 to i8**, !dbg !68
        %3 = load i8*, i8** %2, align 8, !dbg !68
        call void @free(i8* %3) #2, !dbg !69
        %4 = bitcast %struct.my_struct* %agg.result to i8*, !dbg !70
        %5 = bitcast %struct.my_struct* %S to i8*, !dbg !70
        call void @llvm.memcpy.p0i8.p0i8.i64(i8* align 8 %4, i8* align 8 %5, i64 144, i1 false), !dbg !70
        ret void, !dbg !71
        }

        ; Function Attrs: noinline nounwind uwtable
        define dso_local i32 @main() #0 !dbg !72 {
        entry:
        %A = alloca %struct.my_struct, align 8
        %A1 = alloca %struct.my_struct, align 8
        %B = alloca %struct.my_struct, align 8
        %B1 = alloca %struct.my_struct, align 8
        %C = alloca %struct.my_struct, align 8
        %C1 = alloca %struct.my_struct, align 8
        %D = alloca %struct.my_struct, align 8
        %CC = alloca %struct.my_struct, align 8
        %m = alloca i8*, align 8
        %D1 = alloca %struct.my_struct, align 8
        call void @llvm.dbg.declare(metadata %struct.my_struct* %A, metadata !75, metadata !DIExpression()), !dbg !76
        %call = call noalias i8* @malloc(i64 15) #2, !dbg !77
        %a = getelementptr inbounds %struct.my_struct, %struct.my_struct* %A, i32 0, i32 0, !dbg !78
        %0 = bitcast i8** %a to i8*, !dbg !78
        %1 = call i8* @llvm.ptr.annotation.p0i8(i8* %0, i8* getelementptr inbounds ([35 x i8], [35 x i8]* @.str.2, i32 0, i32 0), i8* getelementptr inbounds ([28 x i8], [28 x i8]* @.str.1, i32 0, i32 0), i32 7), !dbg !78
        %2 = bitcast i8* %1 to i8**, !dbg !78
        store i8* %call, i8** %2, align 8, !dbg !79
        call void @llvm.dbg.declare(metadata %struct.my_struct* %A1, metadata !80, metadata !DIExpression()), !dbg !81
        call void @does_something_a(%struct.my_struct* sret %A1, %struct.my_struct* byval align 8 %A), !dbg !82
        call void @llvm.dbg.declare(metadata %struct.my_struct* %B, metadata !83, metadata !DIExpression()), !dbg !84
        %call1 = call noalias i8* @malloc(i64 15) #2, !dbg !85
        %b = getelementptr inbounds %struct.my_struct, %struct.my_struct* %B, i32 0, i32 1, !dbg !86
        %3 = bitcast i8** %b to i8*, !dbg !86
        %4 = call i8* @llvm.ptr.annotation.p0i8(i8* %3, i8* getelementptr inbounds ([35 x i8], [35 x i8]* @.str.2, i32 0, i32 0), i8* getelementptr inbounds ([28 x i8], [28 x i8]* @.str.1, i32 0, i32 0), i32 8), !dbg !86
        %5 = bitcast i8* %4 to i8**, !dbg !86
        store i8* %call1, i8** %5, align 8, !dbg !87
        call void @llvm.dbg.declare(metadata %struct.my_struct* %B1, metadata !88, metadata !DIExpression()), !dbg !89
        call void @does_something_b(%struct.my_struct* sret %B1, %struct.my_struct* byval align 8 %B), !dbg !90
        call void @llvm.dbg.declare(metadata %struct.my_struct* %C, metadata !91, metadata !DIExpression()), !dbg !92
        %call2 = call noalias i8* @malloc(i64 15) #2, !dbg !93
        %c = getelementptr inbounds %struct.my_struct, %struct.my_struct* %C, i32 0, i32 2, !dbg !94
        %6 = bitcast i8** %c to i8*, !dbg !94
        %7 = call i8* @llvm.ptr.annotation.p0i8(i8* %6, i8* getelementptr inbounds ([35 x i8], [35 x i8]* @.str.2, i32 0, i32 0), i8* getelementptr inbounds ([28 x i8], [28 x i8]* @.str.1, i32 0, i32 0), i32 9), !dbg !94
        %8 = bitcast i8* %7 to i8**, !dbg !94
        store i8* %call2, i8** %8, align 8, !dbg !95
        call void @llvm.dbg.declare(metadata %struct.my_struct* %C1, metadata !96, metadata !DIExpression()), !dbg !97
        call void @does_something_c(%struct.my_struct* sret %C1, %struct.my_struct* byval align 8 %C), !dbg !98
        call void @llvm.dbg.declare(metadata %struct.my_struct* %D, metadata !99, metadata !DIExpression()), !dbg !100
        call void @llvm.dbg.declare(metadata %struct.my_struct* %CC, metadata !101, metadata !DIExpression()), !dbg !102
        %call3 = call noalias i8* @malloc(i64 15) #2, !dbg !103
        %d = getelementptr inbounds %struct.my_struct, %struct.my_struct* %D, i32 0, i32 3, !dbg !104
        %9 = bitcast i8** %d to i8*, !dbg !104
        %10 = call i8* @llvm.ptr.annotation.p0i8(i8* %9, i8* getelementptr inbounds ([35 x i8], [35 x i8]* @.str.2, i32 0, i32 0), i8* getelementptr inbounds ([28 x i8], [28 x i8]* @.str.1, i32 0, i32 0), i32 10), !dbg !104
        %11 = bitcast i8* %10 to i8**, !dbg !104
        store i8* %call3, i8** %11, align 8, !dbg !105
        call void @llvm.dbg.declare(metadata i8** %m, metadata !106, metadata !DIExpression()), !dbg !107
        %call4 = call noalias i8* @malloc(i64 15) #2, !dbg !108
        store i8* %call4, i8** %m, align 8, !dbg !107
        call void @llvm.dbg.declare(metadata %struct.my_struct* %D1, metadata !109, metadata !DIExpression()), !dbg !110
        %12 = load i8*, i8** %m, align 8, !dbg !111
        call void @does_something_d(%struct.my_struct* sret %D1, i32 0, %struct.my_struct* byval align 8 %CC, %struct.my_struct* byval align 8 %D, i32 2, i8* %12), !dbg !112
        ret i32 0, !dbg !113
        }

        ; Function Attrs: nounwind
        declare dso_local noalias i8* @malloc(i64) #3

        attributes #0 = { noinline nounwind uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
        attributes #1 = { nounwind readnone speculatable }
        attributes #2 = { nounwind }
        attributes #3 = { nounwind "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
        attributes #4 = { argmemonly nounwind }

        !llvm.dbg.cu = !{!0}
        !llvm.module.flags = !{!6, !7, !8}
        !llvm.ident = !{!9}

        !0 = distinct !DICompileUnit(language: DW_LANG_C99, file: !1, producer: "clang version 8.0.1- (branches/release_80)\", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, enums: !2, retainedTypes: !3, nameTableKind: None)
        !1 = !DIFile(filename: "../test/long_struct/index.c", directory: "/c-resource-leak-checker/build")
        !2 = !{}
        !3 = !{!4}
        !4 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !5, size: 64)
        !5 = !DIBasicType(name: "char", size: 8, encoding: DW_ATE_signed_char)
        !6 = !{i32 2, !"Dwarf Version", i32 4}
        !7 = !{i32 2, !"Debug Info Version", i32 3}
        !8 = !{i32 1, !"wchar_size", i32 4}
        !9 = !{!"clang version 8.0.1- (branches/release_80)\"}
        !10 = distinct !DISubprogram(name: "does_something_a", scope: !1, file: !1, line: 28, type: !11, scopeLine: 28, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition, unit: !0, retainedNodes: !2)
        !11 = !DISubroutineType(types: !12)
        !12 = !{!13, !13}
        !13 = !DIDerivedType(tag: DW_TAG_typedef, name: "my_struct", file: !1, line: 25, baseType: !14)
        !14 = distinct !DICompositeType(tag: DW_TAG_structure_type, name: "my_struct", file: !1, line: 6, size: 1152, elements: !15)
        !15 = !{!16, !17, !18, !19, !20, !21, !22, !23, !24, !25, !26, !27, !28, !29, !30, !31, !32, !33}
        !16 = !DIDerivedType(tag: DW_TAG_member, name: "a", scope: !14, file: !1, line: 7, baseType: !4, size: 64)
        !17 = !DIDerivedType(tag: DW_TAG_member, name: "b", scope: !14, file: !1, line: 8, baseType: !4, size: 64, offset: 64)
        !18 = !DIDerivedType(tag: DW_TAG_member, name: "c", scope: !14, file: !1, line: 9, baseType: !4, size: 64, offset: 128)
        !19 = !DIDerivedType(tag: DW_TAG_member, name: "d", scope: !14, file: !1, line: 10, baseType: !4, size: 64, offset: 192)
        !20 = !DIDerivedType(tag: DW_TAG_member, name: "filler1", scope: !14, file: !1, line: 11, baseType: !4, size: 64, offset: 256)
        !21 = !DIDerivedType(tag: DW_TAG_member, name: "filler2", scope: !14, file: !1, line: 12, baseType: !4, size: 64, offset: 320)
        !22 = !DIDerivedType(tag: DW_TAG_member, name: "filler3", scope: !14, file: !1, line: 13, baseType: !4, size: 64, offset: 384)
        !23 = !DIDerivedType(tag: DW_TAG_member, name: "filler4", scope: !14, file: !1, line: 14, baseType: !4, size: 64, offset: 448)
        !24 = !DIDerivedType(tag: DW_TAG_member, name: "filler5", scope: !14, file: !1, line: 15, baseType: !4, size: 64, offset: 512)
        !25 = !DIDerivedType(tag: DW_TAG_member, name: "filler6", scope: !14, file: !1, line: 16, baseType: !4, size: 64, offset: 576)
        !26 = !DIDerivedType(tag: DW_TAG_member, name: "filler7", scope: !14, file: !1, line: 17, baseType: !4, size: 64, offset: 640)
        !27 = !DIDerivedType(tag: DW_TAG_member, name: "filler8", scope: !14, file: !1, line: 18, baseType: !4, size: 64, offset: 704)
        !28 = !DIDerivedType(tag: DW_TAG_member, name: "filler9", scope: !14, file: !1, line: 19, baseType: !4, size: 64, offset: 768)
        !29 = !DIDerivedType(tag: DW_TAG_member, name: "filler10", scope: !14, file: !1, line: 20, baseType: !4, size: 64, offset: 832)
        !30 = !DIDerivedType(tag: DW_TAG_member, name: "filler11", scope: !14, file: !1, line: 21, baseType: !4, size: 64, offset: 896)
        !31 = !DIDerivedType(tag: DW_TAG_member, name: "filler12", scope: !14, file: !1, line: 22, baseType: !4, size: 64, offset: 960)
        !32 = !DIDerivedType(tag: DW_TAG_member, name: "filler13", scope: !14, file: !1, line: 23, baseType: !4, size: 64, offset: 1024)
        !33 = !DIDerivedType(tag: DW_TAG_member, name: "filler14", scope: !14, file: !1, line: 24, baseType: !4, size: 64, offset: 1088)
        !34 = !DILocalVariable(name: "S", arg: 1, scope: !10, file: !1, line: 28, type: !13)
        !35 = !DILocation(line: 28, column: 57, scope: !10)
        !36 = !DILocation(line: 29, column: 12, scope: !10)
        !37 = !DILocation(line: 29, column: 5, scope: !10)
        !38 = !DILocation(line: 30, column: 12, scope: !10)
        !39 = !DILocation(line: 30, column: 5, scope: !10)
        !40 = distinct !DISubprogram(name: "does_something_b", scope: !1, file: !1, line: 33, type: !11, scopeLine: 33, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition, unit: !0, retainedNodes: !2)
        !41 = !DILocalVariable(name: "S", arg: 1, scope: !40, file: !1, line: 33, type: !13)
        !42 = !DILocation(line: 33, column: 57, scope: !40)
        !43 = !DILocation(line: 34, column: 12, scope: !40)
        !44 = !DILocation(line: 34, column: 5, scope: !40)
        !45 = !DILocation(line: 35, column: 12, scope: !40)
        !46 = !DILocation(line: 35, column: 5, scope: !40)
        !47 = distinct !DISubprogram(name: "does_something_c", scope: !1, file: !1, line: 38, type: !11, scopeLine: 38, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition, unit: !0, retainedNodes: !2)
        !48 = !DILocalVariable(name: "S", arg: 1, scope: !47, file: !1, line: 38, type: !13)
        !49 = !DILocation(line: 38, column: 57, scope: !47)
        !50 = !DILocation(line: 39, column: 12, scope: !47)
        !51 = !DILocation(line: 39, column: 5, scope: !47)
        !52 = !DILocation(line: 40, column: 12, scope: !47)
        !53 = !DILocation(line: 40, column: 5, scope: !47)
        !54 = distinct !DISubprogram(name: "does_something_d", scope: !1, file: !1, line: 43, type: !55, scopeLine: 43, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition, unit: !0, retainedNodes: !2)
        !55 = !DISubroutineType(types: !56)
        !56 = !{!13, !57, !13, !13, !57, !4}
        !57 = !DIBasicType(name: "int", size: 32, encoding: DW_ATE_signed)
        !58 = !DILocalVariable(name: "a", arg: 1, scope: !54, file: !1, line: 43, type: !57)
        !59 = !DILocation(line: 43, column: 51, scope: !54)
        !60 = !DILocalVariable(name: "S", arg: 2, scope: !54, file: !1, line: 43, type: !13)
        !61 = !DILocation(line: 43, column: 64, scope: !54)
        !62 = !DILocalVariable(name: "S1", arg: 3, scope: !54, file: !1, line: 43, type: !13)
        !63 = !DILocation(line: 43, column: 96, scope: !54)
        !64 = !DILocalVariable(name: "c", arg: 4, scope: !54, file: !1, line: 43, type: !57)
        !65 = !DILocation(line: 43, column: 123, scope: !54)
        !66 = !DILocalVariable(name: "m", arg: 5, scope: !54, file: !1, line: 43, type: !4)
        !67 = !DILocation(line: 43, column: 132, scope: !54)
        !68 = !DILocation(line: 44, column: 12, scope: !54)
        !69 = !DILocation(line: 44, column: 5, scope: !54)
        !70 = !DILocation(line: 45, column: 12, scope: !54)
        !71 = !DILocation(line: 45, column: 5, scope: !54)
        !72 = distinct !DISubprogram(name: "main", scope: !1, file: !1, line: 48, type: !73, scopeLine: 48, spFlags: DISPFlagDefinition, unit: !0, retainedNodes: !2)
        !73 = !DISubroutineType(types: !74)
        !74 = !{!57}
        !75 = !DILocalVariable(name: "A", scope: !72, file: !1, line: 49, type: !13)
        !76 = !DILocation(line: 49, column: 15, scope: !72)
        !77 = !DILocation(line: 50, column: 18, scope: !72)
        !78 = !DILocation(line: 50, column: 7, scope: !72)
        !79 = !DILocation(line: 50, column: 9, scope: !72)
        !80 = !DILocalVariable(name: "A1", scope: !72, file: !1, line: 51, type: !13)
        !81 = !DILocation(line: 51, column: 15, scope: !72)
        !82 = !DILocation(line: 51, column: 20, scope: !72)
        !83 = !DILocalVariable(name: "B", scope: !72, file: !1, line: 53, type: !13)
        !84 = !DILocation(line: 53, column: 15, scope: !72)
        !85 = !DILocation(line: 54, column: 18, scope: !72)
        !86 = !DILocation(line: 54, column: 7, scope: !72)
        !87 = !DILocation(line: 54, column: 9, scope: !72)
        !88 = !DILocalVariable(name: "B1", scope: !72, file: !1, line: 55, type: !13)
        !89 = !DILocation(line: 55, column: 15, scope: !72)
        !90 = !DILocation(line: 55, column: 20, scope: !72)
        !91 = !DILocalVariable(name: "C", scope: !72, file: !1, line: 57, type: !13)
        !92 = !DILocation(line: 57, column: 15, scope: !72)
        !93 = !DILocation(line: 58, column: 18, scope: !72)
        !94 = !DILocation(line: 58, column: 7, scope: !72)
        !95 = !DILocation(line: 58, column: 9, scope: !72)
        !96 = !DILocalVariable(name: "C1", scope: !72, file: !1, line: 59, type: !13)
        !97 = !DILocation(line: 59, column: 15, scope: !72)
        !98 = !DILocation(line: 59, column: 20, scope: !72)
        !99 = !DILocalVariable(name: "D", scope: !72, file: !1, line: 61, type: !13)
        !100 = !DILocation(line: 61, column: 15, scope: !72)
        !101 = !DILocalVariable(name: "CC", scope: !72, file: !1, line: 61, type: !13)
        !102 = !DILocation(line: 61, column: 18, scope: !72)
        !103 = !DILocation(line: 62, column: 18, scope: !72)
        !104 = !DILocation(line: 62, column: 7, scope: !72)
        !105 = !DILocation(line: 62, column: 9, scope: !72)
        !106 = !DILocalVariable(name: "m", scope: !72, file: !1, line: 63, type: !4)
        !107 = !DILocation(line: 63, column: 11, scope: !72)
        !108 = !DILocation(line: 63, column: 22, scope: !72)
        !109 = !DILocalVariable(name: "D1", scope: !72, file: !1, line: 64, type: !13)
        !110 = !DILocation(line: 64, column: 15, scope: !72)
        !111 = !DILocation(line: 64, column: 50, scope: !72)
        !112 = !DILocation(line: 64, column: 20, scope: !72)
        !113 = !DILocation(line: 66, column: 1, scope: !72)

    )";


    std::map<unsigned, bool> instructionNumToExpectedResultOfIfUnwrapIsNull;
    instructionNumToExpectedResultOfIfUnwrapIsNull[0] = true;
    instructionNumToExpectedResultOfIfUnwrapIsNull[1] = true;
    instructionNumToExpectedResultOfIfUnwrapIsNull[2] = true;
    instructionNumToExpectedResultOfIfUnwrapIsNull[3] = true;
    instructionNumToExpectedResultOfIfUnwrapIsNull[4] = true;
    instructionNumToExpectedResultOfIfUnwrapIsNull[5] = true;
    instructionNumToExpectedResultOfIfUnwrapIsNull[6] = true;
    instructionNumToExpectedResultOfIfUnwrapIsNull[7] = true;
    instructionNumToExpectedResultOfIfUnwrapIsNull[8] = true;
    instructionNumToExpectedResultOfIfUnwrapIsNull[9] = true;
    instructionNumToExpectedResultOfIfUnwrapIsNull[10] = true;
    instructionNumToExpectedResultOfIfUnwrapIsNull[11] = true;
    instructionNumToExpectedResultOfIfUnwrapIsNull[12] = true;
    instructionNumToExpectedResultOfIfUnwrapIsNull[13] = true;
    instructionNumToExpectedResultOfIfUnwrapIsNull[14] = true;
    instructionNumToExpectedResultOfIfUnwrapIsNull[15] = true;
    instructionNumToExpectedResultOfIfUnwrapIsNull[16] = true;
    instructionNumToExpectedResultOfIfUnwrapIsNull[17] = true;
    instructionNumToExpectedResultOfIfUnwrapIsNull[18] = true;
    instructionNumToExpectedResultOfIfUnwrapIsNull[19] = true;
    instructionNumToExpectedResultOfIfUnwrapIsNull[20] = true;
    instructionNumToExpectedResultOfIfUnwrapIsNull[21] = true;
    instructionNumToExpectedResultOfIfUnwrapIsNull[22] = true;
    instructionNumToExpectedResultOfIfUnwrapIsNull[23] = true;
    instructionNumToExpectedResultOfIfUnwrapIsNull[24] = true;
    instructionNumToExpectedResultOfIfUnwrapIsNull[25] = true;
    instructionNumToExpectedResultOfIfUnwrapIsNull[26] = true;
    instructionNumToExpectedResultOfIfUnwrapIsNull[27] = true;
    instructionNumToExpectedResultOfIfUnwrapIsNull[28] = true;
    instructionNumToExpectedResultOfIfUnwrapIsNull[29] = true;
    instructionNumToExpectedResultOfIfUnwrapIsNull[30] = true;
    instructionNumToExpectedResultOfIfUnwrapIsNull[31] = true;
    instructionNumToExpectedResultOfIfUnwrapIsNull[32] = true;
    instructionNumToExpectedResultOfIfUnwrapIsNull[33] = true;
    instructionNumToExpectedResultOfIfUnwrapIsNull[34] = true;
    instructionNumToExpectedResultOfIfUnwrapIsNull[35] = true;
    instructionNumToExpectedResultOfIfUnwrapIsNull[36] = true;
    instructionNumToExpectedResultOfIfUnwrapIsNull[37] = true;
    instructionNumToExpectedResultOfIfUnwrapIsNull[38] = true;
    instructionNumToExpectedResultOfIfUnwrapIsNull[39] = true;
    instructionNumToExpectedResultOfIfUnwrapIsNull[40] = true;
    instructionNumToExpectedResultOfIfUnwrapIsNull[41] = true;
    instructionNumToExpectedResultOfIfUnwrapIsNull[42] = true;
    instructionNumToExpectedResultOfIfUnwrapIsNull[43] = true;
    instructionNumToExpectedResultOfIfUnwrapIsNull[44] = true;
    instructionNumToExpectedResultOfIfUnwrapIsNull[45] = true;
    instructionNumToExpectedResultOfIfUnwrapIsNull[46] = true;
    instructionNumToExpectedResultOfIfUnwrapIsNull[47] = true;
    instructionNumToExpectedResultOfIfUnwrapIsNull[48] = true;
    instructionNumToExpectedResultOfIfUnwrapIsNull[49] = true;
    instructionNumToExpectedResultOfIfUnwrapIsNull[50] = true;
    instructionNumToExpectedResultOfIfUnwrapIsNull[51] = true;
    instructionNumToExpectedResultOfIfUnwrapIsNull[52] = true;
    instructionNumToExpectedResultOfIfUnwrapIsNull[53] = true;
    instructionNumToExpectedResultOfIfUnwrapIsNull[54] = true;
    instructionNumToExpectedResultOfIfUnwrapIsNull[55] = true;
    instructionNumToExpectedResultOfIfUnwrapIsNull[56] = true;
    instructionNumToExpectedResultOfIfUnwrapIsNull[57] = true;
    instructionNumToExpectedResultOfIfUnwrapIsNull[58] = true;
    instructionNumToExpectedResultOfIfUnwrapIsNull[59] = true;
    instructionNumToExpectedResultOfIfUnwrapIsNull[60] = true;
    instructionNumToExpectedResultOfIfUnwrapIsNull[61] = true;
    instructionNumToExpectedResultOfIfUnwrapIsNull[62] = true;
    instructionNumToExpectedResultOfIfUnwrapIsNull[63] = true;
    instructionNumToExpectedResultOfIfUnwrapIsNull[64] = true;
    instructionNumToExpectedResultOfIfUnwrapIsNull[65] = true;
    instructionNumToExpectedResultOfIfUnwrapIsNull[66] = false;
    instructionNumToExpectedResultOfIfUnwrapIsNull[67] = false;
    instructionNumToExpectedResultOfIfUnwrapIsNull[68] = false;
    instructionNumToExpectedResultOfIfUnwrapIsNull[69] = false;
    instructionNumToExpectedResultOfIfUnwrapIsNull[70] = false;
    instructionNumToExpectedResultOfIfUnwrapIsNull[71] = false;
    instructionNumToExpectedResultOfIfUnwrapIsNull[72] = false;
    instructionNumToExpectedResultOfIfUnwrapIsNull[73] = false;
    instructionNumToExpectedResultOfIfUnwrapIsNull[74] = true;
    instructionNumToExpectedResultOfIfUnwrapIsNull[75] = false;
    instructionNumToExpectedResultOfIfUnwrapIsNull[76] = true;
    instructionNumToExpectedResultOfIfUnwrapIsNull[77] = true;
    instructionNumToExpectedResultOfIfUnwrapIsNull[78] = true;
    instructionNumToExpectedResultOfIfUnwrapIsNull[79] = true;
    instructionNumToExpectedResultOfIfUnwrapIsNull[80] = true;
    instructionNumToExpectedResultOfIfUnwrapIsNull[81] = true;
    instructionNumToExpectedResultOfIfUnwrapIsNull[82] = true;
    instructionNumToExpectedResultOfIfUnwrapIsNull[83] = true;
    instructionNumToExpectedResultOfIfUnwrapIsNull[84] = true;
    instructionNumToExpectedResultOfIfUnwrapIsNull[85] = true;
    instructionNumToExpectedResultOfIfUnwrapIsNull[86] = true;
    instructionNumToExpectedResultOfIfUnwrapIsNull[87] = true;
    instructionNumToExpectedResultOfIfUnwrapIsNull[88] = true;
    instructionNumToExpectedResultOfIfUnwrapIsNull[89] = true;
    instructionNumToExpectedResultOfIfUnwrapIsNull[90] = true;
    instructionNumToExpectedResultOfIfUnwrapIsNull[91] = true;
    instructionNumToExpectedResultOfIfUnwrapIsNull[92] = true;
    instructionNumToExpectedResultOfIfUnwrapIsNull[93] = true;
    instructionNumToExpectedResultOfIfUnwrapIsNull[94] = true;
    instructionNumToExpectedResultOfIfUnwrapIsNull[95] = true;
    instructionNumToExpectedResultOfIfUnwrapIsNull[96] = true;
    instructionNumToExpectedResultOfIfUnwrapIsNull[97] = true;
    instructionNumToExpectedResultOfIfUnwrapIsNull[98] = true;
    instructionNumToExpectedResultOfIfUnwrapIsNull[99] = true;
    instructionNumToExpectedResultOfIfUnwrapIsNull[100] = true;
    instructionNumToExpectedResultOfIfUnwrapIsNull[101] = true;
    instructionNumToExpectedResultOfIfUnwrapIsNull[102] = true;
    instructionNumToExpectedResultOfIfUnwrapIsNull[103] = true;
    instructionNumToExpectedResultOfIfUnwrapIsNull[104] = true;
    instructionNumToExpectedResultOfIfUnwrapIsNull[105] = true;
    instructionNumToExpectedResultOfIfUnwrapIsNull[106] = true;
    instructionNumToExpectedResultOfIfUnwrapIsNull[107] = true;
    instructionNumToExpectedResultOfIfUnwrapIsNull[108] = true;
    instructionNumToExpectedResultOfIfUnwrapIsNull[109] = true;
    instructionNumToExpectedResultOfIfUnwrapIsNull[110] = true;
    instructionNumToExpectedResultOfIfUnwrapIsNull[111] = true;
    instructionNumToExpectedResultOfIfUnwrapIsNull[112] = true;
    instructionNumToExpectedResultOfIfUnwrapIsNull[113] = true;
    instructionNumToExpectedResultOfIfUnwrapIsNull[114] = true;
    instructionNumToExpectedResultOfIfUnwrapIsNull[115] = true;
    instructionNumToExpectedResultOfIfUnwrapIsNull[116] = true;
    instructionNumToExpectedResultOfIfUnwrapIsNull[117] = true;
    

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
                bool expectedEqResult = instructionNumToExpectedResultOfIfUnwrapIsNull[instructionNumber] == (rlc_dataflow::unwrapValuePointerToStruct(&instruction) == NULL);
                if (!expectedEqResult) {
                    return false;
                }
                instructionNumber++; 
            }
        }
    }


    return true; 
}