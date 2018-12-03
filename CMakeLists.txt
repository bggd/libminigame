cmake_minimum_required(VERSION 3.10)

project(libminigame C CXX)

include(CTest)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

    
if(BUILD_TESTING)

  add_library(stb_image OBJECT src/third_party/stb_image.c)
  add_library(stb_vorbis OBJECT src/third_party/stb_vorbis.c)
  add_library(third_party_glfw STATIC src/third_party/glfw.c)
  add_library(gtest STATIC third_party/googletest/googletest/src/gtest-all.cc)
  target_include_directories(gtest PRIVATE third_party/googletest/googletest/include third_party/googletest/googletest)

  set(Libraries stb_image stb_vorbis third_party_glfw gtest)
  if (CMAKE_CXX_COMPILER_ID STREQUAL GNU)
    find_package(Threads REQUIRED)
    set(Libraries ${Libraries} Threads::Threads)
    set(Libraries ${Libraries} glfw)
  endif()
  find_package(OpenAL REQUIRED)
  set(Libraries ${Libraries} ${OPENAL_LIBRARY})

  add_executable(test_libminigame test/main.cpp)
  target_compile_options(test_libminigame PRIVATE
    $<$<CXX_COMPILER_ID:GNU>: -pedantic -Wall -Wextra -Werror>
    $<$<CXX_COMPILER_ID:MSVC>: /permissive- /W4 /WX>
  )
  target_include_directories(test_libminigame PRIVATE
    third_party/googletest/googletest/include
    ${OPENAL_INCLUDE_DIR}
  )
  target_link_libraries(test_libminigame PRIVATE ${Libraries})
  add_test(NAME test COMMAND test_libminigame WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/test_data)

endif()
