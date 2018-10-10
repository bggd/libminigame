cmake_minimum_required(VERSION 3.10)

project(libminigame)

include(CTest)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(BUILD_TESTING)

  add_executable(test_libminigame test/main.cpp)
  if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    target_compile_options(test_libminigame PRIVATE "-pthread")
  elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    target_compile_options(test_libminigame PRIVATE "/W4" "/permissive-")
  endif()
  target_include_directories(test_libminigame PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/third_party/boost-assert/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/third_party/boost-core/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/third_party/boost-config/include"
  )
  add_test(NAME test COMMAND test_libminigame)

endif()
