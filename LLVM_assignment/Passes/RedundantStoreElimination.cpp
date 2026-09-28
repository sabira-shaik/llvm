#include "llvm/Transforms/Utils/RedundantStoreElimination.h"
#include "llvm/ADT/DenseMap.h"
#include "llvm/IR/Instructions.h"
#include "llvm/Support/raw_ostream.h"
#include <vector>

using namespace llvm;

PreservedAnalyses
RedundantStoreEliminationPass::run(Function &F, FunctionAnalysisManager &AM) {
  std::vector<Instruction *> ToErase;

  for (BasicBlock &BB : F) {
    // value currently held by each local variable, one map per block
    DenseMap<AllocaInst *, Value *> Current;

    for (Instruction &I : BB) {
      // a call can write to anything we are tracking
      if (isa<CallBase>(&I)) {
        Current.clear();
        continue;
      }

      auto *SI = dyn_cast<StoreInst>(&I);
      if (!SI)
        continue;                        // only interested in stores

      auto *Var = dyn_cast<AllocaInst>(SI->getPointerOperand());
      if (!Var || !SI->isSimple()) {
        Current.clear();                 // unknown pointer: forget everything
        continue;                        // and never delete a volatile store
      }

      Value *V = SI->getValueOperand();

      if (Current.lookup(Var) == V) {
        errs() << "Redundant store eliminated: " << I << "\n";
        ToErase.push_back(SI);           // variable already holds this value
        continue;
      }

      Current[Var] = V;                  // remember the new value
    }
  }

  for (Instruction *I : ToErase)
    I->eraseFromParent();

  return ToErase.empty() ? PreservedAnalyses::all()
                         : PreservedAnalyses::none();
}
