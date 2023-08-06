#ifndef LIB_IMG_ASSERT_H
#define LIB_IMG_ASSERT_H

#include <console_log.hpp>

#ifndef LIB_IMG_DEBUG_LVL
    #define LIB_IMG_DEBUG_LVL 0
#endif // LIB_IMG_DEBUG_LVL

#define IMG_ABORT(reason, ...)                            \
    do {                                                  \
        IMG_LOG_ABORT(reason __VA_OPT__(, ) __VA_ARGS__); \
        abort();                                          \
    } while (0)

#define IMG_ASSERT(condition, msg, ...)                     \
    do {                                                    \
        if (!(condition)) {                                 \
            IMG_LOG_ASSERT(msg __VA_OPT__(, ) __VA_ARGS__); \
            abort();                                        \
        }                                                   \
    } while (0)

#if LIB_IMG_DEBUG_LVL > 0
    #define IMG_DEBUG_ASSERT(condition, msg, ...) IMG_ASSERT(condition, msg __VA_OPT__(, ) __VA_ARGS__)
#else
    #define IMG_DEBUG_ASSERT(condition, msg, ...)
#endif // LIB_IMG_DEBUG_LVL > 0

#endif // LIB_IMG_ASSERT_H