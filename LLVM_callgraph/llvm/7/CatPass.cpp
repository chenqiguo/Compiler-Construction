#include "llvm/ADT/Statistic.h"
#include "llvm/Pass.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Instructions.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Transforms/IPO/PassManagerBuilder.h"
#include "llvm/Transforms/Utils/BasicBlockUtils.h"
#include <iostream>
#include <map>
#include <vector>
// 7
using namespace llvm;

namespace {
  enum randomUses {TBC, invoked, notInvoked};
  struct random_info_t {
    randomUses r;
    uint32_t level;
  };

  struct CAT : public ModulePass {
    static char ID; 
    std::map<Function *,random_info_t> randomInfo;

    CAT() : ModulePass(ID) { }

    bool runOnModule(Module &M) override {
      errs() << "Module \"" << M.getName() << "\"\n";

      errs() << " Identify functions affected directly\n" ;
      tagFunctionsDirectlyAffected(M);

      errs() << " Identify functions affected indirectly\n" ;
      identifyFunctionsIndirectlyAffected(M);

      errs() << " Identify functions not affected\n" ;
      identifyFunctionsNotAffected(M);
      printStatus(M);

      return false;
    }

    void printStatus (Module &M){
      errs() << "   Functions affected:\n";
      for (auto &F : M) {
        if (randomInfo[&F].r == invoked){
          errs() << "    Level " << randomInfo[&F].level << ": " << F.getName() << "\n";
        }
      }

      errs() << "   Functions not affected:\n";
      for (auto &F : M) {
        if (randomInfo[&F].r == notInvoked){
          errs() << "    " << F.getName() << "\n";
        }
      }

      return ;
    }

    void tagFunctionsDirectlyAffected (Module &M){
      for (auto &F : M) {

        /* Initialize the information about F.
         */
        randomInfo[&F].r = TBC;
        randomInfo[&F].level = 0;

        /* Analyze F.
         */
        for (auto &B : F) {
          for (auto &I : B) {
            if (auto call = dyn_cast<CallInst>(&I)){

              /* Analyze a call instruction included in F.
               */
              auto callee = call->getCalledFunction();
              if (  (callee == NULL)              ||
                    (callee->getName() == "rand") ){
                randomInfo[&F].r = invoked;
                randomInfo[&F].level = 0;
              }
            }
          }
        }
      }

      return ;
    }

    void identifyFunctionsIndirectlyAffected (Module &M){
      bool changed;
      do {
        changed = false;

        for (auto &F : M) {
          if (randomInfo[&F].r != invoked){
            continue ;
          }// only find the rand() function or already computed function

          for (auto &U : F.uses()){
            auto user = U.getUser(); // inst uses F
            if (auto callInst = dyn_cast<Instruction>(user)){
              auto caller = callInst->getFunction();
              switch (randomInfo[caller].r){
                case TBC:
                  randomInfo[caller].r = invoked;
                  randomInfo[caller].level = randomInfo[&F].level + 1;
                  changed = true;
                  break ;
  
                case invoked:
                  if (randomInfo[caller].level > (randomInfo[&F].level + 1)){
                    randomInfo[caller].level = randomInfo[&F].level + 1;
                    changed = true;
                  }
                  break ;
              }
            }
          }

        }

      } while (changed);

      return ;
    }

    void identifyFunctionsNotAffected (Module &M){
      for (auto &F : M) {
        if (F.empty()) continue ;
        if (randomInfo[&F].r != TBC) continue ;

        randomInfo[&F].r = notInvoked;
      }

      return ;
    }

    void getAnalysisUsage(AnalysisUsage &AU) const override {
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
