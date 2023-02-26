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
#include "llvm/IR/Constants.h"
#include <string.h>

using namespace llvm;
using namespace std;
bool debug = 0;
bool print_GEN_KILL = 0;
bool print_IN_OUT = 0;

namespace {
  struct CAT : public FunctionPass {
    static char ID; 

    CAT() : FunctionPass(ID) {}

    // const static uint32_t CAT_new_ID = 0,
    //                       CAT_get_ID = 1,
    //                       CAT_set_ID = 2,
    //                       CAT_add_ID = 3,
    //                       CAT_sub_ID = 4;
    // const vector<string> CAT_func_names = {"CAT_new", "CAT_get", "CAT_set", "CAT_add", "CAT_sub"};
    // const vector<uint32_t> CAT_func_IDs = {CAT_new_ID, CAT_get_ID, CAT_set_ID, CAT_add_ID, CAT_sub_ID};
    // map<string, uint32_t> CAT_func_map = {
    //   {"CAT_new", CAT_new_ID},
    //   {"CAT_get", CAT_get_ID},
    //   {"CAT_set", CAT_set_ID},
    //   {"CAT_add", CAT_add_ID},
    //   {"CAT_sub", CAT_sub_ID}
    // };

    // This function is invoked once at the initialization phase of the compiler
    // The LLVM IR of functions isn't ready at this point
    bool doInitialization (Module &M) override {
      //errs() << "Hello LLVM World at \"doInitialization\"\n" ;
      return false;
    }

    void getGenKill(Instruction &i, vector<Instruction *> &insts, vector<SparseBitVector<>> &GENs, vector<SparseBitVector<>> &KILLs) {
      /*HW2*/
      // Create a SparseBitVector deniting GEN and KILL set for each instruction;
      // Assign a bit to each element in this set: 1 denotes in this set, 0 denotes not in this set
      int index;
      GENs.push_back(SparseBitVector<>());
      KILLs.push_back(SparseBitVector<>());
      
      // Only care about variables created by CAT_new 2, CAT_add 0, CAT_sub 1 (GEN and KILL)
      if (!isa<CallInst>(i)) {
        return;
      }

      CallInst *callInst = cast<CallInst>(&i);
      Function *callee = callInst->getCalledFunction();

      if (callee->getName() == "CAT_new") {
        auto it = std::find(insts.begin(), insts.end(), &i);
        index = std::distance(insts.begin(), it);
        GENs.back().set(index);

        for (auto u : i.users()) { // get instructions u using the var defined by i
          if (!isa<CallInst>(u)) {
            continue;
          }
          CallInst *callInst_u = cast<CallInst>(u);
          Function *callee_u = callInst_u->getCalledFunction();
          if ((callee_u->getName() == "CAT_new") || (callee_u->getName() == "CAT_set") || (callee_u->getName() == "CAT_add") || (callee_u->getName() == "CAT_sub")) {
            // Get the instruction callInst_arg0 that defines the arg0 in instruction callInst_u
            CallInst *callInst_arg0 = dyn_cast<CallInst>(callInst_u->getArgOperand(0));
            if (callInst_arg0 == callInst) {
              Instruction *inst_u = dyn_cast<Instruction>(callInst_u);
              auto it = std::find(insts.begin(), insts.end(), inst_u);
              index = std::distance(insts.begin(), it);
              KILLs.back().set(index);
            }
          }
        }
      }

      else if ((callee->getName() == "CAT_set") || (callee->getName() == "CAT_add") || (callee->getName() == "CAT_sub")) {
        auto it = std::find(insts.begin(), insts.end(), &i);
        index = std::distance(insts.begin(), it);
        GENs.back().set(index);

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
          if ((callee_u->getName() == "CAT_new") || (callee_u->getName() == "CAT_set") || (callee_u->getName() == "CAT_add") || (callee_u->getName() == "CAT_sub")) {
            Instruction *callInst_u_def = dyn_cast<Instruction>(callInst_u->getArgOperand(0)); // == callInst_arg0?
            if ((callInst_arg0 == callInst_u_def) && (callInst_u != callInst)) { // KILL should NOT include instruction i itself
              Instruction *inst_u = dyn_cast<Instruction>(callInst_u);
              auto it = std::find(insts.begin(), insts.end(), inst_u);
              index = std::distance(insts.begin(), it);
              KILLs.back().set(index);
            }
          } 
        }
      }
    }

    void getInOut(Function &F, vector<Instruction *> &insts, vector<SparseBitVector<>> &GENs, vector<SparseBitVector<>> &KILLs, vector<SparseBitVector<>> &INs, vector<SparseBitVector<>> &OUTs, vector<SparseBitVector<>> &oldOUTs) {
      /*HW3*/
      // initialize with empty sets
      for (auto &bb : F) {
        for (auto &i : bb) {
          OUTs.push_back(SparseBitVector<>());
          INs.push_back(SparseBitVector<>());
          oldOUTs.push_back(SparseBitVector<>());
        }
      }

      bool change_flag = 1; // denotes changes to any OUT set occur
      int instIndex; // index of instructions within a function
      int predIndex; // index of predecessor
      bool isFirst; // is it the first instruction within a bb

      while(change_flag) {
        change_flag = 0;
        instIndex = 0;
        for (auto &bb : F) {
          isFirst = 1;
          for (auto &i : bb) {
            if (instIndex){ // for each instruction other than the ENTRY (first instruction in each function)
              // INs
              // for inst whose predecessor is in the same bb
              if (!isFirst) {
                predIndex = instIndex-1;
                INs[instIndex] = OUTs[predIndex];
                // for (auto &&out : OUTs[predIndex]){
                //   INs[instIndex].set(out);
                // }
              }
              // for inst that is the first one within a bb,
              // IN is union of terminators for all predecessor bb
              else {
                for (auto *predBB : predecessors(&bb)) {
                  //errs() << "HERE1! predBB test!\n";
                  if (predBB) {
                    Instruction *predTe = predBB->getTerminator();
                    if (predTe) {
                      auto it = std::find(insts.begin(), insts.end(), predTe);
                      predIndex = std::distance(insts.begin(), it);
                      //INs[instIndex] = INs[instIndex] + OUTs[predIndex]; //<- error!!!
                      for (auto &&out : OUTs[predIndex]){
                        INs[instIndex].set(out);
                      }
                    }
                  }
                }
              }
            }

            // OUTs
            //SparseBitVector<> tempOUTs;
            // for (auto &&g : GENs[instIndex]){
            //   tempOUTs.set(g);
            // }
            SparseBitVector<> difference; // IN[i] - KILL[i]
            difference = INs[instIndex] - KILLs[instIndex];
            // for (auto &&diff : difference){ 
            //   tempOUTs.set(diff); 
            // }
            //tempOUTs = GENs[instIndex] | difference;
            OUTs[instIndex] = GENs[instIndex] | difference;

            if (OUTs[instIndex] != oldOUTs[instIndex]) { //if (OUTs[instIndex] != tempOUTs)
              change_flag = 1;
              //OUTs[instIndex] = tempOUTs;
              oldOUTs[instIndex] = OUTs[instIndex];
            }
            
            instIndex++;
            isFirst = 0;
          }
        }
      }
    }

    void constantPropagation (vector<Instruction *> &insts, vector<SparseBitVector<>> &INs) {
      /*HW4*/
      for (int idx = 0; idx < insts.size(); idx++) { // loop through every instruction in a function
        if (!isa<CallInst>(insts[idx])) continue; // only consider CAT var

        Instruction *i = insts[idx];
        CallInst *callInst = cast<CallInst>(i);
        Function *callee = callInst->getCalledFunction();
        SparseBitVector<> ins = INs[idx]; // ins is the IN set for this instruction

        bool isSet = 0; // flag for CAT_set (for constant propagation)
        //bool done = 0; // ???bug???
        Instruction *reachDef = NULL; // for constant propagation here
        ConstantInt *foldConst = NULL; // probable result from contant folding
        ConstantInt *resultConstant = NULL; // final result (if exists)
        int64_t reachResult; // for constant propagation
        int64_t foldResult; // probable result from contant folding
        set<int64_t> consts; // set of probable constant value

        // only care about when we need to get the CAT value
        // case 1) get the CAT value in function CAT_get, with arg0
        if (callee->getName() == "CAT_get") { // constant propagation for CAT_new and CAT_set
          // Get the instruction inst_arg0 that defines the arg0 in instruction callInst
          Instruction *inst_arg0 = dyn_cast<Instruction>(callInst->getArgOperand(0));
          for (auto &&in : ins) {
            Instruction *inInst = insts.at(in);
            if (CallInst *in_callInst = dyn_cast<CallInst>(inInst)) {
              Function *in_callee = in_callInst->getCalledFunction();
              if ((in_callee->getName() == "CAT_new") && (inInst == inst_arg0)) {
                // cannot done since there may be a CAT_set or CAT_add or CAT_sub after this CAT_new
                // (note that a CAT var will always be initialized (CAT_new) first before set or add or sub)
                reachDef = inInst;
                isSet = 0;
                foldConst = NULL;
              }
              else if (in_callee->getName() == "CAT_set") {
                // check whether the CATData in this CAT_set func is the same as inst_arg0 or not
                Instruction *inInst_arg0 = dyn_cast<Instruction>(in_callInst->getArgOperand(0));
                if (inInst_arg0 == inst_arg0) {
                  reachDef = inInst;
                  isSet = 1;
                  foldConst = NULL;
                }
              }
              else if ((in_callee->getName() == "CAT_add") || (in_callee->getName() == "CAT_sub")) {
                Instruction *inInst_arg0 = dyn_cast<Instruction>(in_callInst->getArgOperand(0));
                if (inInst_arg0 == inst_arg0) {
                  // if the lastest def inst is a CAT_add or CAT_sub, cannot replace this with constant
                  reachDef = NULL; // for probable constant folding later
                  isSet = 0; 
                  foldConst = constantFolding(inInst, insts, INs);
                }
              }
            }   
            if (reachDef) {
              Value *val;
              if (CallInst *reachDef_callInst = dyn_cast<CallInst>(reachDef)) {
                val = reachDef_callInst->getArgOperand(isSet);
                if (ConstantInt *constant = dyn_cast<ConstantInt>(val)) {
                  resultConstant = constant;
                  reachResult = constant->getSExtValue();
                  consts.insert(reachResult);
                }
              }
            }
            else if (foldConst) { // CAT_add, CAT_sub
              resultConstant = foldConst;
              foldResult = foldConst->getSExtValue();
              consts.insert(foldResult);
            }
            // else {
            //   done = 1; // cannot get reach def, nor fold const -> cannot replace with const
            // }
          }
          // only replace when the definition of v that reach n are all of the same const! (see slides DFAs)
          if ((consts.size() == 1) /*&& !done*/) {
            // get the element in set
            auto firstEle = consts.begin();
            int64_t result = *firstEle;
            resultConstant = ConstantInt::get(resultConstant->getType(), result);
            BasicBlock::iterator ii(i);
            BasicBlock *bb = i->getParent();
            ReplaceInstWithValue(bb->getInstList(), ii, resultConstant);
          }
        }
      }
    }

    ConstantInt * constantFolding (Instruction *i, vector<Instruction *> &insts, vector<SparseBitVector<>> &INs) {
      /*HW4*/
      CallInst *callInst = cast<CallInst>(i); // i is the CAT_add or CAT_sub inst in the IN set of original CAT_get
      Function *callee = callInst->getCalledFunction();
      auto it = std::find(insts.begin(), insts.end(), i);
      int idx = std::distance(insts.begin(), it);
      SparseBitVector<> ins = INs[idx]; // ins is the IN set for this CAT_add or CAT_sub inst

      Instruction *reachDef1 = NULL; // for constant folding (v1)
      Instruction *reachDef2 = NULL; // for constant folding (v2)
      ConstantInt *foldConst1 = NULL; // probable result from contant folding, for v1
      ConstantInt *foldConst2 = NULL; // probable result from contant folding, for v2
      bool isSet1 = 0; // flag for CAT_set (for constant folding)
      bool isSet2 = 0; // flag for CAT_set (for constant folding)
      bool v1_exist = 0;
      bool v2_exist = 0;
      int64_t result1, result2, result;
      set<int64_t> consts; // set of probable constant value
      ConstantInt *resultConstant = NULL; // final result (if exists)
      ConstantInt *typeConstant = NULL; // a tmp constant to get the result, used to convert int to const
      
      // case 2) get the CAT value in function CAT_add or CAT_sub, with arg0
      // note: only can fold when both v1 and v2 are constants!
      if ((callee->getName() == "CAT_add") || (callee->getName() == "CAT_sub")) { // constant folding for CAT_add and CAT_sub
        // Get the instruction inst_argn that defines the argn in instruction callInst
        Instruction *inst_arg0 = dyn_cast<Instruction>(callInst->getArgOperand(0)); //result
        Instruction *inst_arg1 = dyn_cast<Instruction>(callInst->getArgOperand(1)); //v1
        Instruction *inst_arg2 = dyn_cast<Instruction>(callInst->getArgOperand(2)); //v2
        for (auto &&in : ins) {
          v2_exist = 0;
          v2_exist = 0;
          Instruction *inInst = insts.at(in);
          if (CallInst *in_callInst = dyn_cast<CallInst>(inInst)) {
            Function *in_callee = in_callInst->getCalledFunction();
            if (in_callee->getName() == "CAT_new") {
              if (inInst == inst_arg1) { // may replace v1 with constant
                reachDef1 = inInst;
                isSet1 = 0;
                foldConst1 = NULL;
              }
              else if (inInst == inst_arg2) { // may replace v2 with constant
                reachDef2 = inInst;
                isSet2 = 0;
                foldConst2 = NULL;
              }
            }
            else if (in_callee->getName() == "CAT_set") {
              // check whether the CATData in this CAT_set func is the same as inst_arg1(2) or not
              Instruction *inInst_arg0 = dyn_cast<Instruction>(in_callInst->getArgOperand(0));
              if (inInst_arg0 == inst_arg1) {
                // may replace v1 with constant
                reachDef1 = inInst;
                isSet1 = 1;
                foldConst1 = NULL;
              }
              else if (inInst_arg0 == inst_arg2) {
                // may replace v2 with constant
                reachDef2 = inInst;
                isSet2 = 1;
                foldConst2 = NULL;
              }
            }
            else if ((in_callee->getName() == "CAT_add") || (in_callee->getName() == "CAT_sub")) {
              Instruction *inInst_arg0 = dyn_cast<Instruction>(in_callInst->getArgOperand(0));
              if (inInst_arg0 == inst_arg1) {
                // if the def inst is a CAT_add or CAT_sub, cannot replace this with constant directly
                reachDef1 = NULL; // for probable constant folding later
                isSet1 = 0;
                foldConst1 = constantFolding(inInst, insts, INs); // instead, call constant folding
              }
              else if (inInst_arg0 == inst_arg2) {
                reachDef2 = NULL; // for probable constant folding later
                isSet2 = 0;
                foldConst2 = constantFolding(inInst, insts, INs); // instead, call constant folding
              }
            }
          }
          if (reachDef1) {
            Value *val1; //v1
            if (CallInst *reachDef1_callInst = dyn_cast<CallInst>(reachDef1)) {
              val1 = reachDef1_callInst->getArgOperand(isSet1);
              if (ConstantInt *constant1 = dyn_cast<ConstantInt>(val1)) {
                typeConstant = constant1;
                result1 = constant1->getSExtValue();
                v1_exist = 1;
              }
            }
          }
          else if (foldConst1) {
            typeConstant = foldConst1;
            result1 = foldConst1->getSExtValue();
            v1_exist = 1;
          }

          if (reachDef2) {
            Value *val2; //v2
            if (CallInst *reachDef2_callInst = dyn_cast<CallInst>(reachDef2)) {
              val2 = reachDef2_callInst->getArgOperand(isSet2);
              if (ConstantInt *constant2 = dyn_cast<ConstantInt>(val2)) {
                typeConstant = constant2;
                result2 = constant2->getSExtValue();
                v2_exist = 1;
              }
            }
          }
          else if (foldConst2) {
            typeConstant = foldConst2;
            result2 = foldConst2->getSExtValue();
            v2_exist = 1;
          }

          if (v1_exist && v2_exist) { // only can fold when both v1 and v2 are constants!
            result = result1 + result2;
            consts.insert(result);
            resultConstant = ConstantInt::get(typeConstant->getType(), result);
          }
        }

        if (consts.size() == 1) {
          // get the element in set
          auto firstEle = consts.begin();
          int64_t finalResult = *firstEle;
          resultConstant = ConstantInt::get(typeConstant->getType(), finalResult);
        }
      }
      return resultConstant;
    }

    // This function is invoked once per function compiled
    // The LLVM IR of the input functions is ready and it can be analyzed and/or transformed
    bool runOnFunction (Function &F) override {
      //errs() << "Hello LLVM World at \"runOnFunction\"\n" ;

      // declaration for liveness analysis
      vector<Instruction *> insts; // vector of all instructions in this function
      vector<SparseBitVector<>> GENs; // vector of GEN sets for each inst
      vector<SparseBitVector<>> KILLs; // vector of KILL sets for each inst
      vector<SparseBitVector<>> INs; // vector of IN sets for each inst
      vector<SparseBitVector<>> OUTs; // vector of OUT sets for each inst
      vector<SparseBitVector<>> oldOUTs; // vector of OUT sets for each inst in the last iteration
      
      for (auto &bb : F) {
        for (auto &i : bb) {
          insts.push_back(&i);
        }
      }
      for (auto &bb : F) {
        for (auto &i : bb) {
          getGenKill(i, insts, GENs, KILLs);
        }
      }

      /*HW3*/
      getInOut(F, insts, GENs, KILLs, INs, OUTs, oldOUTs);

      /*HW4*/
      // constant propagation for CAT_new and CAT_set;
      // includes potential constant folding for CAT_add and CAT_sub
      constantPropagation(insts, INs);

      // output the result for HW2
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

      // output the result for HW3
      if (print_IN_OUT) {
        errs() << "START FUNCTION: " << F.getName() << "\n";
        for (int j = 0; j < insts.size(); j++) {
          Instruction *inst = insts[j];
          errs() << "INSTRUCTION: " << *inst << "\n";
          errs() << "***************** IN\n{\n";
          for (auto &&in : INs[j]) {
            errs() << " " << *insts.at(in) << "\n";
          }
          errs() << "}\n**************************************\n";
          errs() << "***************** OUT\n{\n";
          for (auto &&out : OUTs[j]) {
            errs() << " " << *insts.at(out) << "\n";
          }
          errs() << "}\n**************************************\n\n\n\n";
        }
      }
      
      // clear
      insts.clear();
      GENs.clear();
      KILLs.clear();
      INs.clear();
      OUTs.clear();
      oldOUTs.clear();
      
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
