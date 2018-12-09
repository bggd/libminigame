cmake_minimum_required(VERSION 3.10)

project(libminigame C CXX)

include(CTest)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(Libraries)

if (NOT WIN32 AND CMAKE_CXX_COMPILER_ID STREQUAL GNU)
  find_package(Threads REQUIRED)
  set(Libraries ${Libraries} Threads::Threads)

  set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
  set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
  set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
  set(GLFW_USE_OSMESA ON CACHE BOOL "" FORCE)
  add_subdirectory(third_party/glfw)
  set(Libraries ${Libraries} glfw)
endif()

find_package(OpenAL REQUIRED)
set(Libraries ${Libraries} ${OPENAL_LIBRARY})

add_library(minigame SHARED 
  src/minigame.cpp
  src/third_party/stb_image.c
  src/third_party/stb_vorbis.c
  src/third_party/glfw.c
)
target_include_directories(minigame PRIVATE ${OPENAL_INCLUDE_DIR})
target_link_libraries(minigame PRIVATE ${OPENAL_LIBRARY})
target_compile_options(minigame PRIVATE
  $<$<CXX_COMPILER_ID:GNU>: -pedantic -Wall -Wextra -Werror>
  $<$<CXX_COMPILER_ID:MSVC>: /permissive- /W4 /WX>
)

   
if(BUILD_TESTING)

  add_library(third_party OBJECT
    src/third_party/stb_image.c
    src/third_party/stb_vorbis.c
    src/third_party/glfw.c
  )

  add_library(gtest STATIC third_party/googletest/googletest/src/gtest-all.cc)
  target_include_directories(gtest PRIVATE third_party/googletest/googletest/include third_party/googletest/googletest)

  set(Libraries ${Libraries} gtest)

  add_executable(test_libminigame $<TARGET_OBJECTS:third_party> test/main.cpp)
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

  add_executable(test_libminigame_capi test/c_api.cpp)
  target_compile_options(test_libminigame_capi PRIVATE
    $<$<CXX_COMPILER_ID:GNU>: -pedantic -Wall -Wextra -Werror>
    $<$<CXX_COMPILER_ID:MSVC>: /permissive- /W4 /WX>
  )
  target_include_directories(test_libminigame_capi PRIVATE
    third_party/googletest/googletest/include
    ${OPENAL_INCLUDE_DIR}
  )
target_link_libraries(test_libminigame_capi PRIVATE minigame ${Libraries})
  add_test(NAME test_capi COMMAND test_libminigame_capi WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/test_data)


endif()
