#include "llvm/Pass.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Transforms/IPO/PassManagerBuilder.h"
#include "llvm/Analysis/LoopInfo.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/Dominators.h"
#include "llvm/Transforms/Utils/LoopUtils.h"

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
      auto loopID = 0;
      auto& LI = getAnalysis<LoopInfoWrapperPass>().getLoopInfo();
      auto& DT = getAnalysis<DominatorTreeWrapperPass>().getDomTree();
      errs() << "Function: " << F.getName() << "\n";
      for (auto i : LI){
        auto loop = &*i;
        analyzeLoop(LI, loop, DT, "#", false, loopID);
      }
      return false;
    }

    void getAnalysisUsage(AnalysisUsage &AU) const override {
      AU.addRequired<DominatorTreeWrapperPass>();
      AU.addRequired<LoopInfoWrapperPass>();
      AU.setPreservesAll();
    }

    private: 
    void analyzeLoop (LoopInfo &LI, Loop *loop, DominatorTree &DT, string prefix, bool printBasicBlocks, int &loopID){
      errs() << prefix << " Loop " << loopID++ ;
      errs() << " has" << (loop->isLoopSimplifyForm() ? "" : " not") << " been normalized ";
      errs() << "and it has" << (loop->isLCSSAForm(DT) ? "" : " not") << " been LCSSA normalized ";
      errs() << "\n";

      if (!loop->isLCSSAForm(DT)){
        errs() << prefix << "   Transform the loop to LCSSA form\n";
        formLCSSA(*loop, DT, &LI, nullptr);
        analyzeLoop(LI, loop, DT, prefix, printBasicBlocks, --loopID);
        return ;
      }

      errs() << prefix << "   Pre-header: ";
      loop->getLoopPreheader()->print(errs());
      errs() << "\n";

      errs() << prefix << "   Header: ";
      loop->getHeader()->print(errs());
      errs() << "\n";
      
      errs() << prefix << "   Latch: ";
      loop->getLoopLatch()->print(errs());
      errs() << "\n";

      SmallVector<BasicBlock *, 8> EBs;
      loop->getExitBlocks(EBs);
      for (auto exitBB : EBs){
        errs() << prefix << "   Exit block: ";
        exitBB->print(errs());
      }
      errs() << "\n";


      /* Analyze basic blocks that compose the loop given as input.
       */
      errs() << prefix << "   Invariants\n";
      for (auto bbi = loop->block_begin(); bbi != loop->block_end(); ++bbi){
        BasicBlock *bb = *bbi;
  
        /* Analyze the basic block included in the current loop.
         */
        //TODO

        /* Print the basic block.
         */
        if (printBasicBlocks){
          bb->print(errs());
        }

        for (auto& instr_iter : *bb){
          auto instr = &instr_iter;
          if (loop->isLoopInvariant(instr)){
            errs() << prefix << "     Instruction \"" << *instr << "\" is invariant\n";
            continue ;
          }
          if (loop->hasLoopInvariantOperands(instr)){
            errs() << prefix << "     Instruction \"" << *instr << "\" has only invariant operands\n";
          }
        }
      }

      /* Analyze sub-loops of the loop given as input.
       */
      auto subLoops = loop->getSubLoops();
      errs() << prefix << " -> " << subLoops.size() << " Sub-loops\n";
      for (auto j : subLoops){
        auto subloop = &*j;

        analyzeLoop(LI, subloop, DT, prefix + '#', printBasicBlocks, loopID);
      }
      errs() << prefix << " No more sub-loops\n";
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
