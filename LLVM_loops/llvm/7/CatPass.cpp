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

      OptimizationRemarkEmitter ORE(&F);
      errs() << "Function: " << F.getName() << "\n";
      for (auto i : LI){
        auto loop = &*i;
        analyzeLoop(LI, loop, DT, SE, AC, ORE);
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
      AssumptionCache &AC,
      OptimizationRemarkEmitter &ORE
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
         * Identify the trip count.
         */
        auto tripCount = SE.getSmallConstantTripCount(loop);
        if (tripCount > 0){

          /*
           * The loop has a constant trip count.
           */
          errs() << "   Trip count: " << tripCount << "\n";

          /*
           * Try to unroll the loop
           */
          auto forceUnroll = false;
          auto allowRuntime = false;
          auto allowExpensiveTripCount = true;
          auto preserveCondBr = false;
          auto preserveOnlyFirst = false;
          auto unrolled = UnrollLoop(
            loop, 2, 
            tripCount, 
            forceUnroll, 
            allowRuntime, allowExpensiveTripCount, 
            preserveCondBr, preserveOnlyFirst, 
            0, 0, 
            false,
            &LI, &SE, &DT, &AC, &ORE, 
            true);

          /*
           * Check if the loop unrolled.
           */
          switch (unrolled){
            case LoopUnrollResult::FullyUnrolled :
              errs() << "   Fully unrolled\n";
              return true ;

            case LoopUnrollResult::PartiallyUnrolled :
              errs() << "   Partially unrolled\n";
              return true ;

            case LoopUnrollResult::Unmodified :
              errs() << "   Not unrolled\n";
              break ;

            default:
              abort();
          }
        }
      }

      /* 
       * Analyze sub-loops of the loop given as input.
       */
      auto subLoops = loop->getSubLoops();
      for (auto j : subLoops){
        auto subloop = &*j;
        if (analyzeLoop(LI, subloop, DT, SE, AC, ORE)){
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
