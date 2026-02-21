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

enum Instructions
{
    Instructions_ADD
};

static Registers registers = {};

static u16 get_AF();
static void set_AF(u16 value);

static u16 get_BC();
static void set_BC(u16 value);

static u16 get_DE();
static void set_DE(u16 value);

static u16 get_HL();
static void set_HL(u16 value);

static void cpu_init();
static void cpu_process(u8* data, u64 data_size);
static void execute(Instructions instruction);