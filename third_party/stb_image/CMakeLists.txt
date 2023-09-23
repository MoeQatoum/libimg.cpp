cmake_minimum_required(VERSION 3.27)

set(LIB_STB_IMG stb_img CACHE INTERNAL "stb project name")
set(LIB_STB_IMG_INCLUDE_DIR ${CMAKE_CURRENT_LIST_DIR} CACHE INTERNAL "stb_img include directory")

set(STB_IMG_TCMALLOC_FLAGS        -fno-builtin-malloc -fno-builtin-calloc -fno-builtin-realloc -fno-builtin-free)
set(STB_IMG_TCMALLOC_LINKED_FLAGS -ltcmalloc_minimal)

set(STD_LIB_CPP_FLAGS        -stdlib=libc++)
set(STD_LIB_CXX_LINKER_FLAGS -lc++ -nostdlib++)

set(STB_IMG_REL_BUILD_FLAGS -O3 -DNDEBUG)
set(STB_IMG_DBG_BUILD_FLAGS -O0 -ggdb3)

# set(LIB_STB_IMG_PIC ${LIB_IMG_SHARED})
set(LIB_STB_IMG_PIC OFF)

project(
    ${LIB_STB_IMG}
    LANGUAGES CXX
)

file(GLOB_RECURSE LIB_STB_IMG_SOURCES
    ${LIB_STB_IMG_INCLUDE_DIR}/*.cpp
)

add_library(${LIB_STB_IMG} STATIC ${LIB_STB_IMG_SOURCES})

set_target_properties(${LIB_STB_IMG} PROPERTIES
    POSITION_INDEPENDENT_CODE ${LIB_STB_IMG_PIC}
    CXX_STANDARD              23
    CXX_STANDARD_REQUIRED     ON
    CXX_EXTENSIONS            OFF
)

target_include_directories(${LIB_STB_IMG} PUBLIC ${LIB_STB_IMG_INCLUDE_DIR})

target_compile_options(${LIB_STB_IMG} PRIVATE
    $<$<COMPILE_LANGUAGE:CXX>:$<$<CONFIG:RELEASE>: ${STB_IMG_REL_BUILD_FLAGS} ${STD_LIB_CPP_FLAGS} $<$<BOOL:${LIB_IMG_USE_TCMALLOC}>: ${STB_IMG_TCMALLOC_FLAGS}>>>
    $<$<COMPILE_LANGUAGE:CXX>:$<$<CONFIG:DEBUG>:   ${STB_IMG_DBG_BUILD_FLAGS} ${STD_LIB_CPP_FLAGS} $<$<BOOL:${LIB_IMG_USE_TCMALLOC}>: ${STB_IMG_TCMALLOC_FLAGS}>>>
)

target_link_options(${LIB_STB_IMG} PRIVATE
    $<$<COMPILE_LANGUAGE:CXX>: ${STD_LIB_CXX_LINKER_FLAGS} $<$<BOOL:${LIB_IMG_USE_TCMALLOC}>: ${STB_IMG_TCMALLOC_LINKED_FLAGS}>>
)