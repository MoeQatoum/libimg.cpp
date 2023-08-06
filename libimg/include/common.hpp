#ifndef LIB_IMG_COMMON_H
#define LIB_IMG_COMMON_H

#define LIB_IMG_MAX_PIXELS_W_H 65535

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
