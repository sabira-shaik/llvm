# Strength Reduction

Custom LLVM pass: `strength-reduction`
Source: `Passes/StrengthReduction.cpp`

Replaces an integer multiply by a power of two with a left shift, which is
cheaper on every target: `mul X, 2^k` becomes `shl X, k`.

## Setup

```bash
# absolute path to the build directory from README step 2
LLVM_BUILD=/path/to/llvm/build

CLANG=$LLVM_BUILD/bin/clang
OPT=$LLVM_BUILD/bin/opt
```

## 1. Generate unoptimized IR

```bash
$CLANG -S -emit-llvm -O0 -Xclang -disable-O0-optnone strength_reduction.c -o strength_reduction.ll
```

`-Xclang -disable-O0-optnone` is required. Plain `-O0` marks the function
`optnone`, and the pass manager then skips our pass, so nothing happens.

## 2. Apply the pass

```bash
$OPT -S -passes='strength-reduction' strength_reduction.ll -o strength_reduction_opt.ll
```

No prerequisite pass here. The constants are already literal operands of the
multiply, so the pattern matches directly on the `-O0` IR:

```llvm
%mul  = mul nsw i32 2, %0     ->    %mul  = shl nsw i32 %0, 1
%mul1 = mul nsw i32 %1, 8     ->    %mul1 = shl nsw i32 %1, 3
```

Note the constant sits on the left in one case and the right in the other.
At `-O0` clang does not canonicalize commutative operands, so the pass checks
both sides.
