#include "DependencyGraph.hpp"

#include <algorithm>

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

Node *DependencyGraph::get(Function *func, StringRef var) {
  // Generating a hash from func and var
  std::string hash = func->getName().str() + HASH_SEPARATOR + var.str();

  if (nodes.count(hash) == 0) {
    // Creating new node
    Node *node = new Node;
    node->func = func;
    nodes[hash] = node;
    return node;
  }

  return nodes[hash];
}

Node *DependencyGraph::get(Function *func, StringRef var,
                           AllocaInst *allocaInst) {
  Node *node = get(func, var);
  node->allocaInst = allocaInst;
  return node;
}

void DependencyGraph::clear() {
  for (const auto &pair : nodes) {
    delete pair.second;
  }
}