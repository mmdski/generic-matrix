set(LSAN_AVAIL False)

# macOS - Homebrew Clang has LSAN
# XCode Clang is "AppleClang" and it does not have LSAN
if("${CMAKE_SYSTEM_NAME}" STREQUAL "Darwin" AND("${CMAKE_C_COMPILER_ID}" STREQUAL "Clang"))
  set(LSAN_AVAIL True)

# Linux - GCC has LSAN
elseif("${CMAKE_SYSTEM_NAME}" STREQUAL "Linux")
  set(LSAN_AVAIL True)
endif()

# set environment varaiables if Asan build type
if("${CMAKE_BUILD_TYPE}" STREQUAL "Asan")
  set(ASAN_ENV "ASAN_OPTIONS=abort_on_error=0")

  # detect leaks when using LSAN
  if(LSAN_AVAIL)
    string(CONCAT ASAN_ENV ${ASAN_ENV}
      ":detect_leaks=1\;"
      "LSAN_OPTIONS=suppressions=${CMAKE_CURRENT_SOURCE_DIR}/lsan.supp")
  endif()
endif()

macro(add_gmat_test basename)
  add_executable(${basename} ${basename}.cc)
  target_link_libraries(${basename} GTest::gtest GTest::gtest_main)
  gtest_add_tests(TARGET ${basename}
    TEST_LIST ${basename}-tests)

  # set environment varaiables if Asan build type
  if("${CMAKE_BUILD_TYPE}" STREQUAL "Asan")
    set(ASAN_ENV "ASAN_OPTIONS=abort_on_error=0")

    # detect leaks when using LSAN
    if(LSAN_AVAIL)
      string(CONCAT ASAN_ENV ${ASAN_ENV}
        ":detect_leaks=1\;"
        "LSAN_OPTIONS=suppressions=${CMAKE_CURRENT_SOURCE_DIR}/lsan.supp")
    endif()

    set_tests_properties(${${basename}-tests} PROPERTIES ENVIRONMENT ${ASAN_ENV})
  endif()
endmacro()

add_gmat_test(matrix_double_test)
