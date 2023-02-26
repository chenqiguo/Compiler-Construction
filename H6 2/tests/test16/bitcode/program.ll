; ModuleID = 'program.bc'
source_filename = "program.c"
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@.str = private unnamed_addr constant [14 x i8] c"H1: \09Y = %ld\0A\00", align 1
@.str.1 = private unnamed_addr constant [20 x i8] c"H1: \09Value 1 = %ld\0A\00", align 1
@.str.2 = private unnamed_addr constant [20 x i8] c"H1: \09Value 2 = %ld\0A\00", align 1
@.str.3 = private unnamed_addr constant [19 x i8] c"H1: \09Result = %ld\0A\00", align 1
@.str.4 = private unnamed_addr constant [15 x i8] c"Result 2 = %d\0A\00", align 1
@.str.5 = private unnamed_addr constant [23 x i8] c"CAT invocations = %ld\0A\00", align 1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @another_execution(i32, i8*) #0 {
  %3 = alloca i32, align 4
  %4 = alloca i8*, align 8
  store i32 %0, i32* %3, align 4
  store i8* %1, i8** %4, align 8
  %5 = load i8*, i8** %4, align 8
  %6 = call i64 @CAT_get(i8* %5)
  %7 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([14 x i8], [14 x i8]* @.str, i32 0, i32 0), i64 %6)
  %8 = load i8*, i8** %4, align 8
  %9 = load i8*, i8** %4, align 8
  %10 = load i8*, i8** %4, align 8
  call void @CAT_add(i8* %8, i8* %9, i8* %10)
  %11 = load i8*, i8** %4, align 8
  %12 = call i64 @CAT_get(i8* %11)
  %13 = trunc i64 %12 to i32
  ret i32 %13
}

declare dso_local i32 @printf(i8*, ...) #1

declare dso_local i64 @CAT_get(i8*) #1

declare dso_local void @CAT_add(i8*, i8*, i8*) #1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @CAT_execution(i32, i8*, i32) #0 {
  %4 = alloca i32, align 4
  %5 = alloca i8*, align 8
  %6 = alloca i32, align 4
  %7 = alloca i8*, align 8
  %8 = alloca i8*, align 8
  store i32 %0, i32* %4, align 4
  store i8* %1, i8** %5, align 8
  store i32 %2, i32* %6, align 4
  %9 = call i8* @CAT_new(i64 8)
  store i8* %9, i8** %7, align 8
  %10 = load i32, i32* %4, align 4
  %11 = icmp sgt i32 %10, 10
  br i1 %11, label %12, label %16

; <label>:12:                                     ; preds = %3
  %13 = load i8*, i8** %5, align 8
  %14 = load i8*, i8** %7, align 8
  %15 = load i8*, i8** %7, align 8
  call void @CAT_add(i8* %13, i8* %14, i8* %15)
  br label %16

; <label>:16:                                     ; preds = %12, %3
  %17 = load i8*, i8** %5, align 8
  %18 = call i64 @CAT_get(i8* %17)
  %19 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([20 x i8], [20 x i8]* @.str.1, i32 0, i32 0), i64 %18)
  %20 = load i32, i32* %6, align 4
  %21 = icmp sgt i32 %20, 0
  br i1 %21, label %22, label %26

; <label>:22:                                     ; preds = %16
  %23 = load i8*, i8** %7, align 8
  %24 = call i64 @CAT_get(i8* %23)
  %25 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([20 x i8], [20 x i8]* @.str.2, i32 0, i32 0), i64 %24)
  br label %26

; <label>:26:                                     ; preds = %22, %16
  %27 = call i8* @CAT_new(i64 0)
  store i8* %27, i8** %8, align 8
  %28 = load i8*, i8** %8, align 8
  %29 = load i8*, i8** %5, align 8
  %30 = load i8*, i8** %7, align 8
  call void @CAT_add(i8* %28, i8* %29, i8* %30)
  %31 = load i8*, i8** %8, align 8
  %32 = call i64 @CAT_get(i8* %31)
  %33 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([19 x i8], [19 x i8]* @.str.3, i32 0, i32 0), i64 %32)
  ret void
}

declare dso_local i8* @CAT_new(i64) #1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main(i32, i8**) #0 {
  %3 = alloca i32, align 4
  %4 = alloca i32, align 4
  %5 = alloca i8**, align 8
  %6 = alloca i8*, align 8
  store i32 0, i32* %3, align 4
  store i32 %0, i32* %4, align 4
  store i8** %1, i8*** %5, align 8
  %7 = call i8* @CAT_new(i64 8)
  store i8* %7, i8** %6, align 8
  %8 = load i32, i32* %4, align 4
  %9 = load i8*, i8** %6, align 8
  %10 = load i32, i32* %4, align 4
  %11 = add nsw i32 %10, 1
  call void @CAT_execution(i32 %8, i8* %9, i32 %11)
  %12 = load i32, i32* %4, align 4
  %13 = load i8*, i8** %6, align 8
  %14 = call i32 @another_execution(i32 %12, i8* %13)
  %15 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([15 x i8], [15 x i8]* @.str.4, i32 0, i32 0), i32 %14)
  %16 = call i64 @CAT_invocations()
  %17 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([23 x i8], [23 x i8]* @.str.5, i32 0, i32 0), i64 %16)
  ret i32 0
}

declare dso_local i64 @CAT_invocations() #1

attributes #0 = { noinline nounwind optnone uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.module.flags = !{!0}
!llvm.ident = !{!1}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{!"clang version 8.0.0 (tags/RELEASE_800/final)"}
