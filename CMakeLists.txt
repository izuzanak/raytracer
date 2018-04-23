
cmake_minimum_required(VERSION 3.1.0)

project(raytracer)

enable_testing()

set(GCC_COMPILE_FLAGS "-O3 -ffast-math -Wall -Wfatal-errors")
set(CMAKE_CXX_FLAGS ${GCC_COMPILE_FLAGS})
set(CMAKE_EXE_LINKER_FLAGS ${GCC_COMPILE_FLAGS})

#set(CMAKE_BUILD_TYPE Debug)
#set(CMAKE_BUILD_TYPE Release)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(RENDER_LIB_MODE STATIC) # [STATIC | SHARED | MODULE]

add_definitions("'-DCMAKE_BUILD_DIR=\"${CMAKE_CURRENT_BINARY_DIR}\"'")

# - default cclib directory -
if (NOT CCLIB_DIR)
  set(CCLIB_DIR "../cclib")
endif()

include_directories(render/include ${CCLIB_DIR}/lib)
link_directories(${CCLIB_DIR}/build)

add_subdirectory(render)
add_subdirectory(tests)
add_subdirectory(rt_demo)

