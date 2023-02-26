#include "llvm/Pass.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Transforms/IPO/PassManagerBuilder.h"
#include "llvm/Analysis/LoopInfo.h"
#include "llvm/Analysis/ScalarEvolution.h"
#include "llvm/Analysis/ScalarEvolutionExpressions.h"
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
      auto &LI = getAnalysis<LoopInfoWrapperPass>().getLoopInfo();
      ScalarEvolution *SE =  &getAnalysis<ScalarEvolutionWrapperPass>().getSE();

      errs() << "Function: " << F.getName() << "\n";
      for (auto i = LI.begin(); i != LI.end(); ++i){
        auto loop = *i;
        errs() << " New loop\n";
        for (auto bbi = loop->block_begin(); bbi != loop->block_end(); ++bbi){
          auto bb = *bbi;
          for (auto &inst : *bb){

            const SCEV *S = SE->getSCEV(&inst);

            if (auto AR = dyn_cast<SCEVAddRecExpr>(S)){
              errs() << "   Instruction ";
              inst.print(errs());
              errs() << " is SCEVAddRecExpr\n" ;

              errs() << "     SCE: " ;
              AR->print(errs());
              errs() << "\n";
              continue ;
            }

            errs() << "     SCE: " ;
            S->print(errs());
            errs() << "\n";
          }
        }
      }
      return false;
    }

    void getAnalysisUsage(AnalysisUsage &AU) const override {
      AU.addRequired<LoopInfoWrapperPass>();
      AU.addRequired<ScalarEvolutionWrapperPass>();
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
