; ModuleID = 'program.bc'
source_filename = "program.c"
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@.str = private unnamed_addr constant [14 x i8] c"H1: \09Y = %ld\0A\00", align 1
@.str.1 = private unnamed_addr constant [20 x i8] c"H1: \09Value 2 = %ld\0A\00", align 1
@.str.2 = private unnamed_addr constant [20 x i8] c"H1: \09Value 1 = %ld\0A\00", align 1
@.str.3 = private unnamed_addr constant [15 x i8] c"Result 2 = %d\0A\00", align 1
@.str.4 = private unnamed_addr constant [23 x i8] c"CAT invocations = %ld\0A\00", align 1

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
  %10 = load i8*, i8** %7, align 8
  %11 = call i64 @CAT_get(i8* %10)
  %12 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([20 x i8], [20 x i8]* @.str.1, i32 0, i32 0), i64 %11)
  %13 = load i32, i32* %4, align 4
  %14 = icmp sgt i32 %13, 10
  br i1 %14, label %15, label %20

; <label>:15:                                     ; preds = %3
  %16 = call i8* @CAT_new(i64 8)
  store i8* %16, i8** %7, align 8
  %17 = load i8*, i8** %5, align 8
  %18 = load i8*, i8** %7, align 8
  %19 = load i8*, i8** %7, align 8
  call void @CAT_add(i8* %17, i8* %18, i8* %19)
  br label %20

; <label>:20:                                     ; preds = %15, %3
  %21 = load i8*, i8** %7, align 8
  %22 = call i64 @CAT_get(i8* %21)
  %23 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([20 x i8], [20 x i8]* @.str.1, i32 0, i32 0), i64 %22)
  %24 = load i8*, i8** %5, align 8
  %25 = call i64 @CAT_get(i8* %24)
  %26 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([20 x i8], [20 x i8]* @.str.2, i32 0, i32 0), i64 %25)
  %27 = load i32, i32* %6, align 4
  %28 = icmp sgt i32 %27, 0
  br i1 %28, label %29, label %33

; <label>:29:                                     ; preds = %20
  %30 = load i8*, i8** %7, align 8
  %31 = call i64 @CAT_get(i8* %30)
  %32 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([20 x i8], [20 x i8]* @.str.1, i32 0, i32 0), i64 %31)
  br label %33

; <label>:33:                                     ; preds = %29, %20
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
  %15 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([15 x i8], [15 x i8]* @.str.3, i32 0, i32 0), i32 %14)
  %16 = call i64 @CAT_invocations()
  %17 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([23 x i8], [23 x i8]* @.str.4, i32 0, i32 0), i64 %16)
  ret i32 0
}

declare dso_local i64 @CAT_invocations() #1

attributes #0 = { noinline nounwind optnone uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.module.flags = !{!0}
!llvm.ident = !{!1}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{!"clang version 8.0.0 (tags/RELEASE_800/final)"}
