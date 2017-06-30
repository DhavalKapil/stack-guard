set(CMAKE_CXX_COMPILER "clang++")
set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Xclang -load -Xclang ../StackShieldPass/LLVMStackShieldPass.so")

add_executable(simple_test simple_test.cpp)
add_executable(global_test global_test.cpp)
add_executable(pointer_test pointer_test.cpp)
add_executable(pointer2_test pointer2_test.cpp)
add_executable(procedure_test procedure_test.cpp)