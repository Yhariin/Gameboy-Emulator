#pragma once

enum LogColor : u32
{
    LogColor_None = 0,
    LogColor_Debug,
    LogColor_Warn,
    LogColor_Error,
    LogColor_COUNT
};

#define SET_DEBUG_LOG_COLOR(color)
CORE_EXPORT(void _debug_log(LogColor log_color, const char *format, ...));


#if DEBUG

#define LOG_DEBUG(format, ...) _debug_log(LogColor_Debug, format, ##__VA_ARGS__)
#define LOG_WARN(format,  ...) _debug_log(LogColor_Warn,  format, ##__VA_ARGS__)
#define LOG_ERROR(format, ...) _debug_log(LogColor_Error, format, ##__VA_ARGS__)
#define LOG_TRACE(format, ...) _debug_log(LogColor_None, format, ##__VA_ARGS__)

#else
#define LOG_DEBUG(format, ...)
#define LOG_WARN(format,  ...)
#define LOG_ERROR(format, ...)
#define LOG_TRACE(format, ...)
#endif

///////////////////////////////////////
// Asserts
#if COMPILER_MSVC || COMPILER_CLANG
#define DEBUGBREAK() __debugbreak()
// #elif COMPILER_CLANG
// #define DEBUGBREAK() __builtin_trap()
#else
#define DEBUGBREAK()
#endif

#define SLEEP(x)

#if DEBUG
#define ASSERT(condition, ...) \
do { \
    if (!(condition)) { \
        LOG_ERROR("Assertion failed '%s' failed at %s:%d | ", #condition, __FILE__, __LINE__); \
        __VA_OPT__(LOG_ERROR( __VA_ARGS__)); \
        fflush(stdout); \
        SLEEP(10); \
        DEBUGBREAK(); \
    } \
} while(0)
#else
#define ASSERT(condition, ...)
#endif