cmake_minimum_required(VERSION 3.15)
project(js-randomness-predictor-c C)

# Ensure linking against the C++ runtime
enable_language(CXX)

# Needed if Z3 static lib has C++ symbols
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# Set C standard
set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Get compile options based on platform
set(COMPILE_OPTIONS)
if (CMAKE_C_COMPILER_ID MATCHES "Clang" OR CMAKE_C_COMPILER_ID STREQUAL "GNU")
    set(COMPILE_OPTIONS ${CMAKE_C_FLAGS} -O2 -Wunused-variable -Wunused-parameter -Wall -Wextra -Wpedantic)
elseif (CMAKE_C_COMPILER_ID STREQUAL "MSVC")
    set(COMPILE_OPTIONS /W4) # MSVC warning level 4
endif()
# Add compile options
add_compile_options(${COMPILE_OPTIONS})

# Set ${platform}-${arc}
if(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
  set(EXTRA_LIBS c++)
  if(CMAKE_SYSTEM_PROCESSOR STREQUAL "arm64")
    set(Z3_PLATFORM "darwin-arm64")
  else()
    set(Z3_PLATFORM "darwin-x64")
  endif()
elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
  set(EXTRA_LIBS stdc++)
  set(Z3_PLATFORM "linux-x64")
#elseif(CMAKE_SYSTEM_NAME STREQUAL "Windows")
#  set(EXTRA_LIBS "") # MSVC handles this
#  set(Z3_PLATFORM "win32-x64") 
else()
  message(FATAL_ERROR "Unsupported platform: ${CMAKE_SYSTEM_NAME} ${CMAKE_SYSTEM_PROCESSOR}")
endif()

# Set z3 lib file name
#if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
#  set(Z3_LIB_FILE_NAME "libz3.lib")
#else()
  set(Z3_LIB_FILE_NAME "libz3.a")
#endif()

# Get all .c sources
file(GLOB_RECURSE C_SOURCES "${CMAKE_SOURCE_DIR}/src/*.c")

# Add .c sources to executable
add_executable(js-randomness-predictor-c
  ${C_SOURCES}
)

# Include directories for Z3's C API
target_include_directories(js-randomness-predictor-c
  PRIVATE
    third_party/z3/${Z3_PLATFORM}/include
)

# Link to Z3 library
target_link_libraries(js-randomness-predictor-c
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/third_party/z3/${Z3_PLATFORM}/bin/${Z3_LIB_FILE_NAME}
    ${EXTRA_LIBS}
)

message(STATUS "Z3 platform: ${Z3_PLATFORM}")
message(STATUS "Z3 lib: ${Z3_LIB_FILE_NAME}")