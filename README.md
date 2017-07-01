# stack-guard

A toy implementation of 'Stack Guard' on top of the [LLVM](http://llvm.org/) compiler toolchain. 

Instrumentation code is added during compilation process to insert and verify stack canaries. Local variables (on the stack) are reordered to prevent buffers overflowing into other local variables. Vulnerable buffers are identified by performing a simple version of static taint analysis. Dependencies are maintained between function calls and pointer manipulations.

**Note**: This was developed as a hobby project for fun. It should only be used for educational purposes. Some of its features could be implemented in a much simpler way, but I choose techniques which involved more of LLVM API, just to get used to it.

## Installation

StackGuard requires `cmake` >= 3.8. It has been tested on llvm 3.5 and clang 3.5.

To build:

```
mkdir build && cd build
cmake -DLLVM_ROOT=/path/to/llvm/build ..
make
```

## Usage

```
clang -Xload -load -Xload /path/to/StackGuard/build/StackGuardPass/LLVMStackGuardPass.so <source code>
```

Tests are present in `./tests`. Run them using:

```
make test
```

## License

StackGuard is licensed under the [MIT license](https://dhaval.mit-license.org/2017/license.txt).
