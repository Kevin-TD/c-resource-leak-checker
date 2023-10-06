; ModuleID = '../test/nesting_test/subdir/runner.c'
source_filename = "../test/nesting_test/subdir/runner.c"
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

%struct.my_struct = type { i8*, i8* }
%struct.struct_2 = type { i8*, i8* }
%struct.my_struct_3 = type { i8*, i8* }

@.str = private unnamed_addr constant [48 x i8] c"TOOL_CHECKER MustCall target = _ methods = free\00", section "llvm.metadata"
@.str.1 = private unnamed_addr constant [37 x i8] c"../test/nesting_test/subdir/runner.c\00", section "llvm.metadata"
@.str.2 = private unnamed_addr constant [45 x i8] c"TOOL_CHECKER Calls target = _ methods = free\00", section "llvm.metadata"
@.str.3 = private unnamed_addr constant [54 x i8] c"TOOL_CHECKER Calls target = _.FIELD(x) methods = free\00", section "llvm.metadata"
@.str.4 = private unnamed_addr constant [46 x i8] c"../test/nesting_test/subdir/../call_structs.h\00", section "llvm.metadata"
@llvm.global.annotations = appending global [2 x { i8*, i8*, i8*, i32 }] [{ i8*, i8*, i8*, i32 } { i8* bitcast (i8* (i8*, i8*, i8*)* @creates_obligation to i8*), i8* getelementptr inbounds ([48 x i8], [48 x i8]* @.str, i32 0, i32 0), i8* getelementptr inbounds ([37 x i8], [37 x i8]* @.str.1, i32 0, i32 0), i32 7 }, { i8*, i8*, i8*, i32 } { i8* bitcast ({ i8*, i8* } (i8*, i8*)* @does_something to i8*), i8* getelementptr inbounds ([54 x i8], [54 x i8]* @.str.3, i32 0, i32 0), i8* getelementptr inbounds ([37 x i8], [37 x i8]* @.str.1, i32 0, i32 0), i32 14 }], section "llvm.metadata"

; Function Attrs: noinline nounwind uwtable
define dso_local void @does_free(i8* %s) #0 !dbg !10 {
entry:
  %s.addr = alloca i8*, align 8
  store i8* %s, i8** %s.addr, align 8
  call void @llvm.dbg.declare(metadata i8** %s.addr, metadata !13, metadata !DIExpression()), !dbg !14
  %s.addr1 = bitcast i8** %s.addr to i8*
  call void @llvm.var.annotation(i8* %s.addr1, i8* getelementptr inbounds ([48 x i8], [48 x i8]* @.str, i32 0, i32 0), i8* getelementptr inbounds ([37 x i8], [37 x i8]* @.str.1, i32 0, i32 0), i32 3)
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
define dso_local i8* @creates_obligation(i8* %s, i8* %X.coerce0, i8* %X.coerce1) #0 !dbg !18 {
entry:
  %X = alloca %struct.my_struct, align 8
  %s.addr = alloca i8*, align 8
  %str = alloca i8*, align 8
  %0 = bitcast %struct.my_struct* %X to { i8*, i8* }*
  %1 = getelementptr inbounds { i8*, i8* }, { i8*, i8* }* %0, i32 0, i32 0
  store i8* %X.coerce0, i8** %1, align 8
  %2 = getelementptr inbounds { i8*, i8* }, { i8*, i8* }* %0, i32 0, i32 1
  store i8* %X.coerce1, i8** %2, align 8
  store i8* %s, i8** %s.addr, align 8
  call void @llvm.dbg.declare(metadata i8** %s.addr, metadata !27, metadata !DIExpression()), !dbg !28
  %s.addr1 = bitcast i8** %s.addr to i8*
  call void @llvm.var.annotation(i8* %s.addr1, i8* getelementptr inbounds ([45 x i8], [45 x i8]* @.str.2, i32 0, i32 0), i8* getelementptr inbounds ([37 x i8], [37 x i8]* @.str.1, i32 0, i32 0), i32 7)
  call void @llvm.dbg.declare(metadata %struct.my_struct* %X, metadata !29, metadata !DIExpression()), !dbg !30
  %X2 = bitcast %struct.my_struct* %X to i8*
  call void @llvm.var.annotation(i8* %X2, i8* getelementptr inbounds ([54 x i8], [54 x i8]* @.str.3, i32 0, i32 0), i8* getelementptr inbounds ([37 x i8], [37 x i8]* @.str.1, i32 0, i32 0), i32 7)
  %3 = load i8*, i8** %s.addr, align 8, !dbg !31
  call void @free(i8* %3) #2, !dbg !32
  %x = getelementptr inbounds %struct.my_struct, %struct.my_struct* %X, i32 0, i32 0, !dbg !33
  %4 = bitcast i8** %x to i8*, !dbg !33
  %5 = call i8* @llvm.ptr.annotation.p0i8(i8* %4, i8* getelementptr inbounds ([48 x i8], [48 x i8]* @.str, i32 0, i32 0), i8* getelementptr inbounds ([46 x i8], [46 x i8]* @.str.4, i32 0, i32 0), i32 7), !dbg !33
  %6 = bitcast i8* %5 to i8**, !dbg !33
  %7 = load i8*, i8** %6, align 8, !dbg !33
  call void @free(i8* %7) #2, !dbg !34
  call void @llvm.dbg.declare(metadata i8** %str, metadata !35, metadata !DIExpression()), !dbg !36
  %call = call noalias i8* @malloc(i64 15) #2, !dbg !37
  store i8* %call, i8** %str, align 8, !dbg !36
  %8 = load i8*, i8** %str, align 8, !dbg !38
  ret i8* %8, !dbg !39
}

; Function Attrs: nounwind
declare i8* @llvm.ptr.annotation.p0i8(i8*, i8*, i8*, i32) #2

; Function Attrs: nounwind
declare dso_local noalias i8* @malloc(i64) #3

; Function Attrs: noinline nounwind uwtable
define dso_local { i8*, i8* } @does_something(i8* %S.coerce0, i8* %S.coerce1) #0 !dbg !40 {
entry:
  %retval = alloca %struct.my_struct, align 8
  %S = alloca %struct.my_struct, align 8
  %0 = bitcast %struct.my_struct* %S to { i8*, i8* }*
  %1 = getelementptr inbounds { i8*, i8* }, { i8*, i8* }* %0, i32 0, i32 0
  store i8* %S.coerce0, i8** %1, align 8
  %2 = getelementptr inbounds { i8*, i8* }, { i8*, i8* }* %0, i32 0, i32 1
  store i8* %S.coerce1, i8** %2, align 8
  call void @llvm.dbg.declare(metadata %struct.my_struct* %S, metadata !43, metadata !DIExpression()), !dbg !44
  %S1 = bitcast %struct.my_struct* %S to i8*
  call void @llvm.var.annotation(i8* %S1, i8* getelementptr inbounds ([54 x i8], [54 x i8]* @.str.3, i32 0, i32 0), i8* getelementptr inbounds ([37 x i8], [37 x i8]* @.str.1, i32 0, i32 0), i32 14)
  %x = getelementptr inbounds %struct.my_struct, %struct.my_struct* %S, i32 0, i32 0, !dbg !45
  %3 = bitcast i8** %x to i8*, !dbg !45
  %4 = call i8* @llvm.ptr.annotation.p0i8(i8* %3, i8* getelementptr inbounds ([48 x i8], [48 x i8]* @.str, i32 0, i32 0), i8* getelementptr inbounds ([46 x i8], [46 x i8]* @.str.4, i32 0, i32 0), i32 7), !dbg !45
  %5 = bitcast i8* %4 to i8**, !dbg !45
  %6 = load i8*, i8** %5, align 8, !dbg !45
  call void @free(i8* %6) #2, !dbg !46
  %7 = bitcast %struct.my_struct* %retval to i8*, !dbg !47
  %8 = bitcast %struct.my_struct* %S to i8*, !dbg !47
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* align 8 %7, i8* align 8 %8, i64 16, i1 false), !dbg !47
  %9 = bitcast %struct.my_struct* %retval to { i8*, i8* }*, !dbg !48
  %10 = load { i8*, i8* }, { i8*, i8* }* %9, align 8, !dbg !48
  ret { i8*, i8* } %10, !dbg !48
}

; Function Attrs: argmemonly nounwind
declare void @llvm.memcpy.p0i8.p0i8.i64(i8* nocapture writeonly, i8* nocapture readonly, i64, i1) #4

; Function Attrs: noinline nounwind uwtable
define dso_local i32 @main() #0 !dbg !49 {
entry:
  %k = alloca %struct.my_struct, align 8
  %m = alloca %struct.struct_2, align 8
  %km = alloca %struct.my_struct_3, align 8
  %s = alloca i8*, align 8
  %y7 = alloca i8*, align 8
  call void @llvm.dbg.declare(metadata %struct.my_struct* %k, metadata !53, metadata !DIExpression()), !dbg !54
  call void @llvm.dbg.declare(metadata %struct.struct_2* %m, metadata !55, metadata !DIExpression()), !dbg !61
  call void @llvm.dbg.declare(metadata %struct.my_struct_3* %km, metadata !62, metadata !DIExpression()), !dbg !67
  %call = call noalias i8* @malloc(i64 15) #2, !dbg !68
  %x = getelementptr inbounds %struct.my_struct, %struct.my_struct* %k, i32 0, i32 0, !dbg !69
  %0 = bitcast i8** %x to i8*, !dbg !69
  %1 = call i8* @llvm.ptr.annotation.p0i8(i8* %0, i8* getelementptr inbounds ([48 x i8], [48 x i8]* @.str, i32 0, i32 0), i8* getelementptr inbounds ([46 x i8], [46 x i8]* @.str.4, i32 0, i32 0), i32 7), !dbg !69
  %2 = bitcast i8* %1 to i8**, !dbg !69
  store i8* %call, i8** %2, align 8, !dbg !70
  %call1 = call noalias i8* @malloc(i64 15) #2, !dbg !71
  %y = getelementptr inbounds %struct.my_struct, %struct.my_struct* %k, i32 0, i32 1, !dbg !72
  %3 = bitcast i8** %y to i8*, !dbg !72
  %4 = call i8* @llvm.ptr.annotation.p0i8(i8* %3, i8* getelementptr inbounds ([48 x i8], [48 x i8]* @.str, i32 0, i32 0), i8* getelementptr inbounds ([46 x i8], [46 x i8]* @.str.4, i32 0, i32 0), i32 8), !dbg !72
  %5 = bitcast i8* %4 to i8**, !dbg !72
  store i8* %call1, i8** %5, align 8, !dbg !73
  %call2 = call noalias i8* @malloc(i64 15) #2, !dbg !74
  %y3 = getelementptr inbounds %struct.struct_2, %struct.struct_2* %m, i32 0, i32 1, !dbg !75
  %6 = bitcast i8** %y3 to i8*, !dbg !75
  %7 = call i8* @llvm.ptr.annotation.p0i8(i8* %6, i8* getelementptr inbounds ([48 x i8], [48 x i8]* @.str, i32 0, i32 0), i8* getelementptr inbounds ([46 x i8], [46 x i8]* @.str.4, i32 0, i32 0), i32 13), !dbg !75
  %8 = bitcast i8* %7 to i8**, !dbg !75
  store i8* %call2, i8** %8, align 8, !dbg !76
  %call4 = call noalias i8* @malloc(i64 15) #2, !dbg !77
  %x5 = getelementptr inbounds %struct.my_struct_3, %struct.my_struct_3* %km, i32 0, i32 0, !dbg !78
  %9 = bitcast i8** %x5 to i8*, !dbg !78
  %10 = call i8* @llvm.ptr.annotation.p0i8(i8* %9, i8* getelementptr inbounds ([48 x i8], [48 x i8]* @.str, i32 0, i32 0), i8* getelementptr inbounds ([46 x i8], [46 x i8]* @.str.4, i32 0, i32 0), i32 17), !dbg !78
  %11 = bitcast i8* %10 to i8**, !dbg !78
  store i8* %call4, i8** %11, align 8, !dbg !79
  call void @llvm.dbg.declare(metadata i8** %s, metadata !80, metadata !DIExpression()), !dbg !81
  %call6 = call noalias i8* @malloc(i64 15) #2, !dbg !82
  store i8* %call6, i8** %s, align 8, !dbg !81
  call void @llvm.dbg.declare(metadata i8** %y7, metadata !83, metadata !DIExpression()), !dbg !84
  %12 = load i8*, i8** %s, align 8, !dbg !85
  %13 = bitcast %struct.my_struct* %k to { i8*, i8* }*, !dbg !86
  %14 = getelementptr inbounds { i8*, i8* }, { i8*, i8* }* %13, i32 0, i32 0, !dbg !86
  %15 = load i8*, i8** %14, align 8, !dbg !86
  %16 = getelementptr inbounds { i8*, i8* }, { i8*, i8* }* %13, i32 0, i32 1, !dbg !86
  %17 = load i8*, i8** %16, align 8, !dbg !86
  %call8 = call i8* @creates_obligation(i8* %12, i8* %15, i8* %17), !dbg !86
  store i8* %call8, i8** %y7, align 8, !dbg !84
  %18 = load i8*, i8** %y7, align 8, !dbg !87
  call void @free(i8* %18) #2, !dbg !88
  %y9 = getelementptr inbounds %struct.my_struct, %struct.my_struct* %k, i32 0, i32 1, !dbg !89
  %19 = bitcast i8** %y9 to i8*, !dbg !89
  %20 = call i8* @llvm.ptr.annotation.p0i8(i8* %19, i8* getelementptr inbounds ([48 x i8], [48 x i8]* @.str, i32 0, i32 0), i8* getelementptr inbounds ([46 x i8], [46 x i8]* @.str.4, i32 0, i32 0), i32 8), !dbg !89
  %21 = bitcast i8* %20 to i8**, !dbg !89
  %22 = load i8*, i8** %21, align 8, !dbg !89
  call void @does_free(i8* %22), !dbg !90
  %y10 = getelementptr inbounds %struct.struct_2, %struct.struct_2* %m, i32 0, i32 1, !dbg !91
  %23 = bitcast i8** %y10 to i8*, !dbg !91
  %24 = call i8* @llvm.ptr.annotation.p0i8(i8* %23, i8* getelementptr inbounds ([48 x i8], [48 x i8]* @.str, i32 0, i32 0), i8* getelementptr inbounds ([46 x i8], [46 x i8]* @.str.4, i32 0, i32 0), i32 13), !dbg !91
  %25 = bitcast i8* %24 to i8**, !dbg !91
  %26 = load i8*, i8** %25, align 8, !dbg !91
  call void @does_free(i8* %26), !dbg !92
  %x11 = getelementptr inbounds %struct.my_struct_3, %struct.my_struct_3* %km, i32 0, i32 0, !dbg !93
  %27 = bitcast i8** %x11 to i8*, !dbg !93
  %28 = call i8* @llvm.ptr.annotation.p0i8(i8* %27, i8* getelementptr inbounds ([48 x i8], [48 x i8]* @.str, i32 0, i32 0), i8* getelementptr inbounds ([46 x i8], [46 x i8]* @.str.4, i32 0, i32 0), i32 17), !dbg !93
  %29 = bitcast i8* %28 to i8**, !dbg !93
  %30 = load i8*, i8** %29, align 8, !dbg !93
  call void @does_free(i8* %30), !dbg !94
  ret i32 0, !dbg !95
}

attributes #0 = { noinline nounwind uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { nounwind readnone speculatable }
attributes #2 = { nounwind }
attributes #3 = { nounwind "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #4 = { argmemonly nounwind }

!llvm.dbg.cu = !{!0}
!llvm.module.flags = !{!6, !7, !8}
!llvm.ident = !{!9}

!0 = distinct !DICompileUnit(language: DW_LANG_C99, file: !1, producer: "clang version 8.0.1- (branches/release_80)", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, enums: !2, retainedTypes: !3, nameTableKind: None)
!1 = !DIFile(filename: "../test/nesting_test/subdir/runner.c", directory: "/c-resource-leak-checker/build")
!2 = !{}
!3 = !{!4}
!4 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !5, size: 64)
!5 = !DIBasicType(name: "char", size: 8, encoding: DW_ATE_signed_char)
!6 = !{i32 2, !"Dwarf Version", i32 4}
!7 = !{i32 2, !"Debug Info Version", i32 3}
!8 = !{i32 1, !"wchar_size", i32 4}
!9 = !{!"clang version 8.0.1- (branches/release_80)"}
!10 = distinct !DISubprogram(name: "does_free", scope: !1, file: !1, line: 3, type: !11, scopeLine: 3, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition, unit: !0, retainedNodes: !2)
!11 = !DISubroutineType(types: !12)
!12 = !{null, !4}
!13 = !DILocalVariable(name: "s", arg: 1, scope: !10, file: !1, line: 3, type: !4)
!14 = !DILocation(line: 3, column: 22, scope: !10)
!15 = !DILocation(line: 4, column: 10, scope: !10)
!16 = !DILocation(line: 4, column: 5, scope: !10)
!17 = !DILocation(line: 5, column: 1, scope: !10)
!18 = distinct !DISubprogram(name: "creates_obligation", scope: !1, file: !1, line: 7, type: !19, scopeLine: 7, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition, unit: !0, retainedNodes: !2)
!19 = !DISubroutineType(types: !20)
!20 = !{!4, !4, !21}
!21 = !DIDerivedType(tag: DW_TAG_typedef, name: "my_struct", file: !22, line: 9, baseType: !23)
!22 = !DIFile(filename: "../test/nesting_test/subdir/../call_structs.h", directory: "/c-resource-leak-checker/build")
!23 = distinct !DICompositeType(tag: DW_TAG_structure_type, name: "my_struct", file: !22, line: 6, size: 128, elements: !24)
!24 = !{!25, !26}
!25 = !DIDerivedType(tag: DW_TAG_member, name: "x", scope: !23, file: !22, line: 7, baseType: !4, size: 64)
!26 = !DIDerivedType(tag: DW_TAG_member, name: "y", scope: !23, file: !22, line: 8, baseType: !4, size: 64, offset: 64)
!27 = !DILocalVariable(name: "s", arg: 1, scope: !18, file: !1, line: 7, type: !4)
!28 = !DILocation(line: 7, column: 49, scope: !18)
!29 = !DILocalVariable(name: "X", arg: 2, scope: !18, file: !1, line: 7, type: !21)
!30 = !DILocation(line: 7, column: 76, scope: !18)
!31 = !DILocation(line: 8, column: 10, scope: !18)
!32 = !DILocation(line: 8, column: 5, scope: !18)
!33 = !DILocation(line: 9, column: 12, scope: !18)
!34 = !DILocation(line: 9, column: 5, scope: !18)
!35 = !DILocalVariable(name: "str", scope: !18, file: !1, line: 10, type: !4)
!36 = !DILocation(line: 10, column: 11, scope: !18)
!37 = !DILocation(line: 10, column: 24, scope: !18)
!38 = !DILocation(line: 11, column: 12, scope: !18)
!39 = !DILocation(line: 11, column: 5, scope: !18)
!40 = distinct !DISubprogram(name: "does_something", scope: !1, file: !1, line: 14, type: !41, scopeLine: 14, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition, unit: !0, retainedNodes: !2)
!41 = !DISubroutineType(types: !42)
!42 = !{!23, !21}
!43 = !DILocalVariable(name: "S", arg: 1, scope: !40, file: !1, line: 14, type: !21)
!44 = !DILocation(line: 14, column: 55, scope: !40)
!45 = !DILocation(line: 15, column: 12, scope: !40)
!46 = !DILocation(line: 15, column: 5, scope: !40)
!47 = !DILocation(line: 16, column: 12, scope: !40)
!48 = !DILocation(line: 16, column: 5, scope: !40)
!49 = distinct !DISubprogram(name: "main", scope: !1, file: !1, line: 19, type: !50, scopeLine: 19, spFlags: DISPFlagDefinition, unit: !0, retainedNodes: !2)
!50 = !DISubroutineType(types: !51)
!51 = !{!52}
!52 = !DIBasicType(name: "int", size: 32, encoding: DW_ATE_signed)
!53 = !DILocalVariable(name: "k", scope: !49, file: !1, line: 20, type: !23)
!54 = !DILocation(line: 20, column: 22, scope: !49)
!55 = !DILocalVariable(name: "m", scope: !49, file: !1, line: 21, type: !56)
!56 = !DIDerivedType(tag: DW_TAG_typedef, name: "my_struct_2", file: !22, line: 14, baseType: !57)
!57 = distinct !DICompositeType(tag: DW_TAG_structure_type, name: "struct_2", file: !22, line: 11, size: 128, elements: !58)
!58 = !{!59, !60}
!59 = !DIDerivedType(tag: DW_TAG_member, name: "x", scope: !57, file: !22, line: 12, baseType: !4, size: 64)
!60 = !DIDerivedType(tag: DW_TAG_member, name: "y", scope: !57, file: !22, line: 13, baseType: !4, size: 64, offset: 64)
!61 = !DILocation(line: 21, column: 17, scope: !49)
!62 = !DILocalVariable(name: "km", scope: !49, file: !1, line: 22, type: !63)
!63 = distinct !DICompositeType(tag: DW_TAG_structure_type, name: "my_struct_3", file: !22, line: 16, size: 128, elements: !64)
!64 = !{!65, !66}
!65 = !DIDerivedType(tag: DW_TAG_member, name: "x", scope: !63, file: !22, line: 17, baseType: !4, size: 64)
!66 = !DIDerivedType(tag: DW_TAG_member, name: "y", scope: !63, file: !22, line: 18, baseType: !4, size: 64, offset: 64)
!67 = !DILocation(line: 22, column: 24, scope: !49)
!68 = !DILocation(line: 24, column: 18, scope: !49)
!69 = !DILocation(line: 24, column: 7, scope: !49)
!70 = !DILocation(line: 24, column: 9, scope: !49)
!71 = !DILocation(line: 25, column: 18, scope: !49)
!72 = !DILocation(line: 25, column: 7, scope: !49)
!73 = !DILocation(line: 25, column: 9, scope: !49)
!74 = !DILocation(line: 28, column: 18, scope: !49)
!75 = !DILocation(line: 28, column: 7, scope: !49)
!76 = !DILocation(line: 28, column: 9, scope: !49)
!77 = !DILocation(line: 29, column: 19, scope: !49)
!78 = !DILocation(line: 29, column: 8, scope: !49)
!79 = !DILocation(line: 29, column: 10, scope: !49)
!80 = !DILocalVariable(name: "s", scope: !49, file: !1, line: 31, type: !4)
!81 = !DILocation(line: 31, column: 11, scope: !49)
!82 = !DILocation(line: 31, column: 22, scope: !49)
!83 = !DILocalVariable(name: "y", scope: !49, file: !1, line: 32, type: !4)
!84 = !DILocation(line: 32, column: 11, scope: !49)
!85 = !DILocation(line: 32, column: 34, scope: !49)
!86 = !DILocation(line: 32, column: 15, scope: !49)
!87 = !DILocation(line: 34, column: 10, scope: !49)
!88 = !DILocation(line: 34, column: 5, scope: !49)
!89 = !DILocation(line: 35, column: 17, scope: !49)
!90 = !DILocation(line: 35, column: 5, scope: !49)
!91 = !DILocation(line: 37, column: 17, scope: !49)
!92 = !DILocation(line: 37, column: 5, scope: !49)
!93 = !DILocation(line: 38, column: 18, scope: !49)
!94 = !DILocation(line: 38, column: 5, scope: !49)
!95 = !DILocation(line: 42, column: 1, scope: !49)
