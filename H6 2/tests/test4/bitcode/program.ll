; ModuleID = 'program.bc'
source_filename = "program.c"
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@.str = private unnamed_addr constant [5 x i8] c"%ld\0A\00", align 1
@.str.1 = private unnamed_addr constant [23 x i8] c"CAT invocations = %ld\0A\00", align 1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i64 @generic_C_function(i8*) #0 {
  %2 = alloca i64, align 8
  %3 = alloca i8*, align 8
  %4 = alloca i8*, align 8
  %5 = alloca i8**, align 8
  %6 = alloca i64, align 8
  store i8* %0, i8** %3, align 8
  %7 = call noalias i8* @malloc(i64 8) #3
  %8 = bitcast i8* %7 to i8**
  store i8** %8, i8*** %5, align 8
  %9 = call i8* @CAT_new(i64 8)
  store i8* %9, i8** %4, align 8
  %10 = load i8*, i8** %4, align 8
  %11 = load i8**, i8*** %5, align 8
  store i8* %10, i8** %11, align 8
  %12 = load i8*, i8** %3, align 8
  %13 = call i64 @CAT_get(i8* %12)
  %14 = icmp sgt i64 %13, 10
  br i1 %14, label %15, label %23

; <label>:15:                                     ; preds = %1
  %16 = load i8*, i8** %4, align 8
  %17 = call i64 @CAT_get(i8* %16)
  store i64 %17, i64* %6, align 8
  %18 = load i8**, i8*** %5, align 8
  %19 = bitcast i8** %18 to i8*
  call void @free(i8* %19) #3
  %20 = load i64, i64* %6, align 8
  %21 = mul nsw i64 %20, 51
  %22 = sdiv i64 %21, 2
  store i64 %22, i64* %2, align 8
  br label %26

; <label>:23:                                     ; preds = %1
  %24 = load i8**, i8*** %5, align 8
  %25 = bitcast i8** %24 to i8*
  call void @free(i8* %25) #3
  store i64 0, i64* %2, align 8
  br label %26

; <label>:26:                                     ; preds = %23, %15
  %27 = load i64, i64* %2, align 8
  ret i64 %27
}

; Function Attrs: nounwind
declare dso_local noalias i8* @malloc(i64) #1

declare dso_local i8* @CAT_new(i64) #2

declare dso_local i64 @CAT_get(i8*) #2

; Function Attrs: nounwind
declare dso_local void @free(i8*) #1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main(i32, i8**) #0 {
  %3 = alloca i32, align 4
  %4 = alloca i32, align 4
  %5 = alloca i8**, align 8
  store i32 0, i32* %3, align 4
  store i32 %0, i32* %4, align 4
  store i8** %1, i8*** %5, align 8
  %6 = call i8* @CAT_new(i64 12)
  %7 = call i64 @generic_C_function(i8* %6)
  %8 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @.str, i32 0, i32 0), i64 %7)
  %9 = call i64 @CAT_invocations()
  %10 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([23 x i8], [23 x i8]* @.str.1, i32 0, i32 0), i64 %9)
  ret i32 0
}

declare dso_local i32 @printf(i8*, ...) #2

declare dso_local i64 @CAT_invocations() #2

attributes #0 = { noinline nounwind optnone uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { nounwind "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #2 = { "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #3 = { nounwind }

!llvm.module.flags = !{!0}
!llvm.ident = !{!1}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{!"clang version 8.0.0 (tags/RELEASE_800/final)"}
