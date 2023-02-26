; ModuleID = 'program.bc'
source_filename = "program.c"
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@.str = private unnamed_addr constant [5 x i8] c"ciao\00", align 1
@.str.1 = private unnamed_addr constant [5 x i8] c"%ld\0A\00", align 1
@.str.2 = private unnamed_addr constant [23 x i8] c"CAT invocations = %ld\0A\00", align 1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main(i32, i8**) #0 {
  %3 = alloca i32, align 4
  %4 = alloca i32, align 4
  %5 = alloca i8**, align 8
  %6 = alloca i8*, align 8
  %7 = alloca i32, align 4
  %8 = alloca i32, align 4
  %9 = alloca i32, align 4
  %10 = alloca i32, align 4
  store i32 0, i32* %3, align 4
  store i32 %0, i32* %4, align 4
  store i8** %1, i8*** %5, align 8
  %11 = call i8* @CAT_new(i64 1)
  %12 = ptrtoint i8* %11 to i32
  store i32 %12, i32* %7, align 4
  %13 = call i8* @CAT_new(i64 1)
  %14 = ptrtoint i8* %13 to i32
  store i32 %14, i32* %8, align 4
  %15 = call i8* @CAT_new(i64 1)
  %16 = ptrtoint i8* %15 to i32
  store i32 %16, i32* %9, align 4
  %17 = call i8* @CAT_new(i64 1)
  %18 = ptrtoint i8* %17 to i32
  store i32 %18, i32* %10, align 4
  %19 = load i32, i32* %4, align 4
  %20 = icmp sgt i32 %19, 100
  br i1 %20, label %21, label %40

; <label>:21:                                     ; preds = %2
  %22 = load i8**, i8*** %5, align 8
  %23 = getelementptr inbounds i8*, i8** %22, i64 80
  %24 = load i8*, i8** %23, align 8
  %25 = call i32 @strcmp(i8* %24, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @.str, i32 0, i32 0))
  %26 = icmp eq i32 %25, 0
  br i1 %26, label %27, label %33

; <label>:27:                                     ; preds = %21
  %28 = load i32, i32* %7, align 4
  %29 = sext i32 %28 to i64
  %30 = inttoptr i64 %29 to i8*
  %31 = call i64 @CAT_get(i8* %30)
  %32 = call i8* @CAT_new(i64 %31)
  store i8* %32, i8** %6, align 8
  br label %39

; <label>:33:                                     ; preds = %21
  %34 = load i32, i32* %8, align 4
  %35 = sext i32 %34 to i64
  %36 = inttoptr i64 %35 to i8*
  %37 = call i64 @CAT_get(i8* %36)
  %38 = call i8* @CAT_new(i64 %37)
  store i8* %38, i8** %6, align 8
  br label %39

; <label>:39:                                     ; preds = %33, %27
  br label %56

; <label>:40:                                     ; preds = %2
  %41 = load i32, i32* %4, align 4
  %42 = icmp sgt i32 %41, 10
  br i1 %42, label %43, label %49

; <label>:43:                                     ; preds = %40
  %44 = load i32, i32* %9, align 4
  %45 = sext i32 %44 to i64
  %46 = inttoptr i64 %45 to i8*
  %47 = call i64 @CAT_get(i8* %46)
  %48 = call i8* @CAT_new(i64 %47)
  store i8* %48, i8** %6, align 8
  br label %55

; <label>:49:                                     ; preds = %40
  %50 = load i32, i32* %10, align 4
  %51 = sext i32 %50 to i64
  %52 = inttoptr i64 %51 to i8*
  %53 = call i64 @CAT_get(i8* %52)
  %54 = call i8* @CAT_new(i64 %53)
  store i8* %54, i8** %6, align 8
  br label %55

; <label>:55:                                     ; preds = %49, %43
  br label %56

; <label>:56:                                     ; preds = %55, %39
  %57 = load i8*, i8** %6, align 8
  %58 = call i64 @CAT_get(i8* %57)
  %59 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @.str.1, i32 0, i32 0), i64 %58)
  %60 = call i64 @CAT_invocations()
  %61 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([23 x i8], [23 x i8]* @.str.2, i32 0, i32 0), i64 %60)
  ret i32 0
}

declare dso_local i8* @CAT_new(i64) #1

declare dso_local i32 @strcmp(i8*, i8*) #1

declare dso_local i64 @CAT_get(i8*) #1

declare dso_local i32 @printf(i8*, ...) #1

declare dso_local i64 @CAT_invocations() #1

attributes #0 = { noinline nounwind optnone uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.module.flags = !{!0}
!llvm.ident = !{!1}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{!"clang version 8.0.0 (tags/RELEASE_800/final)"}
