#include "llvm/IR/PassManager.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Instructions.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Plugins/PassPlugin.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;

struct MyPass : OptionalPassInfoMixin<MyPass> {

    PreservedAnalyses run(
        Function &F,
        FunctionAnalysisManager &) {

        errs() << "Function: " << F.getName() << "\n";

        for (BasicBlock &BB : F) {

            errs() << " BasicBlock: "
                   << BB.getName() << "\n";

            for (Instruction &I : BB) {

                errs() << "  Instruction: ";

                I.print(errs());

                errs() << "\n";
            }
        }

        return PreservedAnalyses::all();
    }
};


void registerPassBuilderCallbacks(PassBuilder &PB) {

    PB.registerPipelineParsingCallback(
        [](StringRef Name,
           FunctionPassManager &FPM,
           ArrayRef<PassBuilder::PipelineElement>) {

            if (Name == "my-pass") {

                FPM.addPass(MyPass());

                return true;
            }

            return false;
        });
}


// Plugin information
llvm::PassPluginLibraryInfo getMyPassPluginInfo() {

    return {
        LLVM_PLUGIN_API_VERSION,
        "MyPass",
        LLVM_VERSION_STRING,
        registerPassBuilderCallbacks
    };
}


// LLVM plugin entry point
extern "C" LLVM_ATTRIBUTE_WEAK
llvm::PassPluginLibraryInfo llvmGetPassPluginInfo() {

    return getMyPassPluginInfo();
}