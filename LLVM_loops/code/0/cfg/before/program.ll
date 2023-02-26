; ModuleID = 'program.bc'
source_filename = "program.c"
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; Function Attrs: norecurse nounwind readnone uwtable
define dso_local i32 @myF(i32) local_unnamed_addr #0 {
  %2 = add nsw i32 %0, 1
  ret i32 %2
}

; Function Attrs: norecurse nounwind readnone uwtable
define dso_local i32 @main(i32, i8** nocapture readnone) local_unnamed_addr #0 {
  %3 = icmp sgt i32 %0, 0
  br i1 %3, label %6, label %4

; <label>:4:                                      ; preds = %6, %2
  %5 = phi i32 [ 0, %2 ], [ %9, %6 ]
  ret i32 %5

; <label>:6:                                      ; preds = %2, %6
  %7 = phi i32 [ %10, %6 ], [ 0, %2 ]
  %8 = phi i32 [ %9, %6 ], [ 0, %2 ]
  %9 = tail call i32 @myF(i32 %8)
  %10 = add nuw nsw i32 %7, 1
  %11 = icmp eq i32 %10, %0
  br i1 %11, label %4, label %6
}

attributes #0 = { norecurse nounwind readnone uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.module.flags = !{!0}
!llvm.ident = !{!1}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{!"clang version 8.0.0 (tags/RELEASE_800/final)"}
