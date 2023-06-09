; ModuleID = '../test/test21.c'
source_filename = "../test/test21.c"
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

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
define dso_local i32 @free0(i8* %p) #0 !dbg !40 {
entry:
  %p.addr = alloca i8*, align 8
  store i8* %p, i8** %p.addr, align 8
  call void @llvm.dbg.declare(metadata i8** %p.addr, metadata !44, metadata !DIExpression()), !dbg !45
  %0 = load i8*, i8** %p.addr, align 8, !dbg !46
  call void @free(i8* %0) #4, !dbg !47
  ret i32 1, !dbg !48
}

; Function Attrs: nounwind
declare dso_local void @free(i8*) #2

; Function Attrs: noinline nounwind uwtable
define dso_local i32 @free1(i8* %p) #0 !dbg !49 {
entry:
  %p.addr = alloca i8*, align 8
  store i8* %p, i8** %p.addr, align 8
  call void @llvm.dbg.declare(metadata i8** %p.addr, metadata !50, metadata !DIExpression()), !dbg !51
  %0 = load i8*, i8** %p.addr, align 8, !dbg !52
  call void @free(i8* %0) #4, !dbg !53
  ret i32 1, !dbg !54
}

; Function Attrs: noinline nounwind uwtable
define dso_local i32 @free2(i8* %p) #0 !dbg !55 {
entry:
  %p.addr = alloca i8*, align 8
  store i8* %p, i8** %p.addr, align 8
  call void @llvm.dbg.declare(metadata i8** %p.addr, metadata !56, metadata !DIExpression()), !dbg !57
  %0 = load i8*, i8** %p.addr, align 8, !dbg !58
  call void @free(i8* %0) #4, !dbg !59
  ret i32 1, !dbg !60
}

; Function Attrs: noinline nounwind uwtable
define dso_local i32 @free3(i8* %p) #0 !dbg !61 {
entry:
  %p.addr = alloca i8*, align 8
  store i8* %p, i8** %p.addr, align 8
  call void @llvm.dbg.declare(metadata i8** %p.addr, metadata !62, metadata !DIExpression()), !dbg !63
  %0 = load i8*, i8** %p.addr, align 8, !dbg !64
  call void @free(i8* %0) #4, !dbg !65
  ret i32 1, !dbg !66
}

; Function Attrs: noinline nounwind uwtable
define dso_local i32 @main() #0 !dbg !67 {
entry:
  %retval = alloca i32, align 4
  %str = alloca i8*, align 8
  %a = alloca i32, align 4
  store i32 0, i32* %retval, align 4
  call void @llvm.dbg.declare(metadata i8** %str, metadata !70, metadata !DIExpression()), !dbg !71
  call void @llvm.dbg.declare(metadata i32* %a, metadata !72, metadata !DIExpression()), !dbg !73
  %call = call i32 @getchar(), !dbg !74
  store i32 %call, i32* %a, align 4, !dbg !73
  %call1 = call noalias i8* @malloc(i64 15) #4, !dbg !75
  store i8* %call1, i8** %str, align 8, !dbg !76
  %0 = load i8*, i8** %str, align 8, !dbg !77
  %call2 = call i32 @free0(i8* %0), !dbg !79
  %tobool = icmp ne i32 %call2, 0, !dbg !79
  br i1 %tobool, label %if.then, label %if.else, !dbg !80

if.then:                                          ; preds = %entry
  br label %if.end, !dbg !81

if.else:                                          ; preds = %entry
  br label %if.end

if.end:                                           ; preds = %if.else, %if.then
  %1 = load i32, i32* %a, align 4, !dbg !83
  %cmp = icmp eq i32 %1, -15, !dbg !85
  br i1 %cmp, label %if.then3, label %if.else4, !dbg !86

if.then3:                                         ; preds = %if.end
  br label %if.end9, !dbg !87

if.else4:                                         ; preds = %if.end
  %2 = load i8*, i8** %str, align 8, !dbg !89
  %call5 = call i32 @free1(i8* %2), !dbg !91
  %tobool6 = icmp ne i32 %call5, 0, !dbg !91
  br i1 %tobool6, label %if.then7, label %if.end8, !dbg !92

if.then7:                                         ; preds = %if.else4
  br label %if.end8, !dbg !93

if.end8:                                          ; preds = %if.then7, %if.else4
  br label %if.end9

if.end9:                                          ; preds = %if.end8, %if.then3
  br label %while.cond, !dbg !95

while.cond:                                       ; preds = %while.body, %if.end9
  %3 = load i8*, i8** %str, align 8, !dbg !96
  %call10 = call i32 @free2(i8* %3), !dbg !97
  %tobool11 = icmp ne i32 %call10, 0, !dbg !95
  br i1 %tobool11, label %while.body, label %while.end, !dbg !95

while.body:                                       ; preds = %while.cond
  br label %while.cond, !dbg !95, !llvm.loop !98

while.end:                                        ; preds = %while.cond
  %4 = load i8*, i8** %str, align 8, !dbg !100
  %call12 = call i32 @free3(i8* %4), !dbg !102
  br label %for.cond, !dbg !102

for.cond:                                         ; preds = %for.cond, %while.end
  br label %for.cond, !dbg !103, !llvm.loop !105

return:                                           ; No predecessors!
  %5 = load i32, i32* %retval, align 4, !dbg !108
  ret i32 %5, !dbg !108
}

declare dso_local i32 @getchar() #3

attributes #0 = { noinline nounwind uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { nounwind readnone speculatable }
attributes #2 = { nounwind "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #3 = { "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #4 = { nounwind }

!llvm.dbg.cu = !{!0}
!llvm.module.flags = !{!6, !7, !8}
!llvm.ident = !{!9}

!0 = distinct !DICompileUnit(language: DW_LANG_C99, file: !1, producer: "clang version 8.0.1- (branches/release_80)", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, enums: !2, retainedTypes: !3, nameTableKind: None)
!1 = !DIFile(filename: "../test/test21.c", directory: "/c-resource-leak-checker/build")
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
!42 = !{!43, !13}
!43 = !DIBasicType(name: "int", size: 32, encoding: DW_ATE_signed)
!44 = !DILocalVariable(name: "p", arg: 1, scope: !40, file: !1, line: 21, type: !13)
!45 = !DILocation(line: 21, column: 17, scope: !40)
!46 = !DILocation(line: 22, column: 10, scope: !40)
!47 = !DILocation(line: 22, column: 5, scope: !40)
!48 = !DILocation(line: 23, column: 5, scope: !40)
!49 = distinct !DISubprogram(name: "free1", scope: !1, file: !1, line: 26, type: !41, scopeLine: 26, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition, unit: !0, retainedNodes: !2)
!50 = !DILocalVariable(name: "p", arg: 1, scope: !49, file: !1, line: 26, type: !13)
!51 = !DILocation(line: 26, column: 17, scope: !49)
!52 = !DILocation(line: 27, column: 10, scope: !49)
!53 = !DILocation(line: 27, column: 5, scope: !49)
!54 = !DILocation(line: 28, column: 5, scope: !49)
!55 = distinct !DISubprogram(name: "free2", scope: !1, file: !1, line: 31, type: !41, scopeLine: 31, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition, unit: !0, retainedNodes: !2)
!56 = !DILocalVariable(name: "p", arg: 1, scope: !55, file: !1, line: 31, type: !13)
!57 = !DILocation(line: 31, column: 17, scope: !55)
!58 = !DILocation(line: 32, column: 10, scope: !55)
!59 = !DILocation(line: 32, column: 5, scope: !55)
!60 = !DILocation(line: 33, column: 5, scope: !55)
!61 = distinct !DISubprogram(name: "free3", scope: !1, file: !1, line: 36, type: !41, scopeLine: 36, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition, unit: !0, retainedNodes: !2)
!62 = !DILocalVariable(name: "p", arg: 1, scope: !61, file: !1, line: 36, type: !13)
!63 = !DILocation(line: 36, column: 17, scope: !61)
!64 = !DILocation(line: 37, column: 10, scope: !61)
!65 = !DILocation(line: 37, column: 5, scope: !61)
!66 = !DILocation(line: 38, column: 5, scope: !61)
!67 = distinct !DISubprogram(name: "main", scope: !1, file: !1, line: 43, type: !68, scopeLine: 43, spFlags: DISPFlagDefinition, unit: !0, retainedNodes: !2)
!68 = !DISubroutineType(types: !69)
!69 = !{!43}
!70 = !DILocalVariable(name: "str", scope: !67, file: !1, line: 44, type: !4)
!71 = !DILocation(line: 44, column: 10, scope: !67)
!72 = !DILocalVariable(name: "a", scope: !67, file: !1, line: 45, type: !43)
!73 = !DILocation(line: 45, column: 8, scope: !67)
!74 = !DILocation(line: 45, column: 12, scope: !67)
!75 = !DILocation(line: 47, column: 19, scope: !67)
!76 = !DILocation(line: 47, column: 8, scope: !67)
!77 = !DILocation(line: 49, column: 14, scope: !78)
!78 = distinct !DILexicalBlock(scope: !67, file: !1, line: 49, column: 8)
!79 = !DILocation(line: 49, column: 8, scope: !78)
!80 = !DILocation(line: 49, column: 8, scope: !67)
!81 = !DILocation(line: 51, column: 4, scope: !82)
!82 = distinct !DILexicalBlock(scope: !78, file: !1, line: 49, column: 20)
!83 = !DILocation(line: 56, column: 8, scope: !84)
!84 = distinct !DILexicalBlock(scope: !67, file: !1, line: 56, column: 8)
!85 = !DILocation(line: 56, column: 10, scope: !84)
!86 = !DILocation(line: 56, column: 8, scope: !67)
!87 = !DILocation(line: 58, column: 4, scope: !88)
!88 = distinct !DILexicalBlock(scope: !84, file: !1, line: 56, column: 18)
!89 = !DILocation(line: 58, column: 21, scope: !90)
!90 = distinct !DILexicalBlock(scope: !84, file: !1, line: 58, column: 15)
!91 = !DILocation(line: 58, column: 15, scope: !90)
!92 = !DILocation(line: 58, column: 15, scope: !84)
!93 = !DILocation(line: 60, column: 4, scope: !94)
!94 = distinct !DILexicalBlock(scope: !90, file: !1, line: 58, column: 27)
!95 = !DILocation(line: 62, column: 4, scope: !67)
!96 = !DILocation(line: 62, column: 17, scope: !67)
!97 = !DILocation(line: 62, column: 11, scope: !67)
!98 = distinct !{!98, !95, !99}
!99 = !DILocation(line: 64, column: 4, scope: !67)
!100 = !DILocation(line: 66, column: 15, scope: !101)
!101 = distinct !DILexicalBlock(scope: !67, file: !1, line: 66, column: 4)
!102 = !DILocation(line: 66, column: 9, scope: !101)
!103 = !DILocation(line: 66, column: 4, scope: !104)
!104 = distinct !DILexicalBlock(scope: !101, file: !1, line: 66, column: 4)
!105 = distinct !{!105, !106, !107}
!106 = !DILocation(line: 66, column: 4, scope: !101)
!107 = !DILocation(line: 68, column: 4, scope: !101)
!108 = !DILocation(line: 74, column: 1, scope: !67)
