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
  %6 = alloca i32, align 4
  store i32 %0, i32* %2, align 4
  %7 = call i8* @CAT_new(i64 5)
  store i8* %7, i8** %3, align 8
  %8 = load i8*, i8** %3, align 8
  %9 = call i64 @CAT_get(i8* %8)
  %10 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([20 x i8], [20 x i8]* @.str, i32 0, i32 0), i64 %9)
  store i32 0, i32* %6, align 4
  br label %11

; <label>:11:                                     ; preds = %47, %1
  %12 = load i32, i32* %6, align 4
  %13 = load i32, i32* %2, align 4
  %14 = icmp slt i32 %12, %13
  br i1 %14, label %15, label %50

; <label>:15:                                     ; preds = %11
  %16 = call i8* @CAT_new(i64 8)
  store i8* %16, i8** %4, align 8
  %17 = load i32, i32* %2, align 4
  %18 = icmp sgt i32 %17, 10
  br i1 %18, label %19, label %23

; <label>:19:                                     ; preds = %15
  %20 = load i8*, i8** %4, align 8
  %21 = load i8*, i8** %4, align 8
  %22 = load i8*, i8** %4, align 8
  call void @CAT_add(i8* %20, i8* %21, i8* %22)
  br label %23

; <label>:23:                                     ; preds = %19, %15
  %24 = load i8*, i8** %4, align 8
  %25 = call i64 @CAT_get(i8* %24)
  %26 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([20 x i8], [20 x i8]* @.str.1, i32 0, i32 0), i64 %25)
  %27 = call i8* @CAT_new(i64 0)
  store i8* %27, i8** %5, align 8
  %28 = load i8*, i8** %5, align 8
  %29 = load i8*, i8** %3, align 8
  %30 = load i8*, i8** %4, align 8
  call void @CAT_add(i8* %28, i8* %29, i8* %30)
  %31 = load i8*, i8** %5, align 8
  %32 = load i8*, i8** %3, align 8
  %33 = load i8*, i8** %5, align 8
  call void @CAT_add(i8* %31, i8* %32, i8* %33)
  %34 = load i8*, i8** %5, align 8
  %35 = load i8*, i8** %5, align 8
  %36 = load i8*, i8** %5, align 8
  call void @CAT_add(i8* %34, i8* %35, i8* %36)
  %37 = load i32, i32* %2, align 4
  %38 = icmp sgt i32 %37, 20
  br i1 %38, label %39, label %43

; <label>:39:                                     ; preds = %23
  %40 = load i8*, i8** %5, align 8
  %41 = load i8*, i8** %3, align 8
  %42 = load i8*, i8** %3, align 8
  call void @CAT_add(i8* %40, i8* %41, i8* %42)
  br label %43

; <label>:43:                                     ; preds = %39, %23
  %44 = load i8*, i8** %5, align 8
  %45 = call i64 @CAT_get(i8* %44)
  %46 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([19 x i8], [19 x i8]* @.str.2, i32 0, i32 0), i64 %45)
  br label %47

; <label>:47:                                     ; preds = %43
  %48 = load i32, i32* %6, align 4
  %49 = add nsw i32 %48, 1
  store i32 %49, i32* %6, align 4
  br label %11

; <label>:50:                                     ; preds = %11
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
