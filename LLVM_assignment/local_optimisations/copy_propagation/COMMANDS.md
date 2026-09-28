# Copy Propagation

Custom LLVM pass: `copy-propagation`
Source: `Passes/CopyPropagation.cpp`

When a value is stored to a local variable and then loaded straight back, uses
the stored value directly instead of reloading it. Tracks the current value of
each stack slot one basic block at a time.

## Setup

```bash
# absolute path to the build directory from README step 2
LLVM_BUILD=/path/to/llvm/build

CLANG=$LLVM_BUILD/bin/clang
OPT=$LLVM_BUILD/bin/opt
```

## 1. Generate unoptimized IR

```bash
$CLANG -S -emit-llvm -O0 -Xclang -disable-O0-optnone copy_propagation.c -o copy_propagation.ll
```

`-Xclang -disable-O0-optnone` is required. Plain `-O0` marks the function
`optnone`, and the pass manager then skips our pass, so nothing happens.

## 2. Apply the pass

```bash
$OPT -S -passes='copy-propagation' copy_propagation.ll -o copy_propagation_opt.ll
```

No prerequisite pass here. This pass works on memory directly, and is itself
the step that makes value-based passes usable on `-O0` IR:

```llvm
%0 = load i32, ptr %d
store i32 %0, ptr %c
%1 = load i32, ptr %c     ->    replaced by %0    (the copy c = d)
%2 = load i32, ptr %b     ->    replaced by i32 4 (constant forwarded)
%add = add nsw i32 %1, %2 ->    %add = add nsw i32 %0, 4
%3 = load i32, ptr %e     ->    replaced by %add
```

`mem2reg` is the stock LLVM pass that serves the same purpose, and goes further
by deleting the stack slots altogether.
