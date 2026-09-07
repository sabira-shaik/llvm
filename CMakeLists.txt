cmake_minimum_required(VERSION 3.20)

project(MyPass)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(LLVM REQUIRED CONFIG)

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")

include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})

add_library(MyPass MODULE MyPass.cpp)

set_target_properties(MyPass PROPERTIES
    PREFIX ""
    WINDOWS_EXPORT_ALL_SYMBOLS ON
)

llvm_map_components_to_libnames(LLVM_LIBS
    Core
    Support
    Passes
    Analysis
    TransformUtils
)

target_link_libraries(MyPass PRIVATE ${LLVM_LIBS})