; ModuleID = '../test/test7.c'
source_filename = "../test/test7.c"
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

@.str = private unnamed_addr constant [11 x i8] c"helloworld\00", align 1
@.str.1 = private unnamed_addr constant [28 x i8] c"String = %s,  Address = %u\0A\00", align 1

; Function Attrs: noinline nounwind uwtable
define dso_local i32 @main() #0 !dbg !10 {
entry:
  %retval = alloca i32, align 4
  %str = alloca i8*, align 8
  %a = alloca i32, align 4
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
  call void @free(i8* %3) #4, !dbg !27
  %4 = load i32, i32* %a, align 4, !dbg !28
  %cmp = icmp eq i32 %4, -15, !dbg !30
  br i1 %cmp, label %if.then, label %if.else, !dbg !31

if.then:                                          ; preds = %entry
  br label %if.end16, !dbg !32

if.else:                                          ; preds = %entry
  %5 = load i32, i32* %a, align 4, !dbg !34
  %cmp4 = icmp eq i32 %5, -10, !dbg !36
  br i1 %cmp4, label %if.then5, label %if.else14, !dbg !37

if.then5:                                         ; preds = %if.else
  %6 = load i32, i32* %a, align 4, !dbg !38
  %cmp6 = icmp eq i32 %6, -5, !dbg !41
  br i1 %cmp6, label %if.then7, label %if.else8, !dbg !42

if.then7:                                         ; preds = %if.then5
  br label %if.end13, !dbg !43

if.else8:                                         ; preds = %if.then5
  %7 = load i32, i32* %a, align 4, !dbg !45
  %cmp9 = icmp eq i32 %7, -100, !dbg !47
  br i1 %cmp9, label %if.then10, label %if.else11, !dbg !48

if.then10:                                        ; preds = %if.else8
  br label %if.end, !dbg !49

if.else11:                                        ; preds = %if.else8
  %8 = load i8*, i8** %str, align 8, !dbg !51
  %call12 = call i8* @realloc(i8* %8, i64 25) #4, !dbg !53
  br label %if.end

if.end:                                           ; preds = %if.else11, %if.then10
  br label %if.end13

if.end13:                                         ; preds = %if.end, %if.then7
  br label %if.end15, !dbg !54

if.else14:                                        ; preds = %if.else
  %9 = load i8*, i8** %str, align 8, !dbg !55
  call void @free(i8* %9) #4, !dbg !57
  br label %if.end15

if.end15:                                         ; preds = %if.else14, %if.end13
  br label %if.end16

if.end16:                                         ; preds = %if.end15, %if.then
  ret i32 0, !dbg !58
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
declare dso_local void @free(i8*) #3

; Function Attrs: nounwind
declare dso_local i8* @realloc(i8*, i64) #3

attributes #0 = { noinline nounwind uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { nounwind readnone speculatable }
attributes #2 = { "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #3 = { nounwind "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #4 = { nounwind }

!llvm.dbg.cu = !{!0}
!llvm.module.flags = !{!6, !7, !8}
!llvm.ident = !{!9}

!0 = distinct !DICompileUnit(language: DW_LANG_C99, file: !1, producer: "clang version 8.0.1- (branches/release_80)", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, enums: !2, retainedTypes: !3, nameTableKind: None)
!1 = !DIFile(filename: "../test/test7.c", directory: "/C-resource-leak-checker/build")
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
!26 = !DILocation(line: 14, column: 9, scope: !10)
!27 = !DILocation(line: 14, column: 4, scope: !10)
!28 = !DILocation(line: 18, column: 8, scope: !29)
!29 = distinct !DILexicalBlock(scope: !10, file: !1, line: 18, column: 8)
!30 = !DILocation(line: 18, column: 10, scope: !29)
!31 = !DILocation(line: 18, column: 8, scope: !10)
!32 = !DILocation(line: 20, column: 5, scope: !33)
!33 = distinct !DILexicalBlock(scope: !29, file: !1, line: 18, column: 18)
!34 = !DILocation(line: 21, column: 14, scope: !35)
!35 = distinct !DILexicalBlock(scope: !29, file: !1, line: 21, column: 14)
!36 = !DILocation(line: 21, column: 16, scope: !35)
!37 = !DILocation(line: 21, column: 14, scope: !29)
!38 = !DILocation(line: 22, column: 13, scope: !39)
!39 = distinct !DILexicalBlock(scope: !40, file: !1, line: 22, column: 13)
!40 = distinct !DILexicalBlock(scope: !35, file: !1, line: 21, column: 24)
!41 = !DILocation(line: 22, column: 15, scope: !39)
!42 = !DILocation(line: 22, column: 13, scope: !40)
!43 = !DILocation(line: 24, column: 9, scope: !44)
!44 = distinct !DILexicalBlock(scope: !39, file: !1, line: 22, column: 22)
!45 = !DILocation(line: 25, column: 18, scope: !46)
!46 = distinct !DILexicalBlock(scope: !39, file: !1, line: 25, column: 18)
!47 = !DILocation(line: 25, column: 20, scope: !46)
!48 = !DILocation(line: 25, column: 18, scope: !39)
!49 = !DILocation(line: 27, column: 9, scope: !50)
!50 = distinct !DILexicalBlock(scope: !46, file: !1, line: 25, column: 29)
!51 = !DILocation(line: 29, column: 21, scope: !52)
!52 = distinct !DILexicalBlock(scope: !46, file: !1, line: 28, column: 14)
!53 = !DILocation(line: 29, column: 13, scope: !52)
!54 = !DILocation(line: 32, column: 5, scope: !40)
!55 = !DILocation(line: 34, column: 14, scope: !56)
!56 = distinct !DILexicalBlock(scope: !35, file: !1, line: 33, column: 10)
!57 = !DILocation(line: 34, column: 9, scope: !56)
!58 = !DILocation(line: 39, column: 4, scope: !10)
