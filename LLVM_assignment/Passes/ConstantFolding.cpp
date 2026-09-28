#include "llvm/Transforms/Utils/ConstantFolding.h"
#include "llvm/Analysis/ConstantFolding.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Constants.h"
#include "llvm/Support/raw_ostream.h"
#include <vector>

using namespace llvm;

PreservedAnalyses
ConstantFoldingPass::run(Function &F, FunctionAnalysisManager &AM) {
  std::vector<Instruction *> ToErase;
  const DataLayout &DL = F.getDataLayout();

  for (BasicBlock &BB : F) {
    for (Instruction &I : BB) {
      if (I.isTerminator() || isa<PHINode>(&I))
        continue;                        // control flow, not arithmetic

      // returns null unless every operand is already a constant
      Constant *Folded = ConstantFoldInstruction(&I, DL);
      if (!Folded)
        continue;

      errs() << "Constant folding applied to: " << I
             << "   (result " << *Folded << ")\n";

      I.replaceAllUsesWith(Folded);      // use the computed value
      ToErase.push_back(&I);             // mark the old instruction
    }
  }

  for (Instruction *I : ToErase)
    I->eraseFromParent();

  return ToErase.empty() ? PreservedAnalyses::all()
                         : PreservedAnalyses::none();
}
