# Redundant Store Elimination

Custom LLVM pass: `redundant-store-elimination`
Source: `Passes/RedundantStoreElimination.cpp`

Deletes a store that writes the value the variable already holds, tracking the
current value of each stack slot one basic block at a time.

## Setup

```bash
# absolute path to the build directory from README step 2
LLVM_BUILD=/path/to/llvm/build

CLANG=$LLVM_BUILD/bin/clang
OPT=$LLVM_BUILD/bin/opt
```

## 1. Generate unoptimized IR

```bash
$CLANG -S -emit-llvm -O0 -Xclang -disable-O0-optnone redundant_store_elimination.c -o redundant_store_elimination.ll
```

`-Xclang -disable-O0-optnone` is required. Plain `-O0` marks the function
`optnone`, and the pass manager then skips our pass, so nothing happens.

## 2. Apply the pass

```bash
$OPT -S -passes='redundant-store-elimination' redundant_store_elimination.ll -o redundant_store_elimination_opt.ll
```

No prerequisite pass here. This pass works on memory directly, so it does not
need the values promoted out of stack slots first:

```llvm
store i32 3, ptr %a
store i32 4, ptr %b
store i32 3, ptr %a     ->    deleted, %a already holds 3
store i32 4, ptr %b     ->    deleted, %b already holds 4
```

Note that LLVM's stock `dce` makes no change to this file. Nothing here is dead
by DCE's definition: every value has a user, and a `store` is a side effect
rather than an unused result. Removing a store requires proving nobody reads
the location again, which is a different optimization (`dse`).
