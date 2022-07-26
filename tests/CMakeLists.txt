# determine if ASAN and LSAN are available
# Windows compiler
if(MSVC)
  # add ASAN flag https://docs.microsoft.com/en-us/cpp/sanitizers/asan
  set(ASAN_COMPILE_OPTION /fsanitize=address)
  set(ASAN_LINK_OPTION)
  set(ASAN_AVAIL True)
  set(LSAN_AVAIL False)

# macOS
elseif("${CMAKE_SYSTEM_NAME}" STREQUAL "Darwin")
  # assume Homebrew Clang and has ASAN and LSAN
  if(("${CMAKE_C_COMPILER_ID}" STREQUAL "Clang"))
    set(ASAN_COMPILE_OPTION -fsanitize=address)
    set(ASAN_LINK_OPTION -fsanitize=address)
    set(ASAN_AVAIL True)
    set(LSAN_AVAIL True)

  # Apple Clang has ASAN but not LSAN
  elseif(("${CMAKE_C_COMPILER_ID}" STREQUAL "AppleClang"))
    set(ASAN_COMPILE_OPTION -fsanitize=address)
    set(ASAN_LINK_OPTION -fsanitize=address)
    set(ASAN_AVAIL True)
    set(LSAN_AVAIL False)

  # assume anything else on macOS has neither
  else()
    set(ASAN_AVAIL False)
    set(LSAN_AVAIL False)
  endif()

# Linux
elseif("${CMAKE_SYSTEM_NAME}" STREQUAL "Linux")
  set(ASAN_COMPILE_OPTION -fsanitize=address)
  set(ASAN_LINK_OPTION -fsanitize=address)
  set(ASAN_AVAIL True)
  set(LSAN_AVAIL True)

# unknown, assume no ASAN or LSAN
else()
  set(ASAN_AVAIL False)
  set(LSAN_AVAIL False)
endif()

# set flags if Asans build type and ASAN and LSAN are available
if("${CMAKE_BUILD_TYPE}" STREQUAL "Asan" AND ASAN_AVAIL)
  set(USING_ASAN True)
  message(STATUS "Using ASAN")
  add_compile_options(${ASAN_COMPILE_OPTION})
  add_link_options(${ASAN_LINK_OPTION})

  if(LSAN_AVAIL)
    set(USING_LSAN True)
    message(STATUS "Using LSAN")
  endif()
endif()

macro(add_gmat_test basename)
  add_gmat_exe(${basename} ${basename}.c)

  if(USING_ASAN)
    add_test(NAME ${basename} COMMAND ${basename})

    # detect leaks when using LSAN
    if(USING_LSAN)
      set_tests_properties(${basename} PROPERTIES ENVIRONMENT
        "ASAN_OPTIONS=detect_leaks=1:abort_on_error=0;LSAN_OPTIONS=suppressions=${CMAKE_CURRENT_SOURCE_DIR}/lsan.supp")
    else()
      set_tests_properties(${basename} PROPERTIES ENVIRONMENT
        "ASAN_OPTIONS=abort_on_error=0")
    endif()

  else()
    add_test(NAME ${basename} COMMAND ${basename})
  endif()
endmacro()

add_gmat_test(test_new)