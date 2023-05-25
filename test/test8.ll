; ModuleID = '../test/test8.c'
source_filename = "../test/test8.c"
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

@.str = private unnamed_addr constant [11 x i8] c"helloworld\00", align 1
@.str.1 = private unnamed_addr constant [28 x i8] c"String = %s,  Address = %u\0A\00", align 1
@.str.2 = private unnamed_addr constant [6 x i8] c"hello\00", align 1

; Function Attrs: noinline nounwind uwtable
define dso_local i8* @trivially_malloc(i64 %__size) #0 !dbg !10 {
entry:
  %retval = alloca i8*, align 8
  %__size.addr = alloca i64, align 8
  store i64 %__size, i64* %__size.addr, align 8
  call void @llvm.dbg.declare(metadata i64* %__size.addr, metadata !17, metadata !DIExpression()), !dbg !18
  %0 = load i64, i64* %__size.addr, align 8, !dbg !19
  %call = call noalias i8* @malloc(i64 %0) #4, !dbg !20
  %1 = load i8*, i8** %retval, align 8, !dbg !21
  ret i8* %1, !dbg !21
}

; Function Attrs: nounwind readnone speculatable
declare void @llvm.dbg.declare(metadata, metadata, metadata) #1

; Function Attrs: nounwind
declare dso_local noalias i8* @malloc(i64) #2

; Function Attrs: noinline nounwind uwtable
define dso_local i32 @main() #0 !dbg !22 {
entry:
  %retval = alloca i32, align 4
  %str = alloca i8*, align 8
  %a = alloca i32, align 4
  %str1 = alloca i8*, align 8
  store i32 0, i32* %retval, align 4
  call void @llvm.dbg.declare(metadata i8** %str, metadata !26, metadata !DIExpression()), !dbg !27
  call void @llvm.dbg.declare(metadata i32* %a, metadata !28, metadata !DIExpression()), !dbg !29
  %call = call i32 @getchar(), !dbg !30
  store i32 %call, i32* %a, align 4, !dbg !29
  %call1 = call noalias i8* @malloc(i64 15) #4, !dbg !31
  store i8* %call1, i8** %str, align 8, !dbg !32
  %0 = load i8*, i8** %str, align 8, !dbg !33
  %call2 = call i8* @strcpy(i8* %0, i8* getelementptr inbounds ([11 x i8], [11 x i8]* @.str, i32 0, i32 0)) #4, !dbg !34
  %1 = load i8*, i8** %str, align 8, !dbg !35
  %2 = load i8*, i8** %str, align 8, !dbg !36
  %call3 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([28 x i8], [28 x i8]* @.str.1, i32 0, i32 0), i8* %1, i8* %2), !dbg !37
  %3 = load i8*, i8** %str, align 8, !dbg !38
  %call4 = call i8* @realloc(i8* %3, i64 25) #4, !dbg !39
  store i8* %call4, i8** %str, align 8, !dbg !40
  %4 = load i8*, i8** %str, align 8, !dbg !41
  %call5 = call i8* @strcat(i8* %4, i8* getelementptr inbounds ([6 x i8], [6 x i8]* @.str.2, i32 0, i32 0)) #4, !dbg !42
  %5 = load i8*, i8** %str, align 8, !dbg !43
  %6 = load i8*, i8** %str, align 8, !dbg !44
  %call6 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([28 x i8], [28 x i8]* @.str.1, i32 0, i32 0), i8* %5, i8* %6), !dbg !45
  %7 = load i32, i32* %a, align 4, !dbg !46
  %cmp = icmp eq i32 %7, -15, !dbg !48
  br i1 %cmp, label %if.then, label %if.else, !dbg !49

if.then:                                          ; preds = %entry
  br label %if.end15, !dbg !50

if.else:                                          ; preds = %entry
  %8 = load i32, i32* %a, align 4, !dbg !52
  %cmp7 = icmp eq i32 %8, -10, !dbg !54
  br i1 %cmp7, label %if.then8, label %if.else13, !dbg !55

if.then8:                                         ; preds = %if.else
  call void @llvm.dbg.declare(metadata i8** %str1, metadata !56, metadata !DIExpression()), !dbg !58
  %call9 = call noalias i8* @malloc(i64 15) #4, !dbg !59
  store i8* %call9, i8** %str1, align 8, !dbg !60
  %9 = load i32, i32* %a, align 4, !dbg !61
  %cmp10 = icmp eq i32 %9, -9, !dbg !63
  br i1 %cmp10, label %if.then11, label %if.else12, !dbg !64

if.then11:                                        ; preds = %if.then8
  %10 = load i8*, i8** %str1, align 8, !dbg !65
  call void @free(i8* %10) #4, !dbg !67
  br label %if.end, !dbg !68

if.else12:                                        ; preds = %if.then8
  %11 = load i8*, i8** %str1, align 8, !dbg !69
  call void @free(i8* %11) #4, !dbg !71
  br label %if.end

if.end:                                           ; preds = %if.else12, %if.then11
  br label %if.end14, !dbg !72

if.else13:                                        ; preds = %if.else
  %12 = load i8*, i8** %str, align 8, !dbg !73
  call void @free(i8* %12) #4, !dbg !75
  br label %if.end14

if.end14:                                         ; preds = %if.else13, %if.end
  br label %if.end15

if.end15:                                         ; preds = %if.end14, %if.then
  ret i32 0, !dbg !76
}

declare dso_local i32 @getchar() #3

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

!llvm.dbg.cu = !{!0}
!llvm.module.flags = !{!6, !7, !8}
!llvm.ident = !{!9}

!0 = distinct !DICompileUnit(language: DW_LANG_C99, file: !1, producer: "clang version 8.0.1- (branches/release_80)", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, enums: !2, retainedTypes: !3, nameTableKind: None)
!1 = !DIFile(filename: "../test/test8.c", directory: "/C-resource-leak-checker/build")
!2 = !{}
!3 = !{!4}
!4 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !5, size: 64)
!5 = !DIBasicType(name: "char", size: 8, encoding: DW_ATE_signed_char)
!6 = !{i32 2, !"Dwarf Version", i32 4}
!7 = !{i32 2, !"Debug Info Version", i32 3}
!8 = !{i32 1, !"wchar_size", i32 4}
!9 = !{!"clang version 8.0.1- (branches/release_80)"}
!10 = distinct !DISubprogram(name: "trivially_malloc", scope: !1, file: !1, line: 8, type: !11, scopeLine: 8, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition, unit: !0, retainedNodes: !2)
!11 = !DISubroutineType(types: !12)
!12 = !{!13, !14}
!13 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: null, size: 64)
!14 = !DIDerivedType(tag: DW_TAG_typedef, name: "size_t", file: !15, line: 62, baseType: !16)
!15 = !DIFile(filename: "/usr/lib/llvm-8/lib/clang/8.0.1/include/stddef.h", directory: "")
!16 = !DIBasicType(name: "long unsigned int", size: 64, encoding: DW_ATE_unsigned)
!17 = !DILocalVariable(name: "__size", arg: 1, scope: !10, file: !1, line: 8, type: !14)
!18 = !DILocation(line: 8, column: 31, scope: !10)
!19 = !DILocation(line: 9, column: 12, scope: !10)
!20 = !DILocation(line: 9, column: 5, scope: !10)
!21 = !DILocation(line: 10, column: 1, scope: !10)
!22 = distinct !DISubprogram(name: "main", scope: !1, file: !1, line: 12, type: !23, scopeLine: 12, spFlags: DISPFlagDefinition, unit: !0, retainedNodes: !2)
!23 = !DISubroutineType(types: !24)
!24 = !{!25}
!25 = !DIBasicType(name: "int", size: 32, encoding: DW_ATE_signed)
!26 = !DILocalVariable(name: "str", scope: !22, file: !1, line: 13, type: !4)
!27 = !DILocation(line: 13, column: 10, scope: !22)
!28 = !DILocalVariable(name: "a", scope: !22, file: !1, line: 14, type: !25)
!29 = !DILocation(line: 14, column: 8, scope: !22)
!30 = !DILocation(line: 14, column: 12, scope: !22)
!31 = !DILocation(line: 17, column: 19, scope: !22)
!32 = !DILocation(line: 17, column: 8, scope: !22)
!33 = !DILocation(line: 18, column: 11, scope: !22)
!34 = !DILocation(line: 18, column: 4, scope: !22)
!35 = !DILocation(line: 19, column: 43, scope: !22)
!36 = !DILocation(line: 19, column: 48, scope: !22)
!37 = !DILocation(line: 19, column: 4, scope: !22)
!38 = !DILocation(line: 22, column: 27, scope: !22)
!39 = !DILocation(line: 22, column: 19, scope: !22)
!40 = !DILocation(line: 22, column: 8, scope: !22)
!41 = !DILocation(line: 23, column: 11, scope: !22)
!42 = !DILocation(line: 23, column: 4, scope: !22)
!43 = !DILocation(line: 24, column: 43, scope: !22)
!44 = !DILocation(line: 24, column: 48, scope: !22)
!45 = !DILocation(line: 24, column: 4, scope: !22)
!46 = !DILocation(line: 27, column: 9, scope: !47)
!47 = distinct !DILexicalBlock(scope: !22, file: !1, line: 27, column: 9)
!48 = !DILocation(line: 27, column: 11, scope: !47)
!49 = !DILocation(line: 27, column: 9, scope: !22)
!50 = !DILocation(line: 29, column: 5, scope: !51)
!51 = distinct !DILexicalBlock(scope: !47, file: !1, line: 27, column: 19)
!52 = !DILocation(line: 30, column: 14, scope: !53)
!53 = distinct !DILexicalBlock(scope: !47, file: !1, line: 30, column: 14)
!54 = !DILocation(line: 30, column: 16, scope: !53)
!55 = !DILocation(line: 30, column: 14, scope: !47)
!56 = !DILocalVariable(name: "str1", scope: !57, file: !1, line: 31, type: !4)
!57 = distinct !DILexicalBlock(scope: !53, file: !1, line: 30, column: 24)
!58 = !DILocation(line: 31, column: 15, scope: !57)
!59 = !DILocation(line: 32, column: 25, scope: !57)
!60 = !DILocation(line: 32, column: 14, scope: !57)
!61 = !DILocation(line: 34, column: 13, scope: !62)
!62 = distinct !DILexicalBlock(scope: !57, file: !1, line: 34, column: 13)
!63 = !DILocation(line: 34, column: 15, scope: !62)
!64 = !DILocation(line: 34, column: 13, scope: !57)
!65 = !DILocation(line: 35, column: 18, scope: !66)
!66 = distinct !DILexicalBlock(scope: !62, file: !1, line: 34, column: 22)
!67 = !DILocation(line: 35, column: 13, scope: !66)
!68 = !DILocation(line: 36, column: 9, scope: !66)
!69 = !DILocation(line: 37, column: 18, scope: !70)
!70 = distinct !DILexicalBlock(scope: !62, file: !1, line: 36, column: 16)
!71 = !DILocation(line: 37, column: 13, scope: !70)
!72 = !DILocation(line: 40, column: 5, scope: !57)
!73 = !DILocation(line: 42, column: 14, scope: !74)
!74 = distinct !DILexicalBlock(scope: !53, file: !1, line: 41, column: 10)
!75 = !DILocation(line: 42, column: 9, scope: !74)
!76 = !DILocation(line: 47, column: 4, scope: !22)
