#ifndef LIB_IMG_COMMON_H
#define LIB_IMG_COMMON_H

#include <iostream>

#define LOG_COLOR_Y   "\033[93m"
#define LOG_COLOR_R   "\033[91m"
#define LOG_COLOR_W   "\033[0m"
#define LOG_BOLD_FONT "\033[1m"

static_assert(sizeof(int) == 4, "size of int, %d != 4");
static_assert(sizeof(float) == 4, "size of float, %d != 4");
static_assert(sizeof(double) == 8, "size of double, %d != 8");

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

#define IMG_ABORT(reason, ...)                                                                       \
    do {                                                                                             \
        fprintf(stderr,                                                                              \
                LOG_COLOR_R "ABORT %s:%d at %s: " LOG_COLOR_W LOG_BOLD_FONT reason LOG_COLOR_W "\n", \
                __FILE_NAME__,                                                                       \
                __LINE__,                                                                            \
                __PRETTY_FUNCTION__ __VA_OPT__(, ) __VA_ARGS__);                                     \
        abort();                                                                                     \
    } while (0)

#define IMG_WARN(warn_msg, ...) fprintf(stderr, LOG_COLOR_Y "WARNING: " LOG_BOLD_FONT warn_msg LOG_COLOR_W "\n" __VA_OPT__(, ) __VA_ARGS__)

#ifdef IMG_DEBUG
    #define IMG_DEBUG_LVL 1
#else
    #define IMG_DEBUG_LVL 0
#endif // IMG_DEBUG

#define IMG_ASSERT(condition, msg, ...)                                                                   \
    do {                                                                                                  \
        if (!(condition)) {                                                                               \
            fprintf(stderr,                                                                               \
                    LOG_COLOR_R "ASSERTION %s:%d at %s: " LOG_COLOR_W LOG_BOLD_FONT msg LOG_COLOR_W "\n", \
                    __FILE_NAME__,                                                                        \
                    __LINE__,                                                                             \
                    __PRETTY_FUNCTION__ __VA_OPT__(, ) __VA_ARGS__);                                      \
            abort();                                                                                      \
        }                                                                                                 \
    } while (0)

#if IMG_DEBUG_LVL > 0
    #define IMG_DEBUG_ASSERT(condition, msg, ...) IMG_ASSERT(condition, msg __VA_OPT__(, ) __VA_ARGS__)
#else
    #define IMG_DEBUG_ASSERT(condition, msg) \
        do {                                 \
        } while (0);
#endif // IMG_DEBUG_LVL > 0

#endif // LIB_IMG_COMMON_H
