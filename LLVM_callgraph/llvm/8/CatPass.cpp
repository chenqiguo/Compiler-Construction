#include "llvm/ADT/Statistic.h"
#include "llvm/Pass.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Instructions.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Transforms/IPO/PassManagerBuilder.h"
#include "llvm/Transforms/Utils/BasicBlockUtils.h"
#include "llvm/Transforms/Utils/Cloning.h"
#include <iostream>
#include <map>
#include <vector>
// 8: important!!! function clone
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
      analyzeFunctions(M);

      bool modified = transformFunctions(M);

      analyzeFunctions(M);
      printStatus(M);

      return modified;
    }

    bool transformFunctions (Module &M){
      // transform functions with level>=3
      // to be not affected by rand()
      bool modified = false;

      for (auto &F : M) {
        if (randomInfo[&F].r != invoked){
          continue ;
        }
        if (randomInfo[&F].level <= 2){
          continue ;
        }

        modified |= transformFunction(M, F);
      }

      return modified;
    }

    bool transformFunction (Module &M, Function &F){ // carefully read this and try it!!!!
      std::vector<Instruction *> toDelete;
      errs() << "START " << F.getName() << "\n";

      /* Reduce the impact to F.
       */
      bool modified = false;
      for (auto &B : F) {
        for (auto &I : B) {
          if (auto call = dyn_cast<CallInst>(&I)){

            /* Fetch the callee.
             */
            auto callee = call->getCalledFunction();
            if (callee == NULL){
              continue ;
            }

            /* Check the callee.
             */
            if (callee->getName() == "rand"){ // for inst directly invoked rand() func
              errs() << "Changing invocations to \"rand\" from " << F.getName() << "\n";
              Value *constValue = ConstantInt::get(call->getType(), 1, true);
              call->replaceAllUsesWith(constValue); //Go through the uses list for this definition and make each use point to "V" instead of "this"
              toDelete.push_back(call);
              modified = true;
              continue ;
            }
            if (randomInfo[callee].r != invoked) continue ;
            if (randomInfo[callee].level >= randomInfo[&F].level) continue ;

            /* The callee needs to be cloned.
             */
            errs() << "Cloning " << callee->getName() << " from " << F.getName() << "\n";
            ValueToValueMapTy VMap;
            auto clonedCallee = CloneFunction(callee, VMap);
            call->replaceUsesOfWith(callee, clonedCallee);
            randomInfo[clonedCallee] = randomInfo[callee];

            /* Recursive check the callees of the cloned function.
             */
            modified |= transformFunction(M, *clonedCallee);
          }
        }
      }

      for (auto i : toDelete){
        i->eraseFromParent();

      }

      errs() << "END " << F.getName() << "\n";
      return modified;
    }

    void analyzeFunctions (Module &M) {

      errs() << " Identify functions affected directly\n" ;
      tagFunctionsDirectlyAffected(M);

      errs() << " Identify functions affected indirectly\n" ;
      identifyFunctionsIndirectlyAffected(M);

      errs() << " Identify functions not affected\n" ;
      identifyFunctionsNotAffected(M);
      printStatus(M);

      return ;
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
          }

          for (auto &U : F.uses()){
            auto user = U.getUser();
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
