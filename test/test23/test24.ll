; ModuleID = '../test/test23/test24.c'
source_filename = "../test/test23/test24.c"
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

%struct.my_struct = type { i8*, i8* }
%struct.struct_2 = type { i8*, i8* }
%struct.my_struct_3 = type { i8*, i8* }

@.str = private unnamed_addr constant [48 x i8] c"TOOL_CHECKER MustCall target = _ methods = free\00", section "llvm.metadata"
@.str.1 = private unnamed_addr constant [24 x i8] c"../test/test23/test24.c\00", section "llvm.metadata"
@.str.2 = private unnamed_addr constant [45 x i8] c"TOOL_CHECKER Calls target = _ methods = free\00", section "llvm.metadata"
@.str.3 = private unnamed_addr constant [54 x i8] c"TOOL_CHECKER Calls target = _.FIELD(x) methods = free\00", section "llvm.metadata"
@.str.4 = private unnamed_addr constant [24 x i8] c"../test/test23/test23.h\00", section "llvm.metadata"
@llvm.global.annotations = appending global [2 x { i8*, i8*, i8*, i32 }] [{ i8*, i8*, i8*, i32 } { i8* bitcast (i8* (i8*, i8*, i8*)* @creates_obligation to i8*), i8* getelementptr inbounds ([48 x i8], [48 x i8]* @.str, i32 0, i32 0), i8* getelementptr inbounds ([24 x i8], [24 x i8]* @.str.1, i32 0, i32 0), i32 16 }, { i8*, i8*, i8*, i32 } { i8* bitcast ({ i8*, i8* } (i8*, i8*)* @does_something to i8*), i8* getelementptr inbounds ([54 x i8], [54 x i8]* @.str.3, i32 0, i32 0), i8* getelementptr inbounds ([24 x i8], [24 x i8]* @.str.1, i32 0, i32 0), i32 23 }], section "llvm.metadata"

; Function Attrs: noinline nounwind uwtable
define dso_local void @does_free(i8* %s) #0 !dbg !10 {
entry:
  %s.addr = alloca i8*, align 8
  store i8* %s, i8** %s.addr, align 8
  call void @llvm.dbg.declare(metadata i8** %s.addr, metadata !13, metadata !DIExpression()), !dbg !14
  %s.addr1 = bitcast i8** %s.addr to i8*
  call void @llvm.var.annotation(i8* %s.addr1, i8* getelementptr inbounds ([48 x i8], [48 x i8]* @.str, i32 0, i32 0), i8* getelementptr inbounds ([24 x i8], [24 x i8]* @.str.1, i32 0, i32 0), i32 12)
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
  call void @llvm.dbg.declare(metadata i8** %s.addr, metadata !26, metadata !DIExpression()), !dbg !27
  %s.addr1 = bitcast i8** %s.addr to i8*
  call void @llvm.var.annotation(i8* %s.addr1, i8* getelementptr inbounds ([45 x i8], [45 x i8]* @.str.2, i32 0, i32 0), i8* getelementptr inbounds ([24 x i8], [24 x i8]* @.str.1, i32 0, i32 0), i32 16)
  call void @llvm.dbg.declare(metadata %struct.my_struct* %X, metadata !28, metadata !DIExpression()), !dbg !29
  %X2 = bitcast %struct.my_struct* %X to i8*
  call void @llvm.var.annotation(i8* %X2, i8* getelementptr inbounds ([54 x i8], [54 x i8]* @.str.3, i32 0, i32 0), i8* getelementptr inbounds ([24 x i8], [24 x i8]* @.str.1, i32 0, i32 0), i32 16)
  %3 = load i8*, i8** %s.addr, align 8, !dbg !30
  call void @free(i8* %3) #2, !dbg !31
  %x = getelementptr inbounds %struct.my_struct, %struct.my_struct* %X, i32 0, i32 0, !dbg !32
  %4 = bitcast i8** %x to i8*, !dbg !32
  %5 = call i8* @llvm.ptr.annotation.p0i8(i8* %4, i8* getelementptr inbounds ([48 x i8], [48 x i8]* @.str, i32 0, i32 0), i8* getelementptr inbounds ([24 x i8], [24 x i8]* @.str.4, i32 0, i32 0), i32 7), !dbg !32
  %6 = bitcast i8* %5 to i8**, !dbg !32
  %7 = load i8*, i8** %6, align 8, !dbg !32
  call void @free(i8* %7) #2, !dbg !33
  call void @llvm.dbg.declare(metadata i8** %str, metadata !34, metadata !DIExpression()), !dbg !35
  %call = call noalias i8* @malloc(i64 15) #2, !dbg !36
  store i8* %call, i8** %str, align 8, !dbg !35
  %8 = load i8*, i8** %str, align 8, !dbg !37
  ret i8* %8, !dbg !38
}

; Function Attrs: nounwind
declare i8* @llvm.ptr.annotation.p0i8(i8*, i8*, i8*, i32) #2

; Function Attrs: nounwind
declare dso_local noalias i8* @malloc(i64) #3

; Function Attrs: noinline nounwind uwtable
define dso_local { i8*, i8* } @does_something(i8* %S.coerce0, i8* %S.coerce1) #0 !dbg !39 {
entry:
  %retval = alloca %struct.my_struct, align 8
  %S = alloca %struct.my_struct, align 8
  %0 = bitcast %struct.my_struct* %S to { i8*, i8* }*
  %1 = getelementptr inbounds { i8*, i8* }, { i8*, i8* }* %0, i32 0, i32 0
  store i8* %S.coerce0, i8** %1, align 8
  %2 = getelementptr inbounds { i8*, i8* }, { i8*, i8* }* %0, i32 0, i32 1
  store i8* %S.coerce1, i8** %2, align 8
  call void @llvm.dbg.declare(metadata %struct.my_struct* %S, metadata !42, metadata !DIExpression()), !dbg !43
  %S1 = bitcast %struct.my_struct* %S to i8*
  call void @llvm.var.annotation(i8* %S1, i8* getelementptr inbounds ([54 x i8], [54 x i8]* @.str.3, i32 0, i32 0), i8* getelementptr inbounds ([24 x i8], [24 x i8]* @.str.1, i32 0, i32 0), i32 23)
  %x = getelementptr inbounds %struct.my_struct, %struct.my_struct* %S, i32 0, i32 0, !dbg !44
  %3 = bitcast i8** %x to i8*, !dbg !44
  %4 = call i8* @llvm.ptr.annotation.p0i8(i8* %3, i8* getelementptr inbounds ([48 x i8], [48 x i8]* @.str, i32 0, i32 0), i8* getelementptr inbounds ([24 x i8], [24 x i8]* @.str.4, i32 0, i32 0), i32 7), !dbg !44
  %5 = bitcast i8* %4 to i8**, !dbg !44
  %6 = load i8*, i8** %5, align 8, !dbg !44
  call void @free(i8* %6) #2, !dbg !45
  %7 = bitcast %struct.my_struct* %retval to i8*, !dbg !46
  %8 = bitcast %struct.my_struct* %S to i8*, !dbg !46
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* align 8 %7, i8* align 8 %8, i64 16, i1 false), !dbg !46
  %9 = bitcast %struct.my_struct* %retval to { i8*, i8* }*, !dbg !47
  %10 = load { i8*, i8* }, { i8*, i8* }* %9, align 8, !dbg !47
  ret { i8*, i8* } %10, !dbg !47
}

; Function Attrs: argmemonly nounwind
declare void @llvm.memcpy.p0i8.p0i8.i64(i8* nocapture writeonly, i8* nocapture readonly, i64, i1) #4

; Function Attrs: noinline nounwind uwtable
define dso_local i32 @main() #0 !dbg !48 {
entry:
  %k = alloca %struct.my_struct, align 8
  %m = alloca %struct.struct_2, align 8
  %km = alloca %struct.my_struct_3, align 8
  %s = alloca i8*, align 8
  %y7 = alloca i8*, align 8
  call void @llvm.dbg.declare(metadata %struct.my_struct* %k, metadata !52, metadata !DIExpression()), !dbg !53
  call void @llvm.dbg.declare(metadata %struct.struct_2* %m, metadata !54, metadata !DIExpression()), !dbg !60
  call void @llvm.dbg.declare(metadata %struct.my_struct_3* %km, metadata !61, metadata !DIExpression()), !dbg !66
  %call = call noalias i8* @malloc(i64 15) #2, !dbg !67
  %x = getelementptr inbounds %struct.my_struct, %struct.my_struct* %k, i32 0, i32 0, !dbg !68
  %0 = bitcast i8** %x to i8*, !dbg !68
  %1 = call i8* @llvm.ptr.annotation.p0i8(i8* %0, i8* getelementptr inbounds ([48 x i8], [48 x i8]* @.str, i32 0, i32 0), i8* getelementptr inbounds ([24 x i8], [24 x i8]* @.str.4, i32 0, i32 0), i32 7), !dbg !68
  %2 = bitcast i8* %1 to i8**, !dbg !68
  store i8* %call, i8** %2, align 8, !dbg !69
  %call1 = call noalias i8* @malloc(i64 15) #2, !dbg !70
  %y = getelementptr inbounds %struct.my_struct, %struct.my_struct* %k, i32 0, i32 1, !dbg !71
  %3 = bitcast i8** %y to i8*, !dbg !71
  %4 = call i8* @llvm.ptr.annotation.p0i8(i8* %3, i8* getelementptr inbounds ([48 x i8], [48 x i8]* @.str, i32 0, i32 0), i8* getelementptr inbounds ([24 x i8], [24 x i8]* @.str.4, i32 0, i32 0), i32 8), !dbg !71
  %5 = bitcast i8* %4 to i8**, !dbg !71
  store i8* %call1, i8** %5, align 8, !dbg !72
  %call2 = call noalias i8* @malloc(i64 15) #2, !dbg !73
  %y3 = getelementptr inbounds %struct.struct_2, %struct.struct_2* %m, i32 0, i32 1, !dbg !74
  %6 = bitcast i8** %y3 to i8*, !dbg !74
  %7 = call i8* @llvm.ptr.annotation.p0i8(i8* %6, i8* getelementptr inbounds ([48 x i8], [48 x i8]* @.str, i32 0, i32 0), i8* getelementptr inbounds ([24 x i8], [24 x i8]* @.str.4, i32 0, i32 0), i32 13), !dbg !74
  %8 = bitcast i8* %7 to i8**, !dbg !74
  store i8* %call2, i8** %8, align 8, !dbg !75
  %call4 = call noalias i8* @malloc(i64 15) #2, !dbg !76
  %x5 = getelementptr inbounds %struct.my_struct_3, %struct.my_struct_3* %km, i32 0, i32 0, !dbg !77
  %9 = bitcast i8** %x5 to i8*, !dbg !77
  %10 = call i8* @llvm.ptr.annotation.p0i8(i8* %9, i8* getelementptr inbounds ([48 x i8], [48 x i8]* @.str, i32 0, i32 0), i8* getelementptr inbounds ([24 x i8], [24 x i8]* @.str.4, i32 0, i32 0), i32 17), !dbg !77
  %11 = bitcast i8* %10 to i8**, !dbg !77
  store i8* %call4, i8** %11, align 8, !dbg !78
  call void @llvm.dbg.declare(metadata i8** %s, metadata !79, metadata !DIExpression()), !dbg !80
  %call6 = call noalias i8* @malloc(i64 15) #2, !dbg !81
  store i8* %call6, i8** %s, align 8, !dbg !80
  call void @llvm.dbg.declare(metadata i8** %y7, metadata !82, metadata !DIExpression()), !dbg !83
  %12 = load i8*, i8** %s, align 8, !dbg !84
  %13 = bitcast %struct.my_struct* %k to { i8*, i8* }*, !dbg !85
  %14 = getelementptr inbounds { i8*, i8* }, { i8*, i8* }* %13, i32 0, i32 0, !dbg !85
  %15 = load i8*, i8** %14, align 8, !dbg !85
  %16 = getelementptr inbounds { i8*, i8* }, { i8*, i8* }* %13, i32 0, i32 1, !dbg !85
  %17 = load i8*, i8** %16, align 8, !dbg !85
  %call8 = call i8* @creates_obligation(i8* %12, i8* %15, i8* %17), !dbg !85
  store i8* %call8, i8** %y7, align 8, !dbg !83
  %18 = load i8*, i8** %y7, align 8, !dbg !86
  call void @free(i8* %18) #2, !dbg !87
  %y9 = getelementptr inbounds %struct.my_struct, %struct.my_struct* %k, i32 0, i32 1, !dbg !88
  %19 = bitcast i8** %y9 to i8*, !dbg !88
  %20 = call i8* @llvm.ptr.annotation.p0i8(i8* %19, i8* getelementptr inbounds ([48 x i8], [48 x i8]* @.str, i32 0, i32 0), i8* getelementptr inbounds ([24 x i8], [24 x i8]* @.str.4, i32 0, i32 0), i32 8), !dbg !88
  %21 = bitcast i8* %20 to i8**, !dbg !88
  %22 = load i8*, i8** %21, align 8, !dbg !88
  call void @does_free(i8* %22), !dbg !89
  %y10 = getelementptr inbounds %struct.struct_2, %struct.struct_2* %m, i32 0, i32 1, !dbg !90
  %23 = bitcast i8** %y10 to i8*, !dbg !90
  %24 = call i8* @llvm.ptr.annotation.p0i8(i8* %23, i8* getelementptr inbounds ([48 x i8], [48 x i8]* @.str, i32 0, i32 0), i8* getelementptr inbounds ([24 x i8], [24 x i8]* @.str.4, i32 0, i32 0), i32 13), !dbg !90
  %25 = bitcast i8* %24 to i8**, !dbg !90
  %26 = load i8*, i8** %25, align 8, !dbg !90
  call void @does_free(i8* %26), !dbg !91
  %x11 = getelementptr inbounds %struct.my_struct_3, %struct.my_struct_3* %km, i32 0, i32 0, !dbg !92
  %27 = bitcast i8** %x11 to i8*, !dbg !92
  %28 = call i8* @llvm.ptr.annotation.p0i8(i8* %27, i8* getelementptr inbounds ([48 x i8], [48 x i8]* @.str, i32 0, i32 0), i8* getelementptr inbounds ([24 x i8], [24 x i8]* @.str.4, i32 0, i32 0), i32 17), !dbg !92
  %29 = bitcast i8* %28 to i8**, !dbg !92
  %30 = load i8*, i8** %29, align 8, !dbg !92
  call void @does_free(i8* %30), !dbg !93
  ret i32 0, !dbg !94
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
!1 = !DIFile(filename: "../test/test23/test24.c", directory: "/c-resource-leak-checker/build")
!2 = !{}
!3 = !{!4}
!4 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !5, size: 64)
!5 = !DIBasicType(name: "char", size: 8, encoding: DW_ATE_signed_char)
!6 = !{i32 2, !"Dwarf Version", i32 4}
!7 = !{i32 2, !"Debug Info Version", i32 3}
!8 = !{i32 1, !"wchar_size", i32 4}
!9 = !{!"clang version 8.0.1- (branches/release_80)"}
!10 = distinct !DISubprogram(name: "does_free", scope: !1, file: !1, line: 12, type: !11, scopeLine: 12, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition, unit: !0, retainedNodes: !2)
!11 = !DISubroutineType(types: !12)
!12 = !{null, !4}
!13 = !DILocalVariable(name: "s", arg: 1, scope: !10, file: !1, line: 12, type: !4)
!14 = !DILocation(line: 12, column: 22, scope: !10)
!15 = !DILocation(line: 13, column: 10, scope: !10)
!16 = !DILocation(line: 13, column: 5, scope: !10)
!17 = !DILocation(line: 14, column: 1, scope: !10)
!18 = distinct !DISubprogram(name: "creates_obligation", scope: !1, file: !1, line: 16, type: !19, scopeLine: 16, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition, unit: !0, retainedNodes: !2)
!19 = !DISubroutineType(types: !20)
!20 = !{!4, !4, !21}
!21 = distinct !DICompositeType(tag: DW_TAG_structure_type, name: "my_struct", file: !22, line: 6, size: 128, elements: !23)
!22 = !DIFile(filename: "../test/test23/test23.h", directory: "/c-resource-leak-checker/build")
!23 = !{!24, !25}
!24 = !DIDerivedType(tag: DW_TAG_member, name: "x", scope: !21, file: !22, line: 7, baseType: !4, size: 64)
!25 = !DIDerivedType(tag: DW_TAG_member, name: "y", scope: !21, file: !22, line: 8, baseType: !4, size: 64, offset: 64)
!26 = !DILocalVariable(name: "s", arg: 1, scope: !18, file: !1, line: 16, type: !4)
!27 = !DILocation(line: 16, column: 32, scope: !18)
!28 = !DILocalVariable(name: "X", arg: 2, scope: !18, file: !1, line: 16, type: !21)
!29 = !DILocation(line: 16, column: 52, scope: !18)
!30 = !DILocation(line: 17, column: 10, scope: !18)
!31 = !DILocation(line: 17, column: 5, scope: !18)
!32 = !DILocation(line: 18, column: 12, scope: !18)
!33 = !DILocation(line: 18, column: 5, scope: !18)
!34 = !DILocalVariable(name: "str", scope: !18, file: !1, line: 19, type: !4)
!35 = !DILocation(line: 19, column: 11, scope: !18)
!36 = !DILocation(line: 19, column: 24, scope: !18)
!37 = !DILocation(line: 20, column: 12, scope: !18)
!38 = !DILocation(line: 20, column: 5, scope: !18)
!39 = distinct !DISubprogram(name: "does_something", scope: !1, file: !1, line: 23, type: !40, scopeLine: 23, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition, unit: !0, retainedNodes: !2)
!40 = !DISubroutineType(types: !41)
!41 = !{!21, !21}
!42 = !DILocalVariable(name: "S", arg: 1, scope: !39, file: !1, line: 23, type: !21)
!43 = !DILocation(line: 23, column: 50, scope: !39)
!44 = !DILocation(line: 24, column: 12, scope: !39)
!45 = !DILocation(line: 24, column: 5, scope: !39)
!46 = !DILocation(line: 25, column: 12, scope: !39)
!47 = !DILocation(line: 25, column: 5, scope: !39)
!48 = distinct !DISubprogram(name: "main", scope: !1, file: !1, line: 28, type: !49, scopeLine: 28, spFlags: DISPFlagDefinition, unit: !0, retainedNodes: !2)
!49 = !DISubroutineType(types: !50)
!50 = !{!51}
!51 = !DIBasicType(name: "int", size: 32, encoding: DW_ATE_signed)
!52 = !DILocalVariable(name: "k", scope: !48, file: !1, line: 33, type: !21)
!53 = !DILocation(line: 33, column: 22, scope: !48)
!54 = !DILocalVariable(name: "m", scope: !48, file: !1, line: 34, type: !55)
!55 = !DIDerivedType(tag: DW_TAG_typedef, name: "my_struct_2", file: !22, line: 14, baseType: !56)
!56 = distinct !DICompositeType(tag: DW_TAG_structure_type, name: "struct_2", file: !22, line: 11, size: 128, elements: !57)
!57 = !{!58, !59}
!58 = !DIDerivedType(tag: DW_TAG_member, name: "x", scope: !56, file: !22, line: 12, baseType: !4, size: 64)
!59 = !DIDerivedType(tag: DW_TAG_member, name: "y", scope: !56, file: !22, line: 13, baseType: !4, size: 64, offset: 64)
!60 = !DILocation(line: 34, column: 17, scope: !48)
!61 = !DILocalVariable(name: "km", scope: !48, file: !1, line: 35, type: !62)
!62 = distinct !DICompositeType(tag: DW_TAG_structure_type, name: "my_struct_3", file: !22, line: 16, size: 128, elements: !63)
!63 = !{!64, !65}
!64 = !DIDerivedType(tag: DW_TAG_member, name: "x", scope: !62, file: !22, line: 17, baseType: !4, size: 64)
!65 = !DIDerivedType(tag: DW_TAG_member, name: "y", scope: !62, file: !22, line: 18, baseType: !4, size: 64, offset: 64)
!66 = !DILocation(line: 35, column: 24, scope: !48)
!67 = !DILocation(line: 37, column: 18, scope: !48)
!68 = !DILocation(line: 37, column: 7, scope: !48)
!69 = !DILocation(line: 37, column: 9, scope: !48)
!70 = !DILocation(line: 38, column: 18, scope: !48)
!71 = !DILocation(line: 38, column: 7, scope: !48)
!72 = !DILocation(line: 38, column: 9, scope: !48)
!73 = !DILocation(line: 41, column: 18, scope: !48)
!74 = !DILocation(line: 41, column: 7, scope: !48)
!75 = !DILocation(line: 41, column: 9, scope: !48)
!76 = !DILocation(line: 42, column: 19, scope: !48)
!77 = !DILocation(line: 42, column: 8, scope: !48)
!78 = !DILocation(line: 42, column: 10, scope: !48)
!79 = !DILocalVariable(name: "s", scope: !48, file: !1, line: 44, type: !4)
!80 = !DILocation(line: 44, column: 11, scope: !48)
!81 = !DILocation(line: 44, column: 22, scope: !48)
!82 = !DILocalVariable(name: "y", scope: !48, file: !1, line: 45, type: !4)
!83 = !DILocation(line: 45, column: 11, scope: !48)
!84 = !DILocation(line: 45, column: 34, scope: !48)
!85 = !DILocation(line: 45, column: 15, scope: !48)
!86 = !DILocation(line: 47, column: 10, scope: !48)
!87 = !DILocation(line: 47, column: 5, scope: !48)
!88 = !DILocation(line: 48, column: 17, scope: !48)
!89 = !DILocation(line: 48, column: 5, scope: !48)
!90 = !DILocation(line: 50, column: 17, scope: !48)
!91 = !DILocation(line: 50, column: 5, scope: !48)
!92 = !DILocation(line: 51, column: 18, scope: !48)
!93 = !DILocation(line: 51, column: 5, scope: !48)
!94 = !DILocation(line: 55, column: 1, scope: !48)
