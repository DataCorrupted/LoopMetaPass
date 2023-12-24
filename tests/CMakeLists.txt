set (CMAKE_CXX_STANDARD 14)

add_compile_options(-g -O0 -emit-llvm)

add_library(For OBJECT for.cpp)
add_library(While OBJECT while.cpp)

add_library(Multi OBJECT multi-loops.cpp)
add_library(Nested OBJECT nested-loops.cpp)

add_library(CFG OBJECT cfg-in-loop.cpp)

add_library(Continue OBJECT continue.cpp)
add_library(Break OBJECT break.cpp)

# Does goto loop count as a loop?
add_library(Goto OBJECT goto.cpp)
