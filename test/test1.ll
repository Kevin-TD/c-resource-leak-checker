; ModuleID = '../test/test1.c'
source_filename = "../test/test1.c"
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

; Function Attrs: noinline nounwind uwtable
define dso_local i32 @method1(i32 %a, i32 %b) #0 !dbg !7 {
entry:
  %a.addr = alloca i32, align 4
  %b.addr = alloca i32, align 4
  store i32 %a, i32* %a.addr, align 4
  call void @llvm.dbg.declare(metadata i32* %a.addr, metadata !11, metadata !DIExpression()), !dbg !12
  store i32 %b, i32* %b.addr, align 4
  call void @llvm.dbg.declare(metadata i32* %b.addr, metadata !13, metadata !DIExpression()), !dbg !14
  %0 = load i32, i32* %a.addr, align 4, !dbg !15
  %1 = load i32, i32* %b.addr, align 4, !dbg !16
  %add = add nsw i32 %0, %1, !dbg !17
  ret i32 %add, !dbg !18
}

; Function Attrs: nounwind readnone speculatable
declare void @llvm.dbg.declare(metadata, metadata, metadata) #1

; Function Attrs: noinline nounwind uwtable
define dso_local i32 @method2(i32 %a, i32 %b) #0 !dbg !19 {
entry:
  %a.addr = alloca i32, align 4
  %b.addr = alloca i32, align 4
  store i32 %a, i32* %a.addr, align 4
  call void @llvm.dbg.declare(metadata i32* %a.addr, metadata !20, metadata !DIExpression()), !dbg !21
  store i32 %b, i32* %b.addr, align 4
  call void @llvm.dbg.declare(metadata i32* %b.addr, metadata !22, metadata !DIExpression()), !dbg !23
  %0 = load i32, i32* %a.addr, align 4, !dbg !24
  %1 = load i32, i32* %b.addr, align 4, !dbg !25
  %sub = sub nsw i32 %0, %1, !dbg !26
  ret i32 %sub, !dbg !27
}

; Function Attrs: noinline nounwind uwtable
define dso_local i32 @method3(i32 %a) #0 !dbg !28 {
entry:
  %a.addr = alloca i32, align 4
  store i32 %a, i32* %a.addr, align 4
  call void @llvm.dbg.declare(metadata i32* %a.addr, metadata !31, metadata !DIExpression()), !dbg !32
  %0 = load i32, i32* %a.addr, align 4, !dbg !33
  ret i32 %0, !dbg !34
}

; Function Attrs: noinline nounwind uwtable
define dso_local i32 @main() #0 !dbg !35 {
entry:
  %retval = alloca i32, align 4
  %a = alloca i32, align 4
  %b = alloca i32, align 4
  %c = alloca i32, align 4
  store i32 0, i32* %retval, align 4
  call void @llvm.dbg.declare(metadata i32* %a, metadata !38, metadata !DIExpression()), !dbg !39
  store i32 5, i32* %a, align 4, !dbg !39
  call void @llvm.dbg.declare(metadata i32* %b, metadata !40, metadata !DIExpression()), !dbg !41
  store i32 6, i32* %b, align 4, !dbg !41
  call void @llvm.dbg.declare(metadata i32* %c, metadata !42, metadata !DIExpression()), !dbg !43
  %call = call i32 (...) @getchar(), !dbg !44
  store i32 %call, i32* %c, align 4, !dbg !43
  %0 = load i32, i32* %c, align 4, !dbg !45
  %cmp = icmp sgt i32 %0, 15, !dbg !47
  br i1 %cmp, label %if.then, label %if.else, !dbg !48

if.then:                                          ; preds = %entry
  %1 = load i32, i32* %a, align 4, !dbg !49
  %2 = load i32, i32* %a, align 4, !dbg !51
  %call1 = call i32 @method1(i32 %1, i32 %2), !dbg !52
  store i32 %call1, i32* %a, align 4, !dbg !53
  br label %if.end, !dbg !54

if.else:                                          ; preds = %entry
  %3 = load i32, i32* %a, align 4, !dbg !55
  %4 = load i32, i32* %b, align 4, !dbg !57
  %call2 = call i32 @method2(i32 %3, i32 %4), !dbg !58
  store i32 %call2, i32* %a, align 4, !dbg !59
  br label %if.end

if.end:                                           ; preds = %if.else, %if.then
  %5 = load i32, i32* %c, align 4, !dbg !60
  %call3 = call i32 @method3(i32 %5), !dbg !61
  %6 = load i32, i32* %a, align 4, !dbg !62
  %7 = load i32, i32* %c, align 4, !dbg !63
  %sub = sub nsw i32 %6, %7, !dbg !64
  %call4 = call i32 @method3(i32 %sub), !dbg !65
  %call5 = call i32 @method2(i32 %call3, i32 %call4), !dbg !66
  store i32 %call5, i32* %b, align 4, !dbg !67
  ret i32 0, !dbg !68
}

declare dso_local i32 @getchar(...) #2

attributes #0 = { noinline nounwind uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { nounwind readnone speculatable }
attributes #2 = { "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.dbg.cu = !{!0}
!llvm.module.flags = !{!3, !4, !5}
!llvm.ident = !{!6}

!0 = distinct !DICompileUnit(language: DW_LANG_C99, file: !1, producer: "clang version 8.0.1- (branches/release_80)", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, enums: !2, nameTableKind: None)
!1 = !DIFile(filename: "../test/test1.c", directory: "/resource leak checker/build")
!2 = !{}
!3 = !{i32 2, !"Dwarf Version", i32 4}
!4 = !{i32 2, !"Debug Info Version", i32 3}
!5 = !{i32 1, !"wchar_size", i32 4}
!6 = !{!"clang version 8.0.1- (branches/release_80)"}
!7 = distinct !DISubprogram(name: "method1", scope: !1, file: !1, line: 1, type: !8, scopeLine: 1, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition, unit: !0, retainedNodes: !2)
!8 = !DISubroutineType(types: !9)
!9 = !{!10, !10, !10}
!10 = !DIBasicType(name: "int", size: 32, encoding: DW_ATE_signed)
!11 = !DILocalVariable(name: "a", arg: 1, scope: !7, file: !1, line: 1, type: !10)
!12 = !DILocation(line: 1, column: 17, scope: !7)
!13 = !DILocalVariable(name: "b", arg: 2, scope: !7, file: !1, line: 1, type: !10)
!14 = !DILocation(line: 1, column: 24, scope: !7)
!15 = !DILocation(line: 2, column: 10, scope: !7)
!16 = !DILocation(line: 2, column: 14, scope: !7)
!17 = !DILocation(line: 2, column: 12, scope: !7)
!18 = !DILocation(line: 2, column: 3, scope: !7)
!19 = distinct !DISubprogram(name: "method2", scope: !1, file: !1, line: 5, type: !8, scopeLine: 5, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition, unit: !0, retainedNodes: !2)
!20 = !DILocalVariable(name: "a", arg: 1, scope: !19, file: !1, line: 5, type: !10)
!21 = !DILocation(line: 5, column: 17, scope: !19)
!22 = !DILocalVariable(name: "b", arg: 2, scope: !19, file: !1, line: 5, type: !10)
!23 = !DILocation(line: 5, column: 24, scope: !19)
!24 = !DILocation(line: 6, column: 10, scope: !19)
!25 = !DILocation(line: 6, column: 14, scope: !19)
!26 = !DILocation(line: 6, column: 12, scope: !19)
!27 = !DILocation(line: 6, column: 3, scope: !19)
!28 = distinct !DISubprogram(name: "method3", scope: !1, file: !1, line: 9, type: !29, scopeLine: 9, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition, unit: !0, retainedNodes: !2)
!29 = !DISubroutineType(types: !30)
!30 = !{!10, !10}
!31 = !DILocalVariable(name: "a", arg: 1, scope: !28, file: !1, line: 9, type: !10)
!32 = !DILocation(line: 9, column: 17, scope: !28)
!33 = !DILocation(line: 10, column: 10, scope: !28)
!34 = !DILocation(line: 10, column: 3, scope: !28)
!35 = distinct !DISubprogram(name: "main", scope: !1, file: !1, line: 13, type: !36, scopeLine: 13, spFlags: DISPFlagDefinition, unit: !0, retainedNodes: !2)
!36 = !DISubroutineType(types: !37)
!37 = !{!10}
!38 = !DILocalVariable(name: "a", scope: !35, file: !1, line: 14, type: !10)
!39 = !DILocation(line: 14, column: 7, scope: !35)
!40 = !DILocalVariable(name: "b", scope: !35, file: !1, line: 15, type: !10)
!41 = !DILocation(line: 15, column: 7, scope: !35)
!42 = !DILocalVariable(name: "c", scope: !35, file: !1, line: 16, type: !10)
!43 = !DILocation(line: 16, column: 7, scope: !35)
!44 = !DILocation(line: 16, column: 11, scope: !35)
!45 = !DILocation(line: 18, column: 7, scope: !46)
!46 = distinct !DILexicalBlock(scope: !35, file: !1, line: 18, column: 7)
!47 = !DILocation(line: 18, column: 9, scope: !46)
!48 = !DILocation(line: 18, column: 7, scope: !35)
!49 = !DILocation(line: 19, column: 17, scope: !50)
!50 = distinct !DILexicalBlock(scope: !46, file: !1, line: 18, column: 15)
!51 = !DILocation(line: 19, column: 20, scope: !50)
!52 = !DILocation(line: 19, column: 9, scope: !50)
!53 = !DILocation(line: 19, column: 7, scope: !50)
!54 = !DILocation(line: 20, column: 3, scope: !50)
!55 = !DILocation(line: 22, column: 17, scope: !56)
!56 = distinct !DILexicalBlock(scope: !46, file: !1, line: 21, column: 8)
!57 = !DILocation(line: 22, column: 20, scope: !56)
!58 = !DILocation(line: 22, column: 9, scope: !56)
!59 = !DILocation(line: 22, column: 7, scope: !56)
!60 = !DILocation(line: 24, column: 23, scope: !35)
!61 = !DILocation(line: 24, column: 15, scope: !35)
!62 = !DILocation(line: 24, column: 35, scope: !35)
!63 = !DILocation(line: 24, column: 39, scope: !35)
!64 = !DILocation(line: 24, column: 37, scope: !35)
!65 = !DILocation(line: 24, column: 27, scope: !35)
!66 = !DILocation(line: 24, column: 7, scope: !35)
!67 = !DILocation(line: 24, column: 5, scope: !35)
!68 = !DILocation(line: 27, column: 3, scope: !35)
