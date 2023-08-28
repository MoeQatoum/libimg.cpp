#ifndef LIB_IMG_CONSOLE_LOG_H
#define LIB_IMG_CONSOLE_LOG_H

#include <iostream>

#define IMG_LOG_CLR_Y     "\033[93m"
#define IMG_LOG_CLR_R     "\033[91m"
#define IMG_LOG_CLR_W     "\033[0m"
#define IMG_LOG_BOLD_FONT "\033[1m"

#define IMG_LOG_ERR(msg, ...)                                                                               \
    fprintf(stderr,                                                                                         \
            IMG_LOG_CLR_R "[ERR_] %s:%d at %s: " IMG_LOG_CLR_W IMG_LOG_BOLD_FONT reason IMG_LOG_CLR_W "\n", \
            __FILE_NAME__,                                                                                  \
            __LINE__,                                                                                       \
            __PRETTY_FUNCTION__ __VA_OPT__(, ) __VA_ARGS__)

#define IMG_LOG_ABORT(msg, ...)                                                                          \
    fprintf(stderr,                                                                                      \
            IMG_LOG_CLR_R "[ABRT] %s:%d at %s: " IMG_LOG_CLR_W IMG_LOG_BOLD_FONT msg IMG_LOG_CLR_W "\n", \
            __FILE_NAME__,                                                                               \
            __LINE__,                                                                                    \
            __PRETTY_FUNCTION__ __VA_OPT__(, ) __VA_ARGS__)

#define IMG_LOG_ASSERT(msg, ...)                                                                         \
    fprintf(stderr,                                                                                      \
            IMG_LOG_CLR_R "[ASRT] %s:%d at %s: " IMG_LOG_CLR_W IMG_LOG_BOLD_FONT msg IMG_LOG_CLR_W "\n", \
            __FILE_NAME__,                                                                               \
            __LINE__,                                                                                    \
            __PRETTY_FUNCTION__ __VA_OPT__(, ) __VA_ARGS__)

#define IMG_LOG_WARN(msg, ...) \
    fprintf(stderr,            \
            IMG_LOG_CLR_Y "[WARN] " IMG_LOG_CLR_W IMG_LOG_BOLD_FONT msg IMG_LOG_CLR_W "\n" __VA_OPT__(, ) __VA_ARGS__)

#define IMG_LOG_INFO(msg, ...) \
    fprintf(stderr, "[INFO] " IMG_LOG_BOLD_FONT msg IMG_LOG_CLR_W "\n" __VA_OPT__(, ) __VA_ARGS__)

#endif // LIB_IMG_CONSOLE_LOG_H
