
set(LIB_IMG_INCLUDE_DIR         ${CMAKE_CURRENT_LIST_DIR}/include       CACHE INTERNAL "libimg include directory")
set(LIB_IMG_SRC_DIR             ${CMAKE_CURRENT_LIST_DIR}/src           CACHE INTERNAL "libimg source directory")
set(LIB_IMG_LIB_INSTALL_DIR     ${CMAKE_INSTALL_PREFIX}/libimg/lib      CACHE INTERNAL "libimg lib install directory")
set(LIB_IMG_INCLUDE_INSTALL_DIR ${CMAKE_INSTALL_PREFIX}/libimg/include  CACHE INTERNAL "libimg include install directory")

# set(LIB_IMG_TCMALLOC_FLAGS       -ltcmalloc_minimal -fno-builtin-malloc -fno-builtin-calloc -fno-builtin-realloc -fno-builtin-free)
set(LIB_IMG_RELEASE_BUILD_OPTIONS -O3 -Wall -Wextra -Wreorder-ctor -Wpadded -Wpedantic -Wdouble-promotion)
set(LIB_IMG_DEBUG_BUILD_OPTIONS   -O0 -ggdb -Wall -Wextra -Wreorder-ctor -Wconversion -Wpadded -Wpedantic -Wdouble-promotion -Wswitch-enum)

file(GLOB_RECURSE LIB_IMG_SOURCES
    ${LIB_IMG_SRC_DIR}/*.cpp
)

if(LIB_IMG_SHARED)
    set(_type SHARED)
else()
    set(_type STATIC)
endif()

add_library(${LIB_IMG} ${_type} ${LIB_IMG_SOURCES})

target_link_libraries(${LIB_IMG} PRIVATE ${LIB_STB_IMG})

target_include_directories(${LIB_IMG}
    PUBLIC  ${LIB_IMG_INCLUDE_DIR}
)

target_compile_options(${LIB_IMG}
    PRIVATE $<$<COMPILE_LANGUAGE:CXX>:$<$<CONFIG:RELEASE>: ${LIB_IMG_RELEASE_BUILD_OPTIONS} ${LIB_IMG_TCMALLOC_FLAGS}>>
    PRIVATE $<$<COMPILE_LANGUAGE:CXX>:$<$<CONFIG:DEBUG>:   ${LIB_IMG_DEBUG_BUILD_OPTIONS}   ${LIB_IMG_TCMALLOC_FLAGS}>>
)

target_compile_definitions(${LIB_IMG} 
    PRIVATE $<$<CONFIG:DEBUG>: LIB_IMG_DEBUG>
    PRIVATE $<$<BOOL:${LIB_IMG_SHARED}>: LIB_IMG_EXPORT>
    PUBLIC  $<IF:$<BOOL:${LIB_IMG_SHARED}>,,LIB_IMG_STATIC>
)

#if windows
# set_target_properties(${LIB_IMG} 
#     PROPERTIES OUTPUT_NAME $<IF:$<BOOL:${LIB_IMG_SHARED}>,,${LIB_IMG}_static>
# )


if (LIB_IMG_INSTALL)
    install(
        TARGETS ${LIB_IMG}
        DESTINATION ${LIB_IMG_LIB_INSTALL_DIR}
    )

    install(
        DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/include/
        DESTINATION ${LIB_IMG_INCLUDE_INSTALL_DIR}
        FILES_MATCHING 
        PATTERN "*.hpp"
        PATTERN "*.h"   
    )

    if (MSVC)
        install(
            FILES $<TARGET_PDB_FILE:${LIB_IMG}> 
            DESTINATION ${LIB_IMG_LIB_INSTALL_DIR}
            OPTIONAL
        )
    endif()
endif()
