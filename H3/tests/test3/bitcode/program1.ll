; ModuleID = 'program1.bc'
source_filename = "program1.c"
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@.str = private unnamed_addr constant [20 x i8] c"H1: \09Value 2 = %ld\0A\00", align 1
@.str.1 = private unnamed_addr constant [19 x i8] c"H1: \09Result = %ld\0A\00", align 1
@.str.2 = private unnamed_addr constant [20 x i8] c"H1: \09Value 1 = %ld\0A\00", align 1

; Function Attrs: nounwind uwtable
define dso_local void @CAT_execution(i32) local_unnamed_addr #0 {
  %2 = icmp sgt i32 %0, 100
  br i1 %2, label %24, label %3

; <label>:3:                                      ; preds = %1
  %4 = icmp sgt i32 %0, 0
  br i1 %4, label %5, label %9

; <label>:5:                                      ; preds = %24, %3
  %6 = phi i8* [ %25, %24 ], [ undef, %3 ]
  %7 = icmp sgt i32 %0, 10
  %8 = icmp sgt i32 %0, 20
  br label %10

; <label>:9:                                      ; preds = %19, %3
  ret void

; <label>:10:                                     ; preds = %19, %5
  %11 = phi i32 [ 0, %5 ], [ %22, %19 ]
  %12 = tail call i8* @CAT_new(i64 8) #3
  br i1 %7, label %13, label %14

; <label>:13:                                     ; preds = %10
  tail call void @CAT_add(i8* %12, i8* %12, i8* %12) #3
  br label %14

; <label>:14:                                     ; preds = %13, %10
  %15 = tail call i64 @CAT_get(i8* %12) #3
  %16 = tail call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([20 x i8], [20 x i8]* @.str, i64 0, i64 0), i64 %15)
  %17 = tail call i8* @CAT_new(i64 0) #3
  tail call void @CAT_add(i8* %17, i8* %6, i8* %12) #3
  tail call void @CAT_add(i8* %17, i8* %6, i8* %17) #3
  tail call void @CAT_add(i8* %17, i8* %17, i8* %17) #3
  br i1 %8, label %18, label %19

; <label>:18:                                     ; preds = %14
  tail call void @CAT_add(i8* %17, i8* %6, i8* %6) #3
  br label %19

; <label>:19:                                     ; preds = %18, %14
  %20 = tail call i64 @CAT_get(i8* %17) #3
  %21 = tail call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([19 x i8], [19 x i8]* @.str.1, i64 0, i64 0), i64 %20)
  %22 = add nuw nsw i32 %11, 1
  %23 = icmp eq i32 %22, %0
  br i1 %23, label %9, label %10

; <label>:24:                                     ; preds = %1
  %25 = tail call i8* @CAT_new(i64 5) #3
  %26 = tail call i64 @CAT_get(i8* %25) #3
  %27 = tail call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([20 x i8], [20 x i8]* @.str.2, i64 0, i64 0), i64 %26)
  br label %5
}

declare dso_local i8* @CAT_new(i64) local_unnamed_addr #1

declare dso_local void @CAT_add(i8*, i8*, i8*) local_unnamed_addr #1

; Function Attrs: nounwind
declare dso_local i32 @printf(i8* nocapture readonly, ...) local_unnamed_addr #2

declare dso_local i64 @CAT_get(i8*) local_unnamed_addr #1

; Function Attrs: nounwind uwtable
define dso_local i32 @main(i32, i8** nocapture readnone) local_unnamed_addr #0 {
  tail call void @CAT_execution(i32 %0)
  ret i32 0
}

attributes #0 = { nounwind uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #2 = { nounwind "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #3 = { nounwind }

!llvm.module.flags = !{!0}
!llvm.ident = !{!1}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{!"clang version 8.0.0 (tags/RELEASE_800/final)"}
