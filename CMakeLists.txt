cmake_minimum_required(VERSION 3.14.0)
project(gmatrix VERSION 0.1.0)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED True)

set(CMAKE_CXX_STANDARD 14)

if(MSVC)
  add_compile_options(/guard:cf /analyze /permissive- /W4 /WX)

  # add debug flags for ASAN build
  if("${CMAKE_BUILD_TYPE}" STREQUAL "Asan")
    add_compile_options(/Zi)
  endif()

  macro(link_math target)
  endmacro()

else()
  add_compile_options(-Wall -Wextra -pedantic -Werror)

  # add debug flags for ASAN build
  if("${CMAKE_BUILD_TYPE}" STREQUAL "Asan")
    add_compile_options(-g3)
  endif()

  macro(link_math target)
    target_link_libraries(${target} m)
  endmacro()
endif()

include_directories(include)

add_subdirectory(src)

macro(add_gmat_exe basename)
  add_executable(${basename} ${basename}.c)
  target_link_libraries(${basename} PRIVATE gmatrix)
endmacro()

add_subdirectory(app)

include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG release-1.12.1
)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()
include(GoogleTest)

add_subdirectory(tests)
