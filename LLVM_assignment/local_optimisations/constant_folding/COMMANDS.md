# Constant Folding

Custom LLVM pass: `constant-folding`
Source: `Passes/ConstantFolding.cpp`

Evaluates an instruction at compile time when all of its operands are already
constants, and replaces it with the result.

## Setup

```bash
# absolute path to the build directory from README step 2
LLVM_BUILD=/path/to/llvm/build

CLANG=$LLVM_BUILD/bin/clang
OPT=$LLVM_BUILD/bin/opt
```

## 1. Generate unoptimized IR

```bash
$CLANG -S -emit-llvm -O0 -Xclang -disable-O0-optnone constant_folding.c -o constant_folding.ll
```

`-Xclang -disable-O0-optnone` is required. Plain `-O0` marks the function
`optnone`, and the pass manager then skips our pass, so nothing happens.

## 2. Promote the variables out of memory

```bash
$OPT -S -passes='mem2reg' constant_folding.ll -o constant_folding_mem2reg.ll
```

Needed first. At `-O0` the constants live in stack slots, so the arithmetic
reads them with loads:

```llvm
store i32 2, ptr %a
%0 = load i32, ptr %a
%add = add nsw i32 4, %0     ; operand is a load, not a constant
```

`ConstantFoldInstruction` only fires when *every* operand is a constant, so
without `mem2reg` the pass makes zero changes. After `mem2reg`:

```llvm
%add = add nsw i32 4, 2      ; now foldable
```

## 3. Apply the pass

```bash
$OPT -S -passes='constant-folding' constant_folding_mem2reg.ll -o constant_folding_opt.ll
```

Or both stages in one invocation:

```bash
$OPT -S -passes='mem2reg,constant-folding' constant_folding.ll -o constant_folding_opt.ll
```
