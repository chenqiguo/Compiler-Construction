#include "llvm/Pass.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Instructions.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Transforms/IPO/PassManagerBuilder.h"
#include <string.h>

using namespace llvm;
using namespace std;

namespace {
  struct CAT : public FunctionPass {
    static char ID; 

    CAT() : FunctionPass(ID) {}

    // This function is invoked once at the initialization phase of the compiler
    // The LLVM IR of functions isn't ready at this point
    bool doInitialization (Module &M) override {
      //errs() << "Hello LLVM World at \"doInitialization\"\n" ;
      return false;
    }

    // This function is invoked once per function compiled
    // The LLVM IR of the input functions is ready and it can be analyzed and/or transformed
    bool runOnFunction (Function &F) override {
      //errs() << "Hello LLVM World at \"runOnFunction\"\n" ;
      /*HW0*/
      //errs() << "Function \"" << F.getName() << "\"\n";
      //F.print(errs());

      /*HW1*/
      map<string, int> counter_CAT;
      counter_CAT["CAT_new"] = 0;
      counter_CAT["CAT_get"] = 0;
      counter_CAT["CAT_set"] = 0;
      counter_CAT["CAT_add"] = 0;
      counter_CAT["CAT_sub"] = 0;
      for (auto &bb : F) {
        for (auto &i : bb) {
          if (!isa<CallInst>(i)) {
            continue;
          }
          CallInst *callInst = cast<CallInst>(&i);
          Function *callee = callInst->getCalledFunction();
          if (counter_CAT.find(callee->getName()) != counter_CAT.end())
            counter_CAT[callee->getName()]++;
        }
      }

      for (auto &pair : counter_CAT) {
        if (pair.second)
          errs() << "H1: \"" <<  F.getName() << "\": " << pair.first <<": " << to_string(pair.second) <<"\n";
      }
      
      return false;
    }

    // We don't modify the program, so we preserve all analyses.
    // The LLVM IR of functions isn't ready at this point
    void getAnalysisUsage(AnalysisUsage &AU) const override {
      //errs() << "Hello LLVM World at \"getAnalysisUsage\"\n" ;
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
