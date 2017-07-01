set(CMAKE_CXX_COMPILER "clang++")
set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Xclang -load -Xclang ../StackGuardPass/LLVMStackGuardPass.so")

add_executable(simple_test simple_test.cpp)
add_executable(conditional_test conditional_test.cpp)
add_executable(global_test global_test.cpp)
add_executable(pointer_test pointer_test.cpp)
add_executable(pointer2_test pointer2_test.cpp)
add_executable(procedure_test procedure_test.cpp)