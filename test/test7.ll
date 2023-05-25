; ModuleID = '../test/test7.c'
source_filename = "../test/test7.c"
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

@.str = private unnamed_addr constant [11 x i8] c"helloworld\00", align 1
@.str.1 = private unnamed_addr constant [28 x i8] c"String = %s,  Address = %u\0A\00", align 1

; Function Attrs: noinline nounwind uwtable
define dso_local i32 @otherFunc(i8* %str) #0 !dbg !10 {
entry:
  %retval = alloca i32, align 4
  %str.addr = alloca i8*, align 8
  %a = alloca i32, align 4
  store i8* %str, i8** %str.addr, align 8
  call void @llvm.dbg.declare(metadata i8** %str.addr, metadata !14, metadata !DIExpression()), !dbg !15
  call void @llvm.dbg.declare(metadata i32* %a, metadata !16, metadata !DIExpression()), !dbg !17
  %call = call i32 @getchar(), !dbg !18
  store i32 %call, i32* %a, align 4, !dbg !17
  %0 = load i32, i32* %a, align 4, !dbg !19
  %cmp = icmp eq i32 %0, -15, !dbg !21
  br i1 %cmp, label %if.then, label %if.else, !dbg !22

if.then:                                          ; preds = %entry
  br label %if.end4, !dbg !23

if.else:                                          ; preds = %entry
  %1 = load i32, i32* %a, align 4, !dbg !25
  %cmp1 = icmp eq i32 %1, -10, !dbg !27
  br i1 %cmp1, label %if.then2, label %if.else3, !dbg !28

if.then2:                                         ; preds = %if.else
  br label %if.end, !dbg !29

if.else3:                                         ; preds = %if.else
  %2 = load i8*, i8** %str.addr, align 8, !dbg !31
  call void @free(i8* %2) #4, !dbg !33
  br label %if.end

if.end:                                           ; preds = %if.else3, %if.then2
  br label %if.end4

if.end4:                                          ; preds = %if.end, %if.then
  %3 = load i32, i32* %retval, align 4, !dbg !34
  ret i32 %3, !dbg !34
}

; Function Attrs: nounwind readnone speculatable
declare void @llvm.dbg.declare(metadata, metadata, metadata) #1

declare dso_local i32 @getchar() #2

; Function Attrs: nounwind
declare dso_local void @free(i8*) #3

; Function Attrs: noinline nounwind uwtable
define dso_local i32 @main() #0 !dbg !35 {
entry:
  %retval = alloca i32, align 4
  %str = alloca i8*, align 8
  %a = alloca i32, align 4
  store i32 0, i32* %retval, align 4
  call void @llvm.dbg.declare(metadata i8** %str, metadata !38, metadata !DIExpression()), !dbg !39
  call void @llvm.dbg.declare(metadata i32* %a, metadata !40, metadata !DIExpression()), !dbg !41
  %call = call i32 @getchar(), !dbg !42
  store i32 %call, i32* %a, align 4, !dbg !41
  %call1 = call noalias i8* @malloc(i64 15) #4, !dbg !43
  store i8* %call1, i8** %str, align 8, !dbg !44
  %0 = load i8*, i8** %str, align 8, !dbg !45
  %call2 = call i8* @strcpy(i8* %0, i8* getelementptr inbounds ([11 x i8], [11 x i8]* @.str, i32 0, i32 0)) #4, !dbg !46
  %1 = load i8*, i8** %str, align 8, !dbg !47
  %2 = load i8*, i8** %str, align 8, !dbg !48
  %call3 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([28 x i8], [28 x i8]* @.str.1, i32 0, i32 0), i8* %1, i8* %2), !dbg !49
  %3 = load i32, i32* %a, align 4, !dbg !50
  %cmp = icmp eq i32 %3, -15, !dbg !52
  br i1 %cmp, label %if.then, label %if.else, !dbg !53

if.then:                                          ; preds = %entry
  br label %if.end16, !dbg !54

if.else:                                          ; preds = %entry
  %4 = load i32, i32* %a, align 4, !dbg !56
  %cmp4 = icmp eq i32 %4, -10, !dbg !58
  br i1 %cmp4, label %if.then5, label %if.else14, !dbg !59

if.then5:                                         ; preds = %if.else
  %5 = load i32, i32* %a, align 4, !dbg !60
  %cmp6 = icmp eq i32 %5, -5, !dbg !63
  br i1 %cmp6, label %if.then7, label %if.else8, !dbg !64

if.then7:                                         ; preds = %if.then5
  br label %if.end13, !dbg !65

if.else8:                                         ; preds = %if.then5
  %6 = load i32, i32* %a, align 4, !dbg !67
  %cmp9 = icmp eq i32 %6, -100, !dbg !69
  br i1 %cmp9, label %if.then10, label %if.else11, !dbg !70

if.then10:                                        ; preds = %if.else8
  br label %if.end, !dbg !71

if.else11:                                        ; preds = %if.else8
  %7 = load i8*, i8** %str, align 8, !dbg !73
  %call12 = call i8* @realloc(i8* %7, i64 25) #4, !dbg !75
  br label %if.end

if.end:                                           ; preds = %if.else11, %if.then10
  br label %if.end13

if.end13:                                         ; preds = %if.end, %if.then7
  br label %if.end15, !dbg !76

if.else14:                                        ; preds = %if.else
  %8 = load i8*, i8** %str, align 8, !dbg !77
  call void @free(i8* %8) #4, !dbg !79
  br label %if.end15

if.end15:                                         ; preds = %if.else14, %if.end13
  br label %if.end16

if.end16:                                         ; preds = %if.end15, %if.then
  %9 = load i8*, i8** %str, align 8, !dbg !80
  %10 = load i8*, i8** %str, align 8, !dbg !81
  %call17 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([28 x i8], [28 x i8]* @.str.1, i32 0, i32 0), i8* %9, i8* %10), !dbg !82
  ret i32 0, !dbg !83
}

; Function Attrs: nounwind
declare dso_local noalias i8* @malloc(i64) #3

; Function Attrs: nounwind
declare dso_local i8* @strcpy(i8*, i8*) #3

declare dso_local i32 @printf(i8*, ...) #2

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
!10 = distinct !DISubprogram(name: "otherFunc", scope: !1, file: !1, line: 5, type: !11, scopeLine: 5, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition, unit: !0, retainedNodes: !2)
!11 = !DISubroutineType(types: !12)
!12 = !{!13, !4}
!13 = !DIBasicType(name: "int", size: 32, encoding: DW_ATE_signed)
!14 = !DILocalVariable(name: "str", arg: 1, scope: !10, file: !1, line: 5, type: !4)
!15 = !DILocation(line: 5, column: 21, scope: !10)
!16 = !DILocalVariable(name: "a", scope: !10, file: !1, line: 6, type: !13)
!17 = !DILocation(line: 6, column: 9, scope: !10)
!18 = !DILocation(line: 6, column: 13, scope: !10)
!19 = !DILocation(line: 8, column: 9, scope: !20)
!20 = distinct !DILexicalBlock(scope: !10, file: !1, line: 8, column: 9)
!21 = !DILocation(line: 8, column: 11, scope: !20)
!22 = !DILocation(line: 8, column: 9, scope: !10)
!23 = !DILocation(line: 10, column: 5, scope: !24)
!24 = distinct !DILexicalBlock(scope: !20, file: !1, line: 8, column: 19)
!25 = !DILocation(line: 11, column: 14, scope: !26)
!26 = distinct !DILexicalBlock(scope: !20, file: !1, line: 11, column: 14)
!27 = !DILocation(line: 11, column: 16, scope: !26)
!28 = !DILocation(line: 11, column: 14, scope: !20)
!29 = !DILocation(line: 13, column: 5, scope: !30)
!30 = distinct !DILexicalBlock(scope: !26, file: !1, line: 11, column: 24)
!31 = !DILocation(line: 15, column: 14, scope: !32)
!32 = distinct !DILexicalBlock(scope: !26, file: !1, line: 14, column: 10)
!33 = !DILocation(line: 15, column: 9, scope: !32)
!34 = !DILocation(line: 17, column: 1, scope: !10)
!35 = distinct !DISubprogram(name: "main", scope: !1, file: !1, line: 19, type: !36, scopeLine: 19, spFlags: DISPFlagDefinition, unit: !0, retainedNodes: !2)
!36 = !DISubroutineType(types: !37)
!37 = !{!13}
!38 = !DILocalVariable(name: "str", scope: !35, file: !1, line: 20, type: !4)
!39 = !DILocation(line: 20, column: 10, scope: !35)
!40 = !DILocalVariable(name: "a", scope: !35, file: !1, line: 21, type: !13)
!41 = !DILocation(line: 21, column: 8, scope: !35)
!42 = !DILocation(line: 21, column: 12, scope: !35)
!43 = !DILocation(line: 23, column: 19, scope: !35)
!44 = !DILocation(line: 23, column: 8, scope: !35)
!45 = !DILocation(line: 24, column: 11, scope: !35)
!46 = !DILocation(line: 24, column: 4, scope: !35)
!47 = !DILocation(line: 25, column: 43, scope: !35)
!48 = !DILocation(line: 25, column: 48, scope: !35)
!49 = !DILocation(line: 25, column: 4, scope: !35)
!50 = !DILocation(line: 29, column: 8, scope: !51)
!51 = distinct !DILexicalBlock(scope: !35, file: !1, line: 29, column: 8)
!52 = !DILocation(line: 29, column: 10, scope: !51)
!53 = !DILocation(line: 29, column: 8, scope: !35)
!54 = !DILocation(line: 31, column: 5, scope: !55)
!55 = distinct !DILexicalBlock(scope: !51, file: !1, line: 29, column: 18)
!56 = !DILocation(line: 32, column: 14, scope: !57)
!57 = distinct !DILexicalBlock(scope: !51, file: !1, line: 32, column: 14)
!58 = !DILocation(line: 32, column: 16, scope: !57)
!59 = !DILocation(line: 32, column: 14, scope: !51)
!60 = !DILocation(line: 33, column: 13, scope: !61)
!61 = distinct !DILexicalBlock(scope: !62, file: !1, line: 33, column: 13)
!62 = distinct !DILexicalBlock(scope: !57, file: !1, line: 32, column: 24)
!63 = !DILocation(line: 33, column: 15, scope: !61)
!64 = !DILocation(line: 33, column: 13, scope: !62)
!65 = !DILocation(line: 35, column: 9, scope: !66)
!66 = distinct !DILexicalBlock(scope: !61, file: !1, line: 33, column: 22)
!67 = !DILocation(line: 36, column: 18, scope: !68)
!68 = distinct !DILexicalBlock(scope: !61, file: !1, line: 36, column: 18)
!69 = !DILocation(line: 36, column: 20, scope: !68)
!70 = !DILocation(line: 36, column: 18, scope: !61)
!71 = !DILocation(line: 38, column: 9, scope: !72)
!72 = distinct !DILexicalBlock(scope: !68, file: !1, line: 36, column: 29)
!73 = !DILocation(line: 40, column: 21, scope: !74)
!74 = distinct !DILexicalBlock(scope: !68, file: !1, line: 39, column: 14)
!75 = !DILocation(line: 40, column: 13, scope: !74)
!76 = !DILocation(line: 43, column: 5, scope: !62)
!77 = !DILocation(line: 45, column: 14, scope: !78)
!78 = distinct !DILexicalBlock(scope: !57, file: !1, line: 44, column: 10)
!79 = !DILocation(line: 45, column: 9, scope: !78)
!80 = !DILocation(line: 48, column: 43, scope: !35)
!81 = !DILocation(line: 48, column: 48, scope: !35)
!82 = !DILocation(line: 48, column: 4, scope: !35)
!83 = !DILocation(line: 55, column: 4, scope: !35)
