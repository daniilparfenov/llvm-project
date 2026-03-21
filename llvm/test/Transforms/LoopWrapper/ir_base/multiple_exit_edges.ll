; ModuleID = './multiple_exit_edges.c'
source_filename = "./multiple_exit_edges.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

; Function Attrs: nounwind uwtable
define dso_local void @multi_exit(i32 noundef %n, i32 noundef %cond1) local_unnamed_addr #0 {
entry:
  %tobool.not = icmp eq i32 %cond1, 0
  %cmp3 = icmp sgt i32 %n, 0
  %or.cond4 = and i1 %tobool.not, %cmp3
  br i1 %or.cond4, label %if.end, label %cleanup

if.end:                                           ; preds = %entry, %if.end
  %i.05 = phi i32 [ %inc, %if.end ], [ 0, %entry ]
  call void (...) @side_effect() #2
  %inc = add nuw nsw i32 %i.05, 1
  %cmp = icmp slt i32 %inc, %n
  %or.cond = and i1 %tobool.not, %cmp
  br i1 %or.cond, label %if.end, label %cleanup, !llvm.loop !5

cleanup:                                          ; preds = %if.end, %entry
  ret void
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
