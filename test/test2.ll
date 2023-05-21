; ModuleID = '../test/test2.c'
source_filename = "../test/test2.c"
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

@.str = private unnamed_addr constant [11 x i8] c"helloworld\00", align 1
@.str.1 = private unnamed_addr constant [28 x i8] c"String = %s,  Address = %u\0A\00", align 1
@.str.2 = private unnamed_addr constant [6 x i8] c"hello\00", align 1

; Function Attrs: noinline nounwind uwtable
define dso_local i32 @main() #0 !dbg !10 {
entry:
  %retval = alloca i32, align 4
  %str = alloca i8*, align 8
  store i32 0, i32* %retval, align 4
  call void @llvm.dbg.declare(metadata i8** %str, metadata !14, metadata !DIExpression()), !dbg !15
  %call = call noalias i8* @malloc(i64 15) #4, !dbg !16
  store i8* %call, i8** %str, align 8, !dbg !17
  %0 = load i8*, i8** %str, align 8, !dbg !18
  %call1 = call i8* @strcpy(i8* %0, i8* getelementptr inbounds ([11 x i8], [11 x i8]* @.str, i32 0, i32 0)), !dbg !19
  %1 = load i8*, i8** %str, align 8, !dbg !20
  %2 = load i8*, i8** %str, align 8, !dbg !21
  %call2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([28 x i8], [28 x i8]* @.str.1, i32 0, i32 0), i8* %1, i8* %2), !dbg !22
  %3 = load i8*, i8** %str, align 8, !dbg !23
  %call3 = call i8* @realloc(i8* %3, i64 25) #4, !dbg !24
  store i8* %call3, i8** %str, align 8, !dbg !25
  %4 = load i8*, i8** %str, align 8, !dbg !26
  %call4 = call i8* @strcat(i8* %4, i8* getelementptr inbounds ([6 x i8], [6 x i8]* @.str.2, i32 0, i32 0)), !dbg !27
  %5 = load i8*, i8** %str, align 8, !dbg !28
  %6 = load i8*, i8** %str, align 8, !dbg !29
  %call5 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([28 x i8], [28 x i8]* @.str.1, i32 0, i32 0), i8* %5, i8* %6), !dbg !30
  ret i32 0, !dbg !31
}

; Function Attrs: nounwind readnone speculatable
declare void @llvm.dbg.declare(metadata, metadata, metadata) #1

; Function Attrs: nounwind
declare dso_local noalias i8* @malloc(i64) #2

declare dso_local i8* @strcpy(i8*, i8*) #3

declare dso_local i32 @printf(i8*, ...) #3

; Function Attrs: nounwind
declare dso_local i8* @realloc(i8*, i64) #2

declare dso_local i8* @strcat(i8*, i8*) #3

attributes #0 = { noinline nounwind uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { nounwind readnone speculatable }
attributes #2 = { nounwind "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #3 = { "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #4 = { nounwind }

!llvm.dbg.cu = !{!0}
!llvm.module.flags = !{!6, !7, !8}
!llvm.ident = !{!9}

!0 = distinct !DICompileUnit(language: DW_LANG_C99, file: !1, producer: "clang version 8.0.1- (branches/release_80)", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, enums: !2, retainedTypes: !3, nameTableKind: None)
!1 = !DIFile(filename: "../test/test2.c", directory: "/resource leak checker/build")
!2 = !{}
!3 = !{!4}
!4 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !5, size: 64)
!5 = !DIBasicType(name: "char", size: 8, encoding: DW_ATE_signed_char)
!6 = !{i32 2, !"Dwarf Version", i32 4}
!7 = !{i32 2, !"Debug Info Version", i32 3}
!8 = !{i32 1, !"wchar_size", i32 4}
!9 = !{!"clang version 8.0.1- (branches/release_80)"}
!10 = distinct !DISubprogram(name: "main", scope: !1, file: !1, line: 4, type: !11, scopeLine: 4, spFlags: DISPFlagDefinition, unit: !0, retainedNodes: !2)
!11 = !DISubroutineType(types: !12)
!12 = !{!13}
!13 = !DIBasicType(name: "int", size: 32, encoding: DW_ATE_signed)
!14 = !DILocalVariable(name: "str", scope: !10, file: !1, line: 5, type: !4)
!15 = !DILocation(line: 5, column: 10, scope: !10)
!16 = !DILocation(line: 8, column: 19, scope: !10)
!17 = !DILocation(line: 8, column: 8, scope: !10)
!18 = !DILocation(line: 9, column: 11, scope: !10)
!19 = !DILocation(line: 9, column: 4, scope: !10)
!20 = !DILocation(line: 10, column: 43, scope: !10)
!21 = !DILocation(line: 10, column: 48, scope: !10)
!22 = !DILocation(line: 10, column: 4, scope: !10)
!23 = !DILocation(line: 13, column: 27, scope: !10)
!24 = !DILocation(line: 13, column: 19, scope: !10)
!25 = !DILocation(line: 13, column: 8, scope: !10)
!26 = !DILocation(line: 14, column: 11, scope: !10)
!27 = !DILocation(line: 14, column: 4, scope: !10)
!28 = !DILocation(line: 15, column: 43, scope: !10)
!29 = !DILocation(line: 15, column: 48, scope: !10)
!30 = !DILocation(line: 15, column: 4, scope: !10)
!31 = !DILocation(line: 19, column: 4, scope: !10)
