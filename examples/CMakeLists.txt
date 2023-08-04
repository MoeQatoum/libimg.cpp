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
            FILES $<TARGET_PDB_FILE:libimg> 
            DESTINATION ${LIB_IMG_EXAMPLE_INSTALL_DIR}
            OPTIONAL
        )
    endif()
endif()

if(CMAKE_BUILD_TYPE STREQUAL RELEASE)
    target_compile_options(${LIB_IMG}-example PRIVATE
        $<$<COMPILE_LANGUAGE:CXX>: -O3 -Wall -Wextra -Wreorder-ctor>
    )
elseif(CMAKE_BUILD_TYPE STREQUAL DEBUG)
    target_compile_options(${LIB_IMG}-example PRIVATE
        $<$<COMPILE_LANGUAGE:CXX>: -O0 -g -Wall -Wextra -Wreorder-ctor -Wconversion>
    )
endif()

install(
    TARGETS ${LIB_IMG}-example
    DESTINATION ${LIB_IMG_EXAMPLE_INSTALL_DIR}
)
