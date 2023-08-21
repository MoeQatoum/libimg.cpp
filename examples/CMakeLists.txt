cmake_minimum_required(VERSION 3.27)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(LIB_IMG_EXAMPLE_INSTALL_DIR ${CMAKE_INSTALL_PREFIX} CACHE INTERNAL "libimg example install directory")

if(LIB_IMG_SHARED)
    if(UNIX)
        set(CMAKE_INSTALL_RPATH ${LIB_IMG_EXAMPLE_INSTALL_DIR})

        if(${CMAKE_SYSTEM_NAME} STREQUAL Darwin)
            set(CMAKE_MACOSX_RPATH ON)
        endif()

    endif()
endif()

add_executable(${LIB_IMG}-example main.cpp)
target_link_libraries(${LIB_IMG}-example PRIVATE ${LIB_IMG})

if(LIB_IMG_SHARED)
        install(
            TARGETS ${LIB_IMG} 
            DESTINATION ${LIB_IMG_EXAMPLE_INSTALL_DIR}
        )
    if (MSVC)
        install(
            FILES $<TARGET_PDB_FILE:${LIB_IMG}> 
            DESTINATION ${LIB_IMG_EXAMPLE_INSTALL_DIR}
            OPTIONAL
        )
    endif()
endif()

target_compile_options(${LIB_IMG}-example
    PRIVATE $<$<COMPILE_LANGUAGE:CXX>:$<$<CONFIG:RELEASE>: ${LIB_IMG_RELEASE_BUILD_OPTIONS}>>
    PRIVATE $<$<COMPILE_LANGUAGE:CXX>:$<$<CONFIG:DEBUG>:   ${LIB_IMG_DEBUG_BUILD_OPTIONS}>>
)

install(
    TARGETS ${LIB_IMG}-example
    DESTINATION ${LIB_IMG_EXAMPLE_INSTALL_DIR}
)

install(
    DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/test_images/
    DESTINATION ${LIB_IMG_EXAMPLE_INSTALL_DIR}/test_images
    FILES_MATCHING
    PATTERN "img-*.*"
)

install(
    DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/noise/
    DESTINATION ${LIB_IMG_EXAMPLE_INSTALL_DIR}/noise
    FILES_MATCHING
    PATTERN "img-*.*"
)

