#define APPLICATION
#include "application.h"

// Global functions imported from core library
static f_debug_log *_debug_log;
static f_temp_arena_begin *temp_arena_begin;
static f_get_scratch_arena *get_scratch_arena;
static f_temp_arena_end *temp_arena_end;
static f_os_window_close *os_window_close;
static f_os_event_to_string *os_event_to_string;

static f_os_file_size *os_file_size;
static f_os_open_file *os_open_file;
static f_os_close_file *os_close_file;
static f_os_read_file *os_read_file;
static f_os_write_file *os_write_file;

#include "emulator/cpu.h"

#include "emulator/cpu.cpp"

extern "C" DLL_EXPORT void load_core_library(LibraryFunctions *f)
{
    _debug_log = f->_debug_log;
    temp_arena_begin = f->temp_arena_begin;
    get_scratch_arena = f->get_scratch_arena;
    temp_arena_end = f->temp_arena_end;
    os_window_close = f->os_window_close;
    os_event_to_string = f->os_event_to_string;

    os_file_size = f->os_file_size;
    os_open_file = f->os_open_file;
    os_close_file = f->os_close_file;
    os_read_file = f->os_read_file;
    os_write_file = f->os_write_file;
}

static void application_initialize(ApplicationState *application_state)
{
    ApplicationState *state = application_state; // alias

    const String rom_path = string_lit("../roms/instr_test.gb");

    OS_FileHandle rom_handle = os_open_file(OS_FileAccessFlag_Read, rom_path);
    DataHeader rom = os_read_file(state->arena, rom_handle, 0, 0);
    // String file_string = c_string_to_String(arena, (i8 *)argv[1], c_strlen((i8 *)argv[1]));

    cpu_init();
    cpu_process((u8 *)rom.data, rom.size);

    // registers.a = 0b0000'0001;
    // registers.f = 0b0000'0001;

    set_AF((u16)257);
    u16 af = get_AF();


}

extern "C" DLL_EXPORT void application_update_and_render(ApplicationState *application_state)
{
    ApplicationState *state = application_state; // alias
    if(!state->initialized)
    {
        application_initialize(state);
        state->initialized = true;
        LOG_DEBUG("ApplicationState initialized\n");

    }


}
