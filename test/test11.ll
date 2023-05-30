; ModuleID = '../test/test11.c'
source_filename = "../test/test11.c"
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

@.str = private unnamed_addr constant [11 x i8] c"helloworld\00", align 1
@.str.1 = private unnamed_addr constant [28 x i8] c"String = %s,  Address = %u\0A\00", align 1
@.str.2 = private unnamed_addr constant [6 x i8] c"hello\00", align 1
@.str.3 = private unnamed_addr constant [3 x i8] c"%s\00", align 1

; Function Attrs: noinline nounwind uwtable
define dso_local i32 @main() #0 !dbg !10 {
entry:
  %retval = alloca i32, align 4
  %str = alloca i8*, align 8
  %a = alloca i32, align 4
  %p = alloca i8*, align 8
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
  br i1 %cmp, label %if.then, label %if.else25, !dbg !37

if.then:                                          ; preds = %entry
  %8 = load i8*, i8** %str, align 8, !dbg !38
  call void @free(i8* %8) #4, !dbg !40
  %9 = load i32, i32* %a, align 4, !dbg !41
  %cmp7 = icmp eq i32 %9, -10, !dbg !43
  br i1 %cmp7, label %if.then8, label %if.else, !dbg !44

if.then8:                                         ; preds = %if.then
  %10 = load i8*, i8** %str, align 8, !dbg !45
  call void @free(i8* %10) #4, !dbg !47
  br label %if.end20, !dbg !48

if.else:                                          ; preds = %if.then
  %11 = load i32, i32* %a, align 4, !dbg !49
  %cmp9 = icmp eq i32 %11, -9, !dbg !51
  br i1 %cmp9, label %if.then10, label %if.else12, !dbg !52

if.then10:                                        ; preds = %if.else
  %12 = load i8*, i8** %str, align 8, !dbg !53
  %call11 = call i8* @realloc(i8* %12, i64 15) #4, !dbg !55
  br label %if.end19, !dbg !56

if.else12:                                        ; preds = %if.else
  %13 = load i32, i32* %a, align 4, !dbg !57
  %cmp13 = icmp eq i32 %13, -10, !dbg !59
  br i1 %cmp13, label %if.then14, label %if.else17, !dbg !60

if.then14:                                        ; preds = %if.else12
  %14 = load i8*, i8** %str, align 8, !dbg !61
  %call15 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @.str.3, i32 0, i32 0), i8* %14), !dbg !63
  call void @llvm.dbg.declare(metadata i8** %p, metadata !64, metadata !DIExpression()), !dbg !65
  %call16 = call noalias i8* @malloc(i64 15) #4, !dbg !66
  store i8* %call16, i8** %p, align 8, !dbg !67
  %15 = load i8*, i8** %p, align 8, !dbg !68
  call void @free(i8* %15) #4, !dbg !69
  br label %if.end, !dbg !70

if.else17:                                        ; preds = %if.else12
  %16 = load i8*, i8** %str, align 8, !dbg !71
  %call18 = call i8* @realloc(i8* %16, i64 15) #4, !dbg !73
  br label %if.end

if.end:                                           ; preds = %if.else17, %if.then14
  br label %if.end19

if.end19:                                         ; preds = %if.end, %if.then10
  br label %if.end20

if.end20:                                         ; preds = %if.end19, %if.then8
  %17 = load i8*, i8** %str, align 8, !dbg !74
  call void @free(i8* %17) #4, !dbg !75
  %18 = load i32, i32* %a, align 4, !dbg !76
  %cmp21 = icmp eq i32 %18, -70, !dbg !78
  br i1 %cmp21, label %if.then22, label %if.end24, !dbg !79

if.then22:                                        ; preds = %if.end20
  %19 = load i8*, i8** %str, align 8, !dbg !80
  %call23 = call i8* @realloc(i8* %19, i64 15) #4, !dbg !82
  br label %if.end24, !dbg !83

if.end24:                                         ; preds = %if.then22, %if.end20
  %20 = load i8*, i8** %str, align 8, !dbg !84
  call void @free(i8* %20) #4, !dbg !85
  br label %if.end31, !dbg !86

if.else25:                                        ; preds = %entry
  %21 = load i32, i32* %a, align 4, !dbg !87
  %cmp26 = icmp eq i32 %21, -20, !dbg !89
  br i1 %cmp26, label %if.then27, label %if.else29, !dbg !90

if.then27:                                        ; preds = %if.else25
  %22 = load i8*, i8** %str, align 8, !dbg !91
  %call28 = call i8* @realloc(i8* %22, i64 25) #4, !dbg !93
  br label %if.end30, !dbg !94

if.else29:                                        ; preds = %if.else25
  %23 = load i8*, i8** %str, align 8, !dbg !95
  call void @free(i8* %23) #4, !dbg !97
  br label %if.end30

if.end30:                                         ; preds = %if.else29, %if.then27
  br label %if.end31

if.end31:                                         ; preds = %if.end30, %if.end24
  ret i32 0, !dbg !98
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
!1 = !DIFile(filename: "../test/test11.c", directory: "/c-resource-leak-checker/build")
!2 = !{}
!3 = !{!4}
!4 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !5, size: 64)
!5 = !DIBasicType(name: "char", size: 8, encoding: DW_ATE_signed_char)
!6 = !{i32 2, !"Dwarf Version", i32 4}
!7 = !{i32 2, !"Debug Info Version", i32 3}
!8 = !{i32 1, !"wchar_size", i32 4}
!9 = !{!"clang version 8.0.1- (branches/release_80)"}
!10 = distinct !DISubprogram(name: "main", scope: !1, file: !1, line: 7, type: !11, scopeLine: 7, spFlags: DISPFlagDefinition, unit: !0, retainedNodes: !2)
!11 = !DISubroutineType(types: !12)
!12 = !{!13}
!13 = !DIBasicType(name: "int", size: 32, encoding: DW_ATE_signed)
!14 = !DILocalVariable(name: "str", scope: !10, file: !1, line: 8, type: !4)
!15 = !DILocation(line: 8, column: 10, scope: !10)
!16 = !DILocalVariable(name: "a", scope: !10, file: !1, line: 9, type: !13)
!17 = !DILocation(line: 9, column: 8, scope: !10)
!18 = !DILocation(line: 9, column: 12, scope: !10)
!19 = !DILocation(line: 11, column: 19, scope: !10)
!20 = !DILocation(line: 11, column: 8, scope: !10)
!21 = !DILocation(line: 12, column: 11, scope: !10)
!22 = !DILocation(line: 12, column: 4, scope: !10)
!23 = !DILocation(line: 13, column: 43, scope: !10)
!24 = !DILocation(line: 13, column: 48, scope: !10)
!25 = !DILocation(line: 13, column: 4, scope: !10)
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
!41 = !DILocation(line: 21, column: 14, scope: !42)
!42 = distinct !DILexicalBlock(scope: !39, file: !1, line: 21, column: 14)
!43 = !DILocation(line: 21, column: 16, scope: !42)
!44 = !DILocation(line: 21, column: 14, scope: !39)
!45 = !DILocation(line: 22, column: 18, scope: !46)
!46 = distinct !DILexicalBlock(scope: !42, file: !1, line: 21, column: 24)
!47 = !DILocation(line: 22, column: 13, scope: !46)
!48 = !DILocation(line: 23, column: 10, scope: !46)
!49 = !DILocation(line: 24, column: 19, scope: !50)
!50 = distinct !DILexicalBlock(scope: !42, file: !1, line: 24, column: 19)
!51 = !DILocation(line: 24, column: 21, scope: !50)
!52 = !DILocation(line: 24, column: 19, scope: !42)
!53 = !DILocation(line: 25, column: 21, scope: !54)
!54 = distinct !DILexicalBlock(scope: !50, file: !1, line: 24, column: 28)
!55 = !DILocation(line: 25, column: 13, scope: !54)
!56 = !DILocation(line: 26, column: 10, scope: !54)
!57 = !DILocation(line: 27, column: 19, scope: !58)
!58 = distinct !DILexicalBlock(scope: !50, file: !1, line: 27, column: 19)
!59 = !DILocation(line: 27, column: 21, scope: !58)
!60 = !DILocation(line: 27, column: 19, scope: !50)
!61 = !DILocation(line: 28, column: 26, scope: !62)
!62 = distinct !DILexicalBlock(scope: !58, file: !1, line: 27, column: 29)
!63 = !DILocation(line: 28, column: 13, scope: !62)
!64 = !DILocalVariable(name: "p", scope: !62, file: !1, line: 29, type: !4)
!65 = !DILocation(line: 29, column: 19, scope: !62)
!66 = !DILocation(line: 30, column: 24, scope: !62)
!67 = !DILocation(line: 30, column: 15, scope: !62)
!68 = !DILocation(line: 31, column: 18, scope: !62)
!69 = !DILocation(line: 31, column: 13, scope: !62)
!70 = !DILocation(line: 33, column: 10, scope: !62)
!71 = !DILocation(line: 34, column: 21, scope: !72)
!72 = distinct !DILexicalBlock(scope: !58, file: !1, line: 33, column: 17)
!73 = !DILocation(line: 34, column: 13, scope: !72)
!74 = !DILocation(line: 40, column: 15, scope: !39)
!75 = !DILocation(line: 40, column: 10, scope: !39)
!76 = !DILocation(line: 42, column: 14, scope: !77)
!77 = distinct !DILexicalBlock(scope: !39, file: !1, line: 42, column: 14)
!78 = !DILocation(line: 42, column: 16, scope: !77)
!79 = !DILocation(line: 42, column: 14, scope: !39)
!80 = !DILocation(line: 43, column: 21, scope: !81)
!81 = distinct !DILexicalBlock(scope: !77, file: !1, line: 42, column: 24)
!82 = !DILocation(line: 43, column: 13, scope: !81)
!83 = !DILocation(line: 44, column: 10, scope: !81)
!84 = !DILocation(line: 46, column: 14, scope: !39)
!85 = !DILocation(line: 46, column: 9, scope: !39)
!86 = !DILocation(line: 51, column: 5, scope: !39)
!87 = !DILocation(line: 52, column: 14, scope: !88)
!88 = distinct !DILexicalBlock(scope: !35, file: !1, line: 52, column: 14)
!89 = !DILocation(line: 52, column: 16, scope: !88)
!90 = !DILocation(line: 52, column: 14, scope: !35)
!91 = !DILocation(line: 53, column: 17, scope: !92)
!92 = distinct !DILexicalBlock(scope: !88, file: !1, line: 52, column: 24)
!93 = !DILocation(line: 53, column: 9, scope: !92)
!94 = !DILocation(line: 54, column: 5, scope: !92)
!95 = !DILocation(line: 56, column: 14, scope: !96)
!96 = distinct !DILexicalBlock(scope: !88, file: !1, line: 55, column: 10)
!97 = !DILocation(line: 56, column: 9, scope: !96)
!98 = !DILocation(line: 61, column: 4, scope: !10)
