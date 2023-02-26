; ModuleID = 'loop.bc'
source_filename = "program.c"
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; Function Attrs: norecurse nounwind readnone uwtable
define i32 @myF(i32) local_unnamed_addr #0 {
  %2 = add nsw i32 %0, 1
  ret i32 %2
}

; Function Attrs: norecurse nounwind readnone uwtable
define i32 @main(i32, i8** nocapture readnone) local_unnamed_addr #0 {
  %3 = icmp sgt i32 %0, 0
  br i1 %3, label %4, label %9

; <label>:4:                                      ; preds = %2
  br label %.peel.begin

.peel.begin:                                      ; preds = %4
  br label %5

; <label>:5:                                      ; preds = %.peel.begin
  %6 = tail call i32 @myF(i32 0)
  %7 = add nuw nsw i32 0, 1
  %8 = icmp eq i32 %7, %0
  br i1 %8, label %.loopexit, label %.peel.next

.peel.next:                                       ; preds = %5
  br label %.peel.next1

.peel.next1:                                      ; preds = %.peel.next
  br label %.peel.newph

.peel.newph:                                      ; preds = %.peel.next1
  br label %11

.loopexit.loopexit:                               ; preds = %11
  %.lcssa.ph = phi i32 [ %14, %11 ]
  br label %.loopexit

.loopexit:                                        ; preds = %.loopexit.loopexit, %5
  %.lcssa = phi i32 [ %6, %5 ], [ %.lcssa.ph, %.loopexit.loopexit ]
  br label %9

; <label>:9:                                      ; preds = %.loopexit, %2
  %10 = phi i32 [ 0, %2 ], [ %.lcssa, %.loopexit ]
  ret i32 %10

; <label>:11:                                     ; preds = %.peel.newph, %11
  %12 = phi i32 [ %15, %11 ], [ %7, %.peel.newph ]
  %13 = phi i32 [ %14, %11 ], [ %6, %.peel.newph ]
  %14 = tail call i32 @myF(i32 %13)
  %15 = add nuw nsw i32 %12, 1
  %16 = icmp eq i32 %15, %0
  br i1 %16, label %.loopexit.loopexit, label %11
}

attributes #0 = { norecurse nounwind readnone uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.module.flags = !{!0}
!llvm.ident = !{!1}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{!"clang version 5.0.0 (tags/RELEASE_500/final)"}
