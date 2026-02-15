static void cpu_init()
{

}

static u16 get_AF()
{
    return ((u16)registers.a << 8) | registers.f;
}

static void set_AF(u16 value)
{
    registers.a = (u8)((value & 0xFF00) >> 8);
    registers.b = (u8)(value & 0xFF);
}

static u16 get_BC()
{
    return ((u16)registers.b << 8) | registers.c;
}

static void set_BC(u16 value)
{
    registers.b = (u8)((value & 0xFF00) >> 8);
    registers.c = (u8)(value & 0xFF);
}

static u16 get_DE()
{
    return ((u16)registers.d << 8) | registers.e;
}

static void set_DE(u16 value)
{
    registers.d = (u8)((value & 0xFF00) >> 8);
    registers.e = (u8)(value & 0xFF);
}

static u16 get_HL()
{
    return ((u16)registers.h << 8) | registers.l;
}

static void set_HL(u16 value)
{
    registers.h = (u8)((value & 0xFF00) >> 8);
    registers.l = (u8)(value & 0xFF);
}