#pragma once

#include "base/base_include.h"
#include "os/os_core.h"

// Imported functions
typedef void f_debug_log(LogColor log_color, const char *format, ...);
typedef TempArena f_temp_arena_begin(Arena *arena);
typedef Arena *f_get_scratch_arena();
typedef void f_temp_arena_end(TempArena temp);
typedef void *f_arena_alloc_align(Arena *arena, u64 size, u64 alignment);
typedef void f_os_window_close(OS_WindowID window_id);
typedef String f_os_event_to_string(Arena *arena, const OS_Event *const event);

typedef String f_string_format(Arena *arena, const char *fmt, ...);
typedef String f_string_cat(Arena *arena, String a, String b);
typedef u64 f_c_strlen(const c8 *str);
typedef String f_c_string_to_String(Arena *arena, const c8 *string, u64 size);

typedef u64 f_os_file_size(OS_FileHandle file);
typedef OS_FileHandle f_os_open_file(OS_FileAccessFlag flags, String path);
typedef void f_os_close_file(OS_FileHandle file);
typedef DataHeader f_os_read_file(Arena *arena, OS_FileHandle file, u64 start, u64 end);
typedef b8 f_os_write_file(OS_FileHandle file, void *data, u64 size);

typedef f64 f_os_time_now_micro();
typedef f64 f_os_time_now_milli();
typedef f64 f_os_time_now_sec();
typedef void f_os_sleep_milli(f64 milliseconds);

struct LibraryFunctions
{
    f_debug_log *_debug_log;
    f_temp_arena_begin *temp_arena_begin;
    f_get_scratch_arena *get_scratch_arena;
    f_temp_arena_end *temp_arena_end;
    f_arena_alloc_align *arena_alloc_align;
    f_os_window_close *os_window_close;
    f_os_event_to_string *os_event_to_string;

    f_string_format *string_format;
    f_string_cat *string_cat;
    f_c_strlen *c_strlen;
    f_c_string_to_String *c_string_to_String;

    f_os_file_size *os_file_size;
    f_os_open_file *os_open_file;
    f_os_close_file *os_close_file;
    f_os_read_file *os_read_file;
    f_os_write_file *os_write_file;

    f_os_time_now_micro *os_time_now_micro;
    f_os_time_now_milli *os_time_now_milli;
    f_os_time_now_sec *os_time_now_sec;
    f_os_sleep_milli *os_sleep_milli;
};

struct ApplicationState
{
    Arena *arena;
    b8 initialized;

    b8 *key_states;

    f64 frame_start;
    f64 dt;

    OS_Window *first_window;
    OS_Window *last_window;
};

typedef void f_load_core_library(LibraryFunctions *f);
typedef void f_application_update_and_render(ApplicationState *application_state);
typedef void f_application_shutdown();

static void application_initialize();