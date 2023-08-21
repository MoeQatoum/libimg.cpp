#ifndef LIB_IMG_COMMON_H
#define LIB_IMG_COMMON_H

#include <limits.h>

#ifndef LIB_IMG_MAX_SIZE
    #define LIB_IMG_MAX_SIZE UINT_MAX
#else
static_assert(LIB_IMG_MAX_SIZE <= UINT_MAX, "`LIB_IMG_MAX_SIZE` can't be grater than `UINT_MAX`");
#endif

#if defined(_WIN32) | defined(_WIN64) | defined(__WIN32__) | defined(__WINDOWS__)
    #ifdef LIB_IMG_EXPORT
        #define LIB_IMG_PUBLIC __declspec(dllexport)
    #else
        #ifdef LIB_IMG_STATIC
            #define LIB_IMG_PUBLIC
        #else
            #define LIB_IMG_PUBLIC __declspec(dllimport)
        #endif
    #endif
#elif defined(__linux__) | defined(__linux) | defined(linux)
    #define LIB_IMG_PUBLIC
#elif defined(__APPLE__)
    #define LIB_IMG_PUBLIC
#endif

#endif // LIB_IMG_COMMON_H
