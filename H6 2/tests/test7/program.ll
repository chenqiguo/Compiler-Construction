; ModuleID = 'program.bc'
source_filename = "program.c"
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@.str = private unnamed_addr constant [17 x i8] c"H1: \09X    = %ld\0A\00", align 1
@.str.1 = private unnamed_addr constant [17 x i8] c"H1: \09Y    = %ld\0A\00", align 1
@.str.2 = private unnamed_addr constant [17 x i8] c"H1: \09Ref1 = %ld\0A\00", align 1
@.str.3 = private unnamed_addr constant [17 x i8] c"H1: \09Ref2 = %ld\0A\00", align 1
@.str.4 = private unnamed_addr constant [23 x i8] c"CAT invocations = %ld\0A\00", align 1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main(i32, i8**) #0 {
  %3 = alloca i32, align 4
  %4 = alloca i32, align 4
  %5 = alloca i8**, align 8
  %6 = alloca i8*, align 8
  %7 = alloca i8*, align 8
  %8 = alloca i8**, align 8
  %9 = alloca i8**, align 8
  store i32 0, i32* %3, align 4
  store i32 %0, i32* %4, align 4
  store i8** %1, i8*** %5, align 8
  %10 = call noalias i8* @malloc(i64 8) #3
  %11 = bitcast i8* %10 to i8**
  store i8** %11, i8*** %8, align 8
  %12 = call noalias i8* @malloc(i64 8) #3
  %13 = bitcast i8* %12 to i8**
  store i8** %13, i8*** %9, align 8
  %14 = call i8* @CAT_new(i64 8)
  store i8* %14, i8** %6, align 8
  %15 = call i8* @CAT_new(i64 42)
  store i8* %15, i8** %7, align 8
  %16 = load i8*, i8** %6, align 8
  %17 = load i8**, i8*** %8, align 8
  store i8* %16, i8** %17, align 8
  %18 = load i8*, i8** %7, align 8
  %19 = load i8**, i8*** %9, align 8
  store i8* %18, i8** %19, align 8
  %20 = load i8**, i8*** %9, align 8
  %21 = load i8*, i8** %20, align 8
  %22 = load i8*, i8** %6, align 8
  %23 = load i8*, i8** %7, align 8
  call void @CAT_add(i8* %21, i8* %22, i8* %23)
  %24 = load i8*, i8** %6, align 8
  %25 = call i64 @CAT_get(i8* %24)
  %26 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([17 x i8], [17 x i8]* @.str, i32 0, i32 0), i64 %25)
  %27 = load i8*, i8** %7, align 8
  %28 = call i64 @CAT_get(i8* %27)
  %29 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([17 x i8], [17 x i8]* @.str.1, i32 0, i32 0), i64 %28)
  %30 = load i8**, i8*** %8, align 8
  %31 = load i8*, i8** %30, align 8
  %32 = call i64 @CAT_get(i8* %31)
  %33 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([17 x i8], [17 x i8]* @.str.2, i32 0, i32 0), i64 %32)
  %34 = load i8**, i8*** %9, align 8
  %35 = load i8*, i8** %34, align 8
  %36 = call i64 @CAT_get(i8* %35)
  %37 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([17 x i8], [17 x i8]* @.str.3, i32 0, i32 0), i64 %36)
  %38 = load i8**, i8*** %8, align 8
  %39 = bitcast i8** %38 to i8*
  call void @free(i8* %39) #3
  %40 = load i8**, i8*** %9, align 8
  %41 = bitcast i8** %40 to i8*
  call void @free(i8* %41) #3
  %42 = call i64 @CAT_invocations()
  %43 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([23 x i8], [23 x i8]* @.str.4, i32 0, i32 0), i64 %42)
  ret i32 0
}

; Function Attrs: nounwind
declare dso_local noalias i8* @malloc(i64) #1

declare dso_local i8* @CAT_new(i64) #2

declare dso_local void @CAT_add(i8*, i8*, i8*) #2

declare dso_local i32 @printf(i8*, ...) #2

declare dso_local i64 @CAT_get(i8*) #2

; Function Attrs: nounwind
declare dso_local void @free(i8*) #1

declare dso_local i64 @CAT_invocations() #2

attributes #0 = { noinline nounwind optnone uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { nounwind "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #2 = { "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #3 = { nounwind }

!llvm.module.flags = !{!0}
!llvm.ident = !{!1}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{!"clang version 8.0.0 (tags/RELEASE_800/final)"}
