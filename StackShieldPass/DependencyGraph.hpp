#ifndef _DEPENDENCY_GRAPH_H
#define _DEPENDENCY_GRAPH_H

#include <llvm/IR/Function.h>
#include <llvm/IR/Instructions.h>

#include <vector>
#include <map>
#include <set>

using namespace llvm;

class Node {
  // The nodes on which this is dependent
  std::vector<Node *> parents;
  // The function in which this is declared
  Function *func;
  // The allocation instruction, if this is a source
  AllocaInst *allocaInst;

public:
  /**
   * Retrieve sources to perform analysis
   */
  std::set<Node *> getSources();
  void addDependent(Node *node);
  Function *getFunction();
  AllocaInst *getAllocaInst();

  friend class DependencyGraph;
};

class DependencyGraph {
  std::map<std::string, Node *> nodes;

public:
  /**
   * Returns the node corresponding to var in func
   * Creates a new one if it does not exists
   */
  Node *get(Function *func, Value *val);
  Node *get(Function *func, Value *val, AllocaInst *allocaInst);

  /**
   * Clears graph
   */
  void clear();
};

#endif