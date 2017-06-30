#ifndef _STACKSHIELD_H
#define _STACKSHIELD_H

#include <llvm/Pass.h>
#include <llvm/IR/Function.h>

using namespace llvm;

namespace {
  struct StackShield : public FunctionPass {
    static char ID;

    StackShield() : FunctionPass(ID) {}

    bool runOnFunction(Function &F);
  };
}

#endif