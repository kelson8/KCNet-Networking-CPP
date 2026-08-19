cmake_minimum_required(VERSION 3.16)
project(base64)

# Taken from here
# https://github.com/tobiaslocker/base64/blob/master/CMakeLists.txt
# I have stripped out the tests and other stuff for this.

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE RelWithDebInfo)
endif()

# Warnings compiler flags
if(MSVC)
  add_compile_options(/W4)
else()
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# Optionally enable clang-tidy
find_program(CLANG_TIDY_EXE NAMES clang-tidy PATHS /opt/homebrew/opt/llvm/bin/)
if(NOT CLANG_TIDY_EXE)
  message(STATUS "clang-tidy not found. Skipping corresponding checks.")
else()
  set(CMAKE_CXX_CLANG_TIDY 
    ${CLANG_TIDY_EXE};
    -header-filter=.*;
    -checks=-*,portability-*,bugprone-*,-bugprone-easily-swappable-parameters,-bugprone-sizeof-expression,clang-analyzer-*;
  )
  message(STATUS "Found clang-tidy: ${CLANG_TIDY_EXE}.")
endif()

# Simplify the use of ASan
option(USE_ASAN "Activate ASan compiler/linker options" OFF)
if(USE_ASAN)
  if(MSVC)
    add_compile_options(/fsanitize=address)
    add_link_options(/fsanitize=address)
    set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
  else()
    add_compile_options(-fsanitize=address -fno-omit-frame-pointer)
    add_link_options(-fsanitize=address)
  endif()
endif()

add_library(base64 INTERFACE)
target_include_directories(base64 INTERFACE include)
