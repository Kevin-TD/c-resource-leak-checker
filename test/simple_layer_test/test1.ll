; ModuleID = '../test/simple_layer_test/test1.c'
source_filename = "../test/simple_layer_test/test1.c"
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

@.str = private unnamed_addr constant [11 x i8] c"helloworld\00", align 1
@.str.1 = private unnamed_addr constant [28 x i8] c"String = %s,  Address = %u\0A\00", align 1
@.str.2 = private unnamed_addr constant [6 x i8] c"hello\00", align 1
@str2 = common dso_local global i8* null, align 8, !dbg !0

; Function Attrs: noinline nounwind uwtable
define dso_local i32 @main() #0 !dbg !13 {
entry:
  %retval = alloca i32, align 4
  %str = alloca i8*, align 8
  store i32 0, i32* %retval, align 4
  call void @llvm.dbg.declare(metadata i8** %str, metadata !17, metadata !DIExpression()), !dbg !18
  %call = call noalias i8* @malloc(i64 15) #4, !dbg !19
  store i8* %call, i8** %str, align 8, !dbg !20
  %0 = load i8*, i8** %str, align 8, !dbg !21
  %call1 = call i8* @strcpy(i8* %0, i8* getelementptr inbounds ([11 x i8], [11 x i8]* @.str, i32 0, i32 0)) #4, !dbg !22
  %1 = load i8*, i8** %str, align 8, !dbg !23
  %2 = load i8*, i8** %str, align 8, !dbg !24
  %call2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([28 x i8], [28 x i8]* @.str.1, i32 0, i32 0), i8* %1, i8* %2), !dbg !25
  %3 = load i8*, i8** %str, align 8, !dbg !26
  %call3 = call i8* @realloc(i8* %3, i64 25) #4, !dbg !27
  store i8* %call3, i8** %str, align 8, !dbg !28
  %4 = load i8*, i8** %str, align 8, !dbg !29
  %call4 = call i8* @strcat(i8* %4, i8* getelementptr inbounds ([6 x i8], [6 x i8]* @.str.2, i32 0, i32 0)) #4, !dbg !30
  %5 = load i8*, i8** %str, align 8, !dbg !31
  %6 = load i8*, i8** %str, align 8, !dbg !32
  %call5 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([28 x i8], [28 x i8]* @.str.1, i32 0, i32 0), i8* %5, i8* %6), !dbg !33
  %7 = load i8*, i8** %str, align 8, !dbg !34
  call void @free(i8* %7) #4, !dbg !35
  %call6 = call noalias i8* @malloc(i64 15) #4, !dbg !36
  store i8* %call6, i8** @str2, align 8, !dbg !37
  %8 = load i8*, i8** @str2, align 8, !dbg !38
  call void @free(i8* %8) #4, !dbg !39
  ret i32 0, !dbg !40
}

; Function Attrs: nounwind readnone speculatable
declare void @llvm.dbg.declare(metadata, metadata, metadata) #1

; Function Attrs: nounwind
declare dso_local noalias i8* @malloc(i64) #2

; Function Attrs: nounwind
declare dso_local i8* @strcpy(i8*, i8*) #2

declare dso_local i32 @printf(i8*, ...) #3

; Function Attrs: nounwind
declare dso_local i8* @realloc(i8*, i64) #2

; Function Attrs: nounwind
declare dso_local i8* @strcat(i8*, i8*) #2

; Function Attrs: nounwind
declare dso_local void @free(i8*) #2

attributes #0 = { noinline nounwind uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { nounwind readnone speculatable }
attributes #2 = { nounwind "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #3 = { "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #4 = { nounwind }

!llvm.dbg.cu = !{!2}
!llvm.module.flags = !{!9, !10, !11}
!llvm.ident = !{!12}

!0 = !DIGlobalVariableExpression(var: !1, expr: !DIExpression())
!1 = distinct !DIGlobalVariable(name: "str2", scope: !2, file: !3, line: 6, type: !6, isLocal: false, isDefinition: true)
!2 = distinct !DICompileUnit(language: DW_LANG_C99, file: !3, producer: "clang version 8.0.1- (branches/release_80)", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, enums: !4, retainedTypes: !5, globals: !8, nameTableKind: None)
!3 = !DIFile(filename: "../test/simple_layer_test/test1.c", directory: "/c-resource-leak-checker/build")
!4 = !{}
!5 = !{!6}
!6 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !7, size: 64)
!7 = !DIBasicType(name: "char", size: 8, encoding: DW_ATE_signed_char)
!8 = !{!0}
!9 = !{i32 2, !"Dwarf Version", i32 4}
!10 = !{i32 2, !"Debug Info Version", i32 3}
!11 = !{i32 1, !"wchar_size", i32 4}
!12 = !{!"clang version 8.0.1- (branches/release_80)"}
!13 = distinct !DISubprogram(name: "main", scope: !3, file: !3, line: 8, type: !14, scopeLine: 8, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !4)
!14 = !DISubroutineType(types: !15)
!15 = !{!16}
!16 = !DIBasicType(name: "int", size: 32, encoding: DW_ATE_signed)
!17 = !DILocalVariable(name: "str", scope: !13, file: !3, line: 9, type: !6)
!18 = !DILocation(line: 9, column: 10, scope: !13)
!19 = !DILocation(line: 12, column: 19, scope: !13)
!20 = !DILocation(line: 12, column: 8, scope: !13)
!21 = !DILocation(line: 13, column: 11, scope: !13)
!22 = !DILocation(line: 13, column: 4, scope: !13)
!23 = !DILocation(line: 14, column: 43, scope: !13)
!24 = !DILocation(line: 14, column: 48, scope: !13)
!25 = !DILocation(line: 14, column: 4, scope: !13)
!26 = !DILocation(line: 17, column: 27, scope: !13)
!27 = !DILocation(line: 17, column: 19, scope: !13)
!28 = !DILocation(line: 17, column: 8, scope: !13)
!29 = !DILocation(line: 18, column: 11, scope: !13)
!30 = !DILocation(line: 18, column: 4, scope: !13)
!31 = !DILocation(line: 19, column: 43, scope: !13)
!32 = !DILocation(line: 19, column: 48, scope: !13)
!33 = !DILocation(line: 19, column: 4, scope: !13)
!34 = !DILocation(line: 21, column: 9, scope: !13)
!35 = !DILocation(line: 21, column: 4, scope: !13)
!36 = !DILocation(line: 23, column: 19, scope: !13)
!37 = !DILocation(line: 23, column: 9, scope: !13)
!38 = !DILocation(line: 24, column: 9, scope: !13)
!39 = !DILocation(line: 24, column: 4, scope: !13)
!40 = !DILocation(line: 26, column: 4, scope: !13)
