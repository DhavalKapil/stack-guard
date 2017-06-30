#include "DependencyGraph.hpp"

#include <algorithm>
#include <sstream>

using namespace llvm;

// A character not allowed in function or variable name
const std::string HASH_SEPARATOR = "#";

std::set<Node *> Node::getSources() {
  std::set<Node *> sources;
  if (parents.empty()) {
    sources.insert(this);
    return sources;
  }
  for (const auto parent : parents) {
    std::set<Node *> parentSet = parent->getSources();
    sources.insert(parentSet.begin(), parentSet.end());
  }
  return sources;
}

void Node::addDependent(Node *ptr) {
  ptr->parents.push_back(this);
}

Function *Node::getFunction() {
  return func;
}

AllocaInst *Node::getAllocaInst() {
  return allocaInst;
}

Node *DependencyGraph::get(Function *func, Value *val) {
  // Generating a hash from func and var
  std::ostringstream oss;
  oss << func->getName().str() << HASH_SEPARATOR << val;
  std::string hash = oss.str();

  if (nodes.count(hash) == 0) {
    // Creating new node
    Node *node = new Node;
    node->func = func;
    nodes[hash] = node;
    return node;
  }

  return nodes[hash];
}

Node *DependencyGraph::get(Function *func, Value *val,
                           AllocaInst *allocaInst) {
  Node *node = get(func, val);
  node->allocaInst = allocaInst;
  return node;
}

void DependencyGraph::clear() {
  for (const auto &pair : nodes) {
    delete pair.second;
  }
}