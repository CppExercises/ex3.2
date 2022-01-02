cmake_minimum_required(VERSION 3.0)

# export compile_commands.json for use with clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS true CACHE BOOL "")
set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Choose the type of build.")

project(Ex32 LANGUAGES CXX)

#set the required CPP standard for the project
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

message(STATUS "-----> Current build type set to: ${CMAKE_BUILD_TYPE}")

# options for clang and gnu
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang" OR "${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
  # strict compilation
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror -Wall -pedantic")
  # enable ASAN
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=address")
  set(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUGS} -fno-omit-frame-pointer -fsanitize=address")
  if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    message(STATUS "-----> Congratulations, you are using the address sanitizer")
  else()
    message(STATUS "-----> Address sanitizer is only turned on for the build type: Debug")
  endif()
else()
message(STATUS "-----> WARNING: ADDRESS SANITIZER IS NOT AVAILABLE")
endif()

# Define our library name
set(MY_LIBRARY "myUniquePtr")

set(MY_LIBRARY_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/include")

# build the library
message(STATUS "Building header-only library")
add_library(${MY_LIBRARY} INTERFACE)
target_include_directories(${MY_LIBRARY} INTERFACE "${MY_LIBRARY_INCLUDE_DIRS}")

# by default tests are built, but this can be turned off using: cmake -DBUILD_TESTS=OFF ..
option(BUILD_TESTS "Build unit tests." ON)
if(BUILD_TESTS)
  # Activate CTest testing framework
  enable_testing()
  # Add all tests in folder "tests" to the testing
  add_subdirectory("tests")
endif()

