; ModuleID = 'program.bc'
source_filename = "program.c"
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@.str = private unnamed_addr constant [23 x i8] c"CAT invocations = %ld\0A\00", align 1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @a_generic_C_function(i8*, i32) #0 {
  %3 = alloca i8*, align 8
  %4 = alloca i32, align 4
  store i8* %0, i8** %3, align 8
  store i32 %1, i32* %4, align 4
  br label %5

; <label>:5:                                      ; preds = %29, %2
  %6 = load i32, i32* %4, align 4
  %7 = icmp slt i32 %6, 100
  br i1 %7, label %8, label %9

; <label>:8:                                      ; preds = %5
  ret void

; <label>:9:                                      ; preds = %5
  %10 = load i8*, i8** %3, align 8
  %11 = call i64 @CAT_get(i8* %10)
  %12 = icmp sgt i64 %11, 10
  br i1 %12, label %13, label %18

; <label>:13:                                     ; preds = %9
  %14 = load i8*, i8** %3, align 8
  %15 = call i64 @CAT_get(i8* %14)
  %16 = sub nsw i64 %15, 1
  %17 = call i8* @CAT_new(i64 %16)
  store i8* %17, i8** %3, align 8
  br label %21

; <label>:18:                                     ; preds = %9
  %19 = load i32, i32* %4, align 4
  %20 = add nsw i32 %19, -1
  store i32 %20, i32* %4, align 4
  br label %21

; <label>:21:                                     ; preds = %18, %13
  %22 = load i32, i32* %4, align 4
  %23 = icmp sgt i32 %22, 10
  br i1 %23, label %24, label %29

; <label>:24:                                     ; preds = %21
  %25 = load i8*, i8** %3, align 8
  %26 = call i64 @CAT_get(i8* %25)
  %27 = sub nsw i64 %26, 1
  %28 = call i8* @CAT_new(i64 %27)
  store i8* %28, i8** %3, align 8
  br label %29

; <label>:29:                                     ; preds = %24, %21
  br label %5
}

declare dso_local i64 @CAT_get(i8*) #1

declare dso_local i8* @CAT_new(i64) #1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main(i32, i8**) #0 {
  %3 = alloca i32, align 4
  %4 = alloca i32, align 4
  %5 = alloca i8**, align 8
  store i32 0, i32* %3, align 4
  store i32 %0, i32* %4, align 4
  store i8** %1, i8*** %5, align 8
  %6 = call i8* @CAT_new(i64 5)
  %7 = load i32, i32* %4, align 4
  call void @a_generic_C_function(i8* %6, i32 %7)
  %8 = call i64 @CAT_invocations()
  %9 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([23 x i8], [23 x i8]* @.str, i32 0, i32 0), i64 %8)
  ret i32 0
}

declare dso_local i32 @printf(i8*, ...) #1

declare dso_local i64 @CAT_invocations() #1

attributes #0 = { noinline nounwind optnone uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.module.flags = !{!0}
!llvm.ident = !{!1}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{!"clang version 8.0.0 (tags/RELEASE_800/final)"}
