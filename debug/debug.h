#ifndef _DEBUG_H
#define _DEBUG_H

#include <stdio.h>      // for fprintf(), stderr
#include <time.h>       // for time(), ctime_r(), time_t
#include <string.h>     // for strlen(), strerror()
#include <errno.h>      // for errno
#include <stdlib.h>     // for abort()
#include <assert.h>     // for assert()

#ifdef DEBUG_COLOR /* Colorize with ANSI escape sequences. */

#define DEBUG_COLOR_RED     "[31m"
#define DEBUG_COLOR_GREEN   "[32m"
#define DEBUG_COLOR_YELLOW  "[33m"
#define DEBUG_COLOR_BLUE    "[34m"
#define DEBUG_COLOR_PURPLE  "[35m"
#define DEBUG_COLOR_RESET   "[m"

#else

#define DEBUG_COLOR_RED     ""
#define DEBUG_COLOR_GREEN   ""
#define DEBUG_COLOR_YELLOW  ""
#define DEBUG_COLOR_BLUE    ""
#define DEBUG_COLOR_PURPLE  ""
#define DEBUG_COLOR_RESET   ""

#endif /* DEBUG_COLOR */


#ifndef NDEBUG

#define DEBUG_LEVEL_OFF         (0) /* */
#define DEBUG_LEVEL_PANIC       (1) /* system is unusable */
#define DEBUG_LEVEL_ERROR       (2) /* error conditions */
#define DEBUG_LEVEL_WARNING     (3) /* warning conditions */
#define DEBUG_LEVEL_INFO        (4) /* informational message */
#define DEBUG_LEVEL_DEBUG       (5) /* debug-level message */
#define DEBUG_LEVEL_TRACE       (6) /* function calls message */

#ifndef debug_printf
#define debug_printf(...)   fprintf(stderr, __VA_ARGS__)
#endif

#ifdef __cplusplus
#define debug_time() (::time(0)) // TODO: current time (seconds)
#else
#define debug_time() (time(0)) // TODO: current time (seconds)
#endif

#ifndef DEBUG_LEVEL
#define DEBUG_LEVEL DEBUG_LEVEL_WARNING /* default debug level */
#endif

#if defined(DEBUG_NO_PREFIX)
#define debug_print_prefix(tag, color)
#elif defined(DEBUG_SHORT_PREFIX)
#define debug_print_prefix(tag, color) do {                             \
    char buf_time[26];                                                  \
    time_t cur_time = debug_time();                                     \
    ctime_r(&cur_time, buf_time);                                       \
    buf_time[strlen(buf_time)-1] = '\0'; /* '\n' -> '\0' */             \
                                                                        \
    debug_printf("%s [" color tag DEBUG_COLOR_RESET "]: ", buf_time);   \
} while (0)
#elif defined(DEBUG_SUBLIME_PREFIX)

#ifndef STRINGIZE
#define STRINGIZE2(x) #x
#define STRINGIZE(x) STRINGIZE2(x)
#endif

#define debug_print_prefix(tag, color) \
    debug_printf(__FILE__ ":" STRINGIZE(__LINE__) ": " tag ": %s: ", __PRETTY_FUNCTION__)

#else /* DEBUG_LONG_PREFIX */
#define debug_print_prefix(tag, color) do {                             \
    char buf_time[26];                                                  \
    time_t cur_time = time(0);                                          \
    ctime_r(&cur_time, buf_time);                                       \
    buf_time[strlen(buf_time)-1] = '\0'; /* '\n' -> '\0' */             \
                                                                        \
    debug_printf("%s [" color tag DEBUG_COLOR_RESET "]: %s:%d(%s): ",   \
            buf_time, __FILE__, __LINE__, __PRETTY_FUNCTION__);         \
} while (0)
#endif

#define DEBUG_PRINT(tag, color, level, ...) do {                        \
    if (DEBUG_LEVEL >= level) {                                         \
        debug_print_prefix(tag, color);                                 \
        debug_printf(__VA_ARGS__);                                      \
        debug_printf("\n");                                             \
    }                                                                   \
} while (0)

#if defined(DEBUG_SUBLIME_PREFIX)

#define panic(...)   do { DEBUG_PRINT("panic", DEBUG_COLOR_PURPLE, DEBUG_LEVEL_PANIC,  __VA_ARGS__); abort(); } while(0)
#define error(...)   DEBUG_PRINT("error",   DEBUG_COLOR_RED,    DEBUG_LEVEL_ERROR,   __VA_ARGS__)
#define warning(...) DEBUG_PRINT("warning", DEBUG_COLOR_YELLOW, DEBUG_LEVEL_WARNING, __VA_ARGS__)
#define info(...)    DEBUG_PRINT("info",    DEBUG_COLOR_GREEN,  DEBUG_LEVEL_INFO,    __VA_ARGS__)
#define debug(...)   DEBUG_PRINT("debug",   DEBUG_COLOR_BLUE,   DEBUG_LEVEL_DEBUG,   __VA_ARGS__)
#define trace()      DEBUG_PRINT("trace",   DEBUG_COLOR_BLUE,   DEBUG_LEVEL_TRACE,   "%s:%d", __PRETTY_FUNCTION__, __LINE__)

//#define log(...)     DEBUG_PRINT("log",     DEBUG_COLOR_RESET,  DEBUG_LEVEL_DEBUG,   __VA_ARGS__)
#define fixme(...)   DEBUG_PRINT("fixme",   DEBUG_COLOR_GREEN,  DEBUG_LEVEL_WARNING, __VA_ARGS__)
#define print_errno(...) DEBUG_PRINT("errno", DEBUG_COLOR_YELLOW, DEBUG_LEVEL_ERROR, strerror(errno))

#else
#define panic(...)   do { DEBUG_PRINT("PANIC", DEBUG_COLOR_PURPLE, DEBUG_LEVEL_PANIC,  __VA_ARGS__); abort(); } while(0)
#define error(...)   DEBUG_PRINT("ERROR",   DEBUG_COLOR_RED,    DEBUG_LEVEL_ERROR,   __VA_ARGS__)
#define warning(...) DEBUG_PRINT("WARNING", DEBUG_COLOR_YELLOW, DEBUG_LEVEL_WARNING, __VA_ARGS__)
#define info(...)    DEBUG_PRINT("INFO",    DEBUG_COLOR_GREEN,  DEBUG_LEVEL_INFO,    __VA_ARGS__)
#define debug(...)   DEBUG_PRINT("DEBUG",   DEBUG_COLOR_BLUE,   DEBUG_LEVEL_DEBUG,   __VA_ARGS__)
#define trace()      DEBUG_PRINT("TRACE",   DEBUG_COLOR_BLUE,   DEBUG_LEVEL_TRACE,   "%s:%d", __PRETTY_FUNCTION__, __LINE__)

//#define log(...)     DEBUG_PRINT("LOG",     DEBUG_COLOR_RESET,  DEBUG_LEVEL_DEBUG,   __VA_ARGS__)
#define fixme(...)   DEBUG_PRINT("FIXME",   DEBUG_COLOR_GREEN,  DEBUG_LEVEL_WARNING, __VA_ARGS__)
#define print_errno(...) DEBUG_PRINT("ERRNO", DEBUG_COLOR_YELLOW, DEBUG_LEVEL_ERROR, strerror(errno))

#endif

#else

#define panic(...)   ((void)0)
#define error(...)   ((void)0)
#define warning(...) ((void)0)
#define info(...)    ((void)0)
#define debug(...)   ((void)0)
#define trace()      ((void)0)

//#define log(...)     ((void)0)
#define fixme(...)   ((void)0)
#define print_errno(...) ((void)0)

#endif /* NDEBUG */

#endif /* _DEBUG_H */
