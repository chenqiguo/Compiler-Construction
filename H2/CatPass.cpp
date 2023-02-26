#include "llvm/Pass.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/Transforms/Utils/BasicBlockUtils.h"
#include "llvm/IR/Instructions.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Transforms/IPO/PassManagerBuilder.h"
#include "llvm/ADT/SmallBitVector.h"
#include "llvm/ADT/BitVector.h"
#include "llvm/ADT/SparseBitVector.h"
#include <string.h>

using namespace llvm;
using namespace std;
bool debug = 0;
bool print_GEN_KILL = 1;

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
      // map<string, int> counter_CAT;
      // counter_CAT["CAT_new"] = 0;
      // counter_CAT["CAT_get"] = 0;
      // counter_CAT["CAT_set"] = 0;
      // counter_CAT["CAT_add"] = 0;
      // counter_CAT["CAT_sub"] = 0;
      // for (auto &bb : F) {
      //   for (auto &i : bb) {
      //     if (!isa<CallInst>(i)) {
      //       continue;
      //     }
      //     CallInst *callInst = cast<CallInst>(&i);
      //     Function *callee = callInst->getCalledFunction();
      //     if (counter_CAT.find(callee->getName()) != counter_CAT.end())
      //       counter_CAT[callee->getName()]++;
      //   }
      // }
      // for (auto &pair : counter_CAT) {
      //   if (pair.second)
      //     errs() << "H1: \"" <<  F.getName() << "\": " << pair.first <<": " << to_string(pair.second) <<"\n";
      // }

      /*HW2*/
      // Initialization
      int index;
      vector<Instruction *> insts; // vector of all instructions in this function
      vector<SparseBitVector<>> GENs; // vector of GEN sets for each inst
      vector<SparseBitVector<>> KILLs; // vector of KILL sets for each inst
      for (auto &bb : F) {
        for (auto &i : bb) {
          insts.push_back(&i);
        }
      }

      // Create a SparseBitVector deniting GEN and KILL set for each instruction;
      // Assign a bit to each element in this set: 1 denotes in this set, 0 denotes not in this set
      for (auto &bb : F) {
        for (auto &i : bb) {
          // insts.push_back(&i);
          GENs.push_back(SparseBitVector<>());
          KILLs.push_back(SparseBitVector<>());
          
          // Only care about variables created by CAT_new 2, CAT_add 0, CAT_sub 1 (GEN and KILL)
          if (!isa<CallInst>(i)) {
            continue;
          }

          CallInst *callInst = cast<CallInst>(&i);
          Function *callee = callInst->getCalledFunction();

          if (callee->getName() == "CAT_new") {
            auto it = std::find(insts.begin(), insts.end(), &i);
            index = std::distance(insts.begin(), it);
            GENs.back().set(index);
            if (debug) {
              errs() << "&&&&&&&&&&&&instruction: " << i << "\n";
              errs() << "***********GEN**************\n";
              errs() << "index: " << to_string(index) <<"\n";
              errs() << "gen inst: " << i <<"\n";
              errs() << "gen inst: " << *insts.at(index) <<"\n";
              errs() << "its gen: ";
              for (auto &&g : GENs.back()) {
                errs() << to_string(g) << " " << *insts.at(g) << "\t";
              }
              errs() << "\n";
            }

            for (auto u : i.users()) { // get instructions u using the var defined by i
              if (!isa<CallInst>(u)) {
                continue;
              }
              CallInst *callInst_u = cast<CallInst>(u);
              Function *callee_u = callInst_u->getCalledFunction();
              if (debug) {
                errs() << "users: " << *callInst_u << "\n";
              }
              if ((callee_u->getName() == "CAT_new") || (callee_u->getName() == "CAT_add") || (callee_u->getName() == "CAT_sub")) {
                // Get the instruction callInst_arg0 that defines the arg0 in instruction callInst_u
                CallInst *callInst_arg0 = dyn_cast<CallInst>(callInst_u->getArgOperand(0));
                if (debug) {
                  errs() << "arg0: " << *callInst_arg0 << "\n";
                }
                if (callInst_arg0 == callInst) {
                  Instruction *inst_u = dyn_cast<Instruction>(callInst_u);
                  auto it = std::find(insts.begin(), insts.end(), inst_u);
                  index = std::distance(insts.begin(), it);
                  KILLs.back().set(index);
                  if (debug) {
                    errs() << "***********KILL**************\n";
                    errs() << "index: " << to_string(index) <<"\n";
                    errs() << "kill inst: " << *inst_u <<"\n";
                    errs() << "its kill: ";
                    for (auto &&k : KILLs.back()) {
                      errs() << to_string(k) << " " << *insts.at(k) << "\t";
                    }
                    errs() << "\n";
                  }
                }

              }
              
            }

          }

          else if ((callee->getName() == "CAT_add") || (callee->getName() == "CAT_sub")) {
            auto it = std::find(insts.begin(), insts.end(), &i);
            index = std::distance(insts.begin(), it);
            GENs.back().set(index);
            if (debug) {
              errs() << "&&&&&&&&&&&&instruction: " << i << "\n";
              errs() << "***********GEN**************\n";
              errs() << "index: " << to_string(index) <<"\n";
              // errs() << "a test: what is user of CAT_add?: "; // <- nothing!!!
              // for (auto u : i.users()) {
              //   errs() << *u << "\n";
              // }
              errs() << "its gen: ";
              for (auto &&g : GENs.back()) {
                errs() << to_string(g) << " " << *insts.at(g) << "\t";
              }
              errs() << "\n";
            }

            // Get the instruction callInst_arg0 that defines the arg0 in instruction callInst
            Instruction *callInst_arg0 = dyn_cast<Instruction>(callInst->getArgOperand(0));
            it = std::find(insts.begin(), insts.end(), callInst_arg0);
            index = std::distance(insts.begin(), it);
            KILLs.back().set(index);

            for (auto u : callInst_arg0->users()) { // get instructions u using the var defined by callInst_arg0
              if (!isa<CallInst>(u)) {
                continue;
              }
              CallInst *callInst_u = cast<CallInst>(u);
              Function *callee_u = callInst_u->getCalledFunction();
              if (debug) {
                errs() << "users: " << *callInst_u << "\n";
              }
              if ((callee_u->getName() == "CAT_new") || (callee_u->getName() == "CAT_add") || (callee_u->getName() == "CAT_sub")) {
                Instruction *callInst_u_def = dyn_cast<Instruction>(callInst_u->getArgOperand(0)); // == callInst_arg0?
                if ((callInst_arg0 == callInst_u_def) && (callInst_u != callInst)) { // KILL should NOT include instruction i itself
                  Instruction *inst_u = dyn_cast<Instruction>(callInst_u);
                  auto it = std::find(insts.begin(), insts.end(), inst_u);
                  index = std::distance(insts.begin(), it);
                  KILLs.back().set(index);
                  if (debug) {
                    errs() << "***********KILL**************\n";
                    errs() << "index: " << to_string(index) <<"\n";
                    errs() << "kill inst: " << *inst_u <<"\n";
                    errs() << "its kill: ";
                    for (auto &&k : KILLs.back()) {
                      errs() << to_string(k) << " " << *insts.at(k) << "\t";
                    }
                    errs() << "\n";
                  }

                }
              }
                
            }
            
          }
          
        }
      }

      // output the result
      if (print_GEN_KILL) {
        errs() << "START FUNCTION: " << F.getName() << "\n";
        for (int j = 0; j < insts.size(); j++) {
          Instruction *inst = insts[j];
          errs() << "INSTRUCTION: " << *inst << "\n";
          errs() << "***************** GEN\n{\n";
          for (auto &&gen : GENs[j]) {
            errs() << " " << *insts.at(gen) << "\n";
          }
          errs() << "}\n**************************************\n";
          errs() << "***************** KILL\n{\n";
          for (auto &&kill : KILLs[j]) {
            errs() << " " << *insts.at(kill) << "\n";
          }
          errs() << "}\n**************************************\n\n\n\n";
        }
      }

      // clear
      insts.clear();
      GENs.clear();
      KILLs.clear();

      if (0) {
        for (auto &&gen : GENs) {
          //errs() <<"HERE! "<< gen.count() <<"\n";
          for (auto &&g : gen) {
            errs() << to_string(g) << " ";
            if (g) errs() << "*****HERE!!! ";
          }
          errs() << "\n";
        }
        errs() << "\n";
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
