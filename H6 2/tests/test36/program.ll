; ModuleID = 'program.bc'
source_filename = "program.c"
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@.str = private unnamed_addr constant [10 x i8] c"HELLO %f\0A\00", align 1
@.str.1 = private unnamed_addr constant [5 x i8] c"WOW\0A\00", align 1
@you_cannot_take_decisions_based_on_function_names.internalD1 = internal global i8* null, align 8
@.str.2 = private unnamed_addr constant [27 x i8] c"This block doesn't matter\0A\00", align 1
@.str.3 = private unnamed_addr constant [7 x i8] c"Hello\0A\00", align 1
@.str.4 = private unnamed_addr constant [9 x i8] c"Hello 2\0A\00", align 1
@.str.5 = private unnamed_addr constant [21 x i8] c"Invoking a function\0A\00", align 1
@.str.6 = private unnamed_addr constant [13 x i8] c"Values: %ld\0A\00", align 1
@.str.7 = private unnamed_addr constant [23 x i8] c"CAT invocations = %ld\0A\00", align 1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @another_function(i32, float, double) #0 {
  %4 = alloca i32, align 4
  %5 = alloca float, align 4
  %6 = alloca double, align 8
  store i32 %0, i32* %4, align 4
  store float %1, float* %5, align 4
  store double %2, double* %6, align 8
  %7 = load i32, i32* %4, align 4
  %8 = sitofp i32 %7 to double
  %9 = load float, float* %5, align 4
  %10 = fpext float %9 to double
  %11 = fadd double %8, %10
  %12 = load double, double* %6, align 8
  %13 = fadd double %11, %12
  %14 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([10 x i8], [10 x i8]* @.str, i32 0, i32 0), double %13)
  %15 = load i32, i32* %4, align 4
  %16 = add nsw i32 %15, 100
  ret i32 %16
}

declare dso_local i32 @printf(i8*, ...) #1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @yet_another_function(i8**, i8*) #0 {
  %3 = alloca i8**, align 8
  %4 = alloca i8*, align 8
  store i8** %0, i8*** %3, align 8
  store i8* %1, i8** %4, align 8
  %5 = load i8**, i8*** %3, align 8
  %6 = icmp ne i8** %5, null
  br i1 %6, label %7, label %11

; <label>:7:                                      ; preds = %2
  %8 = load i8*, i8** %4, align 8
  %9 = load i8*, i8** %4, align 8
  %10 = load i8*, i8** %4, align 8
  call void @CAT_add(i8* %8, i8* %9, i8* %10)
  br label %13

; <label>:11:                                     ; preds = %2
  %12 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @.str.1, i32 0, i32 0))
  br label %13

; <label>:13:                                     ; preds = %11, %7
  ret void
}

declare dso_local void @CAT_add(i8*, i8*, i8*) #1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i8** @you_cannot_take_decisions_based_on_function_names(i8*) #0 {
  %2 = alloca i8**, align 8
  %3 = alloca i8*, align 8
  store i8* %0, i8** %3, align 8
  %4 = load i8*, i8** %3, align 8
  %5 = icmp eq i8* %4, null
  br i1 %5, label %6, label %7

; <label>:6:                                      ; preds = %1
  store i8** null, i8*** %2, align 8
  br label %13

; <label>:7:                                      ; preds = %1
  %8 = load i8*, i8** @you_cannot_take_decisions_based_on_function_names.internalD1, align 8
  %9 = icmp eq i8* %8, null
  br i1 %9, label %10, label %12

; <label>:10:                                     ; preds = %7
  %11 = load i8*, i8** %3, align 8
  store i8* %11, i8** @you_cannot_take_decisions_based_on_function_names.internalD1, align 8
  br label %12

; <label>:12:                                     ; preds = %10, %7
  store i8** @you_cannot_take_decisions_based_on_function_names.internalD1, i8*** %2, align 8
  br label %13

; <label>:13:                                     ; preds = %12, %6
  %14 = load i8**, i8*** %2, align 8
  ret i8** %14
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main(i32, i8**) #0 {
  %3 = alloca i32, align 4
  %4 = alloca i32, align 4
  %5 = alloca i8**, align 8
  %6 = alloca i32, align 4
  %7 = alloca i8**, align 8
  store i32 0, i32* %3, align 4
  store i32 %0, i32* %4, align 4
  store i8** %1, i8*** %5, align 8
  %8 = call i8* @CAT_new(i64 5)
  %9 = ptrtoint i8* %8 to i32
  store i32 %9, i32* %6, align 4
  store i8** null, i8*** %7, align 8
  %10 = load i32, i32* %4, align 4
  %11 = icmp sgt i32 %10, 5
  br i1 %11, label %12, label %14

; <label>:12:                                     ; preds = %2
  %13 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([27 x i8], [27 x i8]* @.str.2, i32 0, i32 0))
  br label %20

; <label>:14:                                     ; preds = %2
  %15 = load i32, i32* %6, align 4
  %16 = sext i32 %15 to i64
  %17 = inttoptr i64 %16 to i8*
  %18 = call i8** @you_cannot_take_decisions_based_on_function_names(i8* %17)
  store i8** %18, i8*** %7, align 8
  %19 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @.str.3, i32 0, i32 0))
  br label %20

; <label>:20:                                     ; preds = %14, %12
  %21 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([9 x i8], [9 x i8]* @.str.4, i32 0, i32 0))
  %22 = load i32, i32* %4, align 4
  %23 = load i32, i32* %4, align 4
  %24 = add nsw i32 %23, 1
  %25 = sitofp i32 %24 to float
  %26 = call i32 @another_function(i32 %22, float %25, double 4.242000e+01)
  %27 = icmp sgt i32 %26, 10
  br i1 %27, label %28, label %36

; <label>:28:                                     ; preds = %20
  %29 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([21 x i8], [21 x i8]* @.str.5, i32 0, i32 0))
  %30 = load i8**, i8*** %7, align 8
  %31 = load i8*, i8** %30, align 8
  %32 = bitcast i8* %31 to i8**
  %33 = load i32, i32* %6, align 4
  %34 = sext i32 %33 to i64
  %35 = inttoptr i64 %34 to i8*
  call void @yet_another_function(i8** %32, i8* %35)
  br label %38

; <label>:36:                                     ; preds = %20
  %37 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([27 x i8], [27 x i8]* @.str.2, i32 0, i32 0))
  br label %38

; <label>:38:                                     ; preds = %36, %28
  %39 = load i32, i32* %6, align 4
  %40 = sext i32 %39 to i64
  %41 = inttoptr i64 %40 to i8*
  %42 = call i64 @CAT_get(i8* %41)
  %43 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([13 x i8], [13 x i8]* @.str.6, i32 0, i32 0), i64 %42)
  %44 = call i64 @CAT_invocations()
  %45 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([23 x i8], [23 x i8]* @.str.7, i32 0, i32 0), i64 %44)
  ret i32 0
}

declare dso_local i8* @CAT_new(i64) #1

declare dso_local i64 @CAT_get(i8*) #1

declare dso_local i64 @CAT_invocations() #1

attributes #0 = { noinline nounwind optnone uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.module.flags = !{!0}
!llvm.ident = !{!1}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{!"clang version 8.0.0 (tags/RELEASE_800/final)"}
