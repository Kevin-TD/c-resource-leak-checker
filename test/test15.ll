; ModuleID = '../test/test15.c'
source_filename = "../test/test15.c"
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
  %b = alloca i32, align 4
  %c = alloca i32, align 4
  %i7 = alloca i32, align 4
  %e = alloca i32, align 4
  %f = alloca i32, align 4
  %m = alloca i32, align 4
  %z = alloca i32, align 4
  %j = alloca i32, align 4
  %i16 = alloca i32, align 4
  %g = alloca i32, align 4
  %h = alloca i32, align 4
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

for.cond:                                         ; preds = %for.inc23, %entry
  %7 = load i32, i32* %i, align 4, !dbg !38
  %cmp = icmp slt i32 %7, 15, !dbg !40
  br i1 %cmp, label %for.body, label %for.end25, !dbg !41

for.body:                                         ; preds = %for.cond
  call void @llvm.dbg.declare(metadata i32* %b, metadata !42, metadata !DIExpression()), !dbg !44
  %8 = load i8*, i8** %str, align 8, !dbg !45
  call void @free(i8* %8) #4, !dbg !46
  call void @llvm.dbg.declare(metadata i32* %c, metadata !47, metadata !DIExpression()), !dbg !48
  call void @llvm.dbg.declare(metadata i32* %i7, metadata !49, metadata !DIExpression()), !dbg !51
  store i32 0, i32* %i7, align 4, !dbg !51
  br label %for.cond8, !dbg !52

for.cond8:                                        ; preds = %for.inc20, %for.body
  %9 = load i32, i32* %i7, align 4, !dbg !53
  %cmp9 = icmp slt i32 %9, 15, !dbg !55
  br i1 %cmp9, label %for.body10, label %for.end22, !dbg !56

for.body10:                                       ; preds = %for.cond8
  call void @llvm.dbg.declare(metadata i32* %e, metadata !57, metadata !DIExpression()), !dbg !59
  %10 = load i8*, i8** %str, align 8, !dbg !60
  call void @free(i8* %10) #4, !dbg !61
  call void @llvm.dbg.declare(metadata i32* %f, metadata !62, metadata !DIExpression()), !dbg !63
  %11 = load i32, i32* %a, align 4, !dbg !64
  %cmp11 = icmp eq i32 %11, -5, !dbg !66
  br i1 %cmp11, label %if.then, label %if.else, !dbg !67

if.then:                                          ; preds = %for.body10
  call void @llvm.dbg.declare(metadata i32* %m, metadata !68, metadata !DIExpression()), !dbg !70
  br label %if.end15, !dbg !71

if.else:                                          ; preds = %for.body10
  %12 = load i32, i32* %a, align 4, !dbg !72
  %cmp12 = icmp eq i32 %12, -20, !dbg !74
  br i1 %cmp12, label %if.then13, label %if.else14, !dbg !75

if.then13:                                        ; preds = %if.else
  call void @llvm.dbg.declare(metadata i32* %z, metadata !76, metadata !DIExpression()), !dbg !78
  br label %if.end, !dbg !79

if.else14:                                        ; preds = %if.else
  call void @llvm.dbg.declare(metadata i32* %j, metadata !80, metadata !DIExpression()), !dbg !82
  br label %if.end

if.end:                                           ; preds = %if.else14, %if.then13
  br label %if.end15

if.end15:                                         ; preds = %if.end, %if.then
  call void @llvm.dbg.declare(metadata i32* %i16, metadata !83, metadata !DIExpression()), !dbg !85
  store i32 0, i32* %i16, align 4, !dbg !85
  br label %for.cond17, !dbg !86

for.cond17:                                       ; preds = %for.inc, %if.end15
  %13 = load i32, i32* %i16, align 4, !dbg !87
  %cmp18 = icmp slt i32 %13, 15, !dbg !89
  br i1 %cmp18, label %for.body19, label %for.end, !dbg !90

for.body19:                                       ; preds = %for.cond17
  call void @llvm.dbg.declare(metadata i32* %g, metadata !91, metadata !DIExpression()), !dbg !93
  %14 = load i8*, i8** %str, align 8, !dbg !94
  call void @free(i8* %14) #4, !dbg !95
  call void @llvm.dbg.declare(metadata i32* %h, metadata !96, metadata !DIExpression()), !dbg !97
  br label %for.inc, !dbg !98

for.inc:                                          ; preds = %for.body19
  %15 = load i32, i32* %i16, align 4, !dbg !99
  %inc = add nsw i32 %15, 1, !dbg !99
  store i32 %inc, i32* %i16, align 4, !dbg !99
  br label %for.cond17, !dbg !100, !llvm.loop !101

for.end:                                          ; preds = %for.cond17
  br label %for.inc20, !dbg !103

for.inc20:                                        ; preds = %for.end
  %16 = load i32, i32* %i7, align 4, !dbg !104
  %inc21 = add nsw i32 %16, 1, !dbg !104
  store i32 %inc21, i32* %i7, align 4, !dbg !104
  br label %for.cond8, !dbg !105, !llvm.loop !106

for.end22:                                        ; preds = %for.cond8
  br label %for.inc23, !dbg !108

for.inc23:                                        ; preds = %for.end22
  %17 = load i32, i32* %i, align 4, !dbg !109
  %inc24 = add nsw i32 %17, 1, !dbg !109
  store i32 %inc24, i32* %i, align 4, !dbg !109
  br label %for.cond, !dbg !110, !llvm.loop !111

for.end25:                                        ; preds = %for.cond
  ret i32 0, !dbg !113
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
!1 = !DIFile(filename: "../test/test15.c", directory: "/c-resource-leak-checker/build")
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
!42 = !DILocalVariable(name: "b", scope: !43, file: !1, line: 19, type: !13)
!43 = distinct !DILexicalBlock(scope: !39, file: !1, line: 18, column: 33)
!44 = !DILocation(line: 19, column: 9, scope: !43)
!45 = !DILocation(line: 20, column: 10, scope: !43)
!46 = !DILocation(line: 20, column: 5, scope: !43)
!47 = !DILocalVariable(name: "c", scope: !43, file: !1, line: 21, type: !13)
!48 = !DILocation(line: 21, column: 9, scope: !43)
!49 = !DILocalVariable(name: "i", scope: !50, file: !1, line: 23, type: !13)
!50 = distinct !DILexicalBlock(scope: !43, file: !1, line: 23, column: 5)
!51 = !DILocation(line: 23, column: 14, scope: !50)
!52 = !DILocation(line: 23, column: 10, scope: !50)
!53 = !DILocation(line: 23, column: 21, scope: !54)
!54 = distinct !DILexicalBlock(scope: !50, file: !1, line: 23, column: 5)
!55 = !DILocation(line: 23, column: 23, scope: !54)
!56 = !DILocation(line: 23, column: 5, scope: !50)
!57 = !DILocalVariable(name: "e", scope: !58, file: !1, line: 24, type: !13)
!58 = distinct !DILexicalBlock(scope: !54, file: !1, line: 23, column: 34)
!59 = !DILocation(line: 24, column: 13, scope: !58)
!60 = !DILocation(line: 25, column: 14, scope: !58)
!61 = !DILocation(line: 25, column: 9, scope: !58)
!62 = !DILocalVariable(name: "f", scope: !58, file: !1, line: 26, type: !13)
!63 = !DILocation(line: 26, column: 13, scope: !58)
!64 = !DILocation(line: 28, column: 13, scope: !65)
!65 = distinct !DILexicalBlock(scope: !58, file: !1, line: 28, column: 13)
!66 = !DILocation(line: 28, column: 15, scope: !65)
!67 = !DILocation(line: 28, column: 13, scope: !58)
!68 = !DILocalVariable(name: "m", scope: !69, file: !1, line: 29, type: !13)
!69 = distinct !DILexicalBlock(scope: !65, file: !1, line: 28, column: 22)
!70 = !DILocation(line: 29, column: 17, scope: !69)
!71 = !DILocation(line: 30, column: 9, scope: !69)
!72 = !DILocation(line: 31, column: 18, scope: !73)
!73 = distinct !DILexicalBlock(scope: !65, file: !1, line: 31, column: 18)
!74 = !DILocation(line: 31, column: 20, scope: !73)
!75 = !DILocation(line: 31, column: 18, scope: !65)
!76 = !DILocalVariable(name: "z", scope: !77, file: !1, line: 32, type: !13)
!77 = distinct !DILexicalBlock(scope: !73, file: !1, line: 31, column: 28)
!78 = !DILocation(line: 32, column: 17, scope: !77)
!79 = !DILocation(line: 33, column: 9, scope: !77)
!80 = !DILocalVariable(name: "j", scope: !81, file: !1, line: 35, type: !13)
!81 = distinct !DILexicalBlock(scope: !73, file: !1, line: 34, column: 14)
!82 = !DILocation(line: 35, column: 17, scope: !81)
!83 = !DILocalVariable(name: "i", scope: !84, file: !1, line: 38, type: !13)
!84 = distinct !DILexicalBlock(scope: !58, file: !1, line: 38, column: 9)
!85 = !DILocation(line: 38, column: 18, scope: !84)
!86 = !DILocation(line: 38, column: 14, scope: !84)
!87 = !DILocation(line: 38, column: 25, scope: !88)
!88 = distinct !DILexicalBlock(scope: !84, file: !1, line: 38, column: 9)
!89 = !DILocation(line: 38, column: 27, scope: !88)
!90 = !DILocation(line: 38, column: 9, scope: !84)
!91 = !DILocalVariable(name: "g", scope: !92, file: !1, line: 39, type: !13)
!92 = distinct !DILexicalBlock(scope: !88, file: !1, line: 38, column: 38)
!93 = !DILocation(line: 39, column: 17, scope: !92)
!94 = !DILocation(line: 40, column: 18, scope: !92)
!95 = !DILocation(line: 40, column: 13, scope: !92)
!96 = !DILocalVariable(name: "h", scope: !92, file: !1, line: 41, type: !13)
!97 = !DILocation(line: 41, column: 17, scope: !92)
!98 = !DILocation(line: 42, column: 9, scope: !92)
!99 = !DILocation(line: 38, column: 34, scope: !88)
!100 = !DILocation(line: 38, column: 9, scope: !88)
!101 = distinct !{!101, !90, !102}
!102 = !DILocation(line: 42, column: 9, scope: !84)
!103 = !DILocation(line: 43, column: 5, scope: !58)
!104 = !DILocation(line: 23, column: 30, scope: !54)
!105 = !DILocation(line: 23, column: 5, scope: !54)
!106 = distinct !{!106, !56, !107}
!107 = !DILocation(line: 43, column: 5, scope: !50)
!108 = !DILocation(line: 44, column: 4, scope: !43)
!109 = !DILocation(line: 18, column: 29, scope: !39)
!110 = !DILocation(line: 18, column: 4, scope: !39)
!111 = distinct !{!111, !41, !112}
!112 = !DILocation(line: 44, column: 4, scope: !35)
!113 = !DILocation(line: 48, column: 4, scope: !10)
