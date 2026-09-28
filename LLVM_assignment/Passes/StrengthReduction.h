#ifndef LLVM_TRANSFORMS_UTILS_STRENGTHREDUCTION_H
#define LLVM_TRANSFORMS_UTILS_STRENGTHREDUCTION_H

#include "llvm/IR/PassManager.h"
#include "llvm/Support/Compiler.h"

namespace llvm {

class Function;

class StrengthReductionPass
    : public OptionalPassInfoMixin<StrengthReductionPass> {
public:
  LLVM_ABI PreservedAnalyses run(Function &F, FunctionAnalysisManager &AM);
};

} // namespace llvm

#endif // LLVM_TRANSFORMS_UTILS_STRENGTHREDUCTION_H
