; ModuleID = 'loop.bc'
source_filename = "program.c"
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; Function Attrs: norecurse nounwind uwtable
define i32 @myF(i32, i32* nocapture) local_unnamed_addr #0 {
  %3 = icmp slt i32 %0, 0
  br i1 %3, label %4, label %7

; <label>:4:                                      ; preds = %2
  %5 = load i32, i32* %1, align 4, !tbaa !2
  %6 = add nsw i32 %5, -1
  store i32 %6, i32* %1, align 4, !tbaa !2
  br label %7

; <label>:7:                                      ; preds = %4, %2
  %8 = add nsw i32 %0, 1
  ret i32 %8
}

; Function Attrs: nounwind uwtable
define i32 @main(i32, i8** nocapture readnone) local_unnamed_addr #1 {
  %3 = alloca i32, align 4
  %4 = bitcast i32* %3 to i8*
  call void @llvm.lifetime.start.p0i8(i64 4, i8* nonnull %4) #3
  store i32 %0, i32* %3, align 4, !tbaa !2
  %5 = icmp sgt i32 %0, 0
  br i1 %5, label %6, label %7

; <label>:6:                                      ; preds = %2
  br label %9

.loopexit:                                        ; preds = %16, %9
  %.lcssa = phi i32 [ %12, %9 ], [ %17, %16 ]
  br label %7

; <label>:7:                                      ; preds = %.loopexit, %2
  %8 = phi i32 [ 0, %2 ], [ %.lcssa, %.loopexit ]
  call void @llvm.lifetime.end.p0i8(i64 4, i8* nonnull %4) #3
  ret i32 %8

; <label>:9:                                      ; preds = %16, %6
  %10 = phi i32 [ 0, %6 ], [ %18, %16 ]
  %11 = phi i32 [ 0, %6 ], [ %17, %16 ]
  %12 = call i32 @myF(i32 %11, i32* nonnull %3)
  %13 = add nuw nsw i32 %10, 1
  %14 = load i32, i32* %3, align 4, !tbaa !2
  %15 = icmp slt i32 %13, %14
  br i1 %15, label %16, label %.loopexit

; <label>:16:                                     ; preds = %9
  %17 = call i32 @myF(i32 %12, i32* nonnull %3)
  %18 = add nuw nsw i32 %13, 1
  %19 = load i32, i32* %3, align 4, !tbaa !2
  %20 = icmp slt i32 %18, %19
  br i1 %20, label %9, label %.loopexit
}

; Function Attrs: argmemonly nounwind
declare void @llvm.lifetime.start.p0i8(i64, i8* nocapture) #2

; Function Attrs: argmemonly nounwind
declare void @llvm.lifetime.end.p0i8(i64, i8* nocapture) #2

attributes #0 = { norecurse nounwind uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { nounwind uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #2 = { argmemonly nounwind }
attributes #3 = { nounwind }

!llvm.module.flags = !{!0}
!llvm.ident = !{!1}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{!"clang version 5.0.0 (tags/RELEASE_500/final)"}
!2 = !{!3, !3, i64 0}
!3 = !{!"int", !4, i64 0}
!4 = !{!"omnipotent char", !5, i64 0}
!5 = !{!"Simple C/C++ TBAA"}
