set(CMAKE_CXX_COMPILER "/usr/local/bin/clang++")
SET( CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Xclang -load -Xclang ../StackShieldPass/LLVMStackShieldPass.so" )
add_executable(tests test.cpp)