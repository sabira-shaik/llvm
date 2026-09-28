#include "llvm/Transforms/Utils/CopyPropagation.h"
#include "llvm/ADT/DenseMap.h"
#include "llvm/IR/Instructions.h"
#include "llvm/Support/raw_ostream.h"
#include <vector>

using namespace llvm;

PreservedAnalyses
CopyPropagationPass::run(Function &F, FunctionAnalysisManager &AM) {
  std::vector<Instruction *> ToErase;

  for (BasicBlock &BB : F) {
    // value currently held by each local variable.
    // one map per block: we never reason across control flow
    DenseMap<AllocaInst *, Value *> Current;

    for (Instruction &I : BB) {
      // a call can write to anything we are tracking
      if (isa<CallBase>(&I)) {
        Current.clear();
        continue;
      }

      if (auto *SI = dyn_cast<StoreInst>(&I)) {
        auto *Var = dyn_cast<AllocaInst>(SI->getPointerOperand());
        if (!Var || !SI->isSimple())
          Current.clear();               // unknown pointer: forget everything
        else
          Current[Var] = SI->getValueOperand();
        continue;
      }

      auto *LI = dyn_cast<LoadInst>(&I);
      if (!LI || !LI->isSimple())
        continue;                        // only plain loads

      auto *Var = dyn_cast<AllocaInst>(LI->getPointerOperand());
      if (!Var)
        continue;                        // not a local variable

      Value *Known = Current.lookup(Var);
      if (!Known || Known->getType() != LI->getType())
        continue;                        // nothing recorded, or wrong type

      errs() << "Copy propagation applied to: " << I << "\n";

      LI->replaceAllUsesWith(Known);     // use the stored value directly
      ToErase.push_back(LI);             // mark the redundant load
    }
  }

  for (Instruction *I : ToErase)
    I->eraseFromParent();

  return ToErase.empty() ? PreservedAnalyses::all()
                         : PreservedAnalyses::none();
}
