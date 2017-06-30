#include "StackShield.hpp"
#include "Unsafe.hpp"

#include <llvm/IR/Instructions.h>
#include <llvm/IR/InstIterator.h>
#include <llvm/IR/LegacyPassManager.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/Transforms/IPO/PassManagerBuilder.h>

#include <map>

using namespace llvm;

char StackShield::ID = 0;

bool StackShield::runOnFunction(Function &F) {
  std::map<StringRef, AllocaInst *> localVariablesMap;
  std::vector<StringRef> vulnerableVariables;
  AllocaInst *firstAllocInst = NULL;

  for (inst_iterator I = inst_begin(F), E = inst_end(F); I!=E; I++) {

    if (AllocaInst *allocaInst = dyn_cast<AllocaInst>(&*I)) {
      // It is allocation instruction - a local variable on stack
      localVariablesMap[allocaInst->getName()] = allocaInst;
      if (firstAllocInst == NULL) {
        firstAllocInst = allocaInst;
      }
    }

    if (CallInst *callInst = dyn_cast<CallInst>(&*I)) {
      Function *func = callInst->getCalledFunction();
      if (func) {
        if (unsafe_functions.find(func->getName().str()) !=
          unsafe_functions.end()) {
          Value *operand = callInst->getArgOperand(0);
          if (GetElementPtrInst *gepInst =
            dyn_cast<GetElementPtrInst>(operand)) {
            StringRef variableName = gepInst->getPointerOperand()->getName();
            vulnerableVariables.push_back(variableName);
          }
        }
      }
    }
  }

  /**
   * Reordering variables
   */
  for (auto const& variable: vulnerableVariables) {
    if (localVariablesMap.count(variable) != 1) {
      errs() << variable << " has not been declared and used in a function.\n";
      continue;
    }

    AllocaInst *allocaInst = localVariablesMap[variable];
    if (allocaInst == firstAllocInst) {
      continue;
    }
    // Moving this allocation instruction from LLVM IR to the start
    allocaInst->moveBefore(firstAllocInst);
    firstAllocInst = allocaInst;
  }

  return false;
}

/**
 * Registring our pass
 */
static RegisterPass<StackShield> X("stack-shield", "Stack Shield Pass",
                             false /* Only looks at CFG */,
                             false /* Analysis Pass */);

static void registerHelloPass(const PassManagerBuilder &,
                               PassManagerBase &PM) {
  PM.add(new StackShield());
}

static RegisterStandardPasses
    RegisterMBAPass(PassManagerBuilder::EP_EarlyAsPossible,
                    registerHelloPass);