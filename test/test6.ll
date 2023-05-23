; ModuleID = '../test/test6.c'
source_filename = "../test/test6.c"
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

@.str = private unnamed_addr constant [10 x i8] c"a mimicry\00", align 1
@.str.1 = private unnamed_addr constant [11 x i8] c"helloworld\00", align 1
@.str.2 = private unnamed_addr constant [28 x i8] c"String = %s,  Address = %u\0A\00", align 1
@.str.3 = private unnamed_addr constant [6 x i8] c"hello\00", align 1

; Function Attrs: noinline nounwind uwtable
define dso_local i8* @strcpy(i8* noalias nonnull %__dest, i8* noalias nonnull %__src) #0 !dbg !10 {
entry:
  %retval = alloca i8*, align 8
  %__dest.addr = alloca i8*, align 8
  %__src.addr = alloca i8*, align 8
  store i8* %__dest, i8** %__dest.addr, align 8
  call void @llvm.dbg.declare(metadata i8** %__dest.addr, metadata !17, metadata !DIExpression()), !dbg !18
  store i8* %__src, i8** %__src.addr, align 8
  call void @llvm.dbg.declare(metadata i8** %__src.addr, metadata !19, metadata !DIExpression()), !dbg !20
  %call = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([10 x i8], [10 x i8]* @.str, i32 0, i32 0)), !dbg !21
  %0 = load i8*, i8** %retval, align 8, !dbg !22
  ret i8* %0, !dbg !22
}

; Function Attrs: nounwind readnone speculatable
declare void @llvm.dbg.declare(metadata, metadata, metadata) #1

declare dso_local i32 @printf(i8*, ...) #2

; Function Attrs: noinline nounwind uwtable
define dso_local i32 @main() #0 !dbg !23 {
entry:
  %retval = alloca i32, align 4
  %str = alloca i8*, align 8
  %a = alloca i32, align 4
  store i32 0, i32* %retval, align 4
  call void @llvm.dbg.declare(metadata i8** %str, metadata !27, metadata !DIExpression()), !dbg !28
  call void @llvm.dbg.declare(metadata i32* %a, metadata !29, metadata !DIExpression()), !dbg !30
  %call = call noalias i8* @malloc(i64 15) #4, !dbg !31
  store i8* %call, i8** %str, align 8, !dbg !32
  %0 = load i8*, i8** %str, align 8, !dbg !33
  %call1 = call i8* @strcpy(i8* %0, i8* getelementptr inbounds ([11 x i8], [11 x i8]* @.str.1, i32 0, i32 0)) #4, !dbg !34
  %1 = load i8*, i8** %str, align 8, !dbg !35
  %2 = load i8*, i8** %str, align 8, !dbg !36
  %call2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([28 x i8], [28 x i8]* @.str.2, i32 0, i32 0), i8* %1, i8* %2), !dbg !37
  %3 = load i8*, i8** %str, align 8, !dbg !38
  %call3 = call i8* @realloc(i8* %3, i64 25) #4, !dbg !39
  store i8* %call3, i8** %str, align 8, !dbg !40
  %4 = load i8*, i8** %str, align 8, !dbg !41
  %call4 = call i8* @strcat(i8* %4, i8* getelementptr inbounds ([6 x i8], [6 x i8]* @.str.3, i32 0, i32 0)) #4, !dbg !42
  %5 = load i8*, i8** %str, align 8, !dbg !43
  %6 = load i8*, i8** %str, align 8, !dbg !44
  %call5 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([28 x i8], [28 x i8]* @.str.2, i32 0, i32 0), i8* %5, i8* %6), !dbg !45
  %7 = load i8*, i8** %str, align 8, !dbg !46
  call void @free(i8* %7) #4, !dbg !47
  %8 = load i8*, i8** %str, align 8, !dbg !48
  %call6 = call i8* @strcpy(i8* %8, i8* getelementptr inbounds ([11 x i8], [11 x i8]* @.str.1, i32 0, i32 0)) #4, !dbg !49
  %9 = load i8*, i8** %str, align 8, !dbg !50
  %call7 = call i32 (i8*, ...) bitcast (i32 (...)* @unknown_function to i32 (i8*, ...)*)(i8* %9), !dbg !51
  ret i32 0, !dbg !52
}

; Function Attrs: nounwind
declare dso_local noalias i8* @malloc(i64) #3

; Function Attrs: nounwind
declare dso_local i8* @realloc(i8*, i64) #3

; Function Attrs: nounwind
declare dso_local i8* @strcat(i8*, i8*) #3

; Function Attrs: nounwind
declare dso_local void @free(i8*) #3

declare dso_local i32 @unknown_function(...) #2

attributes #0 = { noinline nounwind uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { nounwind readnone speculatable }
attributes #2 = { "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #3 = { nounwind "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #4 = { nounwind }

!llvm.dbg.cu = !{!0}
!llvm.module.flags = !{!6, !7, !8}
!llvm.ident = !{!9}

!0 = distinct !DICompileUnit(language: DW_LANG_C99, file: !1, producer: "clang version 8.0.1- (branches/release_80)", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, enums: !2, retainedTypes: !3, nameTableKind: None)
!1 = !DIFile(filename: "../test/test6.c", directory: "/C-resource-leak-checker/build")
!2 = !{}
!3 = !{!4}
!4 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !5, size: 64)
!5 = !DIBasicType(name: "char", size: 8, encoding: DW_ATE_signed_char)
!6 = !{i32 2, !"Dwarf Version", i32 4}
!7 = !{i32 2, !"Debug Info Version", i32 3}
!8 = !{i32 1, !"wchar_size", i32 4}
!9 = !{!"clang version 8.0.1- (branches/release_80)"}
!10 = distinct !DISubprogram(name: "strcpy", scope: !1, file: !1, line: 14, type: !11, scopeLine: 14, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition, unit: !0, retainedNodes: !2)
!11 = !DISubroutineType(types: !12)
!12 = !{!4, !13, !14}
!13 = !DIDerivedType(tag: DW_TAG_restrict_type, baseType: !4)
!14 = !DIDerivedType(tag: DW_TAG_restrict_type, baseType: !15)
!15 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !16, size: 64)
!16 = !DIDerivedType(tag: DW_TAG_const_type, baseType: !5)
!17 = !DILocalVariable(name: "__dest", arg: 1, scope: !10, file: !1, line: 14, type: !13)
!18 = !DILocation(line: 14, column: 33, scope: !10)
!19 = !DILocalVariable(name: "__src", arg: 2, scope: !10, file: !1, line: 14, type: !14)
!20 = !DILocation(line: 14, column: 66, scope: !10)
!21 = !DILocation(line: 15, column: 5, scope: !10)
!22 = !DILocation(line: 16, column: 1, scope: !10)
!23 = distinct !DISubprogram(name: "main", scope: !1, file: !1, line: 18, type: !24, scopeLine: 18, spFlags: DISPFlagDefinition, unit: !0, retainedNodes: !2)
!24 = !DISubroutineType(types: !25)
!25 = !{!26}
!26 = !DIBasicType(name: "int", size: 32, encoding: DW_ATE_signed)
!27 = !DILocalVariable(name: "str", scope: !23, file: !1, line: 19, type: !4)
!28 = !DILocation(line: 19, column: 10, scope: !23)
!29 = !DILocalVariable(name: "a", scope: !23, file: !1, line: 20, type: !26)
!30 = !DILocation(line: 20, column: 8, scope: !23)
!31 = !DILocation(line: 23, column: 19, scope: !23)
!32 = !DILocation(line: 23, column: 8, scope: !23)
!33 = !DILocation(line: 24, column: 11, scope: !23)
!34 = !DILocation(line: 24, column: 4, scope: !23)
!35 = !DILocation(line: 25, column: 43, scope: !23)
!36 = !DILocation(line: 25, column: 48, scope: !23)
!37 = !DILocation(line: 25, column: 4, scope: !23)
!38 = !DILocation(line: 28, column: 27, scope: !23)
!39 = !DILocation(line: 28, column: 19, scope: !23)
!40 = !DILocation(line: 28, column: 8, scope: !23)
!41 = !DILocation(line: 29, column: 11, scope: !23)
!42 = !DILocation(line: 29, column: 4, scope: !23)
!43 = !DILocation(line: 30, column: 43, scope: !23)
!44 = !DILocation(line: 30, column: 48, scope: !23)
!45 = !DILocation(line: 30, column: 4, scope: !23)
!46 = !DILocation(line: 32, column: 9, scope: !23)
!47 = !DILocation(line: 32, column: 4, scope: !23)
!48 = !DILocation(line: 34, column: 11, scope: !23)
!49 = !DILocation(line: 34, column: 4, scope: !23)
!50 = !DILocation(line: 36, column: 21, scope: !23)
!51 = !DILocation(line: 36, column: 4, scope: !23)
!52 = !DILocation(line: 38, column: 4, scope: !23)
