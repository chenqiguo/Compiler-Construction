; ModuleID = 'program.bc'
source_filename = "program.c"
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@.str = private unnamed_addr constant [14 x i8] c"H1: \09Y = %ld\0A\00", align 1
@.str.1 = private unnamed_addr constant [14 x i8] c"H1: \09X = %ld\0A\00", align 1
@.str.2 = private unnamed_addr constant [14 x i8] c"H1: \09Z = %ld\0A\00", align 1
@.str.3 = private unnamed_addr constant [23 x i8] c"CAT invocations = %ld\0A\00", align 1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main(i32, i8**) #0 {
  %3 = alloca i32, align 4
  %4 = alloca i32, align 4
  %5 = alloca i8**, align 8
  %6 = alloca i8*, align 8
  %7 = alloca i8*, align 8
  %8 = alloca i8*, align 8
  %9 = alloca i8**, align 8
  store i32 0, i32* %3, align 4
  store i32 %0, i32* %4, align 4
  store i8** %1, i8*** %5, align 8
  %10 = call noalias i8* @malloc(i64 8) #3
  %11 = bitcast i8* %10 to i8**
  store i8** %11, i8*** %9, align 8
  %12 = call i8* @CAT_new(i64 2)
  store i8* %12, i8** %8, align 8
  %13 = call i8* @CAT_new(i64 8)
  store i8* %13, i8** %6, align 8
  %14 = load i8*, i8** %6, align 8
  %15 = load i8**, i8*** %9, align 8
  store i8* %14, i8** %15, align 8
  %16 = load i8**, i8*** %9, align 8
  %17 = load i8*, i8** %16, align 8
  store i8* %17, i8** %7, align 8
  %18 = load i8*, i8** %7, align 8
  %19 = call i64 @CAT_get(i8* %18)
  %20 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([14 x i8], [14 x i8]* @.str, i32 0, i32 0), i64 %19)
  %21 = load i8*, i8** %6, align 8
  %22 = call i64 @CAT_get(i8* %21)
  %23 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([14 x i8], [14 x i8]* @.str.1, i32 0, i32 0), i64 %22)
  %24 = load i8*, i8** %8, align 8
  %25 = call i64 @CAT_get(i8* %24)
  %26 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([14 x i8], [14 x i8]* @.str.2, i32 0, i32 0), i64 %25)
  %27 = load i8**, i8*** %9, align 8
  %28 = load i8*, i8** %27, align 8
  %29 = load i8**, i8*** %9, align 8
  %30 = load i8*, i8** %29, align 8
  %31 = load i8**, i8*** %9, align 8
  %32 = load i8*, i8** %31, align 8
  call void @CAT_add(i8* %28, i8* %30, i8* %32)
  %33 = load i8*, i8** %7, align 8
  %34 = call i64 @CAT_get(i8* %33)
  %35 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([14 x i8], [14 x i8]* @.str, i32 0, i32 0), i64 %34)
  %36 = load i8*, i8** %6, align 8
  %37 = call i64 @CAT_get(i8* %36)
  %38 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([14 x i8], [14 x i8]* @.str.1, i32 0, i32 0), i64 %37)
  %39 = load i8*, i8** %8, align 8
  %40 = call i64 @CAT_get(i8* %39)
  %41 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([14 x i8], [14 x i8]* @.str.2, i32 0, i32 0), i64 %40)
  %42 = call i64 @CAT_invocations()
  %43 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([23 x i8], [23 x i8]* @.str.3, i32 0, i32 0), i64 %42)
  ret i32 0
}

; Function Attrs: nounwind
declare dso_local noalias i8* @malloc(i64) #1

declare dso_local i8* @CAT_new(i64) #2

declare dso_local i32 @printf(i8*, ...) #2

declare dso_local i64 @CAT_get(i8*) #2

declare dso_local void @CAT_add(i8*, i8*, i8*) #2

declare dso_local i64 @CAT_invocations() #2

attributes #0 = { noinline nounwind optnone uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { nounwind "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #2 = { "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #3 = { nounwind }

!llvm.module.flags = !{!0}
!llvm.ident = !{!1}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{!"clang version 8.0.0 (tags/RELEASE_800/final)"}
