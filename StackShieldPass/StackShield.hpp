#ifndef _STACK_SHIELD_H
#define _STACK_SHIELD_H

#include "DependencyGraph.hpp"

#include <llvm/Pass.h>
#include <llvm/IR/Function.h>

#include <map>
#include <vector>

using namespace llvm;

namespace {
  struct StackShield : public FunctionPass {
    static char ID;

    DependencyGraph *dependencyGraph;
    std::map<Function *, AllocaInst *> firstAllocaInst;
    std::vector<Node *> vulnerableNodes;

    StackShield() : FunctionPass(ID) {}

    bool doInitialization(Module &);
    bool runOnFunction(Function &F);
    bool doFinalization(Module &);
  };
}

#endif