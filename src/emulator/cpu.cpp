static void cpu_init(Arena *arena, u8* rom_data, u64 rom_size)
{
    cpu_state = (CPU_State *)arena_alloc_align(arena, sizeof(CPU_State), sizeof(CPU_State));
    cpu_state->memory = (u8 *)arena_alloc_align(arena, 0xFFFF, sizeof(u8));
    cpu_state->sp = 0xFFFE;

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
                cpu_state->registers.c = cpu_state->memory[cpu_state->pc+1]; // lo
                cpu_state->registers.b = cpu_state->memory[cpu_state->pc+2]; // hi

                cpu_state->pc += 3;
                break;
            }
            // LD [BC], A
            case 0x02:
            {
                u16 address = get_BC();
                if ((address < WRAM_START || address > WRAM_END) && (address < HRAM_START || address > HRAM_END))
                {
                    cpu_state->pc += 1;
                    break;
                }
                cpu_state->memory[address] = cpu_state->registers.a;
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

                f_register_set_z(cpu_state->registers.b == 0);
                f_register_set_n(false);
                f_register_set_h((((cpu_state->registers.b-1) & 0x0F) + 1) > 0x0F);

                cpu_state->pc += 1;
                break;
            }
            // DEC B
            case 0x05:
            {
                cpu_state->registers.b -= 1;

                f_register_set_z(cpu_state->registers.b == 0);
                f_register_set_n(true);
                f_register_set_h(((cpu_state->registers.b+1) & 0x0F) < 1);

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
                u16 lo = (u16)cpu_state->memory[cpu_state->pc+1];
                u16 hi = (u16)cpu_state->memory[cpu_state->pc+2] << 8;
                u16 address = hi | lo;

                if ((address < WRAM_START || address > WRAM_END) && (address < HRAM_START || address > HRAM_END))
                {
                    cpu_state->pc += 3;
                    break;
                }

                // sp[lower address] == lo
                // sp[higher address] == hi
                u8 sp_lo = cpu_state->sp & 0xFF;
                u8 sp_hi = (cpu_state->sp & 0xFF00) >> 8;

                cpu_state->memory[address] = sp_lo;
                cpu_state->memory[address+1] = sp_hi;

                cpu_state->pc += 3;
                break;
            }
            // ADD HL, BC
            case 0x09:
            {
                u16 hl = get_HL();
                u16 bc = get_BC();

                set_HL(hl + bc);

                f_register_set_n(false);
                f_register_set_h(((hl & 0xFFF) + (bc & 0xFFF)) > 0xFFF);
                f_register_set_c((hl+bc) > 0xFFFF);

                cpu_state->pc += 1;
                break;
            }
            // LD A, [BC]
            case 0x0A:
            {
                cpu_state->registers.a = cpu_state->memory[get_BC()];

                cpu_state->pc += 1;
                break;
            }
            // DEC BC
            case 0x0B:
            {
                set_BC(get_BC()-1);

                cpu_state->pc += 1;
                break;
            }
            // INC C
            case 0x0C:
            {
                cpu_state->registers.c += 1;

                f_register_set_z(cpu_state->registers.c == 0);
                f_register_set_n(false);
                f_register_set_h((((cpu_state->registers.c-1) & 0x0F) + 1) > 0x0F);

                cpu_state->pc += 1;
                break;
            }
            // DEC C
            case 0x0D:
            {
                cpu_state->registers.c -= 1;

                f_register_set_z(cpu_state->registers.c == 0);
                f_register_set_n(true);
                f_register_set_h(((cpu_state->registers.c+1) & 0x0F) < 1);

                cpu_state->pc += 1;
                break;
            }
            // LD C, imm8
            case 0x0E:
            {
                cpu_state->registers.c = cpu_state->memory[cpu_state->pc+1];
                cpu_state->pc += 2;
                break;
            }
            // RRCA
            case 0x0F:
            {
                f_register_set_c(bit1 & cpu_state->registers.a);
                cpu_state->registers.a = cpu_state->registers.a >> 1 | (f_register_get_c() << 7);

                cpu_state->pc += 1;
                break;
            }
            // TODO: STOP
            case 0x10:
            {
                cpu_state->pc += 2;
                break;
            }
            // LD DE, imm16
            case 0x11:
            {
                cpu_state->registers.e = cpu_state->memory[cpu_state->pc+1]; // lo
                cpu_state->registers.d = cpu_state->memory[cpu_state->pc+2]; // hi

                cpu_state->pc += 3;
                break;
            }
            // LD [DE], A
            case 0x12:
            {
                u16 address = get_DE();
                if ((address < WRAM_START || address > WRAM_END) && (address < HRAM_START || address > HRAM_END))
                {
                    cpu_state->pc += 1;
                    break;
                }
                cpu_state->memory[address] = cpu_state->registers.a;
                cpu_state->pc += 1;
                break;
            }
            // INC DE
            case 0x13:
            {
                set_DE(get_DE() + 1);
                cpu_state->pc += 1;
                break;
            }
            // INC D
            case 0x14:
            {
                cpu_state->registers.d += 1;

                f_register_set_z(cpu_state->registers.d == 0);
                f_register_set_n(false);
                f_register_set_h((((cpu_state->registers.d-1) & 0x0F) + 1) > 0x0F);

                cpu_state->pc += 1;
                break;
            }
            // DEC D
            case 0x15:
            {
                cpu_state->registers.d -= 1;

                f_register_set_z(cpu_state->registers.d == 0);
                f_register_set_n(true);
                f_register_set_h(((cpu_state->registers.d+1) & 0x0F) < 1);

                cpu_state->pc += 1;
                break;
            }
            // LD D, imm8
            case 0x16:
            {
                cpu_state->registers.d = cpu_state->memory[cpu_state->pc+1];
                cpu_state->pc += 2;
                break;
            }
            // RLA
            case 0x17:
            {
                cpu_state->registers.a = cpu_state->registers.a << 1 | f_register_get_c();

                cpu_state->pc += 1;
                break;
            }
            // JR imm8
            case 0x18:
            {
                cpu_state->pc += (i8)cpu_state->memory[cpu_state->pc+1];

                cpu_state->pc += 2;
                break;
            }
            // ADD HL, DE
            case 0x19:
            {
                u16 hl = get_HL();
                u16 de = get_DE();

                set_HL(hl + de);

                f_register_set_n(false);
                f_register_set_h(((hl & 0xFFF) + (de & 0xFFF)) > 0xFFF);
                f_register_set_c((hl+de) > 0xFFFF);

                cpu_state->pc += 1;
                break;
            }
            // LD A, [DE]
            case 0x1A:
            {
                cpu_state->registers.a = cpu_state->memory[get_DE()];

                cpu_state->pc += 1;
                break;
            }
            // DEC DE
            case 0x1B:
            {
                set_DE(get_DE()-1);

                cpu_state->pc += 1;
                break;
            }
            // INC E
            case 0x1C:
            {
                cpu_state->registers.e += 1;

                f_register_set_z(cpu_state->registers.e == 0);
                f_register_set_n(false);
                f_register_set_h((((cpu_state->registers.e-1) & 0x0F) + 1) > 0x0F);

                cpu_state->pc += 1;
                break;
            }
            // DEC E
            case 0x1D:
            {
                cpu_state->registers.d -= 1;

                f_register_set_z(cpu_state->registers.d == 0);
                f_register_set_n(true);
                f_register_set_h(((cpu_state->registers.d+1) & 0x0F) < 1);

                cpu_state->pc += 1;
                break;
            }
            // LD E, imm8
            case 0x1E:
            {
                cpu_state->registers.e = cpu_state->memory[cpu_state->pc+1];
                cpu_state->pc += 2;
                break;
            }
            // RRA
            case 0x1F:
            {
                cpu_state->registers.a = cpu_state->registers.a >> 1 | (f_register_get_c() << 7);

                cpu_state->pc += 1;
                break;
            }
            // JR NZ, imm8
            case 0x20:
            {
                if (!f_register_get_z())
                {
                    cpu_state->pc += (i8)cpu_state->memory[cpu_state->pc+1];
                }
                cpu_state->pc += 2;
                break;
            }
            // LD HL, imm16
            case 0x21:
            {
                cpu_state->registers.l = cpu_state->memory[cpu_state->pc+1]; // lo
                cpu_state->registers.h = cpu_state->memory[cpu_state->pc+2]; // hi

                cpu_state->pc += 3;
                break;
            }
            // LD[HL+], A
            case 0x22:
            {
                u16 address = get_HL();
                if ((address < WRAM_START || address > WRAM_END) && (address < HRAM_START || address > HRAM_END))
                {
                    cpu_state->pc += 1;
                    break;
                }
                cpu_state->memory[address] = cpu_state->registers.a;
                set_HL(address + 1);
                cpu_state->pc += 1;
                break;
            }
            // INC HL
            case 0x23:
            {
                set_HL(get_HL() + 1);
                cpu_state->pc += 1;
                break;
            }
            // INC H
            case 0x24:
            {
                cpu_state->registers.h += 1;

                f_register_set_z(cpu_state->registers.h == 0);
                f_register_set_n(false);
                f_register_set_h((((cpu_state->registers.h-1) & 0x0F) + 1) > 0x0F);

                cpu_state->pc += 1;
                break;
            }
            // DEC H
            case 0x25:
            {
                cpu_state->registers.h -= 1;

                f_register_set_z(cpu_state->registers.h == 0);
                f_register_set_n(true);
                f_register_set_h(((cpu_state->registers.h+1) & 0x0F) < 1);

                cpu_state->pc += 1;
                break;
            }
            // LD H, imm8
            case 0x26:
            {
                cpu_state->registers.h = cpu_state->memory[cpu_state->pc+1];
                cpu_state->pc += 2;
                break;
            }
            // DAA
            case 0x27:
            {
                u8 a_value = cpu_state->registers.a;

                b8 subtract = f_register_get_n();
                b8 half_carry = f_register_get_h();
                b8 carry = f_register_get_c();

                u8 offset = 0;

                if ((!subtract && a_value & 0x0F > 0x09) || half_carry)
                {
                    offset += 0x06;
                }

                if ((!subtract && a_value & 0xF0 > 0x99) || carry)
                {
                    offset += 0x60;
                    f_register_set_c(true);
                }

                if (subtract)
                {
                    cpu_state->registers.a -= offset;
                }
                else
                {
                    cpu_state->registers.a += offset;
                }

                f_register_set_z(cpu_state->registers.a == 0);
                f_register_set_h(false);
            }
            // JR Z, imm8
            case 0x28:
            {
                if (f_register_get_z())
                {
                    cpu_state->pc += (i8)cpu_state->memory[cpu_state->pc+1];
                }

                cpu_state->pc += 2;
                break;
            }
            // ADD HL, HL
            case 0x29:
            {
                u16 hl = get_HL();

                set_HL(hl + hl);

                f_register_set_n(false);
                f_register_set_h(((hl & 0xFFF) + (hl & 0xFFF)) > 0xFFF);
                f_register_set_c((hl+hl) > 0xFFFF);

                cpu_state->pc += 1;
                break;
            }
            // LD A, [HL+]
            case 0x2A:
            {
                u16 hl = get_HL();
                cpu_state->registers.a = cpu_state->memory[hl];

                set_HL(hl + 1);
                cpu_state->pc += 1;
                break;
            }
            // DEC HL
            case 0x2B:
            {
                set_HL(get_HL()-1);

                cpu_state->pc += 1;
                break;
            }
            // INC L
            case 0x2C:
            {
                cpu_state->registers.l += 1;

                f_register_set_z(cpu_state->registers.l == 0);
                f_register_set_n(false);
                f_register_set_h((((cpu_state->registers.l-1) & 0x0F) + 1) > 0x0F);

                cpu_state->pc += 1;
                break;
            }
            // DEC L
            case 0x2D:
            {
                cpu_state->registers.l -= 1;

                f_register_set_z(cpu_state->registers.l == 0);
                f_register_set_n(true);
                f_register_set_h(((cpu_state->registers.l+1) & 0x0F) < 1);

                cpu_state->pc += 1;
                break;
            }
            // LD L, imm8
            case 0x2E:
            {
                cpu_state->registers.l = cpu_state->memory[cpu_state->pc+1];
                cpu_state->pc += 2;
                break;
            }
            // CPL
            case 0x2F:
            {
                cpu_state->registers.a = ~(cpu_state->registers.a);

                f_register_set_n(true);
                f_register_set_h(true);

                cpu_state->pc += 1;
                break;
            }
            // JR NC, imm8
            case 0x30:
            {
                if (!f_register_get_c())
                {
                    cpu_state->pc += (i8)cpu_state->memory[cpu_state->pc+1];
                }
                cpu_state->pc += 2;
                break;
            }
            // TODO: LD SP, imm16
            case 0x31:
            {

                u16 l = cpu_state->memory[cpu_state->pc+1]; // lo
                u16 h = cpu_state->memory[cpu_state->pc+2]; // hi

                cpu_state->sp = (l | h << 8);
                cpu_state->pc += 3;
                break;
            }
            // LD [HL-], A
            case 0x32:
            {
                u16 address = get_HL();
                if ((address < WRAM_START || address > WRAM_END) && (address < HRAM_START || address > HRAM_END))
                {
                    cpu_state->pc += 1;
                    break;
                }
                cpu_state->memory[address] = cpu_state->registers.a;
                set_HL(address - 1);

                cpu_state->pc += 1;
                break;
            }
            // INC SP
            case 0x33:
            {
                cpu_state->sp += 1;
                cpu_state->pc += 1;
                break;
            }
            // INC [HL]
            case 0x34:
            {

                u8 value = ++cpu_state->memory[get_HL()];

                f_register_set_z(value == 0);
                f_register_set_n(false);
                f_register_set_h((((value-1) & 0x0F) + 1) > 0x0F);

                cpu_state->pc += 1;
                break;
            }
            // DEC [HL]
            case 0x35:
            {
                u8 value = --cpu_state->memory[get_HL()];

                f_register_set_z(value == 0);
                f_register_set_n(true);
                f_register_set_h(((value+1) & 0x0F) < 1);

                cpu_state->pc += 1;
                break;
            }
            // LD [HL], imm8
            case 0x36:
            {
                cpu_state->memory[get_HL()] = cpu_state->memory[cpu_state->pc+1];
                cpu_state->pc += 2;
                break;
            }
            // SCF
            case 0x37:
            {
                f_register_set_n(false);
                f_register_set_h(false);
                f_register_set_c(true);
            }
            // JR C, imm8
            case 0x38:
            {
                if (f_register_get_c())
                {
                    cpu_state->pc += (i8)cpu_state->memory[cpu_state->pc+1];
                }
                cpu_state->pc += 2;
                break;
            }
            // ADD HL, SP
            case 0x39:
            {
                u16 hl = get_HL();
                u16 sp = cpu_state->sp;

                set_HL(hl + sp);

                f_register_set_n(false);
                f_register_set_h(((hl & 0xFFF) + (sp & 0xFFF)) > 0xFFF);
                f_register_set_c((hl+sp) > 0xFFFF);

                cpu_state->pc += 1;
                break;
            }
            // LD A, [HL-]
            case 0x3A:
            {
                u16 hl = get_HL();
                cpu_state->registers.a = cpu_state->memory[hl];

                set_HL(hl - 1);
                cpu_state->pc += 1;
                break;
            }
            // DEC SP
            case 0x3B:
            {
                cpu_state->sp =- 1;

                cpu_state->pc += 1;
                break;
            }
            // INC A
            case 0x3C:
            {
                cpu_state->registers.a += 1;

                f_register_set_z(cpu_state->registers.a == 0);
                f_register_set_n(false);
                f_register_set_h((((cpu_state->registers.a-1) & 0x0F) + 1) > 0x0F);

                cpu_state->pc += 1;
                break;
            }
            // DEC A
            case 0x3D:
            {
                cpu_state->registers.a -= 1;

                f_register_set_z(cpu_state->registers.a == 0);
                f_register_set_n(true);
                f_register_set_h(((cpu_state->registers.a+1) & 0x0F) < 1);

                cpu_state->pc += 1;
            }
            // LD A, imm8
            case 0x3E:
            {
                cpu_state->registers.a = cpu_state->memory[cpu_state->pc+1];
                cpu_state->pc += 2;
                break;
            }
            // CCF
            case 0x3F:
            {
                f_register_set_n(false);
                f_register_set_h(false);
                f_register_set_c(!f_register_get_c());
            }
            // LD B, B
            case 0x40:
            {
                cpu_state->registers.b = cpu_state->registers.b;
                cpu_state->pc += 1;
                break;
            }
            // ADD A, B
            case 0x80:
            {
                cpu_state->registers.a += cpu_state->registers.b;
                cpu_state->pc += 1;
                break;
            }
            // POP BC
            case 0xC1:
            {
                cpu_state->registers.c = cpu_state->memory[cpu_state->sp++]; // lo
                cpu_state->registers.b = cpu_state->memory[cpu_state->sp++]; // hi

                cpu_state->pc += 1;
                break;
            }
            // PUSH BC
            case 0xC5:
            {
                cpu_state->memory[--cpu_state->sp] = cpu_state->registers.b; // hi
                cpu_state->memory[--cpu_state->sp] = cpu_state->registers.c; // lo

                cpu_state->pc += 1;
                break;
            }
            // POP DE
            case 0xD1:
            {
                cpu_state->registers.e = cpu_state->memory[cpu_state->sp++]; // lo
                cpu_state->registers.d = cpu_state->memory[cpu_state->sp++]; // hi

                cpu_state->pc += 1;
                break;
            }
            // PUSH DE
            case 0xD5:
            {
                cpu_state->memory[--cpu_state->sp] = cpu_state->registers.d; // hi
                cpu_state->memory[--cpu_state->sp] = cpu_state->registers.e; // lo

                cpu_state->pc += 1;
                break;
            }
            // POP HL
            case 0xE1:
            {
                cpu_state->registers.l = cpu_state->memory[cpu_state->sp++]; // lo
                cpu_state->registers.h = cpu_state->memory[cpu_state->sp++]; // hi

                cpu_state->pc += 1;
                break;
            }
            // PUSH HL
            case 0xE5:
            {
                cpu_state->memory[--cpu_state->sp] = cpu_state->registers.h; // hi
                cpu_state->memory[--cpu_state->sp] = cpu_state->registers.l; // lo

                cpu_state->pc += 1;
                break;
            }
            // POP AF
            case 0xF1:
            {
                cpu_state->registers.f = cpu_state->memory[cpu_state->sp++]; // lo
                cpu_state->registers.a = cpu_state->memory[cpu_state->sp++]; // hi

                cpu_state->pc += 1;
                break;
            }
            // PUSH AF
            case 0xF5:
            {
                cpu_state->memory[--cpu_state->sp] = cpu_state->registers.a; // hi
                cpu_state->memory[--cpu_state->sp] = cpu_state->registers.f; // lo

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
    cpu_state->registers.a = (u8)((value & 0xFF00) >> 8); // hi
    cpu_state->registers.b = (u8)(value & 0xFF); // lo
}

static u16 get_BC()
{
    return ((u16)cpu_state->registers.b << 8) | cpu_state->registers.c;
}

static void set_BC(u16 value)
{
    cpu_state->registers.b = (u8)((value & 0xFF00) >> 8); // hi
    cpu_state->registers.c = (u8)(value & 0xFF); // lo
}

static u16 get_DE()
{
    return ((u16)cpu_state->registers.d << 8) | cpu_state->registers.e;
}

static void set_DE(u16 value)
{
    cpu_state->registers.d = (u8)((value & 0xFF00) >> 8); // hi
    cpu_state->registers.e = (u8)(value & 0xFF); // lo
}

static u16 get_HL()
{
    return ((u16)cpu_state->registers.h << 8) | cpu_state->registers.l;
}

static void set_HL(u16 value)
{
    cpu_state->registers.h = (u8)((value & 0xFF00) >> 8); // hi
    cpu_state->registers.l = (u8)(value & 0xFF); // lo
}
