; ModuleID = 'program.bc'
source_filename = "program.c"
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@.str = private unnamed_addr constant [21 x i8] c"Values: %ld %ld %ld\0A\00", align 1
@.str.1 = private unnamed_addr constant [23 x i8] c"CAT invocations = %ld\0A\00", align 1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i8** @function_that_complicates_everything(i32, i8**, i8**) #0 {
  %4 = alloca i8**, align 8
  %5 = alloca i32, align 4
  %6 = alloca i8**, align 8
  %7 = alloca i8**, align 8
  store i32 %0, i32* %5, align 4
  store i8** %1, i8*** %6, align 8
  store i8** %2, i8*** %7, align 8
  %8 = load i32, i32* %5, align 4
  %9 = icmp sgt i32 %8, 5
  br i1 %9, label %10, label %12

; <label>:10:                                     ; preds = %3
  %11 = load i8**, i8*** %6, align 8
  store i8** %11, i8*** %4, align 8
  br label %14

; <label>:12:                                     ; preds = %3
  %13 = load i8**, i8*** %7, align 8
  store i8** %13, i8*** %4, align 8
  br label %14

; <label>:14:                                     ; preds = %12, %10
  %15 = load i8**, i8*** %4, align 8
  ret i8** %15
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main(i32, i8**) #0 {
  %3 = alloca i32, align 4
  %4 = alloca i32, align 4
  %5 = alloca i8**, align 8
  %6 = alloca i8*, align 8
  %7 = alloca i8*, align 8
  %8 = alloca i8**, align 8
  %9 = alloca i64, align 8
  %10 = alloca i64, align 8
  %11 = alloca i64, align 8
  store i32 0, i32* %3, align 4
  store i32 %0, i32* %4, align 4
  store i8** %1, i8*** %5, align 8
  %12 = call i8* @CAT_new(i64 5)
  store i8* %12, i8** %6, align 8
  %13 = call i8* @CAT_new(i64 7)
  store i8* %13, i8** %7, align 8
  %14 = load i32, i32* %4, align 4
  %15 = call i8** @function_that_complicates_everything(i32 %14, i8** %6, i8** %7)
  store i8** %15, i8*** %8, align 8
  %16 = load i8**, i8*** %8, align 8
  %17 = load i8*, i8** %16, align 8
  %18 = call i64 @CAT_get(i8* %17)
  store i64 %18, i64* %9, align 8
  %19 = load i8*, i8** %6, align 8
  %20 = call i64 @CAT_get(i8* %19)
  store i64 %20, i64* %10, align 8
  %21 = load i8*, i8** %7, align 8
  %22 = call i64 @CAT_get(i8* %21)
  store i64 %22, i64* %11, align 8
  %23 = load i64, i64* %9, align 8
  %24 = load i64, i64* %10, align 8
  %25 = load i64, i64* %11, align 8
  %26 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([21 x i8], [21 x i8]* @.str, i32 0, i32 0), i64 %23, i64 %24, i64 %25)
  %27 = call i64 @CAT_invocations()
  %28 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([23 x i8], [23 x i8]* @.str.1, i32 0, i32 0), i64 %27)
  ret i32 0
}

declare dso_local i8* @CAT_new(i64) #1

declare dso_local i64 @CAT_get(i8*) #1

declare dso_local i32 @printf(i8*, ...) #1

declare dso_local i64 @CAT_invocations() #1

attributes #0 = { noinline nounwind optnone uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.module.flags = !{!0}
!llvm.ident = !{!1}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{!"clang version 8.0.0 (tags/RELEASE_800/final)"}
