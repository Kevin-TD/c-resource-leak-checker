; ModuleID = '../test/test4.c'
source_filename = "../test/test4.c"
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

@.str = private unnamed_addr constant [12 x i8] c"do nothing!\00", align 1
@.str.1 = private unnamed_addr constant [11 x i8] c"helloworld\00", align 1
@.str.2 = private unnamed_addr constant [28 x i8] c"String = %s,  Address = %u\0A\00", align 1
@.str.3 = private unnamed_addr constant [6 x i8] c"hello\00", align 1

; Function Attrs: noinline nounwind uwtable
define dso_local void @unknown_function(i8* %s) #0 !dbg !10 {
entry:
  %s.addr = alloca i8*, align 8
  store i8* %s, i8** %s.addr, align 8
  call void @llvm.dbg.declare(metadata i8** %s.addr, metadata !13, metadata !DIExpression()), !dbg !14
  %call = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([12 x i8], [12 x i8]* @.str, i32 0, i32 0)), !dbg !15
  ret void, !dbg !16
}

; Function Attrs: nounwind readnone speculatable
declare void @llvm.dbg.declare(metadata, metadata, metadata) #1

declare dso_local i32 @printf(i8*, ...) #2

; Function Attrs: noinline nounwind uwtable
define dso_local i32 @main() #0 !dbg !17 {
entry:
  %retval = alloca i32, align 4
  %str = alloca i8*, align 8
  %a = alloca i32, align 4
  store i32 0, i32* %retval, align 4
  call void @llvm.dbg.declare(metadata i8** %str, metadata !21, metadata !DIExpression()), !dbg !22
  call void @llvm.dbg.declare(metadata i32* %a, metadata !23, metadata !DIExpression()), !dbg !24
  %call = call noalias i8* @malloc(i64 15) #4, !dbg !25
  store i8* %call, i8** %str, align 8, !dbg !26
  %0 = load i8*, i8** %str, align 8, !dbg !27
  %call1 = call i8* @strcpy(i8* %0, i8* getelementptr inbounds ([11 x i8], [11 x i8]* @.str.1, i32 0, i32 0)) #4, !dbg !28
  %1 = load i8*, i8** %str, align 8, !dbg !29
  %2 = load i8*, i8** %str, align 8, !dbg !30
  %call2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([28 x i8], [28 x i8]* @.str.2, i32 0, i32 0), i8* %1, i8* %2), !dbg !31
  %3 = load i8*, i8** %str, align 8, !dbg !32
  %call3 = call i8* @realloc(i8* %3, i64 25) #4, !dbg !33
  store i8* %call3, i8** %str, align 8, !dbg !34
  %4 = load i8*, i8** %str, align 8, !dbg !35
  %call4 = call i8* @strcat(i8* %4, i8* getelementptr inbounds ([6 x i8], [6 x i8]* @.str.3, i32 0, i32 0)) #4, !dbg !36
  %5 = load i8*, i8** %str, align 8, !dbg !37
  %6 = load i8*, i8** %str, align 8, !dbg !38
  %call5 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([28 x i8], [28 x i8]* @.str.2, i32 0, i32 0), i8* %5, i8* %6), !dbg !39
  %7 = load i8*, i8** %str, align 8, !dbg !40
  call void @free(i8* %7) #4, !dbg !41
  %8 = load i8*, i8** %str, align 8, !dbg !42
  call void @unknown_function(i8* %8), !dbg !43
  ret i32 0, !dbg !44
}

; Function Attrs: nounwind
declare dso_local noalias i8* @malloc(i64) #3

; Function Attrs: nounwind
declare dso_local i8* @strcpy(i8*, i8*) #3

; Function Attrs: nounwind
declare dso_local i8* @realloc(i8*, i64) #3

; Function Attrs: nounwind
declare dso_local i8* @strcat(i8*, i8*) #3

; Function Attrs: nounwind
declare dso_local void @free(i8*) #3

attributes #0 = { noinline nounwind uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { nounwind readnone speculatable }
attributes #2 = { "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #3 = { nounwind "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #4 = { nounwind }

!llvm.dbg.cu = !{!0}
!llvm.module.flags = !{!6, !7, !8}
!llvm.ident = !{!9}

!0 = distinct !DICompileUnit(language: DW_LANG_C99, file: !1, producer: "clang version 8.0.1- (branches/release_80)", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, enums: !2, retainedTypes: !3, nameTableKind: None)
!1 = !DIFile(filename: "../test/test4.c", directory: "/C-resource-leak-checker/build")
!2 = !{}
!3 = !{!4}
!4 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !5, size: 64)
!5 = !DIBasicType(name: "char", size: 8, encoding: DW_ATE_signed_char)
!6 = !{i32 2, !"Dwarf Version", i32 4}
!7 = !{i32 2, !"Debug Info Version", i32 3}
!8 = !{i32 1, !"wchar_size", i32 4}
!9 = !{!"clang version 8.0.1- (branches/release_80)"}
!10 = distinct !DISubprogram(name: "unknown_function", scope: !1, file: !1, line: 14, type: !11, scopeLine: 14, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition, unit: !0, retainedNodes: !2)
!11 = !DISubroutineType(types: !12)
!12 = !{null, !4}
!13 = !DILocalVariable(name: "s", arg: 1, scope: !10, file: !1, line: 14, type: !4)
!14 = !DILocation(line: 14, column: 29, scope: !10)
!15 = !DILocation(line: 15, column: 5, scope: !10)
!16 = !DILocation(line: 16, column: 1, scope: !10)
!17 = distinct !DISubprogram(name: "main", scope: !1, file: !1, line: 18, type: !18, scopeLine: 18, spFlags: DISPFlagDefinition, unit: !0, retainedNodes: !2)
!18 = !DISubroutineType(types: !19)
!19 = !{!20}
!20 = !DIBasicType(name: "int", size: 32, encoding: DW_ATE_signed)
!21 = !DILocalVariable(name: "str", scope: !17, file: !1, line: 19, type: !4)
!22 = !DILocation(line: 19, column: 10, scope: !17)
!23 = !DILocalVariable(name: "a", scope: !17, file: !1, line: 20, type: !20)
!24 = !DILocation(line: 20, column: 8, scope: !17)
!25 = !DILocation(line: 23, column: 19, scope: !17)
!26 = !DILocation(line: 23, column: 8, scope: !17)
!27 = !DILocation(line: 24, column: 11, scope: !17)
!28 = !DILocation(line: 24, column: 4, scope: !17)
!29 = !DILocation(line: 25, column: 43, scope: !17)
!30 = !DILocation(line: 25, column: 48, scope: !17)
!31 = !DILocation(line: 25, column: 4, scope: !17)
!32 = !DILocation(line: 28, column: 27, scope: !17)
!33 = !DILocation(line: 28, column: 19, scope: !17)
!34 = !DILocation(line: 28, column: 8, scope: !17)
!35 = !DILocation(line: 29, column: 11, scope: !17)
!36 = !DILocation(line: 29, column: 4, scope: !17)
!37 = !DILocation(line: 30, column: 43, scope: !17)
!38 = !DILocation(line: 30, column: 48, scope: !17)
!39 = !DILocation(line: 30, column: 4, scope: !17)
!40 = !DILocation(line: 32, column: 9, scope: !17)
!41 = !DILocation(line: 32, column: 4, scope: !17)
!42 = !DILocation(line: 34, column: 21, scope: !17)
!43 = !DILocation(line: 34, column: 4, scope: !17)
!44 = !DILocation(line: 36, column: 4, scope: !17)
