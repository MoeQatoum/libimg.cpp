
set(LIB_IMG_INCLUDE_DIR ${CMAKE_CURRENT_LIST_DIR}/include CACHE INTERNAL "libimg include directory")
set(LIB_IMG_SRC_DIR ${CMAKE_CURRENT_LIST_DIR}/src CACHE INTERNAL "libimg source directory")
set(LIB_IMG_LIB_INSTALL_DIR ${CMAKE_INSTALL_PREFIX}/libimg/lib CACHE INTERNAL "libimg lib install directory")
set(LIB_IMG_INCLUDE_INSTALL_DIR ${CMAKE_INSTALL_PREFIX}/libimg/include CACHE INTERNAL "libimg include install directory")

file(GLOB_RECURSE LIB_IMG_SOURCES
    ${LIB_IMG_SRC_DIR}/*.cpp
)

if(LIB_IMG_SHARED)
    add_library(${LIB_IMG} SHARED ${LIB_IMG_SOURCES})
    target_compile_definitions(${LIB_IMG} PRIVATE LIB_IMG_EXPORT)
else()
    add_library(${LIB_IMG} STATIC ${LIB_IMG_SOURCES})
    target_compile_definitions(${LIB_IMG} PUBLIC LIB_IMG_STATIC)
endif()

target_include_directories(${LIB_IMG} PUBLIC ${LIB_IMG_INCLUDE_DIR})

if(CMAKE_BUILD_TYPE STREQUAL RELEASE)
    target_compile_options(${LIB_IMG} PRIVATE
        $<$<COMPILE_LANGUAGE:CXX>: -O3 -Wall -Wextra -Wreorder-ctor>
    )
elseif(CMAKE_BUILD_TYPE STREQUAL DEBUG)
    target_compile_options(${LIB_IMG} PRIVATE
        $<$<COMPILE_LANGUAGE:CXX>: -O0 -g -Wall -Wextra -Wreorder-ctor -Wconversion>
    )
    target_compile_definitions(${LIB_IMG} PRIVATE LIB_IMG_DEBUG_LVL 1)
endif()

# if (LIB_IMG_INSTALL)
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

    if (MSVC AND LIB_IMG_SHARED_LIB)
        install(
            FILES $<TARGET_PDB_FILE:${LIB_IMG}> 
            DESTINATION ${LIB_IMG_LIB_INSTALL_DIR}
            OPTIONAL
        )
    endif()
# endif()
