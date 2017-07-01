#ifndef _STACK_GUARD_H
#define _STACK_GUARD_H

#include "DependencyGraph.hpp"

#include <llvm/Pass.h>
#include <llvm/IR/Function.h>

#include <map>
#include <vector>

using namespace llvm;

namespace {
  struct StackGuard : public FunctionPass {
    static char ID;

    DependencyGraph *dependencyGraph;
    std::map<Function *, AllocaInst *> firstAllocaInst;
    std::vector<Node *> vulnerableNodes;

    StackGuard() : FunctionPass(ID) {}

    bool doInitialization(Module &);
    bool runOnFunction(Function &F);
    bool doFinalization(Module &);
  };
}

#endif