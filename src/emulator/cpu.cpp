static void cpu_init()
{

}

static void cpu_process(u8* data, u64 data_size)
{

    for(u32 i = 0, byte_increment; i < data_size; i += byte_increment)
    {
        byte_increment = 0;

        switch(data[i])
        {
            // NOP
            case 0x00:
            {
                byte_increment = 1;
                break;
            }
            // LD BC, imm16
            case 0x01:
            {
                u16 lo = (u16)(data[i+1]); // low
                u16 hi = (u16)(data[i+2]) << 8; // hi
                set_BC(lo | hi);
                byte_increment = 3;
                break;
            }
            // TODO: LD [BC], A
            case 0x02:
            {
                byte_increment = 1;
                break;
            }
            // INC BC
            case 0x03:
            {
                set_BC(get_BC() + 1);
                byte_increment = 1;
                break;
            }
            // INC B
            case 0x04:
            {
                registers.b += 1;
                byte_increment = 1;
                break;
            }
            // DEC B
            case 0x05:
            {
                registers.b -= 1;
                byte_increment = 1;
                break;
            }
            // LD B, imm8
            case 0x06:
            {
                registers.b = data[i+1];
                byte_increment = 2;
                break;
            }
            // TODO: RLCA
            case 0x07:
            {
                byte_increment = 1;
                break;
            }
            // LD A, imm8
            case 0x3E:
            {
                registers.a = data[i+1];
                byte_increment = 2;
                break;
            }
            // ADD A, B
            case 0x80:
            {
                registers.a += registers.b;
                byte_increment = 1;
                break;
            }
            default:
                break;
        }
    }

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

static void execute(Instructions instruction)
{
    switch (instruction)
    {
    case Instructions_ADD:
    {

        break;
    }

    default:
        break;
    }

}