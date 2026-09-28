#include "llvm/Transforms/Utils/AlgebraicIdentity.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Constants.h"
#include "llvm/Support/raw_ostream.h"
#include <vector>

using namespace llvm;

// is V the constant integer N?
static bool isConst(Value *V, int64_t N) {
  auto *CI = dyn_cast<ConstantInt>(V);
  return CI && CI->getSExtValue() == N;
}

PreservedAnalyses
AlgebraicIdentityPass::run(Function &F, FunctionAnalysisManager &AM) {
  std::vector<Instruction *> ToErase;

  for (BasicBlock &BB : F) {
    for (Instruction &I : BB) {
      auto *BO = dyn_cast<BinaryOperator>(&I);
      if (!BO)
        continue;                        // only binary operators

      Value *op0 = BO->getOperand(0);
      Value *op1 = BO->getOperand(1);

      Constant *Zero = ConstantInt::get(BO->getType(), 0);
      Constant *One  = ConstantInt::get(BO->getType(), 1);

      // what this instruction always computes, or null if no identity applies
      Value *Repl = nullptr;

      switch (BO->getOpcode()) {
      case Instruction::Add:                       // X + 0 -> X
        if (isConst(op1, 0))      Repl = op0;
        else if (isConst(op0, 0)) Repl = op1;      // 0 + X -> X
        break;

      case Instruction::Sub:
        if (op0 == op1)           Repl = Zero;     // X - X -> 0
        else if (isConst(op1, 0)) Repl = op0;      // X - 0 -> X
        break;                                     // 0 - X is -X, NOT X

      case Instruction::Mul:
        if (isConst(op1, 1))      Repl = op0;      // X * 1 -> X
        else if (isConst(op0, 1)) Repl = op1;      // 1 * X -> X
        else if (isConst(op0, 0) || isConst(op1, 0))
                                  Repl = Zero;     // X * 0 -> 0
        break;

      case Instruction::SDiv:
      case Instruction::UDiv:
        // dividing by zero is undefined, so we may assume X != 0
        if (op0 == op1)           Repl = One;      // X / X -> 1
        else if (isConst(op1, 1)) Repl = op0;      // X / 1 -> X
        break;                                     // 1 / X is NOT X

      case Instruction::And:
      case Instruction::Or:
        if (op0 == op1)           Repl = op0;      // X & X -> X,  X | X -> X
        break;

      case Instruction::Xor:
        if (op0 == op1)           Repl = Zero;     // X ^ X -> 0
        break;

      default:
        break;
      }

      if (!Repl)
        continue;                        // no identity matched

      errs() << "Algebraic identity applied to: " << I << "\n";

      BO->replaceAllUsesWith(Repl);      // everyone uses the simpler value
      ToErase.push_back(BO);             // mark the old instruction
    }
  }

  for (Instruction *I : ToErase)
    I->eraseFromParent();

  return ToErase.empty() ? PreservedAnalyses::all()
                         : PreservedAnalyses::none();
}
