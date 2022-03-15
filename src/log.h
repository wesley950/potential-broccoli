#ifndef LOG_H_
#define LOG_H_

#include <stdio.h>

#ifdef NDEBUG

#define LOG(fmt, ...)                                        \
    fprintf(stdout, "[Info] "); \
    fprintf(stdout, fmt, ##__VA_ARGS__);                     \
    fprintf(stdout, "\n");

#define LOG_ERROR(fmt, ...)                                   \
    fprintf(stderr, "[Error] "); \
    fprintf(stderr, fmt, ##__VA_ARGS__);                      \
    fprintf(stderr, "\n");

#else

#define LOG(fmt, ...)                                        \
    fprintf(stdout, "[Info] [%s:%d]: ", __FILE__, __LINE__); \
    fprintf(stdout, fmt, ##__VA_ARGS__);                     \
    fprintf(stdout, "\n");

#define LOG_ERROR(fmt, ...)                                   \
    fprintf(stderr, "[Error] [%s:%d]: ", __FILE__, __LINE__); \
    fprintf(stderr, fmt, ##__VA_ARGS__);                      \
    fprintf(stderr, "\n");

#endif // NDEBUG

#endif
