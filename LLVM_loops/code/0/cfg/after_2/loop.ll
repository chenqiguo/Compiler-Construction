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
  %5 = add i32 %0, -1
  %xtraiter = and i32 %0, 1
  %6 = icmp ult i32 %5, 1
  br i1 %6, label %.loopexit.unr-lcssa, label %.new

.new:                                             ; preds = %4
  %unroll_iter = sub i32 %0, %xtraiter
  br label %15

.loopexit.unr-lcssa.loopexit:                     ; preds = %15
  %.lcssa.ph.ph = phi i32 [ %20, %15 ]
  %.unr.ph = phi i32 [ %21, %15 ]
  %.unr1.ph = phi i32 [ %20, %15 ]
  br label %.loopexit.unr-lcssa

.loopexit.unr-lcssa:                              ; preds = %.loopexit.unr-lcssa.loopexit, %4
  %.lcssa.ph = phi i32 [ undef, %4 ], [ %.lcssa.ph.ph, %.loopexit.unr-lcssa.loopexit ]
  %.unr = phi i32 [ 0, %4 ], [ %.unr.ph, %.loopexit.unr-lcssa.loopexit ]
  %.unr1 = phi i32 [ 0, %4 ], [ %.unr1.ph, %.loopexit.unr-lcssa.loopexit ]
  %lcmp.mod = icmp ne i32 %xtraiter, 0
  br i1 %lcmp.mod, label %.epil.preheader, label %.loopexit

.epil.preheader:                                  ; preds = %.loopexit.unr-lcssa
  br label %7

; <label>:7:                                      ; preds = %.epil.preheader
  %8 = phi i32 [ %.unr, %.epil.preheader ]
  %9 = phi i32 [ %.unr1, %.epil.preheader ]
  %10 = tail call i32 @myF(i32 %9)
  %11 = add nuw nsw i32 %8, 1
  %12 = icmp eq i32 %11, %0
  br label %.loopexit.epilog-lcssa

.loopexit.epilog-lcssa:                           ; preds = %7
  br label %.loopexit

.loopexit:                                        ; preds = %.loopexit.unr-lcssa, %.loopexit.epilog-lcssa
  %.lcssa = phi i32 [ %.lcssa.ph, %.loopexit.unr-lcssa ], [ %10, %.loopexit.epilog-lcssa ]
  br label %13

; <label>:13:                                     ; preds = %.loopexit, %2
  %14 = phi i32 [ 0, %2 ], [ %.lcssa, %.loopexit ]
  ret i32 %14

; <label>:15:                                     ; preds = %15, %.new
  %16 = phi i32 [ 0, %.new ], [ %21, %15 ]
  %17 = phi i32 [ 0, %.new ], [ %20, %15 ]
  %niter = phi i32 [ %unroll_iter, %.new ], [ %niter.nsub.1, %15 ]
  %18 = tail call i32 @myF(i32 %17)
  %19 = add nuw nsw i32 %16, 1
  %niter.nsub = sub i32 %niter, 1
  %20 = tail call i32 @myF(i32 %18)
  %21 = add nuw nsw i32 %19, 1
  %niter.nsub.1 = sub i32 %niter.nsub, 1
  %niter.ncmp.1 = icmp eq i32 %niter.nsub.1, 0
  br i1 %niter.ncmp.1, label %.loopexit.unr-lcssa.loopexit, label %15
}

attributes #0 = { norecurse nounwind readnone uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.module.flags = !{!0}
!llvm.ident = !{!1}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{!"clang version 5.0.0 (tags/RELEASE_500/final)"}
