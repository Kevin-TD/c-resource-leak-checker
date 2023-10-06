; ModuleID = '../test/test21/test21.c'
source_filename = "../test/test21/test21.c"
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

@.str = private unnamed_addr constant [45 x i8] c"TOOL_CHECKER Calls target = _ methods = free\00", section "llvm.metadata"
@.str.1 = private unnamed_addr constant [24 x i8] c"../test/test21/test21.c\00", section "llvm.metadata"

; Function Attrs: noinline nounwind uwtable
define dso_local void @does_free(i8* %s) #0 !dbg !10 {
entry:
  %s.addr = alloca i8*, align 8
  store i8* %s, i8** %s.addr, align 8
  call void @llvm.dbg.declare(metadata i8** %s.addr, metadata !13, metadata !DIExpression()), !dbg !14
  %s.addr1 = bitcast i8** %s.addr to i8*
  call void @llvm.var.annotation(i8* %s.addr1, i8* getelementptr inbounds ([45 x i8], [45 x i8]* @.str, i32 0, i32 0), i8* getelementptr inbounds ([24 x i8], [24 x i8]* @.str.1, i32 0, i32 0), i32 6)
  %0 = load i8*, i8** %s.addr, align 8, !dbg !15
  call void @free(i8* %0) #2, !dbg !16
  ret void, !dbg !17
}

; Function Attrs: nounwind readnone speculatable
declare void @llvm.dbg.declare(metadata, metadata, metadata) #1

; Function Attrs: nounwind
declare void @llvm.var.annotation(i8*, i8*, i8*, i32) #2

; Function Attrs: nounwind
declare dso_local void @free(i8*) #3

; Function Attrs: noinline nounwind uwtable
define dso_local i32 @main() #0 !dbg !18 {
entry:
  %s = alloca i8*, align 8
  call void @llvm.dbg.declare(metadata i8** %s, metadata !22, metadata !DIExpression()), !dbg !23
  %call = call noalias i8* @malloc(i64 15) #2, !dbg !24
  store i8* %call, i8** %s, align 8, !dbg !23
  %0 = load i8*, i8** %s, align 8, !dbg !25
  call void @does_free(i8* %0), !dbg !26
  ret i32 0, !dbg !27
}

; Function Attrs: nounwind
declare dso_local noalias i8* @malloc(i64) #3

attributes #0 = { noinline nounwind uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { nounwind readnone speculatable }
attributes #2 = { nounwind }
attributes #3 = { nounwind "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.dbg.cu = !{!0}
!llvm.module.flags = !{!6, !7, !8}
!llvm.ident = !{!9}

!0 = distinct !DICompileUnit(language: DW_LANG_C99, file: !1, producer: "clang version 8.0.1- (branches/release_80)", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, enums: !2, retainedTypes: !3, nameTableKind: None)
!1 = !DIFile(filename: "../test/test21/test21.c", directory: "/c-resource-leak-checker/build")
!2 = !{}
!3 = !{!4}
!4 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !5, size: 64)
!5 = !DIBasicType(name: "char", size: 8, encoding: DW_ATE_signed_char)
!6 = !{i32 2, !"Dwarf Version", i32 4}
!7 = !{i32 2, !"Debug Info Version", i32 3}
!8 = !{i32 1, !"wchar_size", i32 4}
!9 = !{!"clang version 8.0.1- (branches/release_80)"}
!10 = distinct !DISubprogram(name: "does_free", scope: !1, file: !1, line: 6, type: !11, scopeLine: 6, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition, unit: !0, retainedNodes: !2)
!11 = !DISubroutineType(types: !12)
!12 = !{null, !4}
!13 = !DILocalVariable(name: "s", arg: 1, scope: !10, file: !1, line: 6, type: !4)
!14 = !DILocation(line: 6, column: 22, scope: !10)
!15 = !DILocation(line: 7, column: 10, scope: !10)
!16 = !DILocation(line: 7, column: 5, scope: !10)
!17 = !DILocation(line: 8, column: 1, scope: !10)
!18 = distinct !DISubprogram(name: "main", scope: !1, file: !1, line: 10, type: !19, scopeLine: 10, spFlags: DISPFlagDefinition, unit: !0, retainedNodes: !2)
!19 = !DISubroutineType(types: !20)
!20 = !{!21}
!21 = !DIBasicType(name: "int", size: 32, encoding: DW_ATE_signed)
!22 = !DILocalVariable(name: "s", scope: !18, file: !1, line: 11, type: !4)
!23 = !DILocation(line: 11, column: 11, scope: !18)
!24 = !DILocation(line: 11, column: 22, scope: !18)
!25 = !DILocation(line: 12, column: 15, scope: !18)
!26 = !DILocation(line: 12, column: 5, scope: !18)
!27 = !DILocation(line: 16, column: 1, scope: !18)
