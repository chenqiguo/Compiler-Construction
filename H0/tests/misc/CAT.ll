; ModuleID = '../../../../lib/CAT.bc'
source_filename = "../../../../lib/CAT.c"
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%struct._IO_FILE = type { i32, i8*, i8*, i8*, i8*, i8*, i8*, i8*, i8*, i8*, i8*, i8*, %struct._IO_marker*, %struct._IO_FILE*, i32, i32, i64, i16, i8, [1 x i8], i8*, i64, i8*, i8*, i8*, i8*, i64, i32, [20 x i8] }
%struct._IO_marker = type { %struct._IO_marker*, %struct._IO_FILE*, i32 }
%struct.internal_data_t = type { [20 x i8], i64, [20 x i8] }

@.str = private unnamed_addr constant [21 x i8] c"p6pbbUlpLo0BL1bM2k8K\00", align 1
@stderr = external dso_local local_unnamed_addr global %struct._IO_FILE*, align 8
@.str.1 = private unnamed_addr constant [31 x i8] c"libCAT: ERROR = input is NULL\0A\00", align 1
@.str.2 = private unnamed_addr constant [41 x i8] c"libCAT: ERROR = data has been corrupted\0A\00", align 1

; Function Attrs: nounwind uwtable
define dso_local noalias i8* @CAT_create_signed_value(i64) local_unnamed_addr #0 {
  %2 = tail call noalias i8* @malloc(i64 56) #5
  tail call void @llvm.memcpy.p0i8.p0i8.i64(i8* align 1 %2, i8* align 1 getelementptr inbounds ([21 x i8], [21 x i8]* @.str, i64 0, i64 0), i64 20, i1 false)
  %3 = getelementptr inbounds i8, i8* %2, i64 32
  tail call void @llvm.memcpy.p0i8.p0i8.i64(i8* nonnull align 1 %3, i8* align 1 getelementptr inbounds ([21 x i8], [21 x i8]* @.str, i64 0, i64 0), i64 20, i1 false)
  %4 = getelementptr inbounds i8, i8* %2, i64 24
  %5 = bitcast i8* %4 to i64*
  store i64 %0, i64* %5, align 8, !tbaa !2
  ret i8* %2
}

; Function Attrs: nounwind
declare dso_local noalias i8* @malloc(i64) local_unnamed_addr #1

; Function Attrs: nounwind uwtable
define dso_local i64 @CAT_get_signed_value(i8* readonly) local_unnamed_addr #0 {
  %2 = tail call fastcc %struct.internal_data_t* @internal_check_data(i8* %0)
  %3 = getelementptr inbounds %struct.internal_data_t, %struct.internal_data_t* %2, i64 0, i32 1
  %4 = load i64, i64* %3, align 8, !tbaa !2
  ret i64 %4
}

; Function Attrs: nounwind uwtable
define internal fastcc %struct.internal_data_t* @internal_check_data(i8* readonly) unnamed_addr #0 {
  %2 = icmp eq i8* %0, null
  br i1 %2, label %3, label %6

; <label>:3:                                      ; preds = %1
  %4 = load %struct._IO_FILE*, %struct._IO_FILE** @stderr, align 8, !tbaa !7
  %5 = tail call i64 @fwrite(i8* getelementptr inbounds ([31 x i8], [31 x i8]* @.str.1, i64 0, i64 0), i64 30, i64 1, %struct._IO_FILE* %4) #6
  tail call void @abort() #7
  unreachable

; <label>:6:                                      ; preds = %1
  %7 = bitcast i8* %0 to %struct.internal_data_t*
  %8 = tail call i32 @strncmp(i8* nonnull %0, i8* getelementptr inbounds ([21 x i8], [21 x i8]* @.str, i64 0, i64 0), i64 20) #8
  %9 = icmp eq i32 %8, 0
  br i1 %9, label %10, label %14

; <label>:10:                                     ; preds = %6
  %11 = getelementptr inbounds i8, i8* %0, i64 32
  %12 = tail call i32 @strncmp(i8* nonnull %11, i8* getelementptr inbounds ([21 x i8], [21 x i8]* @.str, i64 0, i64 0), i64 20) #8
  %13 = icmp eq i32 %12, 0
  br i1 %13, label %17, label %14

; <label>:14:                                     ; preds = %10, %6
  %15 = load %struct._IO_FILE*, %struct._IO_FILE** @stderr, align 8, !tbaa !7
  %16 = tail call i64 @fwrite(i8* getelementptr inbounds ([41 x i8], [41 x i8]* @.str.2, i64 0, i64 0), i64 40, i64 1, %struct._IO_FILE* %15) #6
  tail call void @abort() #7
  unreachable

; <label>:17:                                     ; preds = %10
  ret %struct.internal_data_t* %7
}

; Function Attrs: nounwind uwtable
define dso_local void @CAT_binary_sub(i8*, i8* readonly, i8* readonly) local_unnamed_addr #0 {
  %4 = tail call fastcc %struct.internal_data_t* @internal_check_data(i8* %1)
  %5 = tail call fastcc %struct.internal_data_t* @internal_check_data(i8* %2)
  %6 = tail call fastcc %struct.internal_data_t* @internal_check_data(i8* %0)
  %7 = getelementptr inbounds %struct.internal_data_t, %struct.internal_data_t* %4, i64 0, i32 1
  %8 = load i64, i64* %7, align 8, !tbaa !2
  %9 = getelementptr inbounds %struct.internal_data_t, %struct.internal_data_t* %5, i64 0, i32 1
  %10 = load i64, i64* %9, align 8, !tbaa !2
  %11 = sub nsw i64 %8, %10
  %12 = getelementptr inbounds %struct.internal_data_t, %struct.internal_data_t* %6, i64 0, i32 1
  store i64 %11, i64* %12, align 8, !tbaa !2
  ret void
}

; Function Attrs: nounwind uwtable
define dso_local void @CAT_binary_add(i8*, i8* readonly, i8* readonly) local_unnamed_addr #0 {
  %4 = tail call fastcc %struct.internal_data_t* @internal_check_data(i8* %1)
  %5 = tail call fastcc %struct.internal_data_t* @internal_check_data(i8* %2)
  %6 = tail call fastcc %struct.internal_data_t* @internal_check_data(i8* %0)
  %7 = getelementptr inbounds %struct.internal_data_t, %struct.internal_data_t* %4, i64 0, i32 1
  %8 = load i64, i64* %7, align 8, !tbaa !2
  %9 = getelementptr inbounds %struct.internal_data_t, %struct.internal_data_t* %5, i64 0, i32 1
  %10 = load i64, i64* %9, align 8, !tbaa !2
  %11 = add nsw i64 %10, %8
  %12 = getelementptr inbounds %struct.internal_data_t, %struct.internal_data_t* %6, i64 0, i32 1
  store i64 %11, i64* %12, align 8, !tbaa !2
  ret void
}

; Function Attrs: noreturn nounwind
declare dso_local void @abort() local_unnamed_addr #2

; Function Attrs: nounwind readonly
declare dso_local i32 @strncmp(i8* nocapture, i8* nocapture, i64) local_unnamed_addr #3

; Function Attrs: argmemonly nounwind
declare void @llvm.memcpy.p0i8.p0i8.i64(i8* nocapture writeonly, i8* nocapture readonly, i64, i1) #4

; Function Attrs: nounwind
declare i64 @fwrite(i8* nocapture, i64, i64, %struct._IO_FILE* nocapture) local_unnamed_addr #5

attributes #0 = { nounwind uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { nounwind "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #2 = { noreturn nounwind "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #3 = { nounwind readonly "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #4 = { argmemonly nounwind }
attributes #5 = { nounwind }
attributes #6 = { cold }
attributes #7 = { noreturn nounwind }
attributes #8 = { nounwind readonly }

!llvm.module.flags = !{!0}
!llvm.ident = !{!1}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{!"clang version 8.0.0 (tags/RELEASE_800/final)"}
!2 = !{!3, !6, i64 24}
!3 = !{!"", !4, i64 0, !6, i64 24, !4, i64 32}
!4 = !{!"omnipotent char", !5, i64 0}
!5 = !{!"Simple C/C++ TBAA"}
!6 = !{!"long", !4, i64 0}
!7 = !{!8, !8, i64 0}
!8 = !{!"any pointer", !4, i64 0}
