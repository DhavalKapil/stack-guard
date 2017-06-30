#include "StackShield.hpp"
#include "Unsafe.hpp"

#include <llvm/IR/Instructions.h>
#include <llvm/IR/InstIterator.h>
#include <llvm/IR/LegacyPassManager.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/Transforms/IPO/PassManagerBuilder.h>

#include <set>

using namespace llvm;

char StackShield::ID = 0;

bool StackShield::doInitialization(Module &) {
  dependencyGraph = new DependencyGraph;
  firstAllocaInst.clear();
  vulnerableNodes.clear();
  return false;
}

bool StackShield::doFinalization(Module &) {
  std::set<Node *> sources;

  /**
   * Reordering variables
   */
  for (auto node: vulnerableNodes) {
    std::set<Node *> vulnerableSources = node->getSources();
    sources.insert(vulnerableSources.begin(), vulnerableSources.end());
  }

  for (auto node: sources) {
    AllocaInst *allocaInst = node->getAllocaInst();
    Function *func = node->getFunction();
    if (allocaInst == firstAllocaInst[func]) {
      continue;
    }
    // Moving this allocation instruction from LLVM IR to the start
    allocaInst->moveBefore(firstAllocaInst[func]);
    firstAllocaInst[func] = allocaInst;
  }

  dependencyGraph->clear();
  delete dependencyGraph;

  return true;
}

bool StackShield::runOnFunction(Function &F) {

  for (inst_iterator I = inst_begin(F), E = inst_end(F); I!=E; I++) {

    if (AllocaInst *allocaInst = dyn_cast<AllocaInst>(&*I)) {
      // It is allocation instruction - a local variable on stack
      dependencyGraph->get(&F, allocaInst->getName(), allocaInst);
      if (firstAllocaInst.count(&F) == 0) {
        firstAllocaInst[&F] = allocaInst;
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
            vulnerableNodes.push_back(dependencyGraph->get(&F, variableName));
          }
        }
      }
    }
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