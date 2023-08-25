cmake_minimum_required(VERSION 3.27)

set(LIB_IMG img CACHE INTERNAL "project name")

set(CMAKE_CXX_FLAGS_RELEASE "")
set(CMAKE_CXX_FLAGS_DEBUG   "")

project(
    ${LIB_IMG}
    VERSION 0.1.0
    DESCRIPTION "Image Processing Library in C++"
    LANGUAGES CXX
)

# set(LIB_IMG_TCMALLOC_FLAGS       -ltcmalloc_minimal -fno-builtin-malloc -fno-builtin-calloc -fno-builtin-realloc -fno-builtin-free)
set(LIB_IMG_RELEASE_BUILD_OPTIONS -O3 -DNDEBUG -Wall -Wextra -Wreorder-ctor -Wpadded -Wpedantic -Wdouble-promotion)
set(LIB_IMG_DEBUG_BUILD_OPTIONS   -O0 -ggdb3 -Wall -Wextra -Wreorder-ctor -Wconversion -Wpadded -Wpedantic -Wdouble-promotion -Wswitch-enum)

if(UNIX)
    if ("${CMAKE_BUILD_TYPE}" STREQUAL "")
        set(CMAKE_BUILD_TYPE RELEASE)
    else()
        string(TOUPPER ${CMAKE_BUILD_TYPE} CMAKE_BUILD_TYPE)
    endif()
endif()

add_subdirectory(third_party)

add_subdirectory(libimg)

if(LIB_IMG_EXAMPLES)
    add_subdirectory(examples)
endif()
