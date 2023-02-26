; ModuleID = 'program.bc'
source_filename = "program.c"
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@str = private unnamed_addr constant [5 x i8] c"Ciao\00", align 1

; Function Attrs: noinline nounwind uwtable
define dso_local i32 @myF(i32) local_unnamed_addr #0 {
  %2 = tail call i32 @puts(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str, i64 0, i64 0))
  %3 = add nsw i32 %0, 1
  ret i32 %3
}

; Function Attrs: nounwind uwtable
define dso_local i32 @main(i32, i8** nocapture readnone) local_unnamed_addr #1 {
  br label %4

; <label>:3:                                      ; preds = %4
  ret i32 %7

; <label>:4:                                      ; preds = %4, %2
  %5 = phi i32 [ 0, %2 ], [ %8, %4 ]
  %6 = phi i32 [ 0, %2 ], [ %7, %4 ]
  %7 = tail call i32 @myF(i32 %6)
  %8 = add nuw nsw i32 %5, 1
  %9 = icmp eq i32 %8, 10
  br i1 %9, label %3, label %4
}

; Function Attrs: nounwind
declare i32 @puts(i8* nocapture readonly) local_unnamed_addr #2

attributes #0 = { noinline nounwind uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { nounwind uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #2 = { nounwind }

!llvm.module.flags = !{!0}
!llvm.ident = !{!1}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{!"clang version 8.0.0 (tags/RELEASE_800/final)"}
