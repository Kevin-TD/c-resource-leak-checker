; ModuleID = '../test/test18.c'
source_filename = "../test/test18.c"
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

@.str = private unnamed_addr constant [11 x i8] c"helloworld\00", align 1
@.str.1 = private unnamed_addr constant [28 x i8] c"String = %s,  Address = %u\0A\00", align 1
@.str.2 = private unnamed_addr constant [6 x i8] c"hello\00", align 1

; Function Attrs: noinline nounwind uwtable
define dso_local i8* @malloc0(i64 %__size) #0 !dbg !10 {
entry:
  %__size.addr = alloca i64, align 8
  store i64 %__size, i64* %__size.addr, align 8
  call void @llvm.dbg.declare(metadata i64* %__size.addr, metadata !17, metadata !DIExpression()), !dbg !18
  %0 = load i64, i64* %__size.addr, align 8, !dbg !19
  %call = call noalias i8* @malloc(i64 %0) #4, !dbg !20
  ret i8* %call, !dbg !21
}

; Function Attrs: nounwind readnone speculatable
declare void @llvm.dbg.declare(metadata, metadata, metadata) #1

; Function Attrs: nounwind
declare dso_local noalias i8* @malloc(i64) #2

; Function Attrs: noinline nounwind uwtable
define dso_local i8* @malloc1(i64 %__size) #0 !dbg !22 {
entry:
  %__size.addr = alloca i64, align 8
  store i64 %__size, i64* %__size.addr, align 8
  call void @llvm.dbg.declare(metadata i64* %__size.addr, metadata !23, metadata !DIExpression()), !dbg !24
  %0 = load i64, i64* %__size.addr, align 8, !dbg !25
  %call = call noalias i8* @malloc(i64 %0) #4, !dbg !26
  ret i8* %call, !dbg !27
}

; Function Attrs: noinline nounwind uwtable
define dso_local i8* @malloc2(i64 %__size) #0 !dbg !28 {
entry:
  %__size.addr = alloca i64, align 8
  store i64 %__size, i64* %__size.addr, align 8
  call void @llvm.dbg.declare(metadata i64* %__size.addr, metadata !29, metadata !DIExpression()), !dbg !30
  %0 = load i64, i64* %__size.addr, align 8, !dbg !31
  %call = call noalias i8* @malloc(i64 %0) #4, !dbg !32
  ret i8* %call, !dbg !33
}

; Function Attrs: noinline nounwind uwtable
define dso_local i8* @malloc3(i64 %__size) #0 !dbg !34 {
entry:
  %__size.addr = alloca i64, align 8
  store i64 %__size, i64* %__size.addr, align 8
  call void @llvm.dbg.declare(metadata i64* %__size.addr, metadata !35, metadata !DIExpression()), !dbg !36
  %0 = load i64, i64* %__size.addr, align 8, !dbg !37
  %call = call noalias i8* @malloc(i64 %0) #4, !dbg !38
  ret i8* %call, !dbg !39
}

; Function Attrs: noinline nounwind uwtable
define dso_local void @free0(i8* %p) #0 !dbg !40 {
entry:
  %p.addr = alloca i8*, align 8
  store i8* %p, i8** %p.addr, align 8
  call void @llvm.dbg.declare(metadata i8** %p.addr, metadata !43, metadata !DIExpression()), !dbg !44
  %0 = load i8*, i8** %p.addr, align 8, !dbg !45
  call void @free(i8* %0) #4, !dbg !46
  ret void, !dbg !47
}

; Function Attrs: nounwind
declare dso_local void @free(i8*) #2

; Function Attrs: noinline nounwind uwtable
define dso_local void @free1(i8* %p) #0 !dbg !48 {
entry:
  %p.addr = alloca i8*, align 8
  store i8* %p, i8** %p.addr, align 8
  call void @llvm.dbg.declare(metadata i8** %p.addr, metadata !49, metadata !DIExpression()), !dbg !50
  %0 = load i8*, i8** %p.addr, align 8, !dbg !51
  call void @free(i8* %0) #4, !dbg !52
  ret void, !dbg !53
}

; Function Attrs: noinline nounwind uwtable
define dso_local void @free2(i8* %p) #0 !dbg !54 {
entry:
  %p.addr = alloca i8*, align 8
  store i8* %p, i8** %p.addr, align 8
  call void @llvm.dbg.declare(metadata i8** %p.addr, metadata !55, metadata !DIExpression()), !dbg !56
  %0 = load i8*, i8** %p.addr, align 8, !dbg !57
  call void @free(i8* %0) #4, !dbg !58
  ret void, !dbg !59
}

; Function Attrs: noinline nounwind uwtable
define dso_local void @free3(i8* %p) #0 !dbg !60 {
entry:
  %p.addr = alloca i8*, align 8
  store i8* %p, i8** %p.addr, align 8
  call void @llvm.dbg.declare(metadata i8** %p.addr, metadata !61, metadata !DIExpression()), !dbg !62
  %0 = load i8*, i8** %p.addr, align 8, !dbg !63
  call void @free(i8* %0) #4, !dbg !64
  ret void, !dbg !65
}

; Function Attrs: noinline nounwind uwtable
define dso_local i32 @main() #0 !dbg !66 {
entry:
  %retval = alloca i32, align 4
  %str = alloca i8*, align 8
  %a = alloca i32, align 4
  %i = alloca i32, align 4
  store i32 0, i32* %retval, align 4
  call void @llvm.dbg.declare(metadata i8** %str, metadata !70, metadata !DIExpression()), !dbg !71
  call void @llvm.dbg.declare(metadata i32* %a, metadata !72, metadata !DIExpression()), !dbg !73
  %call = call i32 @getchar(), !dbg !74
  store i32 %call, i32* %a, align 4, !dbg !73
  %call1 = call noalias i8* @malloc(i64 15) #4, !dbg !75
  store i8* %call1, i8** %str, align 8, !dbg !76
  %0 = load i8*, i8** %str, align 8, !dbg !77
  %call2 = call i8* @strcpy(i8* %0, i8* getelementptr inbounds ([11 x i8], [11 x i8]* @.str, i32 0, i32 0)) #4, !dbg !78
  %1 = load i8*, i8** %str, align 8, !dbg !79
  %2 = load i8*, i8** %str, align 8, !dbg !80
  %call3 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([28 x i8], [28 x i8]* @.str.1, i32 0, i32 0), i8* %1, i8* %2), !dbg !81
  %3 = load i8*, i8** %str, align 8, !dbg !82
  %call4 = call i8* @realloc(i8* %3, i64 25) #4, !dbg !83
  store i8* %call4, i8** %str, align 8, !dbg !84
  %4 = load i8*, i8** %str, align 8, !dbg !85
  %call5 = call i8* @strcat(i8* %4, i8* getelementptr inbounds ([6 x i8], [6 x i8]* @.str.2, i32 0, i32 0)) #4, !dbg !86
  %5 = load i8*, i8** %str, align 8, !dbg !87
  %6 = load i8*, i8** %str, align 8, !dbg !88
  %call6 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([28 x i8], [28 x i8]* @.str.1, i32 0, i32 0), i8* %5, i8* %6), !dbg !89
  br label %while.cond, !dbg !90

while.cond:                                       ; preds = %sw.epilog, %entry
  %7 = load i32, i32* %a, align 4, !dbg !91
  %cmp = icmp slt i32 %7, 10, !dbg !92
  br i1 %cmp, label %while.body, label %while.end14, !dbg !90

while.body:                                       ; preds = %while.cond
  %8 = load i8*, i8** %str, align 8, !dbg !93
  call void @free(i8* %8) #4, !dbg !95
  %9 = load i8*, i8** %str, align 8, !dbg !96
  call void @free0(i8* %9), !dbg !97
  br label %while.cond7, !dbg !98

while.cond7:                                      ; preds = %while.body9, %while.body
  %10 = load i32, i32* %a, align 4, !dbg !99
  %cmp8 = icmp slt i32 %10, 15, !dbg !100
  br i1 %cmp8, label %while.body9, label %while.end, !dbg !98

while.body9:                                      ; preds = %while.cond7
  %11 = load i8*, i8** %str, align 8, !dbg !101
  call void @free(i8* %11) #4, !dbg !103
  br label %while.cond7, !dbg !98, !llvm.loop !104

while.end:                                        ; preds = %while.cond7
  call void @llvm.dbg.declare(metadata i32* %i, metadata !106, metadata !DIExpression()), !dbg !108
  store i32 0, i32* %i, align 4, !dbg !108
  br label %for.cond, !dbg !109

for.cond:                                         ; preds = %for.inc, %while.end
  %12 = load i32, i32* %i, align 4, !dbg !110
  %cmp10 = icmp slt i32 %12, 15, !dbg !112
  br i1 %cmp10, label %for.body, label %for.end, !dbg !113

for.body:                                         ; preds = %for.cond
  %13 = load i8*, i8** %str, align 8, !dbg !114
  call void @free(i8* %13) #4, !dbg !116
  br label %for.inc, !dbg !117

for.inc:                                          ; preds = %for.body
  %14 = load i32, i32* %i, align 4, !dbg !118
  %inc = add nsw i32 %14, 1, !dbg !118
  store i32 %inc, i32* %i, align 4, !dbg !118
  br label %for.cond, !dbg !119, !llvm.loop !120

for.end:                                          ; preds = %for.cond
  %15 = load i32, i32* %a, align 4, !dbg !122
  %cmp11 = icmp eq i32 %15, -15, !dbg !124
  br i1 %cmp11, label %if.then, label %if.end, !dbg !125

if.then:                                          ; preds = %for.end
  %16 = load i8*, i8** %str, align 8, !dbg !126
  call void @free(i8* %16) #4, !dbg !128
  br label %if.end, !dbg !129

if.end:                                           ; preds = %if.then, %for.end
  %17 = load i32, i32* %a, align 4, !dbg !130
  %inc12 = add nsw i32 %17, 1, !dbg !130
  store i32 %inc12, i32* %a, align 4, !dbg !130
  %18 = load i32, i32* %a, align 4, !dbg !131
  switch i32 %18, label %sw.default [
    i32 0, label %sw.bb
    i32 1, label %sw.bb13
  ], !dbg !132

sw.bb:                                            ; preds = %if.end
  %19 = load i8*, i8** %str, align 8, !dbg !133
  call void @free(i8* %19) #4, !dbg !135
  br label %sw.epilog, !dbg !136

sw.bb13:                                          ; preds = %if.end
  %20 = load i8*, i8** %str, align 8, !dbg !137
  call void @free(i8* %20) #4, !dbg !138
  br label %sw.epilog, !dbg !139

sw.default:                                       ; preds = %if.end
  %21 = load i8*, i8** %str, align 8, !dbg !140
  call void @free(i8* %21) #4, !dbg !141
  br label %sw.epilog, !dbg !142

sw.epilog:                                        ; preds = %sw.default, %sw.bb13, %sw.bb
  br label %while.cond, !dbg !90, !llvm.loop !143

while.end14:                                      ; preds = %while.cond
  %22 = load i8*, i8** %str, align 8, !dbg !145
  call void @free0(i8* %22), !dbg !146
  ret i32 0, !dbg !147
}

declare dso_local i32 @getchar() #3

; Function Attrs: nounwind
declare dso_local i8* @strcpy(i8*, i8*) #2

declare dso_local i32 @printf(i8*, ...) #3

; Function Attrs: nounwind
declare dso_local i8* @realloc(i8*, i64) #2

; Function Attrs: nounwind
declare dso_local i8* @strcat(i8*, i8*) #2

attributes #0 = { noinline nounwind uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { nounwind readnone speculatable }
attributes #2 = { nounwind "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #3 = { "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #4 = { nounwind }

!llvm.dbg.cu = !{!0}
!llvm.module.flags = !{!6, !7, !8}
!llvm.ident = !{!9}

!0 = distinct !DICompileUnit(language: DW_LANG_C99, file: !1, producer: "clang version 8.0.1- (branches/release_80)", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, enums: !2, retainedTypes: !3, nameTableKind: None)
!1 = !DIFile(filename: "../test/test18.c", directory: "/c-resource-leak-checker/build")
!2 = !{}
!3 = !{!4}
!4 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !5, size: 64)
!5 = !DIBasicType(name: "char", size: 8, encoding: DW_ATE_signed_char)
!6 = !{i32 2, !"Dwarf Version", i32 4}
!7 = !{i32 2, !"Debug Info Version", i32 3}
!8 = !{i32 1, !"wchar_size", i32 4}
!9 = !{!"clang version 8.0.1- (branches/release_80)"}
!10 = distinct !DISubprogram(name: "malloc0", scope: !1, file: !1, line: 5, type: !11, scopeLine: 5, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition, unit: !0, retainedNodes: !2)
!11 = !DISubroutineType(types: !12)
!12 = !{!13, !14}
!13 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: null, size: 64)
!14 = !DIDerivedType(tag: DW_TAG_typedef, name: "size_t", file: !15, line: 62, baseType: !16)
!15 = !DIFile(filename: "/usr/lib/llvm-8/lib/clang/8.0.1/include/stddef.h", directory: "")
!16 = !DIBasicType(name: "long unsigned int", size: 64, encoding: DW_ATE_unsigned)
!17 = !DILocalVariable(name: "__size", arg: 1, scope: !10, file: !1, line: 5, type: !14)
!18 = !DILocation(line: 5, column: 22, scope: !10)
!19 = !DILocation(line: 6, column: 19, scope: !10)
!20 = !DILocation(line: 6, column: 12, scope: !10)
!21 = !DILocation(line: 6, column: 5, scope: !10)
!22 = distinct !DISubprogram(name: "malloc1", scope: !1, file: !1, line: 9, type: !11, scopeLine: 9, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition, unit: !0, retainedNodes: !2)
!23 = !DILocalVariable(name: "__size", arg: 1, scope: !22, file: !1, line: 9, type: !14)
!24 = !DILocation(line: 9, column: 22, scope: !22)
!25 = !DILocation(line: 10, column: 19, scope: !22)
!26 = !DILocation(line: 10, column: 12, scope: !22)
!27 = !DILocation(line: 10, column: 5, scope: !22)
!28 = distinct !DISubprogram(name: "malloc2", scope: !1, file: !1, line: 13, type: !11, scopeLine: 13, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition, unit: !0, retainedNodes: !2)
!29 = !DILocalVariable(name: "__size", arg: 1, scope: !28, file: !1, line: 13, type: !14)
!30 = !DILocation(line: 13, column: 22, scope: !28)
!31 = !DILocation(line: 14, column: 19, scope: !28)
!32 = !DILocation(line: 14, column: 12, scope: !28)
!33 = !DILocation(line: 14, column: 5, scope: !28)
!34 = distinct !DISubprogram(name: "malloc3", scope: !1, file: !1, line: 17, type: !11, scopeLine: 17, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition, unit: !0, retainedNodes: !2)
!35 = !DILocalVariable(name: "__size", arg: 1, scope: !34, file: !1, line: 17, type: !14)
!36 = !DILocation(line: 17, column: 22, scope: !34)
!37 = !DILocation(line: 18, column: 19, scope: !34)
!38 = !DILocation(line: 18, column: 12, scope: !34)
!39 = !DILocation(line: 18, column: 5, scope: !34)
!40 = distinct !DISubprogram(name: "free0", scope: !1, file: !1, line: 21, type: !41, scopeLine: 21, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition, unit: !0, retainedNodes: !2)
!41 = !DISubroutineType(types: !42)
!42 = !{null, !13}
!43 = !DILocalVariable(name: "p", arg: 1, scope: !40, file: !1, line: 21, type: !13)
!44 = !DILocation(line: 21, column: 18, scope: !40)
!45 = !DILocation(line: 22, column: 10, scope: !40)
!46 = !DILocation(line: 22, column: 5, scope: !40)
!47 = !DILocation(line: 23, column: 1, scope: !40)
!48 = distinct !DISubprogram(name: "free1", scope: !1, file: !1, line: 25, type: !41, scopeLine: 25, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition, unit: !0, retainedNodes: !2)
!49 = !DILocalVariable(name: "p", arg: 1, scope: !48, file: !1, line: 25, type: !13)
!50 = !DILocation(line: 25, column: 18, scope: !48)
!51 = !DILocation(line: 26, column: 10, scope: !48)
!52 = !DILocation(line: 26, column: 5, scope: !48)
!53 = !DILocation(line: 27, column: 1, scope: !48)
!54 = distinct !DISubprogram(name: "free2", scope: !1, file: !1, line: 29, type: !41, scopeLine: 29, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition, unit: !0, retainedNodes: !2)
!55 = !DILocalVariable(name: "p", arg: 1, scope: !54, file: !1, line: 29, type: !13)
!56 = !DILocation(line: 29, column: 18, scope: !54)
!57 = !DILocation(line: 30, column: 10, scope: !54)
!58 = !DILocation(line: 30, column: 5, scope: !54)
!59 = !DILocation(line: 31, column: 1, scope: !54)
!60 = distinct !DISubprogram(name: "free3", scope: !1, file: !1, line: 33, type: !41, scopeLine: 33, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition, unit: !0, retainedNodes: !2)
!61 = !DILocalVariable(name: "p", arg: 1, scope: !60, file: !1, line: 33, type: !13)
!62 = !DILocation(line: 33, column: 18, scope: !60)
!63 = !DILocation(line: 34, column: 10, scope: !60)
!64 = !DILocation(line: 34, column: 5, scope: !60)
!65 = !DILocation(line: 35, column: 1, scope: !60)
!66 = distinct !DISubprogram(name: "main", scope: !1, file: !1, line: 37, type: !67, scopeLine: 37, spFlags: DISPFlagDefinition, unit: !0, retainedNodes: !2)
!67 = !DISubroutineType(types: !68)
!68 = !{!69}
!69 = !DIBasicType(name: "int", size: 32, encoding: DW_ATE_signed)
!70 = !DILocalVariable(name: "str", scope: !66, file: !1, line: 38, type: !4)
!71 = !DILocation(line: 38, column: 10, scope: !66)
!72 = !DILocalVariable(name: "a", scope: !66, file: !1, line: 39, type: !69)
!73 = !DILocation(line: 39, column: 8, scope: !66)
!74 = !DILocation(line: 39, column: 12, scope: !66)
!75 = !DILocation(line: 41, column: 19, scope: !66)
!76 = !DILocation(line: 41, column: 8, scope: !66)
!77 = !DILocation(line: 42, column: 11, scope: !66)
!78 = !DILocation(line: 42, column: 4, scope: !66)
!79 = !DILocation(line: 43, column: 43, scope: !66)
!80 = !DILocation(line: 43, column: 48, scope: !66)
!81 = !DILocation(line: 43, column: 4, scope: !66)
!82 = !DILocation(line: 45, column: 27, scope: !66)
!83 = !DILocation(line: 45, column: 19, scope: !66)
!84 = !DILocation(line: 45, column: 8, scope: !66)
!85 = !DILocation(line: 46, column: 11, scope: !66)
!86 = !DILocation(line: 46, column: 4, scope: !66)
!87 = !DILocation(line: 47, column: 43, scope: !66)
!88 = !DILocation(line: 47, column: 48, scope: !66)
!89 = !DILocation(line: 47, column: 4, scope: !66)
!90 = !DILocation(line: 49, column: 5, scope: !66)
!91 = !DILocation(line: 49, column: 12, scope: !66)
!92 = !DILocation(line: 49, column: 14, scope: !66)
!93 = !DILocation(line: 50, column: 14, scope: !94)
!94 = distinct !DILexicalBlock(scope: !66, file: !1, line: 49, column: 20)
!95 = !DILocation(line: 50, column: 9, scope: !94)
!96 = !DILocation(line: 51, column: 15, scope: !94)
!97 = !DILocation(line: 51, column: 9, scope: !94)
!98 = !DILocation(line: 53, column: 9, scope: !94)
!99 = !DILocation(line: 53, column: 16, scope: !94)
!100 = !DILocation(line: 53, column: 18, scope: !94)
!101 = !DILocation(line: 54, column: 18, scope: !102)
!102 = distinct !DILexicalBlock(scope: !94, file: !1, line: 53, column: 24)
!103 = !DILocation(line: 54, column: 13, scope: !102)
!104 = distinct !{!104, !98, !105}
!105 = !DILocation(line: 55, column: 9, scope: !94)
!106 = !DILocalVariable(name: "i", scope: !107, file: !1, line: 57, type: !69)
!107 = distinct !DILexicalBlock(scope: !94, file: !1, line: 57, column: 9)
!108 = !DILocation(line: 57, column: 18, scope: !107)
!109 = !DILocation(line: 57, column: 14, scope: !107)
!110 = !DILocation(line: 57, column: 25, scope: !111)
!111 = distinct !DILexicalBlock(scope: !107, file: !1, line: 57, column: 9)
!112 = !DILocation(line: 57, column: 27, scope: !111)
!113 = !DILocation(line: 57, column: 9, scope: !107)
!114 = !DILocation(line: 58, column: 18, scope: !115)
!115 = distinct !DILexicalBlock(scope: !111, file: !1, line: 57, column: 38)
!116 = !DILocation(line: 58, column: 13, scope: !115)
!117 = !DILocation(line: 59, column: 9, scope: !115)
!118 = !DILocation(line: 57, column: 34, scope: !111)
!119 = !DILocation(line: 57, column: 9, scope: !111)
!120 = distinct !{!120, !113, !121}
!121 = !DILocation(line: 59, column: 9, scope: !107)
!122 = !DILocation(line: 61, column: 13, scope: !123)
!123 = distinct !DILexicalBlock(scope: !94, file: !1, line: 61, column: 13)
!124 = !DILocation(line: 61, column: 15, scope: !123)
!125 = !DILocation(line: 61, column: 13, scope: !94)
!126 = !DILocation(line: 62, column: 18, scope: !127)
!127 = distinct !DILexicalBlock(scope: !123, file: !1, line: 61, column: 23)
!128 = !DILocation(line: 62, column: 13, scope: !127)
!129 = !DILocation(line: 63, column: 9, scope: !127)
!130 = !DILocation(line: 64, column: 10, scope: !94)
!131 = !DILocation(line: 66, column: 17, scope: !94)
!132 = !DILocation(line: 66, column: 9, scope: !94)
!133 = !DILocation(line: 68, column: 22, scope: !134)
!134 = distinct !DILexicalBlock(scope: !94, file: !1, line: 66, column: 20)
!135 = !DILocation(line: 68, column: 17, scope: !134)
!136 = !DILocation(line: 69, column: 17, scope: !134)
!137 = !DILocation(line: 71, column: 22, scope: !134)
!138 = !DILocation(line: 71, column: 17, scope: !134)
!139 = !DILocation(line: 72, column: 17, scope: !134)
!140 = !DILocation(line: 74, column: 22, scope: !134)
!141 = !DILocation(line: 74, column: 17, scope: !134)
!142 = !DILocation(line: 75, column: 9, scope: !134)
!143 = distinct !{!143, !90, !144}
!144 = !DILocation(line: 77, column: 5, scope: !66)
!145 = !DILocation(line: 79, column: 11, scope: !66)
!146 = !DILocation(line: 79, column: 5, scope: !66)
!147 = !DILocation(line: 82, column: 4, scope: !66)
