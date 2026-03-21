; ModuleID = './nested_loops.c'
source_filename = "./nested_loops.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

; Function Attrs: nounwind uwtable
define dso_local void @nested_loops(i32 noundef %n, i32 noundef %m) local_unnamed_addr #0 {
entry:
  %cmp14 = icmp sgt i32 %n, 0
  br i1 %cmp14, label %for.cond1.preheader.lr.ph, label %for.cond.cleanup

for.cond1.preheader.lr.ph:                        ; preds = %entry
  %cmp212 = icmp sgt i32 %m, 0
  br label %for.cond1.preheader

for.cond1.preheader:                              ; preds = %for.cond1.preheader.lr.ph, %for.cond.cleanup3
  %i.015 = phi i32 [ 0, %for.cond1.preheader.lr.ph ], [ %inc6, %for.cond.cleanup3 ]
  br i1 %cmp212, label %for.body4, label %for.cond.cleanup3

for.cond.cleanup:                                 ; preds = %for.cond.cleanup3, %entry
  ret void

for.cond.cleanup3:                                ; preds = %for.body4, %for.cond1.preheader
  %inc6 = add nuw nsw i32 %i.015, 1
  %exitcond16.not = icmp eq i32 %inc6, %n
  br i1 %exitcond16.not, label %for.cond.cleanup, label %for.cond1.preheader, !llvm.loop !5

for.body4:                                        ; preds = %for.cond1.preheader, %for.body4
  %j.013 = phi i32 [ %inc, %for.body4 ], [ 0, %for.cond1.preheader ]
  call void (...) @side_effect() #2
  %inc = add nuw nsw i32 %j.013, 1
  %exitcond.not = icmp eq i32 %inc, %m
  br i1 %exitcond.not, label %for.cond.cleanup3, label %for.body4, !llvm.loop !8
}

declare void @side_effect(...) local_unnamed_addr #1

attributes #0 = { nounwind uwtable "frame-pointer"="none" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #1 = { "frame-pointer"="none" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #2 = { nounwind }

!llvm.module.flags = !{!0, !1, !2, !3}
!llvm.ident = !{!4}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 7, !"PIC Level", i32 2}
!2 = !{i32 7, !"PIE Level", i32 2}
!3 = !{i32 7, !"uwtable", i32 1}
!4 = !{!"Ubuntu clang version 14.0.0-1ubuntu1.1"}
!5 = distinct !{!5, !6, !7}
!6 = !{!"llvm.loop.mustprogress"}
!7 = !{!"llvm.loop.unroll.disable"}
!8 = distinct !{!8, !6, !7}
