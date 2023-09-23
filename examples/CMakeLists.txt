cmake_minimum_required(VERSION 3.27)

set(LIB_IMG_EXAMPLE             ${LIB_IMG}-example    CACHE INTERNAL "main")
set(LIB_IMG_EXAMPLE_INSTALL_DIR ${CMAKE_INSTALL_PREFIX} CACHE INTERNAL "libimg example install directory")

add_executable(${LIB_IMG_EXAMPLE} main.cpp)

target_include_directories(${LIB_IMG_EXAMPLE} PRIVATE ${CMAKE_CURRENT_LIST_DIR}/include)

target_link_libraries(${LIB_IMG_EXAMPLE} PRIVATE ${LIB_IMG})

set_target_properties(
    ${LIB_IMG_EXAMPLE} PROPERTIES
    CXX_STANDARD          23
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS        OFF
)

install(
    TARGETS ${LIB_IMG_EXAMPLE}
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
