#include "llvm/Pass.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Transforms/IPO/PassManagerBuilder.h"
#include "llvm/Analysis/LoopInfo.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;
using namespace std;
// llvm1
namespace {
  struct CAT : public FunctionPass {
    static char ID; 
    CAT() : FunctionPass(ID) { }

    bool doInitialization(Module &M) override {
      return false;
    }

    bool runOnFunction(Function &F) override {
      LoopInfo& LI = getAnalysis<LoopInfoWrapperPass>().getLoopInfo();
      errs() << "Function: " << F.getName() << "\n";

      for (auto i : LI){
        Loop *loop = &*i;

        errs() << " New loop\n";
        for (auto bbi = loop->block_begin(); bbi != loop->block_end(); ++bbi){
          BasicBlock *bb = *bbi;
          bb->print(errs());
        }

        vector<Loop *> subLoops = loop->getSubLoops();
        for (auto j : subLoops){
          Loop *subloop = &*j;

          errs() << "#### New sub-loop\n";
          for (auto bbi = subloop->block_begin(); bbi != subloop->block_end(); ++bbi){
            BasicBlock *bb = *bbi;
            bb->print(errs());
          }

        }
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
