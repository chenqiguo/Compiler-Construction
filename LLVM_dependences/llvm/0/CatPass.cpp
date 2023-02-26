#include "llvm/Pass.h"
#include "llvm/IR/Function.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Transforms/IPO/PassManagerBuilder.h"
#include "llvm/Analysis/AliasSetTracker.h"
#include "llvm/Analysis/AliasAnalysis.h"

#include <vector>
//llvm0: intra, alias analysis
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

      AliasAnalysis &aliasAnalysis = getAnalysis< AAResultsWrapperPass >().getAAResults();

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
        auto sizePointer = getPointedElementTypeSize(pointer);
        errs() << "   Pointer: \"" << *pointer << "\"\n" ;
        errs() << "     Size: " << sizePointer << " Bytes\n";

        for (auto &pointer2 : pointers) {
          if (pointer == pointer2) continue ;

          auto sizePointer2 = getPointedElementTypeSize(pointer2);
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

      errs() << " ### Mod/ref\n";
      for (auto &memInst : memInsts) {
        errs() << "   Mem inst: \"" << *memInst << "\"\n" ;

        for (auto &pointer : pointers) {
          auto sizePointer = getPointedElementTypeSize(pointer);
          errs() << "   Pointer: \"" << *pointer << "\"\n" ;
          errs() << "     Size: " << sizePointer << " Bytes\n";
        
          switch (aliasAnalysis.getModRefInfo(memInst, pointer, sizePointer)){
            case ModRefInfo::NoModRef:
              errs() << "     NoModRef\n";
              break ;
            case ModRefInfo::Mod:
              errs() << "     Mod\n";
              break ;
            case ModRefInfo::Ref:
              errs() << "     Ref\n";
              break ;
            case ModRefInfo::ModRef:
              errs() << "     ModRef\n";
              break ;
            default:
              abort();
          }

        }
      }

      return false;
    }

    uint64_t getPointedElementTypeSize (Instruction *pointer){
        uint64_t size = 0;

        if (auto pointerType = dyn_cast<PointerType>(pointer->getType())){
          auto elementPointedType = pointerType->getElementType();
          if (elementPointedType->isSized()){
            size = currM->getDataLayout().getTypeStoreSize(elementPointedType);
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
