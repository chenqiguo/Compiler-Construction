#include "llvm/Pass.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Transforms/IPO/PassManagerBuilder.h"
#include "llvm/Analysis/LoopInfo.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;
using namespace std;

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

        analyzeLoop(loop, "#");
      }

      return false;
    }

    void getAnalysisUsage(AnalysisUsage &AU) const override {
      AU.addRequired<LoopInfoWrapperPass>();
      AU.setPreservesAll();
      return ;
    }

    private: 
    void analyzeLoop (Loop *loop, string prefix){
      errs() << prefix << " New loop\n";
      for (auto bbi = loop->block_begin(); bbi != loop->block_end(); ++bbi){
        BasicBlock *bb = *bbi;
        bb->print(errs());
      }

      vector<Loop *> subLoops = loop->getSubLoops();
      auto numberOfSubloops = subLoops.size();
      errs() << prefix << " -> there " ;
      if (numberOfSubloops == 1){
        errs() << "is ";
      } else {
        errs() << "are " ;
      }
      errs() << numberOfSubloops << " sub-loop";
      if (numberOfSubloops != 1){
        errs() << "s" ;
      }
      errs() << "\n";
      for (auto j : subLoops){
        Loop *subloop = &*j;

        analyzeLoop(subloop, prefix.append("#"));
      }
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
