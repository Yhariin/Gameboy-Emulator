static void cpu_init(Arena *arena, u8* rom_data, u64 rom_size)
{
    cpu_state = (CPU_State *)arena_alloc_align(arena, sizeof(CPU_State), sizeof(CPU_State));
    cpu_state->memory = (u8 *)arena_alloc_align(arena, 0xFFFF, sizeof(u8));
    cpu_state->sp = 0xFFFF;

    ASSERT(rom_size <= 0x8000, "ERROR Rom size must be at most 32kb!");
    memcpy(cpu_state->memory, rom_data, rom_size);

}

static void cpu_process()
{

    while(cpu_state->pc < 0x8000)
    {
        switch(cpu_state->memory[cpu_state->pc])
        {
            // NOP
            case 0x00:
            {
                cpu_state->pc += 1;
                break;
            }
            // LD BC, imm16
            case 0x01:
            {
                u16 lo = (u16)(cpu_state->memory[cpu_state->pc+1]); // low
                u16 hi = (u16)(cpu_state->memory[cpu_state->pc+2]) << 8; // hi
                set_BC(lo | hi);
                cpu_state->pc += 3;
                break;
            }
            // LD [BC], A
            case 0x02:
            {
                cpu_state->memory[get_BC()] = cpu_state->registers.a;
                cpu_state->pc += 1;
                break;
            }
            // INC BC
            case 0x03:
            {
                set_BC(get_BC() + 1);
                cpu_state->pc += 1;
                break;
            }
            // INC B
            case 0x04:
            {
                cpu_state->registers.b += 1;
                cpu_state->pc += 1;
                break;
            }
            // DEC B
            case 0x05:
            {
                cpu_state->registers.b -= 1;
                cpu_state->pc += 1;
                break;
            }
            // LD B, imm8
            case 0x06:
            {
                cpu_state->registers.b = cpu_state->memory[cpu_state->pc+1];
                cpu_state->pc += 2;
                break;
            }
            // RLCA
            case 0x07:
            {
                f_register_set_c(bit8 & cpu_state->registers.a);
                cpu_state->registers.a = cpu_state->registers.a << 1 | f_register_get_c();

                cpu_state->pc += 1;
                break;
            }
            // LD [imm16], sp
            case 0x08:
            {
                u16 hi = (u16)cpu_state->memory[cpu_state->pc+1]; // lo
                u16 lo = (u16)cpu_state->memory[cpu_state->pc+2] << 8; // hi
                u16 address = hi | lo;

                // sp[lower address] == lo
                // sp[higher address] == hi
                u16 sp_value_lo = (u16)cpu_state->memory[cpu_state->sp];
                u16 sp_value_hi = (u16)cpu_state->memory[cpu_state->sp+1] << 8;
                u16 sp_value = sp_value_hi | sp_value_lo;

                cpu_state->memory[address] = (u8)(sp_value_hi >> 8);
                cpu_state->memory[address+1] = (u8)(sp_value_lo);

                cpu_state->pc += 3;
                break;
            }
            // LD A, imm8
            case 0x3E:
            {
                cpu_state->registers.a = cpu_state->memory[cpu_state->pc+1];
                cpu_state->pc += 2;
                break;
            }
            // ADD A, B
            case 0x80:
            {
                cpu_state->registers.a += cpu_state->registers.b;
                cpu_state->pc += 1;
                break;
            }
            default:
                cpu_state->pc += 1;
                break;
        }

    }

}

static b8 f_register_get_z()
{
    return (cpu_state->registers.f & bit7) == bit7;
}

static b8 f_register_get_n()
{
    return (cpu_state->registers.f & bit6) == bit6;
}

static b8 f_register_get_h()
{
    return (cpu_state->registers.f & bit5) == bit5;
}

static b8 f_register_get_c()
{
    return (cpu_state->registers.f & bit4) == bit4;
}

static void f_register_set_z(b8 flag)
{
    if (flag)
    {
        cpu_state->registers.f |= bit7;
    }
    else
    {
        cpu_state->registers.f |= (~bit7);
    }
}

static void f_register_set_n(b8 flag)
{
    if (flag)
    {
        cpu_state->registers.f |= bit6;
    }
    else
    {
        cpu_state->registers.f |= (~bit6);
    }

}

static void f_register_set_h(b8 flag)
{
    if (flag)
    {
        cpu_state->registers.f |= bit5;
    }
    else
    {
        cpu_state->registers.f |= (~bit5);
    }
}

static void f_register_set_c(b8 flag)
{
    if (flag)
    {
        cpu_state->registers.f |= bit4;
    }
    else
    {
        cpu_state->registers.f |= (~bit4);
    }
}

static u16 get_AF()
{
    return ((u16)cpu_state->registers.a << 8) | cpu_state->registers.f;
}

static void set_AF(u16 value)
{
    cpu_state->registers.a = (u8)((value & 0xFF00) >> 8);
    cpu_state->registers.b = (u8)(value & 0xFF);
}

static u16 get_BC()
{
    return ((u16)cpu_state->registers.b << 8) | cpu_state->registers.c;
}

static void set_BC(u16 value)
{
    cpu_state->registers.b = (u8)((value & 0xFF00) >> 8);
    cpu_state->registers.c = (u8)(value & 0xFF);
}

static u16 get_DE()
{
    return ((u16)cpu_state->registers.d << 8) | cpu_state->registers.e;
}

static void set_DE(u16 value)
{
    cpu_state->registers.d = (u8)((value & 0xFF00) >> 8);
    cpu_state->registers.e = (u8)(value & 0xFF);
}

static u16 get_HL()
{
    return ((u16)cpu_state->registers.h << 8) | cpu_state->registers.l;
}

static void set_HL(u16 value)
{
    cpu_state->registers.h = (u8)((value & 0xFF00) >> 8);
    cpu_state->registers.l = (u8)(value & 0xFF);
}
