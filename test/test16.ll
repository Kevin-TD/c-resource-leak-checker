; ModuleID = '../test/test16.c'
source_filename = "../test/test16.c"
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
  %a = alloca i32, align 4
  %i = alloca i32, align 4
  store i32 0, i32* %retval, align 4
  call void @llvm.dbg.declare(metadata i8** %str, metadata !14, metadata !DIExpression()), !dbg !15
  call void @llvm.dbg.declare(metadata i32* %a, metadata !16, metadata !DIExpression()), !dbg !17
  %call = call i32 @getchar(), !dbg !18
  store i32 %call, i32* %a, align 4, !dbg !17
  %call1 = call noalias i8* @malloc(i64 15) #4, !dbg !19
  store i8* %call1, i8** %str, align 8, !dbg !20
  %0 = load i8*, i8** %str, align 8, !dbg !21
  %call2 = call i8* @strcpy(i8* %0, i8* getelementptr inbounds ([11 x i8], [11 x i8]* @.str, i32 0, i32 0)) #4, !dbg !22
  %1 = load i8*, i8** %str, align 8, !dbg !23
  %2 = load i8*, i8** %str, align 8, !dbg !24
  %call3 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([28 x i8], [28 x i8]* @.str.1, i32 0, i32 0), i8* %1, i8* %2), !dbg !25
  %3 = load i8*, i8** %str, align 8, !dbg !26
  %call4 = call i8* @realloc(i8* %3, i64 25) #4, !dbg !27
  store i8* %call4, i8** %str, align 8, !dbg !28
  %4 = load i8*, i8** %str, align 8, !dbg !29
  %call5 = call i8* @strcat(i8* %4, i8* getelementptr inbounds ([6 x i8], [6 x i8]* @.str.2, i32 0, i32 0)) #4, !dbg !30
  %5 = load i8*, i8** %str, align 8, !dbg !31
  %6 = load i8*, i8** %str, align 8, !dbg !32
  %call6 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([28 x i8], [28 x i8]* @.str.1, i32 0, i32 0), i8* %5, i8* %6), !dbg !33
  call void @llvm.dbg.declare(metadata i32* %i, metadata !34, metadata !DIExpression()), !dbg !36
  store i32 0, i32* %i, align 4, !dbg !36
  br label %for.cond, !dbg !37

for.cond:                                         ; preds = %for.inc, %entry
  %7 = load i32, i32* %i, align 4, !dbg !38
  %cmp = icmp slt i32 %7, 15, !dbg !40
  br i1 %cmp, label %for.body, label %for.end, !dbg !41

for.body:                                         ; preds = %for.cond
  %8 = load i8*, i8** %str, align 8, !dbg !42
  call void @free(i8* %8) #4, !dbg !44
  br label %for.inc, !dbg !45

for.inc:                                          ; preds = %for.body
  %9 = load i32, i32* %i, align 4, !dbg !46
  %inc = add nsw i32 %9, 1, !dbg !46
  store i32 %inc, i32* %i, align 4, !dbg !46
  br label %for.cond, !dbg !47, !llvm.loop !48

for.end:                                          ; preds = %for.cond
  ret i32 0, !dbg !50
}

; Function Attrs: nounwind readnone speculatable
declare void @llvm.dbg.declare(metadata, metadata, metadata) #1

declare dso_local i32 @getchar() #2

; Function Attrs: nounwind
declare dso_local noalias i8* @malloc(i64) #3

; Function Attrs: nounwind
declare dso_local i8* @strcpy(i8*, i8*) #3

declare dso_local i32 @printf(i8*, ...) #2

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
!1 = !DIFile(filename: "../test/test16.c", directory: "/c-resource-leak-checker/build")
!2 = !{}
!3 = !{!4}
!4 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !5, size: 64)
!5 = !DIBasicType(name: "char", size: 8, encoding: DW_ATE_signed_char)
!6 = !{i32 2, !"Dwarf Version", i32 4}
!7 = !{i32 2, !"Debug Info Version", i32 3}
!8 = !{i32 1, !"wchar_size", i32 4}
!9 = !{!"clang version 8.0.1- (branches/release_80)"}
!10 = distinct !DISubprogram(name: "main", scope: !1, file: !1, line: 6, type: !11, scopeLine: 6, spFlags: DISPFlagDefinition, unit: !0, retainedNodes: !2)
!11 = !DISubroutineType(types: !12)
!12 = !{!13}
!13 = !DIBasicType(name: "int", size: 32, encoding: DW_ATE_signed)
!14 = !DILocalVariable(name: "str", scope: !10, file: !1, line: 7, type: !4)
!15 = !DILocation(line: 7, column: 10, scope: !10)
!16 = !DILocalVariable(name: "a", scope: !10, file: !1, line: 8, type: !13)
!17 = !DILocation(line: 8, column: 8, scope: !10)
!18 = !DILocation(line: 8, column: 12, scope: !10)
!19 = !DILocation(line: 10, column: 19, scope: !10)
!20 = !DILocation(line: 10, column: 8, scope: !10)
!21 = !DILocation(line: 11, column: 11, scope: !10)
!22 = !DILocation(line: 11, column: 4, scope: !10)
!23 = !DILocation(line: 12, column: 43, scope: !10)
!24 = !DILocation(line: 12, column: 48, scope: !10)
!25 = !DILocation(line: 12, column: 4, scope: !10)
!26 = !DILocation(line: 14, column: 27, scope: !10)
!27 = !DILocation(line: 14, column: 19, scope: !10)
!28 = !DILocation(line: 14, column: 8, scope: !10)
!29 = !DILocation(line: 15, column: 11, scope: !10)
!30 = !DILocation(line: 15, column: 4, scope: !10)
!31 = !DILocation(line: 16, column: 43, scope: !10)
!32 = !DILocation(line: 16, column: 48, scope: !10)
!33 = !DILocation(line: 16, column: 4, scope: !10)
!34 = !DILocalVariable(name: "i", scope: !35, file: !1, line: 18, type: !13)
!35 = distinct !DILexicalBlock(scope: !10, file: !1, line: 18, column: 4)
!36 = !DILocation(line: 18, column: 13, scope: !35)
!37 = !DILocation(line: 18, column: 9, scope: !35)
!38 = !DILocation(line: 18, column: 20, scope: !39)
!39 = distinct !DILexicalBlock(scope: !35, file: !1, line: 18, column: 4)
!40 = !DILocation(line: 18, column: 22, scope: !39)
!41 = !DILocation(line: 18, column: 4, scope: !35)
!42 = !DILocation(line: 19, column: 10, scope: !43)
!43 = distinct !DILexicalBlock(scope: !39, file: !1, line: 18, column: 33)
!44 = !DILocation(line: 19, column: 5, scope: !43)
!45 = !DILocation(line: 20, column: 4, scope: !43)
!46 = !DILocation(line: 18, column: 29, scope: !39)
!47 = !DILocation(line: 18, column: 4, scope: !39)
!48 = distinct !{!48, !41, !49}
!49 = !DILocation(line: 20, column: 4, scope: !35)
!50 = !DILocation(line: 25, column: 4, scope: !10)
