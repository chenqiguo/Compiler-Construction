; ModuleID = 'program.bc'
source_filename = "program.c"
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@.str = private unnamed_addr constant [27 x i8] c"Value at the end: %ld %ld\0A\00", align 1
@.str.1 = private unnamed_addr constant [23 x i8] c"CAT invocations = %ld\0A\00", align 1

; Function Attrs: nounwind uwtable
define dso_local i64 @my_f(i8*, i8*) local_unnamed_addr #0 {
  %3 = tail call i64 @CAT_get(i8* %0) #3
  %4 = icmp eq i64 %3, 0
  br i1 %4, label %5, label %7

; <label>:5:                                      ; preds = %7, %2
  %6 = tail call i64 @CAT_get(i8* %0) #3
  ret i64 %6

; <label>:7:                                      ; preds = %2, %7
  tail call void @CAT_sub(i8* %0, i8* %0, i8* %1) #3
  %8 = tail call i64 @CAT_get(i8* %0) #3
  %9 = icmp eq i64 %8, 0
  br i1 %9, label %5, label %7
}

declare dso_local i64 @CAT_get(i8*) local_unnamed_addr #1

declare dso_local void @CAT_sub(i8*, i8*, i8*) local_unnamed_addr #1

; Function Attrs: nounwind uwtable
define dso_local i32 @main(i32, i8** nocapture readnone) local_unnamed_addr #0 {
  %3 = add nsw i32 %0, 100
  %4 = sext i32 %3 to i64
  %5 = tail call i8* @CAT_new(i64 %4) #3
  %6 = tail call i8* @CAT_new(i64 1) #3
  %7 = tail call i64 @my_f(i8* %5, i8* %6)
  %8 = tail call i64 @CAT_get(i8* %5) #3
  %9 = tail call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([27 x i8], [27 x i8]* @.str, i64 0, i64 0), i64 %8, i64 %7)
  %10 = tail call i64 @CAT_invocations() #3
  %11 = tail call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([23 x i8], [23 x i8]* @.str.1, i64 0, i64 0), i64 %10)
  ret i32 0
}

declare dso_local i8* @CAT_new(i64) local_unnamed_addr #1

; Function Attrs: nounwind
declare dso_local i32 @printf(i8* nocapture readonly, ...) local_unnamed_addr #2

declare dso_local i64 @CAT_invocations() local_unnamed_addr #1

attributes #0 = { nounwind uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #2 = { nounwind "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #3 = { nounwind }

!llvm.module.flags = !{!0}
!llvm.ident = !{!1}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{!"clang version 8.0.0 (tags/RELEASE_800/final)"}
