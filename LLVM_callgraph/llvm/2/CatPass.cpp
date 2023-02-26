#include "llvm/ADT/Statistic.h"
#include "llvm/Pass.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Constants.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Transforms/IPO/PassManagerBuilder.h"
#include "llvm/Transforms/Utils/BasicBlockUtils.h"
#include <iostream>
#include <map>
#include <vector>
// 2
using namespace llvm;
using namespace std;

namespace {
  struct CAT : public ModulePass {
    static char ID; 

    CAT() : ModulePass(ID) { }

    bool runOnModule(Module &M) override {
      errs() << "Module \"" << M.getName() << "\"\n";
      for (auto &F : M) {
        errs() << " Function \"" << F.getName() << "\"\n";
        for (auto &B : F) {
            for (auto &I : B) {
                if (auto call = dyn_cast<CallInst>(&I)){
                  Function *callee = call->getCalledFunction();
                  errs() << "   Calls " << callee->getName() << "\n";
                }
            }
        }
      }

      return false;
    }

    void getAnalysisUsage(AnalysisUsage &AU) const override {
      AU.setPreservesAll();
      return ;
    }
  };
}

// Next there is code to register your pass to "opt"
char CAT::ID = 0;
static RegisterPass<CAT> X("CAT", "Homework for the CAT class");

// Next there is code to register your pass to "clang"
static CAT * _PassMaker = NULL;
static RegisterStandardPasses _RegPass1(PassManagerBuilder::EP_OptimizerLast,
    [](const PassManagerBuilder&, legacy::PassManagerBase& PM) {
        if(!_PassMaker){ PM.add(_PassMaker = new CAT());}}); // ** for -Ox
static RegisterStandardPasses _RegPass2(PassManagerBuilder::EP_EnabledOnOptLevel0,
    [](const PassManagerBuilder&, legacy::PassManagerBase& PM) {
        if(!_PassMaker){ PM.add(_PassMaker = new CAT()); }}); // ** for -O0
