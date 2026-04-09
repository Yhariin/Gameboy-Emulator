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
static f_c_strlen *c_strlen;
static f_c_string_to_String *c_string_to_String;

static f_os_file_size *os_file_size;
static f_os_open_file *os_open_file;
static f_os_close_file *os_close_file;
static f_os_read_file *os_read_file;
static f_os_write_file *os_write_file;

static f_os_time_now_micro *os_time_now_micro;
static f_os_time_now_milli *os_time_now_milli;
static f_os_time_now_sec *os_time_now_sec;
static f_os_sleep_milli *os_sleep_milli;

// #define WRITE_LOG 1

#include "emulator/cart.h"
#include "emulator/cpu.h"
#include "emulator/io.h"
#include "emulator/ppu.h"

#include "emulator/cart.cpp"
#include "emulator/cpu.cpp"
#include "emulator/io.cpp"
#include "emulator/ppu.cpp"

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
    c_strlen = f->c_strlen;
    c_string_to_String = f->c_string_to_String;

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


    // load_cart(state->arena, string_lit("../roms/hello-world.gb"));
    // load_cart(state->arena, string_lit("../roms/test_roms/dmg-acid2.gb"));
    // load_cart(state->arena, string_lit("../roms/test_roms/instr_test.gb"));
    load_cart(state->arena, string_lit("../roms/Dr. Mario.gb"));
    // load_cart(state->arena, string_lit("../roms/mooneye/acceptance/oam_dma/basic.gb"));
    // load_cart(state->arena, string_lit("../roms/mooneye/acceptance/if_ie_registers.gb"));
    // load_cart(state->arena, string_lit("../roms/Tetris.gb"));
    // load_cart(state->arena, string_lit("../roms/test_roms/01-special.gb"));
    // load_cart(state->arena, string_lit("../roms/test_roms/02-interrupts.gb"));
    // load_cart(state->arena, string_lit("../roms/test_roms/03-op sp,hl.gb"));
    // load_cart(state->arena, string_lit("../roms/test_roms/04-op r,imm.gb"));
    // load_cart(state->arena, string_lit("../roms/test_roms/05-op rp.gb"));
    // load_cart(state->arena, string_lit("../roms/test_roms/06-ld r,r.gb"));
    // load_cart(state->arena, string_lit("../roms/test_roms/07-jr,jp,call,ret,rst.gb"));
    // load_cart(state->arena, string_lit("../roms/test_roms/08-misc instrs.gb"));
    // load_cart(state->arena, string_lit("../roms/test_roms/09-op r,r.gb"));
    // load_cart(state->arena, string_lit("../roms/test_roms/10-bit ops.gb"));
    // load_cart(state->arena, string_lit("../roms/test_roms/11-op a,(hl).gb"));

    // out_file =  os_open_file(OS_FileAccessFlag_Write, string_lit("07_logfile.txt"));
    #ifdef WRITE_LOG
        out_file =  os_open_file(OS_FileAccessFlag_Write, string_lit("logfile.txt"));
    #endif

    cpu_init(state->arena);
    cpu_post_boot_init();
    ppu_init(state->arena);


    // out_file =  os_open_file(OS_FileAccessFlag_Write, string_lit("01-special-out.txt"));
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
        u8 cycles = cpu_step();
        cycles_this_frame += cycles;

        cpu_update_timer(cycles);
        ppu_step(cycles);
        dma_step(cycles);

        print_tests();
        itr++;
    }

    // Update framebuffer
    {
        for (int i = 0; i < RES_X * RES_Y; i++)
        {
            u32 color; // ARGB
            switch(ppu_state->frame_buffer[i])
            {
                case 0x00:
                {
                    color = 0xffffffff;
                    break;
                }
                case 0x01:
                {
                    color = 0xffaaaaaa;
                    break;
                }
                case 0x02:
                {
                    color = 0xff555555;
                    break;
                }
                case 0x03:
                {
                    color = 0xff000000;
                    break;
                }
                default:
                {
                    color = 0xffffffff;
                    break;
                }
            }
            state->frame_buffer[i] = color;

            // if (ppu_state->frame_buffer[i] =)
            // state->frame_buffer[]
        }
    }

    if (cart_state->need_save)
    {
        cart_battery_save();
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
    #ifdef WRITE_LOG
        os_close_file(out_file);
    #endif
}