; ModuleID = 'algebraic_identity.c'
source_filename = "algebraic_identity.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; Function Attrs: noinline nounwind uwtable
define dso_local i32 @compute(i32 noundef %a, i32 noundef %b) #0 {
entry:
  %a.addr = alloca i32, align 4
  %b.addr = alloca i32, align 4
  %result = alloca i32, align 4
  store i32 %a, ptr %a.addr, align 4
  store i32 %b, ptr %b.addr, align 4
  %0 = load i32, ptr %a.addr, align 4
  %1 = load i32, ptr %a.addr, align 4
  %div = sdiv i32 %0, %1
  store i32 %div, ptr %result, align 4
  %2 = load i32, ptr %b.addr, align 4
  %3 = load i32, ptr %b.addr, align 4
  %div1 = sdiv i32 %2, %3
  %4 = load i32, ptr %result, align 4
  %mul = mul nsw i32 %4, %div1
  store i32 %mul, ptr %result, align 4
  %5 = load i32, ptr %b.addr, align 4
  %6 = load i32, ptr %b.addr, align 4
  %sub = sub nsw i32 %5, %6
  %7 = load i32, ptr %result, align 4
  %add = add nsw i32 %7, %sub
  store i32 %add, ptr %result, align 4
  %8 = load i32, ptr %result, align 4
  %9 = load i32, ptr %result, align 4
  %div2 = sdiv i32 %9, %8
  store i32 %div2, ptr %result, align 4
  %10 = load i32, ptr %result, align 4
  %11 = load i32, ptr %result, align 4
  %sub3 = sub nsw i32 %11, %10
  store i32 %sub3, ptr %result, align 4
  %12 = load i32, ptr %result, align 4
  %add4 = add nsw i32 %12, 23
  store i32 %add4, ptr %result, align 4
  %13 = load i32, ptr %result, align 4
  ret i32 %13
}

attributes #0 = { noinline nounwind uwtable "frame-pointer"="all" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }

!llvm.module.flags = !{!0, !1, !2, !3}
!llvm.ident = !{!4}

!0 = !{i32 8, !"PIC Level", i32 2}
!1 = !{i32 7, !"PIE Level", i32 2}
!2 = !{i32 7, !"uwtable", i32 2}
!3 = !{i32 7, !"frame-pointer", i32 2}
!4 = !{!"clang version 24.0.0git (https://github.com/llvm/llvm-project.git f4b5a0b141f3538aab572cf13189392c3269bd98)"}
