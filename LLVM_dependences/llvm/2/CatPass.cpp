#include "llvm/Pass.h"
#include "llvm/IR/Function.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Transforms/IPO/PassManagerBuilder.h"
#include "llvm/Analysis/MemoryDependenceAnalysis.h"

#include <vector>
//llvm2: intra, dependence
using namespace std;
using namespace llvm;

namespace {
  struct CAT : public FunctionPass {
    static char ID; 
    Module *currM;

    CAT() : FunctionPass(ID) {}

    bool doInitialization (Module &M) override {
      currM = &M;
      return false;
    }

    bool runOnFunction (Function &F) override {
      errs() << "Function \"" << F.getName() << "\"\n";

      MemoryDependenceResults &MD = getAnalysis< MemoryDependenceWrapperPass >().getMemDep();

      vector< Instruction* > memInsts;
      for (auto &B : F){
        for (auto &I : B){
          if (  isa<LoadInst>(&I)   ||
                isa<StoreInst>(&I)  ){
            memInsts.push_back(&I);
            continue ;
          }
        }
      }

      errs() << " ### Memory accesses\n";
      for (auto &memInst : memInsts) {
        errs() << "   Mem inst: \"" << *memInst << "\"\n" ;

        MemDepResult memInstDeps = MD.getDependency(memInst);
        auto memInst2 = memInstDeps.getInst();
        if (memInst2 != nullptr){
          errs() << "     Dep inst: \"" << *memInst2 << "\"\n" ;
        }
      }

      return false;
    }

    void getAnalysisUsage(AnalysisUsage &AU) const override {
      AU.addRequired< MemoryDependenceWrapperPass >();
      AU.setPreservesAll();
      return;
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
