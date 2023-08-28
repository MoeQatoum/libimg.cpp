#ifndef LIB_IMG_ASSERT_H
#define LIB_IMG_ASSERT_H

#include <string>
#include <utility>

#include "console_log.hpp"

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

#if LIB_IMG_DEBUG
    #define IMG_DEBUG_ASSERT(condition, msg, ...) IMG_ASSERT((condition), msg __VA_OPT__(, ) __VA_ARGS__)
#else
    #define IMG_DEBUG_ASSERT(condition, msg, ...)
#endif // LIB_IMG_DEBUG_LVL > 0

#if 0
    #define UNREACHABLE(...)                                                                                      \
        do {                                                                                                      \
            std::string s{__VA_ARGS__};                                                                           \
            std::cerr << IMG_LOG_CLR_R << "UNREACHABLE:" << __LINE__ << ":" << __FILE__ << IMG_LOG_CLR_W << ": `" \
                      << (s.empty() ? "<no message>" : s) << "`" << std::endl;                                    \
            std::unreachable();                                                                                   \
        } while (0)
#else
    #define UNREACHABLE()
#endif

#endif // LIB_IMG_ASSERT_H