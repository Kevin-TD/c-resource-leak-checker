; ModuleID = '../test/test6/test6.c'
source_filename = "../test/test6/test6.c"
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
  %str1 = alloca i8*, align 8
  %str2 = alloca i8*, align 8
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
  %7 = load i32, i32* %a, align 4, !dbg !34
  %cmp = icmp eq i32 %7, -15, !dbg !36
  br i1 %cmp, label %if.then, label %if.else, !dbg !37

if.then:                                          ; preds = %entry
  %8 = load i8*, i8** %str, align 8, !dbg !38
  call void @free(i8* %8) #4, !dbg !40
  br label %if.end18, !dbg !41

if.else:                                          ; preds = %entry
  %9 = load i32, i32* %a, align 4, !dbg !42
  %cmp7 = icmp eq i32 %9, -10, !dbg !44
  br i1 %cmp7, label %if.then8, label %if.else12, !dbg !45

if.then8:                                         ; preds = %if.else
  call void @llvm.dbg.declare(metadata i8** %str1, metadata !46, metadata !DIExpression()), !dbg !48
  %call9 = call noalias i8* @malloc(i64 15) #4, !dbg !49
  store i8* %call9, i8** %str1, align 8, !dbg !50
  %10 = load i32, i32* %a, align 4, !dbg !51
  %cmp10 = icmp eq i32 %10, -9, !dbg !53
  br i1 %cmp10, label %if.then11, label %if.end, !dbg !54

if.then11:                                        ; preds = %if.then8
  %11 = load i8*, i8** %str1, align 8, !dbg !55
  call void @free(i8* %11) #4, !dbg !57
  br label %if.end, !dbg !58

if.end:                                           ; preds = %if.then11, %if.then8
  %12 = load i8*, i8** %str, align 8, !dbg !59
  call void @free(i8* %12) #4, !dbg !60
  br label %if.end17, !dbg !61

if.else12:                                        ; preds = %if.else
  call void @llvm.dbg.declare(metadata i8** %str2, metadata !62, metadata !DIExpression()), !dbg !64
  %call13 = call noalias i8* @malloc(i64 15) #4, !dbg !65
  store i8* %call13, i8** %str2, align 8, !dbg !66
  %13 = load i32, i32* %a, align 4, !dbg !67
  %cmp14 = icmp eq i32 %13, -9, !dbg !69
  br i1 %cmp14, label %if.then15, label %if.end16, !dbg !70

if.then15:                                        ; preds = %if.else12
  %14 = load i8*, i8** %str2, align 8, !dbg !71
  call void @free(i8* %14) #4, !dbg !73
  br label %if.end16, !dbg !74

if.end16:                                         ; preds = %if.then15, %if.else12
  %15 = load i8*, i8** %str2, align 8, !dbg !75
  call void @free(i8* %15) #4, !dbg !76
  %16 = load i8*, i8** %str, align 8, !dbg !77
  call void @free(i8* %16) #4, !dbg !78
  br label %if.end17

if.end17:                                         ; preds = %if.end16, %if.end
  br label %if.end18

if.end18:                                         ; preds = %if.end17, %if.then
  ret i32 0, !dbg !79
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
!1 = !DIFile(filename: "../test/test6/test6.c", directory: "/c-resource-leak-checker/build")
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
!16 = !DILocalVariable(name: "a", scope: !10, file: !1, line: 7, type: !13)
!17 = !DILocation(line: 7, column: 8, scope: !10)
!18 = !DILocation(line: 7, column: 12, scope: !10)
!19 = !DILocation(line: 10, column: 19, scope: !10)
!20 = !DILocation(line: 10, column: 8, scope: !10)
!21 = !DILocation(line: 11, column: 11, scope: !10)
!22 = !DILocation(line: 11, column: 4, scope: !10)
!23 = !DILocation(line: 12, column: 43, scope: !10)
!24 = !DILocation(line: 12, column: 48, scope: !10)
!25 = !DILocation(line: 12, column: 4, scope: !10)
!26 = !DILocation(line: 15, column: 27, scope: !10)
!27 = !DILocation(line: 15, column: 19, scope: !10)
!28 = !DILocation(line: 15, column: 8, scope: !10)
!29 = !DILocation(line: 16, column: 11, scope: !10)
!30 = !DILocation(line: 16, column: 4, scope: !10)
!31 = !DILocation(line: 17, column: 43, scope: !10)
!32 = !DILocation(line: 17, column: 48, scope: !10)
!33 = !DILocation(line: 17, column: 4, scope: !10)
!34 = !DILocation(line: 19, column: 9, scope: !35)
!35 = distinct !DILexicalBlock(scope: !10, file: !1, line: 19, column: 9)
!36 = !DILocation(line: 19, column: 11, scope: !35)
!37 = !DILocation(line: 19, column: 9, scope: !10)
!38 = !DILocation(line: 20, column: 15, scope: !39)
!39 = distinct !DILexicalBlock(scope: !35, file: !1, line: 19, column: 19)
!40 = !DILocation(line: 20, column: 10, scope: !39)
!41 = !DILocation(line: 21, column: 5, scope: !39)
!42 = !DILocation(line: 22, column: 14, scope: !43)
!43 = distinct !DILexicalBlock(scope: !35, file: !1, line: 22, column: 14)
!44 = !DILocation(line: 22, column: 16, scope: !43)
!45 = !DILocation(line: 22, column: 14, scope: !35)
!46 = !DILocalVariable(name: "str1", scope: !47, file: !1, line: 23, type: !4)
!47 = distinct !DILexicalBlock(scope: !43, file: !1, line: 22, column: 24)
!48 = !DILocation(line: 23, column: 15, scope: !47)
!49 = !DILocation(line: 24, column: 25, scope: !47)
!50 = !DILocation(line: 24, column: 14, scope: !47)
!51 = !DILocation(line: 26, column: 13, scope: !52)
!52 = distinct !DILexicalBlock(scope: !47, file: !1, line: 26, column: 13)
!53 = !DILocation(line: 26, column: 15, scope: !52)
!54 = !DILocation(line: 26, column: 13, scope: !47)
!55 = !DILocation(line: 27, column: 18, scope: !56)
!56 = distinct !DILexicalBlock(scope: !52, file: !1, line: 26, column: 22)
!57 = !DILocation(line: 27, column: 13, scope: !56)
!58 = !DILocation(line: 28, column: 9, scope: !56)
!59 = !DILocation(line: 30, column: 15, scope: !47)
!60 = !DILocation(line: 30, column: 10, scope: !47)
!61 = !DILocation(line: 32, column: 5, scope: !47)
!62 = !DILocalVariable(name: "str2", scope: !63, file: !1, line: 34, type: !4)
!63 = distinct !DILexicalBlock(scope: !43, file: !1, line: 33, column: 10)
!64 = !DILocation(line: 34, column: 13, scope: !63)
!65 = !DILocation(line: 35, column: 25, scope: !63)
!66 = !DILocation(line: 35, column: 14, scope: !63)
!67 = !DILocation(line: 37, column: 13, scope: !68)
!68 = distinct !DILexicalBlock(scope: !63, file: !1, line: 37, column: 13)
!69 = !DILocation(line: 37, column: 15, scope: !68)
!70 = !DILocation(line: 37, column: 13, scope: !63)
!71 = !DILocation(line: 38, column: 18, scope: !72)
!72 = distinct !DILexicalBlock(scope: !68, file: !1, line: 37, column: 22)
!73 = !DILocation(line: 38, column: 13, scope: !72)
!74 = !DILocation(line: 39, column: 9, scope: !72)
!75 = !DILocation(line: 41, column: 14, scope: !63)
!76 = !DILocation(line: 41, column: 9, scope: !63)
!77 = !DILocation(line: 42, column: 14, scope: !63)
!78 = !DILocation(line: 42, column: 9, scope: !63)
!79 = !DILocation(line: 46, column: 4, scope: !10)
