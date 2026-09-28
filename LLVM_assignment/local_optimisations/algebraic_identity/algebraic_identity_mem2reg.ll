; ModuleID = 'algebraic_identity.ll'
source_filename = "algebraic_identity.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; Function Attrs: noinline nounwind uwtable
define dso_local i32 @compute(i32 noundef %a, i32 noundef %b) #0 {
entry:
  %div = sdiv i32 %a, %a
  %div1 = sdiv i32 %b, %b
  %mul = mul nsw i32 %div, %div1
  %sub = sub nsw i32 %b, %b
  %add = add nsw i32 %mul, %sub
  %div2 = sdiv i32 %add, %add
  %sub3 = sub nsw i32 %div2, %div2
  %add4 = add nsw i32 %sub3, 23
  ret i32 %add4
}

attributes #0 = { noinline nounwind uwtable "frame-pointer"="all" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }

!llvm.module.flags = !{!0, !1, !2, !3}
!llvm.ident = !{!4}

!0 = !{i32 8, !"PIC Level", i32 2}
!1 = !{i32 7, !"PIE Level", i32 2}
!2 = !{i32 7, !"uwtable", i32 2}
!3 = !{i32 7, !"frame-pointer", i32 2}
!4 = !{!"clang version 24.0.0git (https://github.com/llvm/llvm-project.git f4b5a0b141f3538aab572cf13189392c3269bd98)"}
