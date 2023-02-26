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

    // This function is invoked once at the initialization phase of the compiler
    // The LLVM IR of functions isn't ready at this point
    bool doInitialization (Module &M) override {
      //errs() << "Hello LLVM World at \"doInitialization\"\n" ;
      return false;
    }

    int getCatType(Function *callee) const{
      string funcName = callee->getName();
      if (funcName == "CAT_new") 
        return 0;
      if (funcName == "CAT_set") 
        return 1;
      if (funcName == "CAT_add") 
        return 2;
      if (funcName == "CAT_sub") 
        return 3;
      if (funcName == "CAT_get") 
        return 4;
      return -1;
    }

    void getGenKill(Instruction &i, vector<Instruction *> &insts, vector<SparseBitVector<>> &GENs, vector<SparseBitVector<>> &KILLs) {
      /*HW2*/
      // Create a SparseBitVector deniting GEN and KILL set for each instruction;
      // Assign a bit to each element in this set: 1 denotes in this set, 0 denotes not in this set
      int index;
      GENs.push_back(SparseBitVector<>());
      KILLs.push_back(SparseBitVector<>());
      
      // Only care about variables created by CAT_new 2, CAT_add 0, CAT_sub 1 (GEN and KILL)
      if (!isa<CallInst>(&i)) {
        return;
      }

      CallInst *callInst = cast<CallInst>(&i);
      Function *callee = callInst->getCalledFunction();

      if (getCatType(callee) == 0) {
        auto it = std::find(insts.begin(), insts.end(), &i);
        index = std::distance(insts.begin(), it);
        GENs.back().set(index);

        for (auto u : i.users()) { // get instructions u using the var defined by i
          if (!isa<CallInst>(u)) {
            continue;
          }
          CallInst *callInst_u = cast<CallInst>(u);
          Function *callee_u = callInst_u->getCalledFunction();
          if ((getCatType(callee_u) == 0) || (getCatType(callee_u) == 1) || (getCatType(callee_u) == 2) || (getCatType(callee_u) == 3)) {
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

      else if ((getCatType(callee) == 1) || (getCatType(callee) == 2) || (getCatType(callee) == 3)) {
        auto it = std::find(insts.begin(), insts.end(), &i);
        index = std::distance(insts.begin(), it);
        GENs.back().set(index);

        // Get the instruction callInst_arg0 that defines the arg0 in instruction callInst
        if (Instruction *callInst_arg0 = dyn_cast<Instruction>(callInst->getArgOperand(0))) {
          it = std::find(insts.begin(), insts.end(), callInst_arg0);
          index = std::distance(insts.begin(), it);
          KILLs.back().set(index);

          for (auto u : callInst_arg0->users()) { // get instructions u using the var defined by callInst_arg0
            if (!isa<CallInst>(u)) {
              continue;
            }
            CallInst *callInst_u = cast<CallInst>(u);
            Function *callee_u = callInst_u->getCalledFunction();
            if ((getCatType(callee_u) == 0) || (getCatType(callee_u) == 1) || (getCatType(callee_u) == 2) || (getCatType(callee_u) == 3)) {
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

    bool constantPropagation (vector<Instruction *> &insts, vector<SparseBitVector<>> &INs, set<Instruction*> &escapeSet, int idx, Value *arg0Value) {//auto???
      /*HW4*/ /*modified to HW5 version*/
      //for (int idx = 0; idx < insts.size(); idx++) { // loop through every instruction in a function
        //if (!isa<CallInst>(insts[idx])) continue; // only consider CAT var
      Instruction *i = insts[idx];
      CallInst *callInst = cast<CallInst>(i);
      Function *callee = callInst->getCalledFunction();
      SparseBitVector<> ins = INs[idx]; // ins is the IN set for this instruction

      bool modified = 0; // return val
      bool isSet = 0; // flag for CAT_set (for constant propagation)
      Instruction *reachDef = NULL; // for constant propagation here
      ConstantInt *foldConst = NULL; // probable result from contant folding
      ConstantInt *resultConstant = NULL; // final result (if exists)
      int64_t reachResult; // for constant propagation
      int64_t foldResult; // probable result from contant folding
      set<int64_t> consts; // set of probable constant value

      if (Instruction *inst_arg0 = dyn_cast<Instruction>(arg0Value)) {
        // if already escaped (from func arg or from memory), ignore (give up optimizing)
        // load inst: e.g., d1 = *pointer; (the mem var already in escapeSet)
        if((escapeSet.find(inst_arg0) == escapeSet.end()) /*&& !isa<LoadInst>(inst_arg0)*/) {
          SparseBitVector<> ins = INs[idx]; // the IN set for CAT_get instruction
          for (auto &&in : ins) {
            Instruction *inInst = insts.at(in);
            if (escapeSet.find(inInst) == escapeSet.end()) { // if this in instruction is not escape
              if (auto in_callInst = dyn_cast<CallInst>(inInst)) {// only care about CAT var
                Function *in_callee = in_callInst->getCalledFunction();
                if ((getCatType(in_callee) == 0) && (inInst == inst_arg0)) {
                  reachDef = inInst;
                  isSet = 0;
                  foldConst = NULL;
                }
                else if (getCatType(in_callee) == 1) {
                  // check whether the CATData in this CAT_set func is the same as inst_arg0 or not
                  Instruction *inInst_arg0 = dyn_cast<Instruction>(in_callInst->getArgOperand(0));
                  if (inInst_arg0 == inst_arg0) {
                    reachDef = inInst;
                    isSet = 1;
                    foldConst = NULL;
                  }
                }
                else if ((getCatType(in_callee) == 2) || (getCatType(in_callee) == 3)) {
                  Instruction *inInst_arg0 = dyn_cast<Instruction>(in_callInst->getArgOperand(0));
                  if (inInst_arg0 == inst_arg0) {
                    // if the lastest def inst is a CAT_add or CAT_sub, cannot replace this with constant
                    reachDef = NULL; // for probable constant folding later
                    isSet = 0; 
                    foldConst = constantFolding(inInst, insts, INs, escapeSet);
                  }
                }
              }
              if (reachDef) { // CAT_new, CAT_set
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
            }
          }
          if (consts.size() == 1) {
            // get the element in set
            auto firstEle = consts.begin();
            int64_t result = *firstEle;
            resultConstant = ConstantInt::get(resultConstant->getType(), result);
            BasicBlock::iterator ii(i);
            BasicBlock *bb = i->getParent();
            ReplaceInstWithValue(bb->getInstList(), ii, resultConstant);
            modified =true;
          }
        }
      }
      return modified;
    }

    ConstantInt * constantFolding (Instruction *i, vector<Instruction *> &insts, vector<SparseBitVector<>> &INs, set<Instruction*> &escapeSet) {
      /*HW4*/
      CallInst *callInst = cast<CallInst>(i); // i is the CAT_add or CAT_sub inst in the IN set of original CAT_get (not escaped)
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
      if ((getCatType(callee) == 2) || (getCatType(callee) == 3)) { // constant folding for CAT_add and CAT_sub
        // Get the instruction inst_argn that defines the argn in instruction callInst
        Instruction *inst_arg0 = dyn_cast<Instruction>(callInst->getArgOperand(0)); //result
        Instruction *inst_arg1 = dyn_cast<Instruction>(callInst->getArgOperand(1)); //v1
        Instruction *inst_arg2 = dyn_cast<Instruction>(callInst->getArgOperand(2)); //v2

        // if already escaped (from func arg or from memory), ignore (give up optimizing)
        // load inst: e.g., d1 = *pointer; (the mem var already in escapeSet)
        if((escapeSet.find(inst_arg0) == escapeSet.end()) && (escapeSet.find(inst_arg1) == escapeSet.end()) && (escapeSet.find(inst_arg2) == escapeSet.end())) {


          for (auto &&in : ins) {
            v2_exist = 0;
            v2_exist = 0;
            Instruction *inInst = insts.at(in);
            if (CallInst *in_callInst = dyn_cast<CallInst>(inInst)) {
              Function *in_callee = in_callInst->getCalledFunction();
              if (getCatType(in_callee) == 0) {
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
              else if (getCatType(in_callee) == 1) {
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
              else if ((getCatType(in_callee) == 2) || (getCatType(in_callee) == 3)) {
                Instruction *inInst_arg0 = dyn_cast<Instruction>(in_callInst->getArgOperand(0));
                if (inInst_arg0 == inst_arg1) {
                  // if the def inst is a CAT_add or CAT_sub, cannot replace this with constant directly
                  reachDef1 = NULL; // for probable constant folding later
                  isSet1 = 0;
                  foldConst1 = constantFolding(inInst, insts, INs, escapeSet); // instead, call constant folding
                }
                else if (inInst_arg0 == inst_arg2) {
                  reachDef2 = NULL; // for probable constant folding later
                  isSet2 = 0;
                  foldConst2 = constantFolding(inInst, insts, INs, escapeSet); // instead, call constant folding
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

        
      }
      return resultConstant;
    }

    bool dealEscape(vector<Instruction *> &insts, vector<SparseBitVector<>> &INs) {
      /*HW5*/
      bool modified = false; // the return value
      // escape: 1) CAT vars as parameters of a function; 2) CAT vars stored in memory
      // we just give up on optimizing the escaped vars in HW5
      set<Instruction*> escapeSet; // set of escaped CAT vars

      // loop through each instruction to fill in the escapeSet and escapeSetSpecific
      for (int idx = 0; idx < insts.size(); idx++) {
        Instruction *i = insts[idx];
        // 1) for every CAT var as a parameter of a function except CAT_func, recognize as escaping
        if (auto funcInst= dyn_cast<CallInst>(i)) {//CallInst*
          if (funcInst->getNumArgOperands() > 0) {
            Function *func = funcInst->getCalledFunction();
            if (getCatType(func) == -1) { // for functions with parameter(s) (except CAT_new, CAT_set, CAT_get, CAT_add, CAT_sub)
              for (int arg_idx = 0; arg_idx < funcInst->getNumArgOperands(); arg_idx++) {
                if (auto escapeInst = dyn_cast<CallInst>(funcInst->getArgOperand(arg_idx))) {
                  Function *escape_callee = escapeInst->getCalledFunction();
                  if (getCatType(escape_callee) == 0) { // only cares about CAT_new because a CAT var can only be initialized by CAT_new
                    escapeSet.insert(escapeInst);
                  }
                }
              }
            }
          }
        }
        // 2) for every CAT var get pointed (i.e., CAT vars that are stored in memory), recognize as escaping
        else if (isa<StoreInst>(i)) { //e.g., case 1: (*pointer) = CAT_new(50); | case 2: d = CAT_new(50); (*pointer) = d;
          auto storeInst = cast<StoreInst>(i);
          // fetch the var (cast to inst) stored in memory by this store instruction
          if (auto escapeInst = dyn_cast<Instruction>(storeInst->getValueOperand())) {//Instruction*
            escapeSet.insert(escapeInst);
          }
        }
      }
      if (debug) {
        errs() << "escapeSet:\n";
        for (auto es: escapeSet) {
          errs() << *es << "\n";
        }
      }

      // loop through every instruction to deal with phi node, or const propagation for non-phiNode
      for(int idx = 0; idx < insts.size(); idx++) {
        if (!isa<CallInst>(insts[idx])) continue;

        Instruction *i = insts[idx];
        CallInst *callInst = cast<CallInst>(i);
        Function *callee = callInst->getCalledFunction();

        bool phi_flag; // denote whether i is a CAT_get related to phi node and it can be a constant
        Instruction *reachDef = NULL;
        set<int64_t> phiValues; // set of probable phi node const values. only can merge(rech def) if all const values are the same, i.e., size == 1

        if (getCatType(callee) == 4) { // only care about CAT_get
          reachDef = NULL;
          auto arg0Value = callInst->getArgOperand(0); //Value*

          // only consider it when it is not from the function argument
          if (!isa<Argument>(arg0Value)) {
            // check if it is a phi node: used to join if branches (i.e., different paths), (test3,test23...)
            // and select one of the value based on how control flow reach this phi node
            // see slides SSA p16
            if (isa<PHINode>(arg0Value)) { // find the reachDef for a CAT_get related to phi node
              auto phiNode = cast<PHINode>(arg0Value); //PHINode*
              phi_flag = 1;
              phiValues.clear();
              for (int br_idx = 0; br_idx < phiNode->getNumIncomingValues(); br_idx++) {
                auto nodeValue = phiNode->getIncomingValue(br_idx); //Value*
                if (isa<Argument>(nodeValue)) {
                  phi_flag = 0; // argument(escape), not a const
                  break;
                }
                else if (isa<PHINode>(nodeValue)) break; //"nested" phi node; break? continue?
                else { // may be a const
                  if (auto nodeInst = dyn_cast<CallInst>(nodeValue)) {//CallInst*
                    // this part is never been used in the tests of HW5!!!
                    // may need to be modified with constant folding (CAT_add, CAT_sub)
                    // may need to use recursion
                    if (auto nodeInst_arg0 = dyn_cast<CallInst>(nodeInst->getArgOperand(0))) {
                      Function *arg0Func = nodeInst_arg0->getCalledFunction();
                      if (getCatType(arg0Func) == 0) { // CAT_new
                        if (auto constValue = dyn_cast<ConstantInt>(nodeInst_arg0->getArgOperand(0))) {
                          phiValues.insert(constValue->getSExtValue());
                        }
                      }
                      else if (getCatType(arg0Func) == 1) { // CAT_set
                        if (auto constValue = dyn_cast<ConstantInt>(nodeInst_arg0->getArgOperand(1))) {
                          phiValues.insert(constValue->getSExtValue());
                        }
                      }
                      // else if CAT_add || CAT_sub
                    }
                    // only this part is used in tests of HW5
                    else if (ConstantInt *nodeConst_arg0 = dyn_cast<ConstantInt>(nodeInst->getArgOperand(0))) {
                      phiValues.insert(nodeConst_arg0->getSExtValue());
                    }
                  }
                }
              }
              if (phiValues.size() != 1) {
                // only can merge(rech def) if all const values from different branches are the same, i.e., size == 1
                phi_flag = 0;
              }
            
              if (phi_flag) {
                reachDef = cast<Instruction>(phiNode->getIncomingValue(0));
                if (auto reachCall = dyn_cast<CallInst>(reachDef)) { // only care about CAT var (CAT_new here)
                  auto value = reachCall->getArgOperand(0);
                  if (isa<ConstantInt>(value)) {
                    ConstantInt *c = cast<ConstantInt>(value);
                    BasicBlock::iterator ii(i);
                    BasicBlock *bb = i->getParent();
                    ReplaceInstWithValue(bb->getInstList(), ii, c);
                    modified = true;
                  }
                }
              }
            }

            else { // not a phi node
              modified = constantPropagation(insts, INs, escapeSet, idx, arg0Value);
            }
          }
        }
        
      }
      return modified;
    }

    // This function is invoked once per function compiled
    // The LLVM IR of the input functions is ready and it can be analyzed and/or transformed
    bool runOnFunction (Function &F) override {
      //errs() << "Hello LLVM World at \"runOnFunction\"\n" ;
      bool modified = false;

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
        for (auto &i : bb) { // i is an object
          getGenKill(i, insts, GENs, KILLs);
        }
      }

      /*HW3*/
      getInOut(F, insts, GENs, KILLs, INs, OUTs, oldOUTs);

      /*HW4*/
      // constant propagation for CAT_new and CAT_set;
      // includes potential constant folding for CAT_add and CAT_sub
      //constantPropagation(insts, INs);

      /*HW5*/
      // remove the assumptions
      // constant propagation for CAT_new and CAT_set;
      // includes potential constant folding for CAT_add and CAT_sub
      modified = dealEscape(insts, INs);

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
      
      //return false;
      return modified;
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
