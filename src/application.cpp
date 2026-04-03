#define APPLICATION
#include "application.h"

// Global functions imported from core library
static f_debug_log *_debug_log;
static f_temp_arena_begin *temp_arena_begin;
static f_get_scratch_arena *get_scratch_arena;
static f_temp_arena_end *temp_arena_end;
static f_arena_alloc_align *arena_alloc_align;
static f_os_window_close *os_window_close;
static f_os_event_to_string *os_event_to_string;

static f_string_format *string_format;
static f_string_cat *string_cat;

static f_os_file_size *os_file_size;
static f_os_open_file *os_open_file;
static f_os_close_file *os_close_file;
static f_os_read_file *os_read_file;
static f_os_write_file *os_write_file;

static f_os_time_now_micro *os_time_now_micro;
static f_os_time_now_milli *os_time_now_milli;
static f_os_time_now_sec *os_time_now_sec;
static f_os_sleep_milli *os_sleep_milli;

#include "emulator/cpu.h"
#include "emulator/rom.h"

#include "emulator/cpu.cpp"
#include "emulator/rom.cpp"

extern "C" DLL_EXPORT void load_core_library(LibraryFunctions *f)
{
    _debug_log = f->_debug_log;
    temp_arena_begin = f->temp_arena_begin;
    get_scratch_arena = f->get_scratch_arena;
    temp_arena_end = f->temp_arena_end;
    arena_alloc_align = f->arena_alloc_align;
    os_window_close = f->os_window_close;
    os_event_to_string = f->os_event_to_string;

    string_format = f->string_format;
    string_cat = f->string_cat;

    os_file_size = f->os_file_size;
    os_open_file = f->os_open_file;
    os_close_file = f->os_close_file;
    os_read_file = f->os_read_file;
    os_write_file = f->os_write_file;

    os_time_now_micro = f->os_time_now_micro;
    os_time_now_milli = f->os_time_now_milli;
    os_time_now_sec = f->os_time_now_sec;
    os_sleep_milli = f->os_sleep_milli;
}

static void application_initialize(ApplicationState *application_state)
{
    ApplicationState *state = application_state; // alias


    // load_rom(string_lit("../roms/hello-world.gb"));
    // load_rom(string_lit("../roms/test_roms/03-op sp,hl.gb");
    // Rom rom = load_rom(state->arena, string_lit("../roms/test_roms/01-special.gb"));
    // Rom rom = load_rom(state->arena, string_lit("../roms/test_roms/02-interrupts.gb"));
    // Rom rom = load_rom(state->arena, string_lit("../roms/test_roms/03-op sp,hl.gb"));
    // Rom rom = load_rom(state->arena, string_lit("../roms/test_roms/04-op r,imm.gb"));
    // Rom rom = load_rom(state->arena, string_lit("../roms/test_roms/05-op rp.gb"));
    // Rom rom = load_rom(state->arena, string_lit("../roms/test_roms/06-ld r,r.gb"));
    Rom rom = load_rom(state->arena, string_lit("../roms/test_roms/07-jr,jp,call,ret,rst.gb"));
    // Rom rom = load_rom(state->arena, string_lit("../roms/test_roms/08-misc instrs.gb"));
    // Rom rom = load_rom(state->arena, string_lit("../roms/test_roms/09-op r,r.gb"));
    // Rom rom = load_rom(state->arena, string_lit("../roms/test_roms/10-bit ops.gb"));
    // Rom rom = load_rom(state->arena, string_lit("../roms/test_roms/11-op a,(hl).gb"));
    // const String rom_path = string_lit("../roms/test1/test1.gb");

    out_file =  os_open_file(OS_FileAccessFlag_Write, string_lit("07_logfile.txt"));

    cpu_init(state->arena, rom);
    cpu_post_boot_init();


    // out_file =  os_open_file(OS_FileAccessFlag_Write, string_lit("01-special-out.txt"));
    // out_file =  os_open_file(OS_FileAccessFlag_Write, string_lit("03_logfile.txt"));
}

extern "C" DLL_EXPORT void application_update_and_render(ApplicationState *application_state)
{
    ApplicationState *state = application_state; // alias
    if(!state->initialized)
    {
        application_initialize(state);
        state->initialized = true;
        LOG_DEBUG("ApplicationState initialized\n");
        // LOG_DEBUG("%f", FRAME_TIME_MS);
    }

    // Emulate one frame
    u32 cycles_this_frame = 0;
    static u64 itr = 1;
    while(cycles_this_frame < T_CYCLES_PER_FRAME)
    {
        // TempArena scratch = temp_arena_begin(get_scratch_arena());
        // String current_state_log = string_format(scratch.arena, "A: %02X F: %02X B: %02X C: %02X D: %02X E: %02X H: %02X L: %02X SP: %04X PC: 00:%04X (%02X %02X %02X %02X)\n",
        // cpu_state->registers.a, cpu_state->registers.f, cpu_state->registers.b, cpu_state->registers.c, cpu_state->registers.d,
        // cpu_state->registers.e, cpu_state->registers.h, cpu_state->registers.l, cpu_state->sp, cpu_state->pc,
        // cpu_state->memory[cpu_state->pc], cpu_state->memory[cpu_state->pc+1], cpu_state->memory[cpu_state->pc+2], cpu_state->memory[cpu_state->pc+3]);


        // os_write_file(out_file, current_state_log.chars, current_state_log.size);

        // cpu_log = string_cat(state->arena, cpu_log, current_state_log);
        u8 cycles = cpu_step();
        cycles_this_frame += cycles;

        cpu_update_timer(cycles);

        print_tests();
        // temp_arena_end(scratch);
        itr++;
    }


    // Wait for any remaining time to match FRAME_TIME_MS
    // TODO: More accurate frame timing
    f64 elapsed = os_time_now_milli() - state->frame_start;
    f64 remaining = FRAME_TIME_MS - elapsed;

    // LOG_DEBUG("elapsed: %f, remaining: %f\n", elapsed, remaining);
    if (elapsed < FRAME_TIME_MS)
    {
        u32 sleep_time = floor(remaining);
        os_sleep_milli(sleep_time);

    }

}

extern "C" DLL_EXPORT void application_shutdown()
{
    // os_close_file(out_file);
}