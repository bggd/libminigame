cmake_minimum_required(VERSION 3.10)

project(libminigame C CXX)

include(CTest)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

    
if(BUILD_TESTING)

  add_library(stb_image src/third_party/stb_image.c)
  add_library(stb_vorbis src/third_party/stb_vorbis.c)

  if (CMAKE_CXX_COMPILER_ID STREQUAL GNU)
    find_package(Threads REQUIRED)
  endif()

  add_executable(test_libminigame test/main.cpp)
  target_compile_options(test_libminigame PRIVATE
    $<$<CXX_COMPILER_ID:GNU>: -pedantic -Wall -Wextra -Werror>
    $<$<CXX_COMPILER_ID:MSVC>: /permissive- /W4 /WX>
  )
  target_link_libraries(test_libminigame PRIVATE
    "$<$<CXX_COMPILER_ID:GNU>: Threads::Threads>"
    stb_image stb_vorbis
  )
  target_include_directories(test_libminigame PRIVATE
    third_party/boost-assert/include
    third_party/boost-core/include
    third_party/boost-config/include
  )
add_test(NAME test COMMAND test_libminigame WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/test_data)

endif()
