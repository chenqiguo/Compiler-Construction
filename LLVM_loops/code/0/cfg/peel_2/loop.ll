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
  br i1 %3, label %4, label %13

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
  br label %9

; <label>:9:                                      ; preds = %.peel.next
  %10 = tail call i32 @myF(i32 %6)
  %11 = add nuw nsw i32 %7, 1
  %12 = icmp eq i32 %11, %0
  br i1 %12, label %.loopexit, label %.peel.next1

.peel.next1:                                      ; preds = %9
  br label %.peel.next2

.peel.next2:                                      ; preds = %.peel.next1
  br label %.peel.newph

.peel.newph:                                      ; preds = %.peel.next2
  br label %15

.loopexit.loopexit:                               ; preds = %15
  %.lcssa.ph = phi i32 [ %18, %15 ]
  br label %.loopexit

.loopexit:                                        ; preds = %.loopexit.loopexit, %9, %5
  %.lcssa = phi i32 [ %6, %5 ], [ %10, %9 ], [ %.lcssa.ph, %.loopexit.loopexit ]
  br label %13

; <label>:13:                                     ; preds = %.loopexit, %2
  %14 = phi i32 [ 0, %2 ], [ %.lcssa, %.loopexit ]
  ret i32 %14

; <label>:15:                                     ; preds = %.peel.newph, %15
  %16 = phi i32 [ %19, %15 ], [ %11, %.peel.newph ]
  %17 = phi i32 [ %18, %15 ], [ %10, %.peel.newph ]
  %18 = tail call i32 @myF(i32 %17)
  %19 = add nuw nsw i32 %16, 1
  %20 = icmp eq i32 %19, %0
  br i1 %20, label %.loopexit.loopexit, label %15
}

attributes #0 = { norecurse nounwind readnone uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.module.flags = !{!0}
!llvm.ident = !{!1}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{!"clang version 5.0.0 (tags/RELEASE_500/final)"}
