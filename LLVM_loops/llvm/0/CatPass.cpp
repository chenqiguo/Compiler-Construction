#include "llvm/Pass.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Transforms/IPO/PassManagerBuilder.h"
#include "llvm/Analysis/LoopInfo.h"
#include "llvm/Support/raw_ostream.h"
// llvm0
using namespace llvm;
using namespace std;

namespace {
  struct CAT : public FunctionPass {
    static char ID; 

    CAT() : FunctionPass(ID) {}

    bool doInitialization (Module &M) override {
      return false;
    }

    bool runOnFunction (Function &F) override {
      errs() << "Function: " << F.getName() << "\n";

      LoopInfo& LI = getAnalysis<LoopInfoWrapperPass>().getLoopInfo();

      for (auto i : LI){
        Loop *loop = &*i;
        errs() << " BEGIN loop: ";
        loop->print(errs());

        for (auto bbi = loop->block_begin(); bbi != loop->block_end(); ++bbi){
          BasicBlock *bb = *bbi;
          bb->print(errs());
        }

        errs() << " END loop\n";
      }

      return false;
    }

    void getAnalysisUsage(AnalysisUsage &AU) const override {
      AU.addRequired<LoopInfoWrapperPass>();
      AU.setPreservesAll();
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
