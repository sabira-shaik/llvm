#ifndef LLVM_TRANSFORMS_UTILS_CONSTANTFOLDING_H
#define LLVM_TRANSFORMS_UTILS_CONSTANTFOLDING_H

#include "llvm/IR/PassManager.h"
#include "llvm/Support/Compiler.h"

namespace llvm {

class Function;

class ConstantFoldingPass : public OptionalPassInfoMixin<ConstantFoldingPass> {
public:
  LLVM_ABI PreservedAnalyses run(Function &F, FunctionAnalysisManager &AM);
};

} // namespace llvm

#endif // LLVM_TRANSFORMS_UTILS_CONSTANTFOLDING_H
