; ModuleID = '../test/test13/test13.c'
source_filename = "../test/test13/test13.c"
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

@.str = private unnamed_addr constant [6 x i8] c"hello\00", align 1
@.str.1 = private unnamed_addr constant [28 x i8] c"String = %s,  Address = %u\0A\00", align 1

; Function Attrs: noinline nounwind uwtable
define dso_local i32 @main() #0 !dbg !10 {
entry:
  %retval = alloca i32, align 4
  %str = alloca i8*, align 8
  %p = alloca i8**, align 8
  store i32 0, i32* %retval, align 4
  call void @llvm.dbg.declare(metadata i8** %str, metadata !14, metadata !DIExpression()), !dbg !15
  call void @llvm.dbg.declare(metadata i8*** %p, metadata !16, metadata !DIExpression()), !dbg !18
  store i8** %str, i8*** %p, align 8, !dbg !18
  %call = call noalias i8* @malloc(i64 15) #4, !dbg !19
  %0 = load i8**, i8*** %p, align 8, !dbg !20
  store i8* %call, i8** %0, align 8, !dbg !21
  %1 = load i8**, i8*** %p, align 8, !dbg !22
  %2 = load i8*, i8** %1, align 8, !dbg !23
  %call1 = call i8* @strcat(i8* %2, i8* getelementptr inbounds ([6 x i8], [6 x i8]* @.str, i32 0, i32 0)) #4, !dbg !24
  %3 = load i8**, i8*** %p, align 8, !dbg !25
  %4 = load i8*, i8** %3, align 8, !dbg !26
  %5 = load i8**, i8*** %p, align 8, !dbg !27
  %6 = load i8*, i8** %5, align 8, !dbg !28
  %call2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([28 x i8], [28 x i8]* @.str.1, i32 0, i32 0), i8* %4, i8* %6), !dbg !29
  %7 = load i8**, i8*** %p, align 8, !dbg !30
  %8 = load i8*, i8** %7, align 8, !dbg !31
  call void @free(i8* %8) #4, !dbg !32
  ret i32 0, !dbg !33
}

; Function Attrs: nounwind readnone speculatable
declare void @llvm.dbg.declare(metadata, metadata, metadata) #1

; Function Attrs: nounwind
declare dso_local noalias i8* @malloc(i64) #2

; Function Attrs: nounwind
declare dso_local i8* @strcat(i8*, i8*) #2

declare dso_local i32 @printf(i8*, ...) #3

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

!0 = distinct !DICompileUnit(language: DW_LANG_C99, file: !1, producer: "clang version 8.0.1- (branches/release_80)", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, enums: !2, retainedTypes: !3, nameTableKind: None)
!1 = !DIFile(filename: "../test/test13/test13.c", directory: "/c-resource-leak-checker/build")
!2 = !{}
!3 = !{!4}
!4 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !5, size: 64)
!5 = !DIBasicType(name: "char", size: 8, encoding: DW_ATE_signed_char)
!6 = !{i32 2, !"Dwarf Version", i32 4}
!7 = !{i32 2, !"Debug Info Version", i32 3}
!8 = !{i32 1, !"wchar_size", i32 4}
!9 = !{!"clang version 8.0.1- (branches/release_80)"}
!10 = distinct !DISubprogram(name: "main", scope: !1, file: !1, line: 5, type: !11, scopeLine: 5, spFlags: DISPFlagDefinition, unit: !0, retainedNodes: !2)
!11 = !DISubroutineType(types: !12)
!12 = !{!13}
!13 = !DIBasicType(name: "int", size: 32, encoding: DW_ATE_signed)
!14 = !DILocalVariable(name: "str", scope: !10, file: !1, line: 6, type: !4)
!15 = !DILocation(line: 6, column: 10, scope: !10)
!16 = !DILocalVariable(name: "p", scope: !10, file: !1, line: 7, type: !17)
!17 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !4, size: 64)
!18 = !DILocation(line: 7, column: 11, scope: !10)
!19 = !DILocation(line: 9, column: 16, scope: !10)
!20 = !DILocation(line: 9, column: 5, scope: !10)
!21 = !DILocation(line: 9, column: 7, scope: !10)
!22 = !DILocation(line: 10, column: 12, scope: !10)
!23 = !DILocation(line: 10, column: 11, scope: !10)
!24 = !DILocation(line: 10, column: 4, scope: !10)
!25 = !DILocation(line: 11, column: 44, scope: !10)
!26 = !DILocation(line: 11, column: 43, scope: !10)
!27 = !DILocation(line: 11, column: 48, scope: !10)
!28 = !DILocation(line: 11, column: 47, scope: !10)
!29 = !DILocation(line: 11, column: 4, scope: !10)
!30 = !DILocation(line: 14, column: 10, scope: !10)
!31 = !DILocation(line: 14, column: 9, scope: !10)
!32 = !DILocation(line: 14, column: 4, scope: !10)
!33 = !DILocation(line: 16, column: 4, scope: !10)
