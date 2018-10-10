cmake_minimum_required(VERSION 3.10)

project(libminigame)

include(CTest)

set_target_properties(libminigame PROPERTIES
  CXX_STANDARD 11
  CXX_STANDARD_REQUIRED YES
  CXX_EXTENSIONS NO
)

if(BUILD_TESTING)

  add_executable(test_libminigame test/main.cpp)
  if(MSVC)
    target_compile_options(test_libminigame PRIVATE "/W4" "/permissive-")
  endif()
  target_include_directories(test_libminigame PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/third_party/boost-assert/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/third_party/boost-core/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/third_party/boost-config/include"
  )
  add_test(NAME test COMMAND test_libminigame)

endif()
