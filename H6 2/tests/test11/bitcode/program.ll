; ModuleID = 'program.bc'
source_filename = "program.c"
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@.str = private unnamed_addr constant [20 x i8] c"H1: \09Value 1 = %ld\0A\00", align 1
@.str.1 = private unnamed_addr constant [20 x i8] c"H1: \09Value 2 = %ld\0A\00", align 1
@.str.2 = private unnamed_addr constant [19 x i8] c"H1: \09Result = %ld\0A\00", align 1
@.str.3 = private unnamed_addr constant [23 x i8] c"CAT invocations = %ld\0A\00", align 1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @CAT_execution(i32) #0 {
  %2 = alloca i32, align 4
  %3 = alloca i8*, align 8
  %4 = alloca i8*, align 8
  %5 = alloca i8*, align 8
  store i32 %0, i32* %2, align 4
  %6 = call i8* @CAT_new(i64 5)
  store i8* %6, i8** %3, align 8
  %7 = load i8*, i8** %3, align 8
  %8 = call i64 @CAT_get(i8* %7)
  %9 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([20 x i8], [20 x i8]* @.str, i32 0, i32 0), i64 %8)
  %10 = call i8* @CAT_new(i64 8)
  store i8* %10, i8** %4, align 8
  %11 = load i32, i32* %2, align 4
  %12 = icmp sgt i32 %11, 10
  br i1 %12, label %13, label %17

; <label>:13:                                     ; preds = %1
  %14 = load i8*, i8** %4, align 8
  %15 = load i8*, i8** %4, align 8
  %16 = load i8*, i8** %4, align 8
  call void @CAT_add(i8* %14, i8* %15, i8* %16)
  br label %17

; <label>:17:                                     ; preds = %13, %1
  %18 = load i8*, i8** %4, align 8
  %19 = call i64 @CAT_get(i8* %18)
  %20 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([20 x i8], [20 x i8]* @.str.1, i32 0, i32 0), i64 %19)
  %21 = call i8* @CAT_new(i64 0)
  store i8* %21, i8** %5, align 8
  %22 = load i8*, i8** %5, align 8
  %23 = load i8*, i8** %3, align 8
  %24 = load i8*, i8** %4, align 8
  call void @CAT_add(i8* %22, i8* %23, i8* %24)
  %25 = load i8*, i8** %5, align 8
  %26 = load i8*, i8** %3, align 8
  %27 = load i8*, i8** %5, align 8
  call void @CAT_add(i8* %25, i8* %26, i8* %27)
  %28 = load i8*, i8** %5, align 8
  %29 = load i8*, i8** %5, align 8
  %30 = load i8*, i8** %5, align 8
  call void @CAT_add(i8* %28, i8* %29, i8* %30)
  %31 = load i8*, i8** %5, align 8
  %32 = load i8*, i8** %3, align 8
  %33 = load i8*, i8** %3, align 8
  call void @CAT_add(i8* %31, i8* %32, i8* %33)
  %34 = load i8*, i8** %5, align 8
  %35 = call i64 @CAT_get(i8* %34)
  %36 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([19 x i8], [19 x i8]* @.str.2, i32 0, i32 0), i64 %35)
  ret void
}

declare dso_local i8* @CAT_new(i64) #1

declare dso_local i32 @printf(i8*, ...) #1

declare dso_local i64 @CAT_get(i8*) #1

declare dso_local void @CAT_add(i8*, i8*, i8*) #1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main(i32, i8**) #0 {
  %3 = alloca i32, align 4
  %4 = alloca i32, align 4
  %5 = alloca i8**, align 8
  store i32 0, i32* %3, align 4
  store i32 %0, i32* %4, align 4
  store i8** %1, i8*** %5, align 8
  %6 = load i32, i32* %4, align 4
  call void @CAT_execution(i32 %6)
  %7 = call i64 @CAT_invocations()
  %8 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([23 x i8], [23 x i8]* @.str.3, i32 0, i32 0), i64 %7)
  ret i32 0
}

declare dso_local i64 @CAT_invocations() #1

attributes #0 = { noinline nounwind optnone uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.module.flags = !{!0}
!llvm.ident = !{!1}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{!"clang version 8.0.0 (tags/RELEASE_800/final)"}
