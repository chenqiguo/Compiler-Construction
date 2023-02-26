#include "llvm/Pass.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Transforms/IPO/PassManagerBuilder.h"
#include "llvm/Analysis/LoopInfo.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Analysis/OptimizationRemarkEmitter.h"
#include "llvm/IR/Dominators.h"
#include "llvm/Transforms/Utils/LoopUtils.h"
#include "llvm/Transforms/Utils/UnrollLoop.h"
#include "llvm/Analysis/AssumptionCache.h"
#include "llvm/Analysis/ScalarEvolution.h"
#include "llvm/Analysis/ScalarEvolutionExpressions.h"

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
      auto& LI = getAnalysis<LoopInfoWrapperPass>().getLoopInfo();
      auto& DT = getAnalysis<DominatorTreeWrapperPass>().getDomTree();
      auto& SE = getAnalysis<ScalarEvolutionWrapperPass>().getSE();
      auto& AC = getAnalysis<AssumptionCacheTracker>().getAssumptionCache(F);

      errs() << "Function: " << F.getName() << "\n";
      for (auto i : LI){
        auto loop = &*i;
        analyzeLoop(LI, loop, DT, SE, AC);
      }

      return false;
    }

    void getAnalysisUsage(AnalysisUsage &AU) const override {
      AU.addRequired<AssumptionCacheTracker>();
      AU.addRequired<DominatorTreeWrapperPass>();
      AU.addRequired<LoopInfoWrapperPass>();
      AU.addRequired<ScalarEvolutionWrapperPass>();

      return ;
    }

    private: 
    bool analyzeLoop (
      LoopInfo &LI, 
      Loop *loop, 
      DominatorTree &DT, 
      ScalarEvolution &SE, 
      AssumptionCache &AC
      ){

      /*
       * Check if the loop has been normalized.
       */
      if (  loop->isLoopSimplifyForm()  &&
            loop->isLCSSAForm(DT)       ){

        /*
         * We found a loop that has been normalized.
         */
        errs() << " Loop: " ;
        loop->print(errs());

        /*
         * Try to peel the loop
         */
        auto peelingCount = 1;
        auto peeled = peelLoop(
          loop, peelingCount,
          &LI, &SE, &DT, &AC, 
          true);
        if (peeled){
          errs() << "   Peeled\n";
          return true ;
        }

        /*
         * The loop didn't unroll.
         */
        errs() << "   Not peeled\n";
      }

      /* 
       * Analyze sub-loops of the loop given as input.
       */
      vector<Loop *> subLoops = loop->getSubLoops();
      for (auto j : subLoops){
        auto subloop = &*j;
        if (analyzeLoop(LI, subloop, DT, SE, AC)){
          return true;
        }
      }

      return false;
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
