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
  br i1 %3, label %4, label %15

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
  %9 = add i32 %0, -1
  %xtraiter = and i32 %0, 1
  %lcmp.mod = icmp ne i32 %xtraiter, 0
  br i1 %lcmp.mod, label %.prol.preheader, label %.prol.loopexit

.prol.preheader:                                  ; preds = %.peel.newph
  br label %10

; <label>:10:                                     ; preds = %.prol.preheader
  %11 = tail call i32 @myF(i32 %6)
  %12 = add nuw nsw i32 %7, 1
  %13 = icmp eq i32 %12, %0
  br label %.prol.loopexit

.prol.loopexit:                                   ; preds = %10, %.peel.newph
  %.lcssa.ph.unr = phi i32 [ undef, %.peel.newph ], [ %11, %10 ]
  %.unr = phi i32 [ %7, %.peel.newph ], [ %12, %10 ]
  %.unr2 = phi i32 [ %6, %.peel.newph ], [ %11, %10 ]
  %14 = icmp ult i32 %9, 1
  br i1 %14, label %.loopexit.loopexit, label %.peel.newph.new

.peel.newph.new:                                  ; preds = %.prol.loopexit
  br label %17

.loopexit.loopexit.unr-lcssa:                     ; preds = %17
  %.lcssa.ph.ph = phi i32 [ %22, %17 ]
  br label %.loopexit.loopexit

.loopexit.loopexit:                               ; preds = %.prol.loopexit, %.loopexit.loopexit.unr-lcssa
  %.lcssa.ph = phi i32 [ %.lcssa.ph.unr, %.prol.loopexit ], [ %.lcssa.ph.ph, %.loopexit.loopexit.unr-lcssa ]
  br label %.loopexit

.loopexit:                                        ; preds = %.loopexit.loopexit, %5
  %.lcssa = phi i32 [ %6, %5 ], [ %.lcssa.ph, %.loopexit.loopexit ]
  br label %15

; <label>:15:                                     ; preds = %.loopexit, %2
  %16 = phi i32 [ 0, %2 ], [ %.lcssa, %.loopexit ]
  ret i32 %16

; <label>:17:                                     ; preds = %17, %.peel.newph.new
  %18 = phi i32 [ %.unr, %.peel.newph.new ], [ %23, %17 ]
  %19 = phi i32 [ %.unr2, %.peel.newph.new ], [ %22, %17 ]
  %20 = tail call i32 @myF(i32 %19)
  %21 = add nuw nsw i32 %18, 1
  %22 = tail call i32 @myF(i32 %20)
  %23 = add nuw nsw i32 %21, 1
  %24 = icmp eq i32 %23, %0
  br i1 %24, label %.loopexit.loopexit.unr-lcssa, label %17
}

attributes #0 = { norecurse nounwind readnone uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.module.flags = !{!0}
!llvm.ident = !{!1}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{!"clang version 5.0.0 (tags/RELEASE_500/final)"}
