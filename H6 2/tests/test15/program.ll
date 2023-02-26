; ModuleID = 'program.bc'
source_filename = "program.c"
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@.str = private unnamed_addr constant [20 x i8] c"H1: \09Value 2 = %ld\0A\00", align 1
@.str.1 = private unnamed_addr constant [19 x i8] c"H1: \09Result = %ld\0A\00", align 1
@.str.2 = private unnamed_addr constant [20 x i8] c"H1: \09Value 1 = %ld\0A\00", align 1
@.str.3 = private unnamed_addr constant [23 x i8] c"CAT invocations = %ld\0A\00", align 1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @CAT_execution(i32) #0 {
  %2 = alloca i32, align 4
  %3 = alloca i8*, align 8
  %4 = alloca i8*, align 8
  %5 = alloca i8*, align 8
  %6 = alloca i32, align 4
  store i32 %0, i32* %2, align 4
  %7 = load i32, i32* %2, align 4
  %8 = icmp slt i32 %7, 100
  br i1 %8, label %9, label %10

; <label>:9:                                      ; preds = %1
  br label %52

; <label>:10:                                     ; preds = %1
  br label %11

; <label>:11:                                     ; preds = %52, %10
  store i32 0, i32* %6, align 4
  br label %12

; <label>:12:                                     ; preds = %48, %11
  %13 = load i32, i32* %6, align 4
  %14 = load i32, i32* %2, align 4
  %15 = icmp slt i32 %13, %14
  br i1 %15, label %16, label %51

; <label>:16:                                     ; preds = %12
  %17 = call i8* @CAT_new(i64 8)
  store i8* %17, i8** %4, align 8
  %18 = load i32, i32* %2, align 4
  %19 = icmp sgt i32 %18, 10
  br i1 %19, label %20, label %24

; <label>:20:                                     ; preds = %16
  %21 = load i8*, i8** %4, align 8
  %22 = load i8*, i8** %4, align 8
  %23 = load i8*, i8** %4, align 8
  call void @CAT_add(i8* %21, i8* %22, i8* %23)
  br label %24

; <label>:24:                                     ; preds = %20, %16
  %25 = load i8*, i8** %4, align 8
  %26 = call i64 @CAT_get(i8* %25)
  %27 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([20 x i8], [20 x i8]* @.str, i32 0, i32 0), i64 %26)
  %28 = call i8* @CAT_new(i64 0)
  store i8* %28, i8** %5, align 8
  %29 = load i8*, i8** %5, align 8
  %30 = load i8*, i8** %3, align 8
  %31 = load i8*, i8** %4, align 8
  call void @CAT_add(i8* %29, i8* %30, i8* %31)
  %32 = load i8*, i8** %5, align 8
  %33 = load i8*, i8** %3, align 8
  %34 = load i8*, i8** %5, align 8
  call void @CAT_add(i8* %32, i8* %33, i8* %34)
  %35 = load i8*, i8** %5, align 8
  %36 = load i8*, i8** %5, align 8
  %37 = load i8*, i8** %5, align 8
  call void @CAT_add(i8* %35, i8* %36, i8* %37)
  %38 = load i32, i32* %2, align 4
  %39 = icmp sgt i32 %38, 20
  br i1 %39, label %40, label %44

; <label>:40:                                     ; preds = %24
  %41 = load i8*, i8** %5, align 8
  %42 = load i8*, i8** %3, align 8
  %43 = load i8*, i8** %3, align 8
  call void @CAT_add(i8* %41, i8* %42, i8* %43)
  br label %44

; <label>:44:                                     ; preds = %40, %24
  %45 = load i8*, i8** %5, align 8
  %46 = call i64 @CAT_get(i8* %45)
  %47 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([19 x i8], [19 x i8]* @.str.1, i32 0, i32 0), i64 %46)
  br label %48

; <label>:48:                                     ; preds = %44
  %49 = load i32, i32* %6, align 4
  %50 = add nsw i32 %49, 1
  store i32 %50, i32* %6, align 4
  br label %12

; <label>:51:                                     ; preds = %12
  br label %57

; <label>:52:                                     ; preds = %9
  %53 = call i8* @CAT_new(i64 5)
  store i8* %53, i8** %3, align 8
  %54 = load i8*, i8** %3, align 8
  %55 = call i64 @CAT_get(i8* %54)
  %56 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([20 x i8], [20 x i8]* @.str.2, i32 0, i32 0), i64 %55)
  br label %11

; <label>:57:                                     ; preds = %51
  ret void
}

declare dso_local i8* @CAT_new(i64) #1

declare dso_local void @CAT_add(i8*, i8*, i8*) #1

declare dso_local i32 @printf(i8*, ...) #1

declare dso_local i64 @CAT_get(i8*) #1

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
