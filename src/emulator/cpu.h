#pragma once

struct Registers
{
    u8 a;
    u8 b;
    u8 c;
    u8 d;
    u8 e;
    u8 f;
    u8 h;
    u8 l;
};

#define ROM_START   0x0000 // The game ROM supplied by the cartridge
#define ROM_END     0x7FFF
#define VRAM_START  0x8000 // Video RAM, where graphics are stored and arranged
#define VRAM_END    0x9FFF
#define SRAM_START  0xA000 // Save RAM, optionally supplied by the cartridge to save data to
#define SRAM_END    0xBFFF
#define WRAM_START  0xC000 // Work RAM, general-purpose RAM for the game to store things in
#define WRAM_END    0xDFFF
#define OAM_START   0xFE00 // Object Attribute Memory, where "objects" are stored
#define OAM_END     0xFE9F
#define IO_START    0xFF00 // Where you control the console
#define IO_END      0xFF7F
#define HRAM_START  0xFF80 // High RAM, a tiny bit of general-purpose RAM with faster access
#define HRAM_END    0xFFFE
#define IE          0xFFFF // A lone I/O byte that's separated from the rest for some reason

// z - Zero flag
// n - Subtraction flag (BCD)
// h - Half Carry flag (BCD)
// c - Carry flag
static b8 f_register_get_z();
static b8 f_register_get_n();
static b8 f_register_get_h();
static b8 f_register_get_c();

static void f_register_set_z(b8 flag);
static void f_register_set_n(b8 flag);
static void f_register_set_h(b8 flag);
static void f_register_set_c(b8 flag);

struct CPU_State
{
    Registers registers;
    u16 pc;
    u16 sp;
    b8 is_halted;
    u8 *memory;
};

static CPU_State *cpu_state = nullptr;

static u16 get_AF();
static void set_AF(u16 value);

static u16 get_BC();
static void set_BC(u16 value);

static u16 get_DE();
static void set_DE(u16 value);

static u16 get_HL();
static void set_HL(u16 value);

static void cpu_init(Arena *arena, u8 *rom_data, u64 rom_size);
static void cpu_process();
static void process_16_bit_opcodes(u8 low);