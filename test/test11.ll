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
  br i1 %cmp, label %if.then, label %if.else24, !dbg !37

if.then:                                          ; preds = %entry
  %8 = load i8*, i8** %str, align 8, !dbg !38
  call void @free(i8* %8) #4, !dbg !40
  %9 = load i32, i32* %a, align 4, !dbg !41
  %cmp7 = icmp eq i32 %9, -10, !dbg !43
  br i1 %cmp7, label %if.then8, label %if.else, !dbg !44

if.then8:                                         ; preds = %if.then
  %10 = load i8*, i8** %str, align 8, !dbg !45
  call void @free(i8* %10) #4, !dbg !47
  br label %if.end19, !dbg !48

if.else:                                          ; preds = %if.then
  %11 = load i32, i32* %a, align 4, !dbg !49
  %cmp9 = icmp eq i32 %11, -9, !dbg !51
  br i1 %cmp9, label %if.then10, label %if.else12, !dbg !52

if.then10:                                        ; preds = %if.else
  %12 = load i8*, i8** %str, align 8, !dbg !53
  %call11 = call i8* @realloc(i8* %12, i64 15) #4, !dbg !55
  br label %if.end18, !dbg !56

if.else12:                                        ; preds = %if.else
  %13 = load i32, i32* %a, align 4, !dbg !57
  %cmp13 = icmp eq i32 %13, -10, !dbg !59
  br i1 %cmp13, label %if.then14, label %if.else16, !dbg !60

if.then14:                                        ; preds = %if.else12
  %14 = load i8*, i8** %str, align 8, !dbg !61
  %call15 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @.str.3, i32 0, i32 0), i8* %14), !dbg !63
  br label %if.end, !dbg !64

if.else16:                                        ; preds = %if.else12
  %15 = load i8*, i8** %str, align 8, !dbg !65
  %call17 = call i8* @realloc(i8* %15, i64 15) #4, !dbg !67
  br label %if.end

if.end:                                           ; preds = %if.else16, %if.then14
  br label %if.end18

if.end18:                                         ; preds = %if.end, %if.then10
  br label %if.end19

if.end19:                                         ; preds = %if.end18, %if.then8
  %16 = load i8*, i8** %str, align 8, !dbg !68
  call void @free(i8* %16) #4, !dbg !69
  %17 = load i32, i32* %a, align 4, !dbg !70
  %cmp20 = icmp eq i32 %17, -70, !dbg !72
  br i1 %cmp20, label %if.then21, label %if.end23, !dbg !73

if.then21:                                        ; preds = %if.end19
  %18 = load i8*, i8** %str, align 8, !dbg !74
  %call22 = call i8* @realloc(i8* %18, i64 15) #4, !dbg !76
  br label %if.end23, !dbg !77

if.end23:                                         ; preds = %if.then21, %if.end19
  %19 = load i8*, i8** %str, align 8, !dbg !78
  call void @free(i8* %19) #4, !dbg !79
  br label %if.end30, !dbg !80

if.else24:                                        ; preds = %entry
  %20 = load i32, i32* %a, align 4, !dbg !81
  %cmp25 = icmp eq i32 %20, -20, !dbg !83
  br i1 %cmp25, label %if.then26, label %if.else28, !dbg !84

if.then26:                                        ; preds = %if.else24
  %21 = load i8*, i8** %str, align 8, !dbg !85
  %call27 = call i8* @realloc(i8* %21, i64 25) #4, !dbg !87
  br label %if.end29, !dbg !88

if.else28:                                        ; preds = %if.else24
  %22 = load i8*, i8** %str, align 8, !dbg !89
  call void @free(i8* %22) #4, !dbg !91
  br label %if.end29

if.end29:                                         ; preds = %if.else28, %if.then26
  br label %if.end30

if.end30:                                         ; preds = %if.end29, %if.end23
  ret i32 0, !dbg !92
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
!1 = !DIFile(filename: "../test/test11.c", directory: "/C-resource-leak-checker/build")
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
!64 = !DILocation(line: 30, column: 10, scope: !62)
!65 = !DILocation(line: 31, column: 21, scope: !66)
!66 = distinct !DILexicalBlock(scope: !58, file: !1, line: 30, column: 17)
!67 = !DILocation(line: 31, column: 13, scope: !66)
!68 = !DILocation(line: 37, column: 15, scope: !39)
!69 = !DILocation(line: 37, column: 10, scope: !39)
!70 = !DILocation(line: 39, column: 14, scope: !71)
!71 = distinct !DILexicalBlock(scope: !39, file: !1, line: 39, column: 14)
!72 = !DILocation(line: 39, column: 16, scope: !71)
!73 = !DILocation(line: 39, column: 14, scope: !39)
!74 = !DILocation(line: 40, column: 21, scope: !75)
!75 = distinct !DILexicalBlock(scope: !71, file: !1, line: 39, column: 24)
!76 = !DILocation(line: 40, column: 13, scope: !75)
!77 = !DILocation(line: 41, column: 10, scope: !75)
!78 = !DILocation(line: 43, column: 14, scope: !39)
!79 = !DILocation(line: 43, column: 9, scope: !39)
!80 = !DILocation(line: 48, column: 5, scope: !39)
!81 = !DILocation(line: 49, column: 14, scope: !82)
!82 = distinct !DILexicalBlock(scope: !35, file: !1, line: 49, column: 14)
!83 = !DILocation(line: 49, column: 16, scope: !82)
!84 = !DILocation(line: 49, column: 14, scope: !35)
!85 = !DILocation(line: 50, column: 17, scope: !86)
!86 = distinct !DILexicalBlock(scope: !82, file: !1, line: 49, column: 24)
!87 = !DILocation(line: 50, column: 9, scope: !86)
!88 = !DILocation(line: 51, column: 5, scope: !86)
!89 = !DILocation(line: 53, column: 14, scope: !90)
!90 = distinct !DILexicalBlock(scope: !82, file: !1, line: 52, column: 10)
!91 = !DILocation(line: 53, column: 9, scope: !90)
!92 = !DILocation(line: 58, column: 4, scope: !10)
