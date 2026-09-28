; ModuleID = 'constant_folding.c'
source_filename = "constant_folding.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; Function Attrs: noinline nounwind uwtable
define dso_local i32 @compute() #0 {
entry:
  %result = alloca i32, align 4
  %a = alloca i32, align 4
  %b = alloca i32, align 4
  %c = alloca i32, align 4
  store i32 0, ptr %result, align 4
  store i32 2, ptr %a, align 4
  store i32 3, ptr %b, align 4
  %0 = load i32, ptr %a, align 4
  %add = add nsw i32 4, %0
  %1 = load i32, ptr %b, align 4
  %add1 = add nsw i32 %add, %1
  store i32 %add1, ptr %c, align 4
  %2 = load i32, ptr %a, align 4
  %3 = load i32, ptr %result, align 4
  %add2 = add nsw i32 %3, %2
  store i32 %add2, ptr %result, align 4
  %4 = load i32, ptr %b, align 4
  %5 = load i32, ptr %result, align 4
  %add3 = add nsw i32 %5, %4
  store i32 %add3, ptr %result, align 4
  %6 = load i32, ptr %c, align 4
  %7 = load i32, ptr %result, align 4
  %mul = mul nsw i32 %7, %6
  store i32 %mul, ptr %result, align 4
  %8 = load i32, ptr %result, align 4
  %div = sdiv i32 %8, 2
  store i32 %div, ptr %result, align 4
  %9 = load i32, ptr %result, align 4
  store i32 %9, ptr %a, align 4
  %10 = load i32, ptr %result, align 4
  ret i32 %10
}

attributes #0 = { noinline nounwind uwtable "frame-pointer"="all" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }

!llvm.module.flags = !{!0, !1, !2, !3}
!llvm.ident = !{!4}

!0 = !{i32 8, !"PIC Level", i32 2}
!1 = !{i32 7, !"PIE Level", i32 2}
!2 = !{i32 7, !"uwtable", i32 2}
!3 = !{i32 7, !"frame-pointer", i32 2}
!4 = !{!"clang version 24.0.0git (https://github.com/llvm/llvm-project.git f4b5a0b141f3538aab572cf13189392c3269bd98)"}
