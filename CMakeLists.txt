cmake_minimum_required(VERSION 3.27)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# if(UNIX)
#     if ("${CMAKE_BUILD_TYPE}" STREQUAL "")
#         set(CMAKE_BUILD_TYPE RELEASE)
#     else()
#         string(TOUPPER ${CMAKE_BUILD_TYPE} CMAKE_BUILD_TYPE)
#     endif()
# endif()

# project(
#     libimg
#     VERSION 0.1.0
#     DESCRIPTION "Image Processing Library in C++"
#     LANGUAGES CXX
# )

add_subdirectory(libimg)

if(LIB_IMG_EXAMPLES)
    add_subdirectory(examples)
endif()
