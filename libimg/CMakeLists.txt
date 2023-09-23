set(LIB_IMG_INCLUDE_DIR         ${CMAKE_CURRENT_LIST_DIR}/include       CACHE INTERNAL "libimg include directory")
set(LIB_IMG_SRC_DIR             ${CMAKE_CURRENT_LIST_DIR}/src           CACHE INTERNAL "libimg source directory")
set(LIB_IMG_LIB_INSTALL_DIR     ${CMAKE_INSTALL_PREFIX}/libimg/lib      CACHE INTERNAL "libimg lib install directory")
set(LIB_IMG_INCLUDE_INSTALL_DIR ${CMAKE_INSTALL_PREFIX}/libimg/include  CACHE INTERNAL "libimg include install directory")

set(LIB_IMG_TCMALLOC_FLAGS        -fno-builtin-malloc -fno-builtin-calloc -fno-builtin-realloc -fno-builtin-free)
set(LIB_IMG_TCMALLOC_LINKED_FLAGS -ltcmalloc_minimal)

set(STD_LIB_CPP_FLAGS        -stdlib=libc++)
set(STD_LIB_CXX_LINKER_FLAGS -lc++ -nostdlib++)

set(LIB_IMG_REL_BUILD_FLAGS -O3 -DNDEBUG)
set(LIB_IMG_REL_WARN_FLAGS  -Wall -Wextra -Wreorder-ctor -Wpedantic -Wdouble-promotion)

set(LIB_IMG_DBG_BUILD_FLAGS -O0 -ggdb3)
set(LIB_IMG_DBG_WARN_FLAGS  -Wall -Wextra -Wreorder-ctor -Wconversion -Wpedantic -Wdouble-promotion -Wswitch-enum)


add_library(${LIB_IMG} INTERFACE)

target_link_libraries(${LIB_IMG} INTERFACE ${LIB_STB_IMG})

target_include_directories(${LIB_IMG}
    INTERFACE ${LIB_IMG_INCLUDE_DIR}
)

target_compile_options(${LIB_IMG} INTERFACE
    $<$<COMPILE_LANGUAGE:CXX>:$<$<CONFIG:RELEASE>: ${LIB_IMG_REL_BUILD_FLAGS} ${STD_LIB_CPP_FLAGS} ${LIB_IMG_REL_WARN_FLAGS} $<$<BOOL:${LIB_IMG_USE_TCMALLOC}>: ${LIB_IMG_TCMALLOC_FLAGS}>>>
    $<$<COMPILE_LANGUAGE:CXX>:$<$<CONFIG:DEBUG>:   ${LIB_IMG_DBG_BUILD_FLAGS} ${STD_LIB_CPP_FLAGS} ${LIB_IMG_DBG_WARN_FLAGS} $<$<BOOL:${LIB_IMG_USE_TCMALLOC}>: ${LIB_IMG_TCMALLOC_FLAGS}>>>
)

target_link_options(${LIB_IMG} INTERFACE
    $<$<COMPILE_LANGUAGE:CXX>: ${STD_LIB_CXX_LINKER_FLAGS} $<$<BOOL:${LIB_IMG_USE_TCMALLOC}>: ${LIB_IMG_TCMALLOC_LINKED_FLAGS}>>
)

target_compile_definitions(${LIB_IMG} INTERFACE
    $<$<CONFIG:DEBUG>: LIB_IMG_DEBUG>
    $<$<BOOL:${LIB_IMG_SHARED}>: LIB_IMG_EXPORT>
    $<IF:$<BOOL:${LIB_IMG_SHARED}>,,LIB_IMG_STATIC>
)

set_target_properties(${LIB_IMG} PROPERTIES
    CXX_STANDARD          23
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS        OFF
)

if (LIB_IMG_INSTALL)
    install(
        DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/include/
        DESTINATION ${LIB_IMG_INCLUDE_INSTALL_DIR}
        FILES_MATCHING 
        PATTERN "*.hpp"
        PATTERN "*.h"   
    )
endif()
