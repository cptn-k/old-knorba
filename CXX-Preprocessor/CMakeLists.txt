cmake_minimum_required (VERSION 2.6)
project (KnoRBA-CXX-Preprocessor)

include ("../Common/cmake/setup-systemwide-include-dir.cmake")

set (LLVM_LINK_COMPONENTS support)
set (LLVM_USED_LIBS clangFormat clangTooling clangBasic clangAST
                    clangLex clangSema clangParse clangRewriteCore)

set (LLVM_BINARY_DIR "${KSYSTEM_DIR}/llvm")

include_directories (
  "${CMAKE_SOURCE_DIR}/../Third-Party/llvm/include"
  "${CMAKE_SOURCE_DIR}/../Third-Party/llvm/tools/clang/include"
  "${LLVM_BINARY_DIR}/include")

add_subdirectory (../Third-Party/llvm ${LLVM_BINARY_DIR})
add_subdirectory (../CPP-Lib "${KSYSTEM_DIR}/CPP-Lib")
add_subdirectory (../Common  "${KSYSTEM_DIR}/Common")

set(CMAKE_CXX_FLAGS "-fno-rtti -D__STDC_LIMIT_MACROS -D__STDC_CONSTANT_MACROS -D__STDC_FORMAT_MACROS -fcolor-diagnostics")

find_path(STD_INCLUDE_DIR stdio.h)
set(STD_EXT_INCLUDE_DIR 
  "/usr/llvm-gcc-4.2/lib/gcc/i686-apple-darwin11/4.2.1/include")

set (BIN_DIR_NAME bin)
set (KPREP_EXE_NAME kprep)
set (KPREP_TARGET_NAME "${BIN_DIR_NAME}/${KPREP_EXE_NAME}") 
set (AST_TEST_EXE_NAME ast-test)
set (AST_TEST_TARGET_NAME "${BIN_DIR_NAME}/${AST_TEST_EXE_NAME}")

configure_file("${CMAKE_SOURCE_DIR}/src/FixedSettings.h.in"
               "${CMAKE_SOURCE_DIR}/src/FixedSettings.h")

#add_executable (bin/test src/test.cpp)

#target_link_libraries(bin/test
#  clangBasic
#  clangLex)

add_executable (${KPREP_TARGET_NAME}
  src/ClangProxy.cpp
  src/Preprocessor.cpp
  src/PragmaParticle.cpp
  src/SourceRangeEx.cpp
  src/SynchronizedBlock.cpp)

target_link_libraries (${KPREP_TARGET_NAME}
  clangBasic
  clangLex
  clangAST
  clangSema
  clangParse
  clangFrontend
  clangRewriteCore
  org-knorba-common)

add_executable (${AST_TEST_TARGET_NAME} 
  src/ast-test.cpp)

target_link_libraries (${AST_TEST_TARGET_NAME}
  clangBasic
  clangLex
  clangAST
  clangSema
  clangParse
  clangFrontend
  clangTooling)
  
