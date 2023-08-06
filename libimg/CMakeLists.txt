
set(LIB_IMG_INCLUDE_DIR ${CMAKE_CURRENT_LIST_DIR}/include CACHE INTERNAL "libimg include directory")
set(LIB_IMG_SRC_DIR ${CMAKE_CURRENT_LIST_DIR}/src CACHE INTERNAL "libimg source directory")
set(LIB_IMG_LIB_INSTALL_DIR ${CMAKE_INSTALL_PREFIX}/libimg/lib CACHE INTERNAL "libimg lib install directory")
set(LIB_IMG_INCLUDE_INSTALL_DIR ${CMAKE_INSTALL_PREFIX}/libimg/include CACHE INTERNAL "libimg include install directory")
set(LIB_IMG_LIBTYPE CACHE STRING "library type, shared/static")

set(LIB_IMG_RELEASE_OPTIONS CACHE STRING "-O3")
set(LIB_IMG_DEBUG_OPTIONS CACHE STRING "-O0 -g -Wall -Wextra -Wreorder-ctor -Wconversion -Wpadded")

set_property(CACHE LIB_IMG_LIBTYPE PROPERTY STRINGS $<IF:$<BOOL:LIB_IMG_SHARED>,SHARED,STATIC>)


file(GLOB_RECURSE LIB_IMG_SOURCES
    ${LIB_IMG_SRC_DIR}/*.cpp
)

add_library(${LIB_IMG} ${LIB_IMG_LIBTYPE} ${LIB_IMG_SOURCES})

target_link_libraries(${LIB_IMG} PRIVATE ${LIB_STB_IMG})

target_include_directories(${LIB_IMG}
    PUBLIC  ${LIB_IMG_INCLUDE_DIR}
    PRIVATE ${CMAKE_CURRENT_LIST_DIR}/private_headers
)

target_compile_options(${LIB_IMG}
    PRIVATE   $<$<COMPILE_LANGUAGE:CXX>:$<$<CONFIG:RELEASE>: ${LIB_IMG_RELEASE_OPTIONS}>>
    PRIVATE   $<$<COMPILE_LANGUAGE:CXX>:$<$<CONFIG:DEBUG>:   ${LIB_IMG_DEBUG_OPTIONS}>>
)

target_compile_definitions(${LIB_IMG} 
    PRIVATE $<$<CONFIG:DEBUG>: LIB_IMG_DEBUG>
    PRIVATE $<$<BOOL:${LIB_IMG_SHARED}>: LIB_IMG_EXPORT>
    PUBLIC  $<IF:$<BOOL:${LIB_IMG_SHARED}>,,LIB_IMG_STATIC>
)

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
