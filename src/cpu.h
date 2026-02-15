#pragma once

typedef struct
{
    u8 a;
    u8 b;
    u8 c;
    u8 d;
    u8 e;
    u8 f;
    u8 h;
    u8 l;
} Registers;


static Registers registers = {0};

static u16 get_AF();
static void set_AF(u16 value);

static u16 get_BC();
static void set_BC(u16 value);

static u16 get_DE();
static void set_DE(u16 value);

static u16 get_HL();
static void set_HL(u16 value);
