; ModuleID = 'program.bc'
source_filename = "program.c"
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@.str = private unnamed_addr constant [20 x i8] c"H1: \09Value 1 = %ld\0A\00", align 1
@.str.1 = private unnamed_addr constant [20 x i8] c"H1: \09Value 2 = %ld\0A\00", align 1
@.str.2 = private unnamed_addr constant [20 x i8] c"H1: \09Value 3 = %ld\0A\00", align 1
@.str.3 = private unnamed_addr constant [20 x i8] c"H1: \09Value 4 = %ld\0A\00", align 1
@.str.4 = private unnamed_addr constant [20 x i8] c"H1: \09Value 5 = %ld\0A\00", align 1
@.str.5 = private unnamed_addr constant [23 x i8] c"CAT invocations = %ld\0A\00", align 1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @CAT_execution(i32) #0 {
  %2 = alloca i32, align 4
  %3 = alloca i8*, align 8
  %4 = alloca i8*, align 8
  %5 = alloca i8*, align 8
  %6 = alloca i8*, align 8
  %7 = alloca i8*, align 8
  store i32 %0, i32* %2, align 4
  %8 = call i8* @CAT_new(i64 5)
  store i8* %8, i8** %3, align 8
  %9 = call i8* @CAT_new(i64 8)
  store i8* %9, i8** %4, align 8
  %10 = call i8* @CAT_new(i64 8)
  store i8* %10, i8** %5, align 8
  %11 = call i8* @CAT_new(i64 8)
  store i8* %11, i8** %6, align 8
  %12 = load i32, i32* %2, align 4
  %13 = icmp sgt i32 %12, 10
  br i1 %13, label %14, label %18

; <label>:14:                                     ; preds = %1
  %15 = load i8*, i8** %3, align 8
  %16 = load i8*, i8** %4, align 8
  %17 = load i8*, i8** %5, align 8
  call void @CAT_add(i8* %15, i8* %16, i8* %17)
  br label %18

; <label>:18:                                     ; preds = %14, %1
  %19 = load i8*, i8** %3, align 8
  %20 = call i64 @CAT_get(i8* %19)
  %21 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([20 x i8], [20 x i8]* @.str, i32 0, i32 0), i64 %20)
  %22 = load i8*, i8** %4, align 8
  %23 = call i64 @CAT_get(i8* %22)
  %24 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([20 x i8], [20 x i8]* @.str.1, i32 0, i32 0), i64 %23)
  %25 = load i8*, i8** %5, align 8
  %26 = call i64 @CAT_get(i8* %25)
  %27 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([20 x i8], [20 x i8]* @.str.2, i32 0, i32 0), i64 %26)
  %28 = load i8*, i8** %6, align 8
  %29 = call i64 @CAT_get(i8* %28)
  %30 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([20 x i8], [20 x i8]* @.str.3, i32 0, i32 0), i64 %29)
  %31 = load i8*, i8** %3, align 8
  %32 = load i8*, i8** %4, align 8
  %33 = load i8*, i8** %5, align 8
  call void @CAT_add(i8* %31, i8* %32, i8* %33)
  %34 = load i8*, i8** %3, align 8
  %35 = call i64 @CAT_get(i8* %34)
  %36 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([20 x i8], [20 x i8]* @.str, i32 0, i32 0), i64 %35)
  %37 = load i8*, i8** %4, align 8
  %38 = call i64 @CAT_get(i8* %37)
  %39 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([20 x i8], [20 x i8]* @.str.1, i32 0, i32 0), i64 %38)
  %40 = load i8*, i8** %5, align 8
  %41 = call i64 @CAT_get(i8* %40)
  %42 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([20 x i8], [20 x i8]* @.str.2, i32 0, i32 0), i64 %41)
  %43 = load i8*, i8** %6, align 8
  %44 = call i64 @CAT_get(i8* %43)
  %45 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([20 x i8], [20 x i8]* @.str.3, i32 0, i32 0), i64 %44)
  %46 = load i32, i32* %2, align 4
  %47 = icmp sgt i32 %46, 20
  br i1 %47, label %48, label %50

; <label>:48:                                     ; preds = %18
  %49 = call i8* @CAT_new(i64 0)
  store i8* %49, i8** %7, align 8
  br label %50

; <label>:50:                                     ; preds = %48, %18
  %51 = load i8*, i8** %3, align 8
  %52 = call i64 @CAT_get(i8* %51)
  %53 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([20 x i8], [20 x i8]* @.str, i32 0, i32 0), i64 %52)
  %54 = load i8*, i8** %4, align 8
  %55 = call i64 @CAT_get(i8* %54)
  %56 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([20 x i8], [20 x i8]* @.str.1, i32 0, i32 0), i64 %55)
  %57 = load i8*, i8** %5, align 8
  %58 = call i64 @CAT_get(i8* %57)
  %59 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([20 x i8], [20 x i8]* @.str.2, i32 0, i32 0), i64 %58)
  %60 = load i8*, i8** %6, align 8
  %61 = call i64 @CAT_get(i8* %60)
  %62 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([20 x i8], [20 x i8]* @.str.3, i32 0, i32 0), i64 %61)
  %63 = load i32, i32* %2, align 4
  %64 = icmp sgt i32 %63, 25
  br i1 %64, label %65, label %69

; <label>:65:                                     ; preds = %50
  %66 = load i8*, i8** %6, align 8
  %67 = call i64 @CAT_get(i8* %66)
  %68 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([20 x i8], [20 x i8]* @.str.4, i32 0, i32 0), i64 %67)
  br label %69

; <label>:69:                                     ; preds = %65, %50
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
  %8 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([23 x i8], [23 x i8]* @.str.5, i32 0, i32 0), i64 %7)
  ret i32 0
}

declare dso_local i64 @CAT_invocations() #1

attributes #0 = { noinline nounwind optnone uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.module.flags = !{!0}
!llvm.ident = !{!1}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{!"clang version 8.0.0 (tags/RELEASE_800/final)"}
