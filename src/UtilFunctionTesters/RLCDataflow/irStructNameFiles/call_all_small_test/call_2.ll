; ModuleID = '../test/call_all_small_test/call_2.c'
source_filename = "../test/call_all_small_test/call_2.c"
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

%struct.my_struct = type { i8*, i8* }

@.str = private unnamed_addr constant [41 x i8] c"Calls target = _.FIELD(a) methods = free\00", section "llvm.metadata"
@.str.1 = private unnamed_addr constant [37 x i8] c"../test/call_all_small_test/call_2.c\00", section "llvm.metadata"
@.str.2 = private unnamed_addr constant [41 x i8] c"Calls target = _.FIELD(b) methods = free\00", section "llvm.metadata"
@.str.3 = private unnamed_addr constant [35 x i8] c"MustCall target = _ methods = free\00", section "llvm.metadata"

; Function Attrs: noinline nounwind uwtable
define dso_local void @calls_all(i8* %S.coerce0, i8* %S.coerce1) #0 !dbg !10 {
entry:
  %S = alloca %struct.my_struct, align 8
  %0 = bitcast %struct.my_struct* %S to { i8*, i8* }*
  %1 = getelementptr inbounds { i8*, i8* }, { i8*, i8* }* %0, i32 0, i32 0
  store i8* %S.coerce0, i8** %1, align 8
  %2 = getelementptr inbounds { i8*, i8* }, { i8*, i8* }* %0, i32 0, i32 1
  store i8* %S.coerce1, i8** %2, align 8
  call void @llvm.dbg.declare(metadata %struct.my_struct* %S, metadata !18, metadata !DIExpression()), !dbg !19
  %S1 = bitcast %struct.my_struct* %S to i8*
  call void @llvm.var.annotation(i8* %S1, i8* getelementptr inbounds ([41 x i8], [41 x i8]* @.str, i32 0, i32 0), i8* getelementptr inbounds ([37 x i8], [37 x i8]* @.str.1, i32 0, i32 0), i32 11)
  %S2 = bitcast %struct.my_struct* %S to i8*
  call void @llvm.var.annotation(i8* %S2, i8* getelementptr inbounds ([41 x i8], [41 x i8]* @.str.2, i32 0, i32 0), i8* getelementptr inbounds ([37 x i8], [37 x i8]* @.str.1, i32 0, i32 0), i32 11)
  %a = getelementptr inbounds %struct.my_struct, %struct.my_struct* %S, i32 0, i32 0, !dbg !20
  %3 = bitcast i8** %a to i8*, !dbg !20
  %4 = call i8* @llvm.ptr.annotation.p0i8(i8* %3, i8* getelementptr inbounds ([35 x i8], [35 x i8]* @.str.3, i32 0, i32 0), i8* getelementptr inbounds ([37 x i8], [37 x i8]* @.str.1, i32 0, i32 0), i32 7), !dbg !20
  %5 = bitcast i8* %4 to i8**, !dbg !20
  %6 = load i8*, i8** %5, align 8, !dbg !20
  call void @free(i8* %6) #2, !dbg !21
  %b = getelementptr inbounds %struct.my_struct, %struct.my_struct* %S, i32 0, i32 1, !dbg !22
  %7 = bitcast i8** %b to i8*, !dbg !22
  %8 = call i8* @llvm.ptr.annotation.p0i8(i8* %7, i8* getelementptr inbounds ([35 x i8], [35 x i8]* @.str.3, i32 0, i32 0), i8* getelementptr inbounds ([37 x i8], [37 x i8]* @.str.1, i32 0, i32 0), i32 8), !dbg !22
  %9 = bitcast i8* %8 to i8**, !dbg !22
  %10 = load i8*, i8** %9, align 8, !dbg !22
  call void @free(i8* %10) #2, !dbg !23
  ret void, !dbg !24
}

; Function Attrs: nounwind readnone speculatable
declare void @llvm.dbg.declare(metadata, metadata, metadata) #1

; Function Attrs: nounwind
declare void @llvm.var.annotation(i8*, i8*, i8*, i32) #2

; Function Attrs: nounwind
declare dso_local void @free(i8*) #3

; Function Attrs: nounwind
declare i8* @llvm.ptr.annotation.p0i8(i8*, i8*, i8*, i32) #2

; Function Attrs: noinline nounwind uwtable
define dso_local i32 @main() #0 !dbg !25 {
entry:
  %retval = alloca i32, align 4
  %test = alloca %struct.my_struct, align 8
  store i32 0, i32* %retval, align 4
  call void @llvm.dbg.declare(metadata %struct.my_struct* %test, metadata !29, metadata !DIExpression()), !dbg !30
  %call = call noalias i8* @malloc(i64 15) #2, !dbg !31
  %a = getelementptr inbounds %struct.my_struct, %struct.my_struct* %test, i32 0, i32 0, !dbg !32
  %0 = bitcast i8** %a to i8*, !dbg !32
  %1 = call i8* @llvm.ptr.annotation.p0i8(i8* %0, i8* getelementptr inbounds ([35 x i8], [35 x i8]* @.str.3, i32 0, i32 0), i8* getelementptr inbounds ([37 x i8], [37 x i8]* @.str.1, i32 0, i32 0), i32 7), !dbg !32
  %2 = bitcast i8* %1 to i8**, !dbg !32
  store i8* %call, i8** %2, align 8, !dbg !33
  %call1 = call noalias i8* @malloc(i64 15) #2, !dbg !34
  %b = getelementptr inbounds %struct.my_struct, %struct.my_struct* %test, i32 0, i32 1, !dbg !35
  %3 = bitcast i8** %b to i8*, !dbg !35
  %4 = call i8* @llvm.ptr.annotation.p0i8(i8* %3, i8* getelementptr inbounds ([35 x i8], [35 x i8]* @.str.3, i32 0, i32 0), i8* getelementptr inbounds ([37 x i8], [37 x i8]* @.str.1, i32 0, i32 0), i32 8), !dbg !35
  %5 = bitcast i8* %4 to i8**, !dbg !35
  store i8* %call1, i8** %5, align 8, !dbg !36
  %6 = bitcast %struct.my_struct* %test to { i8*, i8* }*, !dbg !37
  %7 = getelementptr inbounds { i8*, i8* }, { i8*, i8* }* %6, i32 0, i32 0, !dbg !37
  %8 = load i8*, i8** %7, align 8, !dbg !37
  %9 = getelementptr inbounds { i8*, i8* }, { i8*, i8* }* %6, i32 0, i32 1, !dbg !37
  %10 = load i8*, i8** %9, align 8, !dbg !37
  call void @calls_all(i8* %8, i8* %10), !dbg !37
  ret i32 0, !dbg !38
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
!1 = !DIFile(filename: "../test/call_all_small_test/call_2.c", directory: "/c-resource-leak-checker/build")
!2 = !{}
!3 = !{!4}
!4 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !5, size: 64)
!5 = !DIBasicType(name: "char", size: 8, encoding: DW_ATE_signed_char)
!6 = !{i32 2, !"Dwarf Version", i32 4}
!7 = !{i32 2, !"Debug Info Version", i32 3}
!8 = !{i32 1, !"wchar_size", i32 4}
!9 = !{!"clang version 8.0.1- (branches/release_80)"}
!10 = distinct !DISubprogram(name: "calls_all", scope: !1, file: !1, line: 11, type: !11, scopeLine: 11, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition, unit: !0, retainedNodes: !2)
!11 = !DISubroutineType(types: !12)
!12 = !{null, !13}
!13 = !DIDerivedType(tag: DW_TAG_typedef, name: "my_struct", file: !1, line: 9, baseType: !14)
!14 = distinct !DICompositeType(tag: DW_TAG_structure_type, name: "my_struct", file: !1, line: 6, size: 128, elements: !15)
!15 = !{!16, !17}
!16 = !DIDerivedType(tag: DW_TAG_member, name: "a", scope: !14, file: !1, line: 7, baseType: !4, size: 64)
!17 = !DIDerivedType(tag: DW_TAG_member, name: "b", scope: !14, file: !1, line: 8, baseType: !4, size: 64, offset: 64)
!18 = !DILocalVariable(name: "S", arg: 1, scope: !10, file: !1, line: 11, type: !13)
!19 = !DILocation(line: 11, column: 26, scope: !10)
!20 = !DILocation(line: 12, column: 12, scope: !10)
!21 = !DILocation(line: 12, column: 5, scope: !10)
!22 = !DILocation(line: 13, column: 12, scope: !10)
!23 = !DILocation(line: 13, column: 5, scope: !10)
!24 = !DILocation(line: 14, column: 1, scope: !10)
!25 = distinct !DISubprogram(name: "main", scope: !1, file: !1, line: 16, type: !26, scopeLine: 16, spFlags: DISPFlagDefinition, unit: !0, retainedNodes: !2)
!26 = !DISubroutineType(types: !27)
!27 = !{!28}
!28 = !DIBasicType(name: "int", size: 32, encoding: DW_ATE_signed)
!29 = !DILocalVariable(name: "test", scope: !25, file: !1, line: 17, type: !13)
!30 = !DILocation(line: 17, column: 15, scope: !25)
!31 = !DILocation(line: 18, column: 21, scope: !25)
!32 = !DILocation(line: 18, column: 10, scope: !25)
!33 = !DILocation(line: 18, column: 12, scope: !25)
!34 = !DILocation(line: 19, column: 21, scope: !25)
!35 = !DILocation(line: 19, column: 10, scope: !25)
!36 = !DILocation(line: 19, column: 12, scope: !25)
!37 = !DILocation(line: 20, column: 5, scope: !25)
!38 = !DILocation(line: 22, column: 5, scope: !25)
