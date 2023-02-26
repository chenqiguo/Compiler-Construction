; ModuleID = 'program.bc'
source_filename = "program.c"
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%struct.my_struct_t = type { i8* }

@.str = private unnamed_addr constant [13 x i8] c"Values: %ld\0A\00", align 1
@.str.1 = private unnamed_addr constant [23 x i8] c"CAT invocations = %ld\0A\00", align 1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i64 @my_f(%struct.my_struct_t*, i32) #0 {
  %3 = alloca %struct.my_struct_t*, align 8
  %4 = alloca i32, align 4
  %5 = alloca i8*, align 8
  store %struct.my_struct_t* %0, %struct.my_struct_t** %3, align 8
  store i32 %1, i32* %4, align 4
  %6 = call i8* @CAT_new(i64 6)
  store i8* %6, i8** %5, align 8
  %7 = load i32, i32* %4, align 4
  %8 = icmp ne i32 %7, 0
  br i1 %8, label %9, label %13

; <label>:9:                                      ; preds = %2
  %10 = load %struct.my_struct_t*, %struct.my_struct_t** %3, align 8
  %11 = getelementptr inbounds %struct.my_struct_t, %struct.my_struct_t* %10, i32 0, i32 0
  %12 = load i8*, i8** %11, align 8
  store i8* %12, i8** %5, align 8
  br label %13

; <label>:13:                                     ; preds = %9, %2
  %14 = load i8*, i8** %5, align 8
  %15 = call i64 @CAT_get(i8* %14)
  ret i64 %15
}

declare dso_local i8* @CAT_new(i64) #1

declare dso_local i64 @CAT_get(i8*) #1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main(i32, i8**) #0 {
  %3 = alloca i32, align 4
  %4 = alloca i32, align 4
  %5 = alloca i8**, align 8
  %6 = alloca %struct.my_struct_t, align 8
  %7 = alloca i32, align 4
  store i32 0, i32* %3, align 4
  store i32 %0, i32* %4, align 4
  store i8** %1, i8*** %5, align 8
  %8 = call i8* @CAT_new(i64 5)
  %9 = getelementptr inbounds %struct.my_struct_t, %struct.my_struct_t* %6, i32 0, i32 0
  store i8* %8, i8** %9, align 8
  %10 = call i64 @my_f(%struct.my_struct_t* %6, i32 1)
  %11 = trunc i64 %10 to i32
  store i32 %11, i32* %7, align 4
  %12 = load i32, i32* %7, align 4
  %13 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([13 x i8], [13 x i8]* @.str, i32 0, i32 0), i32 %12)
  %14 = call i64 @CAT_invocations()
  %15 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([23 x i8], [23 x i8]* @.str.1, i32 0, i32 0), i64 %14)
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
