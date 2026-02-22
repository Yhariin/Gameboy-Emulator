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