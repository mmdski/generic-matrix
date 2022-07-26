cmake_minimum_required(VERSION 3.14.0)
project(gmatrix VERSION 0.1.0)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED True)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

if(MSVC)
  add_compile_options(/guard:cf /analyze /permissive- /W4 /WX)

  # add debug and ASAN flags for ASAN build
  # https://docs.microsoft.com/en-us/cpp/sanitizers/asan
  if("${CMAKE_BUILD_TYPE}" STREQUAL "Asan")
    add_compile_options(/Zi /fsanitize=address)
  endif()

else()
  add_compile_options(-Wall -Wextra -pedantic -Werror)

  # add debug and ASAN flags for ASAN build
  if("${CMAKE_BUILD_TYPE}" STREQUAL "Asan")
    add_compile_options(-g -fsanitize=address)
    add_link_options(-fsanitize=address)
  endif()
endif()

include_directories(include)

# build the files in the app subdirectory
add_subdirectory(app)

if(TESTS)
  message(STATUS "Building tests")
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
endif()
