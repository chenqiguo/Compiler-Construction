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
#include "llvm/Analysis/MemoryDependenceAnalysis.h"
#include "llvm/Analysis/AliasSetTracker.h"
#include "llvm/Analysis/AliasAnalysis.h"
#include <string.h>

using namespace llvm;
using namespace std;
bool debug = 0;
bool print_GEN_KILL = 0;
bool print_IN_OUT = 0;

namespace {
  struct CAT : public FunctionPass {
    static char ID; 
    Module *currM;

    CAT() : FunctionPass(ID) {}

    // This function is invoked once at the initialization phase of the compiler
    // The LLVM IR of functions isn't ready at this point
    bool doInitialization (Module &M) override {
      //errs() << "Hello LLVM World at \"doInitialization\"\n" ;
      currM = &M;
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

    void getGenKill(Instruction &i, vector<Instruction *> &insts, vector<SparseBitVector<>> &GENs, vector<SparseBitVector<>> &KILLs, AliasAnalysis &aliasAnalysis) {
      /*HW2*/ /*modified to H6 version*/
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

      if (getCatType(callee) == 0) { // i is CAT_new
        // i) compute GEN set
        auto it = std::find(insts.begin(), insts.end(), &i);
        index = std::distance(insts.begin(), it);
        GENs.back().set(index);
        // newly added in HW6: alias analysis
        if (i.getType()->isPointerTy() && isa<CallInst>(&i)) { // i is CAT_func
          auto sizePointer_i = getPointedElementTypeSize(&i);
          for (auto aaInst : insts) {
            if (aaInst == &i) continue;
            // case a) if aaInst is a pointer of type CAT_func
            if (aaInst->getType()->isPointerTy() && isa<CallInst>(aaInst)) {
              auto aaCallInst = cast<CallInst>(aaInst);
              if (getCatType(aaCallInst->getCalledFunction()) != -1) { // if aaInst is a CAT var inst
                errs() << "aaInst is a pointer of type CAT_func\n";
                errs() << "aaInst: " << *aaInst << "\n";
                auto sizePointer_aa = getPointedElementTypeSize(aaInst);
                switch(aliasAnalysis.alias(&i, sizePointer_i, aaInst, sizePointer_aa)) {
                  case NoAlias:
                    errs() << "     No alias\n" ;
                    break ;
                  case MayAlias: case PartialAlias: case MustAlias:
                    errs() << "     Alias\n" ;
                    // insert into GEN set
                    it = std::find(insts.begin(), insts.end(), aaInst);
                    index = std::distance(insts.begin(), it);
                    GENs.back().set(index);
                    break;
                  default:
                    abort();
                }
              }
            } // end of case a)
            // case b) if aaInst is a storeInst or loadInst
            if (isa<LoadInst>(aaInst) || isa<StoreInst>(aaInst)) { // aaInst is a memInst (load or store)
              errs() << "aaInst is a storeInst or loadInst\n";
              errs() << "aaInst: " << *aaInst << "\n";
              switch(aliasAnalysis.getModRefInfo(aaInst, &i, sizePointer_i)) {
                case ModRefInfo::NoModRef:
                  errs() << "     NoModRef\n";
                  break ;
                case ModRefInfo::Mod: case ModRefInfo::Ref: case ModRefInfo::ModRef:
                  errs() << "     ModRef\n";
                  // insert into GEN set
                  it = std::find(insts.begin(), insts.end(), aaInst);
                  index = std::distance(insts.begin(), it);
                  GENs.back().set(index);
                  break;
                default:
                  abort();
              }
            }
          }
        }

        // ii) compute KILL set
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
              // newly added in HW6: alias analysis
              if (inst_u->getType()->isPointerTy() && isa<CallInst>(inst_u)) { // u is CAT_func <- already satisfy
                auto sizePointer_u = getPointedElementTypeSize(inst_u);
                for (auto aaInst : insts) {
                  if (aaInst == inst_u) continue;
                  // case a) if aaInst is a pointer of type CAT_func
                  if (aaInst->getType()->isPointerTy() && isa<CallInst>(aaInst)) {
                    auto aaCallInst = cast<CallInst>(aaInst);
                    if (getCatType(aaCallInst->getCalledFunction()) != -1) { // if aaInst is a CAT var inst
                      errs() << "aaInst is a pointer of type CAT_func\n";
                      errs() << "aaInst: " << *aaInst << "\n";
                      auto sizePointer_aa = getPointedElementTypeSize(aaInst);
                      switch(aliasAnalysis.alias(inst_u, sizePointer_u, aaInst, sizePointer_aa)) {
                        case NoAlias: case MayAlias: case PartialAlias: 
                          errs() << "     No alias\n" ;
                          break ;
                        case MustAlias:
                          errs() << "     Alias\n" ;
                          // insert into KILL set
                          it = std::find(insts.begin(), insts.end(), aaInst);
                          index = std::distance(insts.begin(), it);
                          KILLs.back().set(index);
                          break;
                        default:
                          abort();
                      }
                    }
                  } // end of case a)
                  // case b) if aaInst is a storeInst or loadInst
                  if (isa<LoadInst>(aaInst) || isa<StoreInst>(aaInst)) { // aaInst is memInst (load or store)
                    errs() << "aaInst is a storeInst or loadInst\n";
                    errs() << "aaInst: " << *aaInst << "\n";
                    switch(aliasAnalysis.getModRefInfo(aaInst, inst_u, sizePointer_u)) {
                      case ModRefInfo::NoModRef:
                        errs() << "     NoModRef\n";
                        break ;
                      case ModRefInfo::Mod: case ModRefInfo::Ref: case ModRefInfo::ModRef:
                        errs() << "     ModRef\n";
                        // insert into KILL set
                        it = std::find(insts.begin(), insts.end(), aaInst);
                        index = std::distance(insts.begin(), it);
                        KILLs.back().set(index);
                        break;
                      default:
                        abort();
                    }
                  }
                }
              }
            }
          }
        }
      }

      else if ((getCatType(callee) == 1) || (getCatType(callee) == 2) || (getCatType(callee) == 3)) {
        // i) compute GEN set
        auto it = std::find(insts.begin(), insts.end(), &i);
        index = std::distance(insts.begin(), it);
        GENs.back().set(index);
        // newly added in HW6: alias analysis
        if (i.getType()->isPointerTy() && isa<CallInst>(&i)) {
          auto sizePointer_i = getPointedElementTypeSize(&i);
          for (auto aaInst : insts) {
            if (aaInst == &i) continue;
            // case a) if aaInst is a pointer of type CAT_func
            if (aaInst->getType()->isPointerTy() && isa<CallInst>(aaInst)) {
              auto aaCallInst = cast<CallInst>(aaInst);
              if (getCatType(aaCallInst->getCalledFunction()) != -1) { // if aaInst is a CAT var inst
                errs() << "aaInst is a pointer of type CAT_func\n";
                errs() << "aaInst: " << *aaInst << "\n";
                auto sizePointer_aa = getPointedElementTypeSize(aaInst);
                switch(aliasAnalysis.alias(&i, sizePointer_i, aaInst, sizePointer_aa)) {
                  case NoAlias:
                    errs() << "     No alias\n" ;
                    break ;
                  case MayAlias: case PartialAlias: case MustAlias:
                    errs() << "     Alias\n" ;
                    // insert into GEN set
                    it = std::find(insts.begin(), insts.end(), aaInst);
                    index = std::distance(insts.begin(), it);
                    GENs.back().set(index);
                    break;
                  default:
                    abort();
                }
              }
            } // end of case a)
            // case b) if aaInst is a storeInst or loadInst
            if (isa<LoadInst>(aaInst) || isa<StoreInst>(aaInst)) { // aaInst is memInst
              errs() << "aaInst is a storeInst or loadInst\n";
              errs() << "aaInst: " << *aaInst << "\n";
              switch(aliasAnalysis.getModRefInfo(aaInst, &i, sizePointer_i)) {
                case ModRefInfo::NoModRef:
                  errs() << "     NoModRef\n";
                  break ;
                case ModRefInfo::Mod: case ModRefInfo::Ref: case ModRefInfo::ModRef:
                  errs() << "     ModRef\n";
                  // insert into GEN set
                  it = std::find(insts.begin(), insts.end(), aaInst);
                  index = std::distance(insts.begin(), it);
                  GENs.back().set(index);
                  break;
                default:
                  abort();
              }
            }
          }
        }

        // ii) compute KILL set
        // Get the instruction callInst_arg0 that defines the arg0 in instruction callInst
        if (Instruction *callInst_arg0 = dyn_cast<Instruction>(callInst->getArgOperand(0))) {
          it = std::find(insts.begin(), insts.end(), callInst_arg0);
          index = std::distance(insts.begin(), it);
          KILLs.back().set(index);
          // newly added in HW6: alias analysis
          if (callInst_arg0->getType()->isPointerTy() && isa<CallInst>(callInst_arg0)) {
            auto sizePointer_arg0 = getPointedElementTypeSize(callInst_arg0);
            for (auto aaInst : insts) {
              if (aaInst == callInst_arg0) continue;
              // case a) if aaInst is a pointer of type CAT_func
              if (aaInst->getType()->isPointerTy() && isa<CallInst>(aaInst)) {
                auto aaCallInst = cast<CallInst>(aaInst);
                if (getCatType(aaCallInst->getCalledFunction()) != -1) { // if aaInst is a CAT var inst
                  errs() << "aaInst is a pointer of type CAT_func\n";
                  errs() << "aaInst: " << *aaInst << "\n";
                  auto sizePointer_aa = getPointedElementTypeSize(aaInst);
                  switch(aliasAnalysis.alias(callInst_arg0, sizePointer_arg0, aaInst, sizePointer_aa)) {
                    case NoAlias: case MayAlias: case PartialAlias: 
                      errs() << "     No alias\n" ;
                      break ;
                    case MustAlias:
                      errs() << "     Alias\n" ;
                      // insert into KILL set
                      it = std::find(insts.begin(), insts.end(), aaInst);
                      index = std::distance(insts.begin(), it);
                      KILLs.back().set(index);
                      break;
                    default:
                      abort();
                  }
                }
              } // end of case a)
              // case b) if aaInst is a storeInst or loadInst
              if (isa<LoadInst>(aaInst) || isa<StoreInst>(aaInst)) { // aaInst is memInst
                errs() << "aaInst is a storeInst or loadInst\n";
                errs() << "aaInst: " << *aaInst << "\n";
                switch(aliasAnalysis.getModRefInfo(aaInst, callInst_arg0, sizePointer_arg0)) {
                  case ModRefInfo::NoModRef:
                    errs() << "     NoModRef\n";
                    break ;
                  case ModRefInfo::Mod: case ModRefInfo::Ref: case ModRefInfo::ModRef:
                    errs() << "     ModRef\n";
                    // insert into KILL set
                    it = std::find(insts.begin(), insts.end(), aaInst);
                    index = std::distance(insts.begin(), it);
                    KILLs.back().set(index);
                    break;
                  default:
                    abort();
                }
              }
            }
          }

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
                // newly added in HW6: alias analysis
                if (inst_u->getType()->isPointerTy() && isa<CallInst>(inst_u)) {
                  auto sizePointer_u = getPointedElementTypeSize(inst_u);
                  for (auto aaInst : insts) {
                    if (aaInst == inst_u) continue;
                    // case a) if aaInst is a pointer of type CAT_func
                    if (aaInst->getType()->isPointerTy() && isa<CallInst>(aaInst)) {
                      auto aaCallInst = cast<CallInst>(aaInst);
                      if (getCatType(aaCallInst->getCalledFunction()) != -1) { // if aaInst is a CAT var inst
                        errs() << "aaInst is a pointer of type CAT_func\n";
                        errs() << "aaInst: " << *aaInst << "\n";
                        auto sizePointer_aa = getPointedElementTypeSize(aaInst);
                        switch(aliasAnalysis.alias(inst_u, sizePointer_u, aaInst, sizePointer_aa)) {
                          case NoAlias: case MayAlias: case PartialAlias: 
                            errs() << "     No alias\n" ;
                            break ;
                          case MustAlias:
                            errs() << "     Alias\n" ;
                            // insert into KILL set
                            it = std::find(insts.begin(), insts.end(), aaInst);
                            index = std::distance(insts.begin(), it);
                            KILLs.back().set(index);
                            break;
                          default:
                            abort();
                        }
                      }
                    } // end of case a)
                    // case b) if aaInst is a storeInst or loadInst
                    else if (isa<LoadInst>(aaInst) || isa<StoreInst>(aaInst)) { // aaInst is memInst
                      errs() << "aaInst is a storeInst or loadInst\n";
                      errs() << "aaInst: " << *aaInst << "\n";
                      switch(aliasAnalysis.getModRefInfo(aaInst, inst_u, sizePointer_u)) {
                        case ModRefInfo::NoModRef:
                          errs() << "     NoModRef\n";
                          break ;
                        case ModRefInfo::Mod: case ModRefInfo::Ref: case ModRefInfo::ModRef:
                          errs() << "     ModRef\n";
                          // insert into KILL set
                          it = std::find(insts.begin(), insts.end(), aaInst);
                          index = std::distance(insts.begin(), it);
                          KILLs.back().set(index);
                          break;
                        default:
                          abort();
                      }
                    }
                  }
                }
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
                      for (auto &&out : OUTs[predIndex]){
                        INs[instIndex].set(out);
                      }
                    }
                  }
                }
              }
            }
            SparseBitVector<> difference; // IN[i] - KILL[i]
            difference = INs[instIndex] - KILLs[instIndex];
            OUTs[instIndex] = GENs[instIndex] | difference;

            if (OUTs[instIndex] != oldOUTs[instIndex]) {
              change_flag = 1;
              oldOUTs[instIndex] = OUTs[instIndex];
            }
            
            instIndex++;
            isFirst = 0;
          }
        }
      }
    }

    bool constProp(vector<Instruction *> &insts, Instruction *inst_arg0, vector<SparseBitVector<>> &INs, /*set<Instruction*> &escapeSet,*/ int idx) {
      /*H6*/
      bool modified = false; // return value
      bool isSet = 0; // flag for CAT_set (for constant propagation)
      Instruction *reachDef = NULL; // for constant propagation here
      ConstantInt *foldConst = NULL; // probable result from contant folding
      set<int64_t> consts; // set of probable constant value
      ConstantInt *resultConstant = NULL; // final result (if exists)
      int64_t reachResult; // for constant propagation
      int64_t foldResult; // probable result from contant folding
      Instruction *i = insts[idx];

      if (auto arg0_callInst = dyn_cast<CallInst>(inst_arg0)) {
        auto tmp = arg0_callInst->getArgOperand(0);
        if (!isa<Argument>(arg0_callInst->getArgOperand(0))) {
          SparseBitVector<> ins = INs[idx]; // the IN set for CAT_get instruction
          for (auto &&in : ins) {
            Instruction *inInst = insts.at(in);
            if (auto in_callInst = dyn_cast<CallInst>(inInst)) {
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
                  foldConst = constantFolding(inInst, insts, INs/*, escapeSet*/);
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

    bool constantPropagation(vector<Instruction *> &insts, vector<SparseBitVector<>> &INs, set<Instruction*> &escapeSet, int idx, Value *arg0Value, AliasAnalysis &aliasAnalysis) {
      /*HW4*/ /*modified to HW6 version*/
      Instruction *i = insts[idx];
      CallInst *callInst = cast<CallInst>(i);
      Function *callee = callInst->getCalledFunction();
      SparseBitVector<> ins = INs[idx]; // ins is the IN set for this instruction

      bool modified = false; // return val
      
      if (Instruction *inst_arg0 = dyn_cast<Instruction>(arg0Value)) { // already checked arg0Value is not arg in func

        if (escapeSet.find(inst_arg0) != escapeSet.end()) { // if inst_arg0 is on memory
          // check alias (or dependence?) <- this may not necessary
          for (auto &escapeInst : escapeSet) { // for all the other (CAT) var on memory
            if (inst_arg0 == escapeInst) continue ;
            switch (aliasAnalysis.alias(MemoryLocation::get(inst_arg0), MemoryLocation::get(escapeInst))) {
              case NoAlias:
                modified = constProp(insts, inst_arg0, INs, /*escapeSet,*/ idx);
                break;
              case MayAlias: case PartialAlias: case MustAlias:
                modified =false;
                break;
              default:
                abort();
            }
          }
        }
        else { // if inst_arg0 is not on memory
          modified = constProp(insts, inst_arg0, INs, /*escapeSet,*/ idx);
        }
      }
      return modified;
    }

    ConstantInt * constantFolding (Instruction *i, vector<Instruction *> &insts, vector<SparseBitVector<>> &INs/*, set<Instruction*> &escapeSet*/) {
      /*HW4*/ /*modified to HW5 version*/
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
        //if((escapeSet.find(inst_arg0) == escapeSet.end()) && (escapeSet.find(inst_arg1) == escapeSet.end()) && (escapeSet.find(inst_arg2) == escapeSet.end())) {
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
                  foldConst1 = constantFolding(inInst, insts, INs/*, escapeSet*/); // instead, call constant folding
                }
                else if (inInst_arg0 == inst_arg2) {
                  reachDef2 = NULL; // for probable constant folding later
                  isSet2 = 0;
                  foldConst2 = constantFolding(inInst, insts, INs/*, escapeSet*/); // instead, call constant folding
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
        //}
      }
      return resultConstant;
    }

    bool dealPhiNode(PHINode *phiNode/*, set<Instruction*> &escapeSet*/) {
      /*HW6*/
      // deal with phi node: determine whether this phiNode can be a const
      // modified from dealEscape() in HW5
      bool phi_flag = 1; // denote whether inst i is a CAT_get related to phi node and it can be a constant
      set<int64_t> phiValues; // set of probable phi node const values. only can merge(rech def) if all const values are the same, i.e., size == 1

      for (int br_idx = 0; br_idx < phiNode->getNumIncomingValues(); br_idx++) {
        auto nodeValue = phiNode->getIncomingValue(br_idx); //Value* (must be a CAT var!)
        if (isa<Argument>(nodeValue)) {
          phi_flag = 0; // argument(escape), not a const
          break;
        }
        else if (isa<PHINode>(nodeValue)) {
          phi_flag = 0;
          break; //"nested" phi node; break? continue? recursion to compute?
        }
        else { // may be a const
          if (auto nodeInst = dyn_cast<CallInst>(nodeValue)) {//CallInst*
            Function *arg0Func = nodeInst->getCalledFunction();
            // if nodeValue is from another non-CAT func or escaped, cannot be a const
            if (getCatType(arg0Func) == -1) {
              phi_flag = 0;
              break;
            }
            // if (escapeSet.find(nodeInst) != escapeSet.end()) {
            //   phi_flag = 0;
            //   break;
            // }

            // nodeInst is a CAT func that not escaped
            if (getCatType(arg0Func) == 0) { // CAT_new
              if (auto constValue = dyn_cast<ConstantInt>(nodeInst->getArgOperand(0))) {
                phiValues.insert(constValue->getSExtValue());
              }
            }
            else if (getCatType(arg0Func) == 1) { // CAT_set
              if (auto constValue = dyn_cast<ConstantInt>(nodeInst->getArgOperand(1))) {
                phiValues.insert(constValue->getSExtValue());
              }
            }
            // else if CAT_add || CAT_sub
            // may need to be modified with constant folding (CAT_add, CAT_sub)
            // may need to use recursion
            // modify!!!
          }
        }
      }
      if (phiValues.size() != 1) {
        // only can merge(rech def) if all const values from different branches are the same, i.e., size == 1
        phi_flag = 0;
      }

      return phi_flag;
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

    void computeMemArg(vector<Instruction *> &insts, set<Instruction*> &memSet, set<Instruction*> &argSet, AliasAnalysis &aliasAnalysis, MemoryDependenceResults &MD) {
      /*HW6*/
      // compute the memSet and argSet, used in aliasDepAnalysis()
      // modified from dealEscape() in HW5

      vector<Instruction*> helperMemVec;
      //vector<Instruction*> helperPtrVec;
      for (auto i : insts) {
        if (isa<LoadInst>(i) || isa<StoreInst>(i)) {
          helperMemVec.push_back(i);
        }
      }
      // for (auto i : insts) {
      //   if (i->getType()->isPointerTy()) {
      //     helperPtrVec.push_back(i);
      //   }
      // }

      // loop through each instruction to fill in the memSet and argSet
      for (int idx = 0; idx < insts.size(); idx++) {
        Instruction *i = insts[idx];
        // 1) for every CAT var as a parameter of a function except CAT_func, recognize as escaping (in argSet)
        if (auto funcInst= dyn_cast<CallInst>(i)) {//CallInst*
          if (funcInst->getNumArgOperands() > 0) {
            Function *func = funcInst->getCalledFunction();
            if (getCatType(func) == -1) { // for functions with parameter(s) (except CAT_new, CAT_set, CAT_get, CAT_add, CAT_sub)
              for (int arg_idx = 0; arg_idx < funcInst->getNumArgOperands(); arg_idx++) {
                auto argValue = funcInst->getArgOperand(arg_idx); //Value*
                if (auto argInst = dyn_cast<Instruction>(argValue)) {
                  if (auto escapeInst = dyn_cast<CallInst>(argInst)) {
                    Function *escape_callee = escapeInst->getCalledFunction();
                    if (getCatType(escape_callee) == 0) { // CAT_new
                      auto sizePointer = getPointedElementTypeSize(argInst);
                      // API: getModRefInfo(const CallBase *Call, const Value *P, LocationSize Size)
                      // CallBase: CallInst* funcInst; Value* argValue; LocationSize? sizePointer
                      // return information about whether paticular call site modifies or reads the specifed mem location
                      switch (aliasAnalysis.getModRefInfo(funcInst, argValue, sizePointer)) {
                        case ModRefInfo::NoModRef:
                          //errs() << "NoModRef!\n";
                          break;
                        case ModRefInfo::Mod: case ModRefInfo::Ref: case ModRefInfo::ModRef:
                          //errs() << "add to argSet!\n";
                          argSet.insert(escapeInst);
                          break;
                        default:
                          abort();
                      }
                    }
                  }
                  else if (argInst->getType()->isPointerTy() /*&& isa<CallInst>(argInst)*/) {//???
                    if (!func->doesNotAccessMemory()) {
                      for (auto memInst : helperMemVec) {
                        // get the operand of memInst
                        if (isa<StoreInst>(memInst)) {
                          if (memInst->getNumOperands() > 1) {
                            auto src = memInst->getOperand(0); //Value*
                            auto dst = memInst->getOperand(1);
                            //errs() << "src: " << *src << "\n";
                            //errs() << "dst: " << *dst << "\n";
                            if (dst == argInst) {
                              if(auto srcInst = dyn_cast<Instruction>(src))
                                argSet.insert(srcInst);
                            }
                          }
                        }
                      }
                    }
                  }
                }
              }
            }
          }
        }
        // 2) for every CAT var get pointed (i.e., CAT vars that are stored in memory), recognize as escaping (in memSet)
        else if (isa<StoreInst>(i)) { //e.g., case 1: (*pointer) = CAT_new(50); | case 2: d = CAT_new(50); (*pointer) = d;
          auto storeInst = cast<StoreInst>(i);
          // fetch the var (cast to inst) stored in memory by this store instruction
          if (auto escapeInst = dyn_cast<Instruction>(storeInst->getValueOperand())) {//Instruction*
            memSet.insert(escapeInst);
          }
        }
      }
    }

    bool aliasDepAnalysis(vector<Instruction *> &insts, vector<SparseBitVector<>> &INs, AliasAnalysis &aliasAnalysis, MemoryDependenceResults &MD) {
      /*HW6*/
      // reference: AliasAnalysis slides
      // modified from dealEscape() in HW5
      // for CAT vars in escaped, optimize them if they are on memory and definitely NOT alias or depend on others (intra!)
      bool modified = false; // the return value
      // escape: 1) CAT vars as parameters of a function; 2) CAT vars stored in memory
      // we only optimize CAT vars in memSet that are not aliased or depends on other CAT vars,
      // and just give up on optimizing the CAT vars that is arg or in memSet that are aliased or depends on other CAT vars
      set<Instruction*> memSet; // set of escaped CAT vars that are on memory <- not used!
      set<Instruction*> argSet; // set of escaped CAT vars that are from func arguments
      computeMemArg(insts, memSet, argSet, aliasAnalysis, MD);
      if (debug) {
        errs() << "argSet:\n";
        for (auto arg: argSet) {
          errs() << *arg << "\n";
        }
      }
      set<Instruction*> escapeSet; // set of CAT vars that are in memSet and definitely alias or depend on others (intra!)
      for (auto i : insts) {
        if (isa<LoadInst>(i) || isa<StoreInst>(i)) {
          escapeSet.insert(i);
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
        //set<int64_t> phiValues; // set of probable phi node const values. only can merge(rech def) if all const values are the same, i.e., size == 1

        if (getCatType(callee) == 4) { // only care about CAT_get
          reachDef = NULL;
          auto arg0Value = callInst->getArgOperand(0); //Value*
          errs() << "CAT_get()!!!\n";

          // only consider it when it is not from the function argument
          if (!isa<Argument>(arg0Value)) {
            // if (Instruction *inst_arg0 = dyn_cast<Instruction>(arg0Value))
            //   if (argSet.find(inst_arg0) != argSet.end())
            //     continue;
            
            errs() << "arg0Value: " << *arg0Value << "\n";
            // check if it is a phi node: used to join if branches (i.e., different paths), (test3,test23...)
            // and select one of the value based on how control flow reach this phi node
            // see slides SSA p16
            if (isa<PHINode>(arg0Value)) { // find the reachDef for a CAT_get related to phi node
              errs() << "Is phi node!\n";
              auto phiNode = cast<PHINode>(arg0Value); //PHINode*
              // deal with phi node: get the flag
              phi_flag = dealPhiNode(phiNode/*, escapeSet*/);
              if (phi_flag) {
                reachDef = cast<Instruction>(phiNode->getIncomingValue(0));
                if (auto reachCall = dyn_cast<CallInst>(reachDef)) { // only care about CAT var (CAT_new and CAT_set here)
                  Function *reachFunc = reachCall->getCalledFunction();
                  //errs() << "CatType is: " << to_string(getCatType(reachFunc)) << "\n";
                  bool isSet = getCatType(reachFunc);
                  auto value = reachCall->getArgOperand(isSet);
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
              // for each arg in argSet, check whether they are dependence
              if (Instruction *inst_arg0 = dyn_cast<Instruction>(arg0Value)) {
                if (argSet.find(inst_arg0) != argSet.end()) {
                }
                else {
                  modified = constantPropagation(insts, INs, escapeSet, idx, arg0Value, aliasAnalysis);
                }
              }
                
              
            }
          }
        }
      }

      return modified;

    }

    // void print_phiNode(vector<Instruction *> &insts) {
    //   errs() << "$$$$$$$$$$$$$$$$print_phiNode begin$$$$$$$$$$$$$$$$\n";
    //   for (auto i : insts) {
    //     if (!isa<CallInst>(i)) continue;

    //     CallInst *callInst = cast<CallInst>(i);
    //     Function *callee = callInst->getCalledFunction();

    //     if (getCatType(callee) == -1) continue;

    //     errs() << "&&&&&&&&&&&&&&&&instruction: " << *i << "\n";
    //     auto arg0Value = callInst->getArgOperand(0); //Value*
    //     errs() << "arg0Value: " << *arg0Value << "\n";
    //     if (isa<PHINode>(arg0Value)) errs() << "arg0Value is phi node!\n";
    //   }
    //   errs() << "$$$$$$$$$$$$$$$$print_phiNode end$$$$$$$$$$$$$$$$\n";
    // }

    // This function is invoked once per function compiled
    // The LLVM IR of the input functions is ready and it can be analyzed and/or transformed
    bool runOnFunction (Function &F) override {
      //errs() << "Hello LLVM World at \"runOnFunction\"\n" ;
      errs() << "Function: " << F.getName() << "\"\n";
      bool modified = false;

      /*HW6*/
      // APIs are from AliasAnalysis slides and LLVM dependences.tar.bz2
      AliasAnalysis &aliasAnalysis = getAnalysis< AAResultsWrapperPass >().getAAResults(); // alias analysis
      MemoryDependenceResults &MD = getAnalysis< MemoryDependenceWrapperPass >().getMemDep(); // dependence

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
          getGenKill(i, insts, GENs, KILLs, aliasAnalysis);
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
      //modified = dealEscape(insts, INs);

      /*HW6*/
      // for debug
      //if (debug) print_phiNode(insts);
      // remove the assumptions, using alias analysis and/or dependence analysis
      // constant propagation for CAT_new and CAT_set;
      // includes potential constant folding for CAT_add and CAT_sub
      modified = aliasDepAnalysis(insts, INs, aliasAnalysis, MD);
      //modified = depAnalysis(insts, INs, MD, modified);
      
      

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
      AU.addRequired< AAResultsWrapperPass >();
      AU.addRequired< MemoryDependenceWrapperPass >();
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
// static RegisterStandardPasses _RegPass2(PassManagerBuilder::EP_EnabledOnOptLevel0,
//     [](const PassManagerBuilder&, legacy::PassManagerBase& PM) {
//         if(!_PassMaker){ PM.add(_PassMaker = new CAT()); }}); // ** for -O0
