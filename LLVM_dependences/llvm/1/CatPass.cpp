#include "llvm/Pass.h"
#include "llvm/IR/Function.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Transforms/IPO/PassManagerBuilder.h"
#include "llvm/Analysis/AliasSetTracker.h"
#include "llvm/Analysis/AliasAnalysis.h"

#include <vector>

using namespace std;
using namespace llvm;

namespace {
  struct CAT : public ModulePass {
    static char ID; 

    CAT() : ModulePass(ID) {}

    bool doInitialization (Module &M) override {
      return false;
    }

    bool runOnModule (Module &M) override {
      for (auto &F : M){
        if (F.empty()){
          continue ;
        }
        errs() << "Function \"" << F.getName() << "\"\n";

        AliasAnalysis &aliasAnalysis = getAnalysis< AAResultsWrapperPass >(F).getAAResults();
        checkFunction(M, F, aliasAnalysis);
      }

      return false;
    }

    void checkFunction (Module &M, Function &F, AliasAnalysis &aliasAnalysis){
      vector< Instruction* > pointers;
      vector< Instruction* > memInsts;
      for (auto &B : F){
        for (auto &I : B){
          if (  I.getType()->isPointerTy()    &&
                isa<CallInst>(&I)             ){
            pointers.push_back(&I);
            continue ;
          }

          if (  isa<LoadInst>(&I)   ||
                isa<StoreInst>(&I)  ){
            memInsts.push_back(&I);
            continue ;
          }
        }
      }

      errs() << " ### Pointers\n";
      for (auto &pointer : pointers) {
        auto sizePointer = getPointedElementTypeSize(M, pointer);
        errs() << "   Pointer: \"" << *pointer << "\"\n" ;
        errs() << "     Size: " << sizePointer << " Bytes\n";

        for (auto &pointer2 : pointers) {
          if (pointer == pointer2) continue ;

          auto sizePointer2 = getPointedElementTypeSize(M, pointer2);
          errs() << "     Pointer2: \"" << *pointer2 << "\"\n" ;
          errs() << "       Size: " << sizePointer2 << " Bytes\n";

          switch (aliasAnalysis.alias(pointer, sizePointer, pointer2, sizePointer2)){
            case NoAlias:
              errs() << "     No alias\n" ;
              break ;

            case MayAlias:
              errs() << "     May alias\n" ;
              break ;

            case PartialAlias:
              errs() << "     Partial alias\n" ;
              break ;

            case MustAlias:
              errs() << "     Must alias\n" ;
              break ;

            default:
              abort();
          }
        }
      }

      errs() << " ### Memory accesses\n";
      for (auto &memInst : memInsts) {
        errs() << "   Mem inst: \"" << *memInst << "\"\n" ;

        for (auto &memInst2 : memInsts) {
          if (memInst == memInst2) continue ;
          errs() << "     Mem inst2: \"" << *memInst2 << "\"\n" ;

          switch (aliasAnalysis.alias(MemoryLocation::get(memInst), MemoryLocation::get(memInst2))){
            case NoAlias:
              errs() << "     No alias\n" ;
              break ;

            case MayAlias:
              errs() << "     May alias\n" ;
              break ;

            case PartialAlias:
              errs() << "     Partial alias\n" ;
              break ;

            case MustAlias:
              errs() << "     Must alias\n" ;
              break ;

            default:
              abort();
          }
        }
      }

      return ;
    }

    uint64_t getPointedElementTypeSize (Module &M, Instruction *pointer){
        uint64_t size = 0;

        if (auto pointerType = dyn_cast<PointerType>(pointer->getType())){
          auto elementPointedType = pointerType->getElementType();
          if (elementPointedType->isSized()){
            size = M.getDataLayout().getTypeStoreSize(elementPointedType);
          }
        }

        return size;
    }

    void getAnalysisUsage(AnalysisUsage &AU) const override {
      AU.addRequired< AAResultsWrapperPass >();
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
