# Algebraic Identity

Custom LLVM pass: `algebraic-identity`
Source: `Passes/AlgebraicIdentity.cpp`

Simplifies expressions whose result follows from the shape of the expression
alone, without knowing the value of X:

```
X - X -> 0      X + 0 -> X      X * 1 -> X
X / X -> 1      X - 0 -> X      X * 0 -> 0
X ^ X -> 0      X / 1 -> X      X & X -> X,  X | X -> X
```

## Setup

```bash
# absolute path to the build directory from README step 2
LLVM_BUILD=/path/to/llvm/build

CLANG=$LLVM_BUILD/bin/clang
OPT=$LLVM_BUILD/bin/opt
```

## 1. Generate unoptimized IR

```bash
$CLANG -S -emit-llvm -O0 -Xclang -disable-O0-optnone algebraic_identity.c -o algebraic_identity.ll
```

`-Xclang -disable-O0-optnone` is required. Plain `-O0` marks the function
`optnone`, and the pass manager then skips our pass, so nothing happens.

## 2. Promote the variables out of memory

```bash
$OPT -S -passes='mem2reg' algebraic_identity.ll -o algebraic_identity_mem2reg.ll
```

Needed first. At `-O0`, `a / a` reads `a` twice, producing two separate load
instructions:

```llvm
%0 = load i32, ptr %a.addr
%1 = load i32, ptr %a.addr
%div = sdiv i32 %0, %1       ; two DIFFERENT SSA values
```

The `X / X` pattern compares values for identity, and `%0` is not `%1`, so it
does not match and the pass makes zero changes. After `mem2reg`:

```llvm
%div = sdiv i32 %a, %a       ; now matches
```

## 3. Apply the pass

```bash
$OPT -S -passes='algebraic-identity' algebraic_identity_mem2reg.ll -o algebraic_identity_opt.ll
```

Or both stages in one invocation:

```bash
$OPT -S -passes='mem2reg,algebraic-identity' algebraic_identity.ll -o algebraic_identity_opt.ll
```

The identities are chained: each one only becomes visible after the previous
one has fired. `result *= (b/b)` is a multiply by `%div1` until `b/b` folds to
`1`, and only then does `X * 1` apply. A single in-order walk is enough because
each result replaces its uses immediately. The whole function collapses to
`ret i32 23`.
