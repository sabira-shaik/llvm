# MCW LLVM Assignment

Custom LLVM passes for local optimizations.

| Pass | Test case |
|---|---|
| `strength-reduction` | `local_optimisations/strength_reduction/` |
| `algebraic-identity` | `local_optimisations/algebraic_identity/` |
| `constant-folding` | `local_optimisations/constant_folding/` |
| `copy-propagation` | `local_optimisations/copy_propagation/` |
| `redundant-store-elimination` | `local_optimisations/redundant_store_elimination/` |

## Layout

All commands below are run from a single working directory holding both this
repository and the LLVM source tree:

```
<root>/
├── llvm-project/               cloned in step 1
├── build/                      created in step 2
└── LLVM_assignment/            this repository
    ├── Passes/                 the pass sources and headers
    └── local_optimisations/    one folder per pass, each with a COMMANDS.md
```

## Workflow

### 1. Clone LLVM

```bash
cd <root>
git clone --depth 1 https://github.com/llvm/llvm-project.git
```

### 2. Configure and build

```bash
cd <root>
mkdir build && cd build
cmake -G Ninja ../llvm-project/llvm \
  -DCMAKE_BUILD_TYPE=Release \
  -DLLVM_ENABLE_PROJECTS="clang;lld" \
  -DLLVM_TARGETS_TO_BUILD=X86 \
  -DLLVM_ENABLE_ASSERTIONS=ON
ninja
```

### 3. Copy the passes into the LLVM tree

```bash
cd <root>
cp LLVM_assignment/Passes/*.cpp llvm-project/llvm/lib/Transforms/Utils/
cp LLVM_assignment/Passes/*.h   llvm-project/llvm/include/llvm/Transforms/Utils/
```

### 4. Register the passes

**`llvm/lib/Transforms/Utils/CMakeLists.txt`** - add each source file:

```cmake
  AlgebraicIdentity.cpp
  ConstantFolding.cpp
  CopyPropagation.cpp
  RedundantStoreElimination.cpp
  StrengthReduction.cpp
```

**`llvm/lib/Passes/PassBuilder.cpp`** - add each header:

```cpp
#include "llvm/Transforms/Utils/AlgebraicIdentity.h"
#include "llvm/Transforms/Utils/ConstantFolding.h"
#include "llvm/Transforms/Utils/CopyPropagation.h"
#include "llvm/Transforms/Utils/RedundantStoreElimination.h"
#include "llvm/Transforms/Utils/StrengthReduction.h"
```

**`llvm/lib/Passes/PassRegistry.def`** - add each pass:

```cpp
FUNCTION_PASS("algebraic-identity", AlgebraicIdentityPass())
FUNCTION_PASS("constant-folding", ConstantFoldingPass())
FUNCTION_PASS("copy-propagation", CopyPropagationPass())
FUNCTION_PASS("redundant-store-elimination", RedundantStoreEliminationPass())
FUNCTION_PASS("strength-reduction", StrengthReductionPass())
```

### 5. Rebuild

```bash
cd <root>/build
ninja opt
```

### 6. Apply the passes

Each folder under `LLVM_assignment/local_optimisations/` has a `COMMANDS.md` with the
exact commands for that pass. Its Setup block expects `LLVM_BUILD` to point at
the build directory from step 2:

```bash
cd <root>/LLVM_assignment/local_optimisations/strength_reduction
LLVM_BUILD=<root>/build
```

Then follow that file.
