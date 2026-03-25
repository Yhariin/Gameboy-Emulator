static void cpu_init(Arena *arena, u8* rom_data, u64 rom_size)
{
    cpu_state = (CPU_State *)arena_alloc_align(arena, sizeof(CPU_State), sizeof(CPU_State));
    cpu_state->memory = (u8 *)arena_alloc_align(arena, 0xFFFF, sizeof(u8));
    cpu_state->sp = 0xFFFE;
    cpu_state->is_halted = false;

    ASSERT(rom_size <= 0x8000, "ERROR Rom size must be at most 32kb!");
    memcpy(cpu_state->memory, rom_data, rom_size);

}

static void cpu_process()
{

    while(cpu_state->pc < 0x8000)
    {
        if (cpu_state->is_halted)
        {
            continue;
        }

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

                f_register_set_z(false);
                f_register_set_n(false);
                f_register_set_h(false);

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

                f_register_set_z(false);
                f_register_set_n(false);
                f_register_set_h(false);

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
                f_register_set_c(bit8 & cpu_state->registers.a);
                cpu_state->registers.a = cpu_state->registers.a << 1 | f_register_get_c();

                f_register_set_z(false);
                f_register_set_n(false);
                f_register_set_h(false);

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
                f_register_set_c(bit1 & cpu_state->registers.a);
                cpu_state->registers.a = cpu_state->registers.a >> 1 | (f_register_get_c() << 7);

                f_register_set_z(false);
                f_register_set_n(false);
                f_register_set_h(false);

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
            // LD reg, reg
            case 0x40:
            case 0x41:
            case 0x42:
            case 0x43:
            case 0x44:
            case 0x45:
            case 0x46:
            case 0x47:
            case 0x48:
            case 0x49:
            case 0x4A:
            case 0x4B:
            case 0x4C:
            case 0x4D:
            case 0x4E:
            case 0x4F:
            case 0x50:
            case 0x51:
            case 0x52:
            case 0x53:
            case 0x54:
            case 0x55:
            case 0x56:
            case 0x57:
            case 0x58:
            case 0x59:
            case 0x5A:
            case 0x5B:
            case 0x5C:
            case 0x5D:
            case 0x5E:
            case 0x5F:
            case 0x60:
            case 0x61:
            case 0x62:
            case 0x63:
            case 0x64:
            case 0x65:
            case 0x66:
            case 0x67:
            case 0x68:
            case 0x69:
            case 0x6A:
            case 0x6B:
            case 0x6C:
            case 0x6D:
            case 0x6E:
            case 0x6F:
            case 0x70:
            case 0x71:
            case 0x72:
            case 0x73:
            case 0x74:
            case 0x75:
            // TODO: HALT
            case 0x76:
            {
                cpu_state->is_halted = true;

                cpu_state->pc += 1;
                break;
            }
            case 0x77:
            case 0x78:
            case 0x79:
            case 0x7A:
            case 0x7B:
            case 0x7C:
            case 0x7D:
            case 0x7E:
            case 0x7F:
            {
                u8 instruction = cpu_state->memory[cpu_state->pc];

                u8 src, dst;

                if ((instruction & 0x40) == 0x40)
                {
                    if ((instruction & 0x0F) < 0x08)
                    {
                        dst = cpu_state->registers.b;
                    }
                    else
                    {
                        dst = cpu_state->registers.c;
                    }
                }
                else if ((instruction & 0x50) == 0x50)
                {
                    if ((instruction & 0x0F) < 0x08)
                    {
                        dst = cpu_state->registers.d;
                    }
                    else
                    {
                        dst = cpu_state->registers.e;
                    }
                }
                else if ((instruction & 0x60) == 0x60)
                {
                    if ((instruction & 0x0F) < 0x08)
                    {
                        dst = cpu_state->registers.h;
                    }
                    else
                    {
                        dst = cpu_state->registers.l;
                    }
                }
                else if ((instruction & 0x70) == 0x70)
                {
                    if ((instruction & 0x0F) < 0x08)
                    {
                        dst = cpu_state->memory[get_HL()];
                    }
                    else
                    {
                        dst = cpu_state->registers.a;
                    }
                }

                switch((instruction & 0x0F) % 8)
                {
                    case 0:
                        src = cpu_state->registers.b;
                        break;
                    case 1:
                        src = cpu_state->registers.c;
                        break;
                    case 2:
                        src = cpu_state->registers.d;
                        break;
                    case 3:
                        src = cpu_state->registers.e;
                        break;
                    case 4:
                        src = cpu_state->registers.h;
                        break;
                    case 5:
                        src = cpu_state->registers.l;
                        break;
                    case 6:
                        src = cpu_state->memory[get_HL()];
                        break;
                    case 7:
                        src = cpu_state->registers.a;
                        break;
                }

                dst = src;

                cpu_state->pc += 1;
                break;
            }
            // ADD A, reg
            case 0x80:
            case 0x81:
            case 0x82:
            case 0x83:
            case 0x84:
            case 0x85:
            case 0x86:
            case 0x87:
            // ADC A, reg
            case 0x88:
            case 0x89:
            case 0x8A:
            case 0x8B:
            case 0x8C:
            case 0x8D:
            case 0x8E:
            case 0x8F:
            // SUB A, reg
            case 0x90:
            case 0x91:
            case 0x92:
            case 0x93:
            case 0x94:
            case 0x95:
            case 0x96:
            case 0x97:
            // ADC A, reg
            case 0x98:
            case 0x99:
            case 0x9A:
            case 0x9B:
            case 0x9C:
            case 0x9D:
            case 0x9E:
            case 0x9F:

            {
                u8 instruction = cpu_state->memory[cpu_state->pc];
                u8 src;
                switch(instruction & 0x0F)
                {
                    case 0x00:
                        src = cpu_state->registers.b;
                        break;
                    case 0x01:
                        src = cpu_state->registers.c;
                        break;
                    case 0x02:
                        src = cpu_state->registers.d;
                        break;
                    case 0x03:
                        src = cpu_state->registers.e;
                        break;
                    case 0x04:
                        src = cpu_state->registers.h;
                        break;
                    case 0x05:
                        src = cpu_state->registers.l;
                        break;
                    case 0x06:
                        src = cpu_state->memory[get_HL()];
                        break;
                    case 0x07:
                        src = cpu_state->registers.a;
                        break;
                    case 0x08:
                        src = cpu_state->registers.b + (u8)f_register_get_c();
                    case 0x09:
                        src = cpu_state->registers.c + (u8)f_register_get_c();
                    case 0x0A:
                        src = cpu_state->registers.d + (u8)f_register_get_c();
                    case 0x0B:
                        src = cpu_state->registers.e + (u8)f_register_get_c();
                    case 0x0C:
                        src = cpu_state->registers.h + (u8)f_register_get_c();
                    case 0x0D:
                        src = cpu_state->registers.l + (u8)f_register_get_c();
                    case 0x0E:
                        src = cpu_state->memory[get_HL()] + (u8)f_register_get_c();
                    case 0x0F:
                        src = cpu_state->registers.a + (u8)f_register_get_c();
                }

                if (instruction & 0x80 == 0x80) // Addition
                {
                    cpu_state->registers.a += src;

                    f_register_set_z(cpu_state->registers.a == 0);
                    f_register_set_n(false);
                    f_register_set_h((((cpu_state->registers.a-src) & 0x0F) + (src & 0x0F)) > 0x0F);
                    f_register_set_c(cpu_state->registers.a > 0xFF);
                }
                else // Subtraction
                {
                    cpu_state->registers.a -= src;

                    f_register_set_z(cpu_state->registers.a == 0);
                    f_register_set_n(true);
                    f_register_set_h(((cpu_state->registers.a+src) & 0x0F) < (src & 0x0F));
                    f_register_set_c(cpu_state->registers.a < 0xFF);
                }

                cpu_state->pc += 1;
                break;
            }
            case 0xA0:
            case 0xA1:
            case 0xA2:
            case 0xA3:
            case 0xA4:
            case 0xA5:
            case 0xA6:
            case 0xA7:
            case 0xA8:
            case 0xA9:
            case 0xAA:
            case 0xAB:
            case 0xAC:
            case 0xAD:
            case 0xAE:
            case 0xAF:
            case 0xB0:
            case 0xB1:
            case 0xB2:
            case 0xB3:
            case 0xB4:
            case 0xB5:
            case 0xB6:
            case 0xB7:
            case 0xB8:
            case 0xB9:
            case 0xBA:
            case 0xBB:
            case 0xBC:
            case 0xBD:
            case 0xBE:
            case 0xBF:
            {
                u8 instruction = cpu_state->memory[cpu_state->pc];
                u8 src;
                switch ((instruction & 0x0F) % 8)
                {
                    case 0x00:
                        src = cpu_state->registers.b;
                        break;
                    case 0x01:
                        src = cpu_state->registers.c;
                        break;
                    case 0x02:
                        src = cpu_state->registers.d;
                        break;
                    case 0x03:
                        src = cpu_state->registers.e;
                        break;
                    case 0x04:
                        src = cpu_state->registers.h;
                        break;
                    case 0x05:
                        src = cpu_state->registers.l;
                        break;
                    case 0x06:
                        src = cpu_state->memory[get_HL()];
                        break;
                    case 0x07:
                        src = cpu_state->registers.a;
                        break;
                }

                if (instruction & 0xA0 == 0xA0)
                {
                    // AND reg
                    if (instruction & 0x0F < 0x08)
                    {
                        cpu_state->registers.a &= src;

                        f_register_set_z(cpu_state->registers.a == 0);
                        f_register_set_n(false);
                        f_register_set_h(true);
                        f_register_set_c(false);
                    }
                    // XOR reg
                    else
                    {
                        cpu_state->registers.a ^= src;

                        f_register_set_z(cpu_state->registers.a == 0);
                        f_register_set_n(false);
                        f_register_set_h(false);
                        f_register_set_c(false);
                    }
                }
                else
                {
                    // OR reg
                    if (instruction & 0x0F < 0x08)
                    {
                        cpu_state->registers.a |= src;

                        f_register_set_z(cpu_state->registers.a == 0);
                        f_register_set_n(false);
                        f_register_set_h(false);
                        f_register_set_c(false);
                    }
                    // CP reg
                    else
                    {
                        f_register_set_z((cpu_state->registers.a - src) == 0);
                        f_register_set_n(true);
                        f_register_set_h(((cpu_state->registers.a+src) & 0x0F) < (src & 0x0F));
                        f_register_set_c(cpu_state->registers.a < 0xFF);
                    }

                }

                cpu_state->pc += 1;
                break;
            }
            // RET NZ
            case 0xC0:
            {
                if (!f_register_get_z())
                {
                    u16 l = cpu_state->memory[cpu_state->sp++];
                    u16 h = cpu_state->memory[cpu_state->sp++];

                    cpu_state->pc = (l | h << 8);
                }
                else
                {
                    cpu_state->pc += 1;
                }

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
            // JP NZ, imm16
            case 0xC2:
            {
                if (!f_register_get_z())
                {
                    u16 l = cpu_state->memory[cpu_state->pc+1];
                    u16 h = cpu_state->memory[cpu_state->pc+2];

                    cpu_state->pc = (l | h << 8);
                }
                else
                {
                    cpu_state->pc += 3;
                }

                break;
            }
            // JP imm16
            case 0xC3:
            {
                u16 l = cpu_state->memory[cpu_state->pc+1];
                u16 h = cpu_state->memory[cpu_state->pc+2];

                cpu_state->pc = (l | h << 8);

                break;
            }
            // CALL NZ, imm16
            case 0xC4:
            {
                if (!f_register_get_z())
                {
                    u16 ret_addr = cpu_state->pc + 3;
                    cpu_state->memory[--cpu_state->sp] = (ret_addr & 0xFF00) >> 8;
                    cpu_state->memory[--cpu_state->sp] = (ret_addr & 0x00FF);

                    u16 l = cpu_state->memory[cpu_state->pc+1];
                    u16 h = cpu_state->memory[cpu_state->pc+2];

                    cpu_state->pc = (l | h << 8);
                }
                else
                {
                    cpu_state->pc += 3;
                }

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
            // ADD A, imm8
            case 0xC6:
            {
                u8 src = cpu_state->memory[cpu_state->pc+1];
                cpu_state->registers.a += src;

                f_register_set_z(cpu_state->registers.a == 0);
                f_register_set_n(false);
                f_register_set_h((((cpu_state->registers.a-src) & 0x0F) + (src & 0x0F)) > 0x0F);
                f_register_set_c(cpu_state->registers.a > 0xFF);

                cpu_state->pc += 2;
                break;
            }
            // RST 00H
            case 0xC7:
            {
                u16 ret_addr = cpu_state->pc + 1;
                cpu_state->memory[--cpu_state->sp] = (ret_addr & 0xFF00) >> 8;
                cpu_state->memory[--cpu_state->sp] = (ret_addr & 0x00FF);

                cpu_state->pc = 0x0000;
                break;
            }
            // RET Z
            case 0xC8:
            {
                if (f_register_get_z())
                {
                    u16 l = cpu_state->memory[cpu_state->sp++];
                    u16 h = cpu_state->memory[cpu_state->sp++];

                    cpu_state->pc = (l | h << 8);
                }
                else
                {
                    cpu_state->pc += 1;
                }

                break;
            }
            // RET
            case 0xC9:
            {
                u16 l = cpu_state->memory[cpu_state->sp++];
                u16 h = cpu_state->memory[cpu_state->sp++];

                cpu_state->pc = (l | h << 8);

                break;
            }
            // JP Z, imm16
            case 0xCA:
            {
                if (f_register_get_z())
                {
                    u16 l = cpu_state->memory[cpu_state->pc+1];
                    u16 h = cpu_state->memory[cpu_state->pc+2];

                    cpu_state->pc = (l | h << 8);
                }
                else
                {
                    cpu_state->pc += 3;
                }

                break;
            }
            // 16-bit opcodes with 0xCB prefix
            case 0xCB:
            {
                process_16_bit_opcodes(cpu_state->memory[cpu_state->pc+1]);

                cpu_state->pc += 2;
                break;
            }
            // CALL Z, imm16
            case 0xCC:
            {
                if (f_register_get_z())
                {
                    u16 ret_addr = cpu_state->pc + 3;
                    cpu_state->memory[--cpu_state->sp] = (ret_addr & 0xFF00) >> 8;
                    cpu_state->memory[--cpu_state->sp] = (ret_addr & 0x00FF);

                    u16 l = cpu_state->memory[cpu_state->pc+1];
                    u16 h = cpu_state->memory[cpu_state->pc+2];

                    cpu_state->pc = (l | h << 8);
                }
                else
                {
                    cpu_state->pc += 3;
                }

                break;
            }
            // CALL imm16
            case 0xCD:
            {
                u16 ret_addr = cpu_state->pc + 3;
                cpu_state->memory[--cpu_state->sp] = (ret_addr & 0xFF00) >> 8;
                cpu_state->memory[--cpu_state->sp] = (ret_addr & 0x00FF);

                u16 l = cpu_state->memory[cpu_state->pc+1];
                u16 h = cpu_state->memory[cpu_state->pc+2];

                cpu_state->pc = (l | h << 8);

                break;
            }
            // ADC A, imm8
            case 0xCE:
            {
                u8 src = cpu_state->memory[cpu_state->pc+1] + (u8)f_register_get_c();
                cpu_state->registers.a += src;

                f_register_set_z(cpu_state->registers.a == 0);
                f_register_set_n(false);
                f_register_set_h((((cpu_state->registers.a-src) & 0x0F) + (src & 0x0F)) > 0x0F);
                f_register_set_c(cpu_state->registers.a > 0xFF);

                break;
            }
            // RST 08H
            {
                u16 ret_addr = cpu_state->pc + 1;
                cpu_state->memory[--cpu_state->sp] = (ret_addr & 0xFF00) >> 8;
                cpu_state->memory[--cpu_state->sp] = (ret_addr & 0x00FF);

                cpu_state->pc = 0x0080;
                break;

            }
            // RET NC
            case 0xD0:
            {
                if (!f_register_get_c())
                {
                    u16 l = cpu_state->memory[cpu_state->sp++];
                    u16 h = cpu_state->memory[cpu_state->sp++];

                    cpu_state->pc = (l | h << 8);
                }
                else
                {
                    cpu_state->pc += 1;
                }

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
            // JP NC, imm16
            case 0xD2:
            {
                if (!f_register_get_c())
                {
                    u16 l = cpu_state->memory[cpu_state->pc+1];
                    u16 h = cpu_state->memory[cpu_state->pc+2];

                    cpu_state->pc = (l | h << 8);
                }
                else
                {
                    cpu_state->pc += 3;
                }

                break;
            }
            // CALL NC, imm16
            case 0xD4:
            {
                if (!f_register_get_c())
                {
                    u16 ret_addr = cpu_state->pc + 3;
                    cpu_state->memory[--cpu_state->sp] = (ret_addr & 0xFF00) >> 8;
                    cpu_state->memory[--cpu_state->sp] = (ret_addr & 0x00FF);

                    u16 l = cpu_state->memory[cpu_state->pc+1];
                    u16 h = cpu_state->memory[cpu_state->pc+2];

                    cpu_state->pc = (l | h << 8);
                }
                else
                {
                    cpu_state->pc += 3;
                }

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
            // SUB imm8
            case 0xD6:
            {
                u8 src = cpu_state->memory[cpu_state->pc+1];
                cpu_state->registers.a -= src;

                f_register_set_z(cpu_state->registers.a == 0);
                f_register_set_n(true);
                f_register_set_h(((cpu_state->registers.a+src) & 0x0F) < (src & 0x0F));
                f_register_set_c(cpu_state->registers.a < 0xFF);

                cpu_state->pc += 2;
                break;
            }
            // RST 10H
            case 0xD7:
            {
                u16 ret_addr = cpu_state->pc + 1;
                cpu_state->memory[--cpu_state->sp] = (ret_addr & 0xFF00) >> 8;
                cpu_state->memory[--cpu_state->sp] = (ret_addr & 0x00FF);

                cpu_state->pc = 0x0010;
                break;
            }
            // RET C
            case 0xD8:
            {
                if (f_register_get_c())
                {
                    u16 l = cpu_state->memory[cpu_state->sp++];
                    u16 h = cpu_state->memory[cpu_state->sp++];

                    cpu_state->pc = (l | h << 8);
                }
                else
                {
                    cpu_state->pc += 1;
                }

                break;
            }
            // TODO: RETI
            case 0xD9:
            {
                cpu_state->pc += 1;
                break;
            }
            // JP C, imm16
            case 0xDA:
            {
                if (f_register_get_c())
                {
                    u16 l = cpu_state->memory[cpu_state->pc+1];
                    u16 h = cpu_state->memory[cpu_state->pc+2];

                    cpu_state->pc = (l | h << 8);
                }
                else
                {
                    cpu_state->pc += 3;
                }

                break;
            }
            // CALL C, imm16
            case 0xDC:
            {
                if (f_register_get_c())
                {
                    u16 ret_addr = cpu_state->pc + 3;
                    cpu_state->memory[--cpu_state->sp] = (ret_addr & 0xFF00) >> 8;
                    cpu_state->memory[--cpu_state->sp] = (ret_addr & 0x00FF);

                    u16 l = cpu_state->memory[cpu_state->pc+1];
                    u16 h = cpu_state->memory[cpu_state->pc+2];

                    cpu_state->pc = (l | h << 8);
                }
                else
                {
                    cpu_state->pc += 3;
                }

                break;
            }
            // SBC A, imm8
            case 0xDE:
            {
                u8 src = cpu_state->memory[cpu_state->pc+1] + (u8)f_register_get_c();
                cpu_state->registers.a -= src;

                f_register_set_z(cpu_state->registers.a == 0);
                f_register_set_n(true);
                f_register_set_h(((cpu_state->registers.a+src) & 0x0F) < (src & 0x0F));
                f_register_set_c(cpu_state->registers.a < 0xFF);

                cpu_state->pc += 2;
                break;
            }
            // RST 18H
            case 0xDF:
            {
                u16 ret_addr = cpu_state->pc + 1;
                cpu_state->memory[--cpu_state->sp] = (ret_addr & 0xFF00) >> 8;
                cpu_state->memory[--cpu_state->sp] = (ret_addr & 0x00FF);

                cpu_state->pc = 0x0018;
                break;
            }
            // LD [imm8], A
            case 0xE0:
            {
                u16 address = 0xFF00 | cpu_state->memory[cpu_state->pc+1];
                cpu_state->memory[address] = cpu_state->registers.a;

                cpu_state->pc += 2;
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
            // LD [C], A
            case 0xE2:
            {
                u16 address = 0xFF00 | cpu_state->registers.c;

                if ((address < WRAM_START || address > WRAM_END) && (address < HRAM_START || address > HRAM_END))
                {
                    cpu_state->pc += 1;
                    break;
                }

                cpu_state->memory[address] = cpu_state->registers.a;

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
            // AND imm8
            case 0xE6:
            {
                cpu_state->registers.a &= cpu_state->memory[cpu_state->pc+1];

                f_register_set_z(cpu_state->registers.a == 0);
                f_register_set_n(false);
                f_register_set_h(true);
                f_register_set_c(false);

                cpu_state->pc += 2;
                break;
            }
            // RST 20H
            case 0xE7:
            {
                u16 ret_addr = cpu_state->pc + 1;
                cpu_state->memory[--cpu_state->sp] = (ret_addr & 0xFF00) >> 8;
                cpu_state->memory[--cpu_state->sp] = (ret_addr & 0x00FF);

                cpu_state->pc = 0x0020;
                break;
            }
            // ADD SP, imm8
            case 0xE8:
            {
                u16 sp = cpu_state->sp;
                u16 imm8 = cpu_state->memory[cpu_state->pc+1];

                cpu_state->sp += imm8;

                f_register_set_z(false);
                f_register_set_n(false);
                f_register_set_h(((sp & 0xFFF) + (imm8 & 0xFFF)) > 0xFFF);
                f_register_set_c((sp + imm8) > 0xFFFF);

                cpu_state->pc += 2;
                break;
            }
            // JP HL
            case 0xE9:
            {
                cpu_state->pc = get_HL();
                break;
            }
            // LD [imm16], A
            case 0xEA:
            {
                u16 lo = (u16)cpu_state->memory[cpu_state->pc+1];
                u16 hi = (u16)cpu_state->memory[cpu_state->pc+2] << 8;
                u16 address = hi | lo;

                if ((address < WRAM_START || address > WRAM_END) && (address < HRAM_START || address > HRAM_END))
                {
                    cpu_state->pc += 3;
                    break;
                }

                cpu_state->memory[address] = cpu_state->registers.a;

                cpu_state->pc += 3;
                break;
            }
            // XOR imm8
            case 0xEE:
            {
                cpu_state->registers.a ^= cpu_state->memory[cpu_state->pc+1];

                f_register_set_z(cpu_state->registers.a == 0);
                f_register_set_n(false);
                f_register_set_h(false);
                f_register_set_c(false);

                cpu_state->pc += 2;
                break;
            }
            // RST 28H
            case 0xEF:
            {
                u16 ret_addr = cpu_state->pc + 1;
                cpu_state->memory[--cpu_state->sp] = (ret_addr & 0xFF00) >> 8;
                cpu_state->memory[--cpu_state->sp] = (ret_addr & 0x00FF);

                cpu_state->pc = 0x0028;
                break;
            }
            // LD A, [imm8]
            case 0xF0:
            {
                u16 address = 0xFF00 | cpu_state->memory[cpu_state->pc+1];
                cpu_state->registers.a = cpu_state->memory[address];

                cpu_state->pc += 2;
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
            // LD A, [C]
            case 0xF2:
            {
                u16 address = 0xFF00 | cpu_state->registers.c;
                cpu_state->registers.a = cpu_state->memory[address];

                cpu_state->pc += 2;
                break;
            }
            // TODO: DI
            case 0xF3:
            {
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
            // OR imm8
            case 0xF6:
            {
                cpu_state->registers.a |= cpu_state->memory[cpu_state->pc+1];

                f_register_set_z(cpu_state->registers.a == 0);
                f_register_set_n(false);
                f_register_set_h(false);
                f_register_set_c(false);

                cpu_state->pc += 2;
                break;
            }
            // RST 30H
            case 0xF7:
            {
                u16 ret_addr = cpu_state->pc + 1;
                cpu_state->memory[--cpu_state->sp] = (ret_addr & 0xFF00) >> 8;
                cpu_state->memory[--cpu_state->sp] = (ret_addr & 0x00FF);

                cpu_state->pc = 0x0030;
                break;
            }
            // LD, SP+imm8
            case 0xF8:
            {
                u16 sp = cpu_state->sp;
                u16 imm8 = cpu_state->memory[cpu_state->pc+1];

                set_HL(sp + imm8);

                f_register_set_z(false);
                f_register_set_n(false);
                f_register_set_h(((sp & 0xFFF) + (imm8 & 0xFFF)) > 0xFFF);
                f_register_set_c((sp + imm8) > 0xFFFF);

                cpu_state->pc += 2;
                break;
            }
            // LD SP, HL
            case 0xF9:
            {
                cpu_state->sp = get_HL();

                cpu_state->pc += 1;
                break;
            }
            // LD A, [imm16]
            case 0xFA:
            {
                u16 lo = (u16)cpu_state->memory[cpu_state->pc+1];
                u16 hi = (u16)cpu_state->memory[cpu_state->pc+2] << 8;
                u16 address = hi | lo;

                cpu_state->registers.a = cpu_state->memory[address];

                cpu_state->pc += 3;
                break;
            }
            // TODO: EI
            case 0xFB:
            {
                cpu_state->pc += 1;
                break;
            }
            // CP imm8
            case 0xFE:
            {
                u8 imm8 = cpu_state->memory[cpu_state->pc+1];

                f_register_set_z((cpu_state->registers.a - imm8) == 0);
                f_register_set_n(true);
                f_register_set_h(((cpu_state->registers.a+imm8) & 0x0F) < (imm8 & 0x0F));
                f_register_set_c(cpu_state->registers.a < 0xFF);

                cpu_state->pc += 2;
                break;
            }
            // RST 38H
            case 0xFF:
            {
                u16 ret_addr = cpu_state->pc + 1;
                cpu_state->memory[--cpu_state->sp] = (ret_addr & 0xFF00) >> 8;
                cpu_state->memory[--cpu_state->sp] = (ret_addr & 0x00FF);

                cpu_state->pc = 0x0038;
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


static void process_16_bit_opcodes(u8 low)
{
    u8 *src;
    switch ((low & 0x0F) % 8)
    {
        case 0x00:
            src = &cpu_state->registers.b;
            break;
        case 0x01:
            src = &cpu_state->registers.c;
            break;
        case 0x02:
            src = &cpu_state->registers.d;
            break;
        case 0x03:
            src = &cpu_state->registers.e;
            break;
        case 0x04:
            src = &cpu_state->registers.h;
            break;
        case 0x05:
            src = &cpu_state->registers.l;
            break;
        case 0x06:
            src = &cpu_state->memory[get_HL()];
            break;
        case 0x07:
            src = &cpu_state->registers.a;
            break;
    }

    switch(low)
    {
        // Rotations
        // RLC
        case 0x00:
        case 0x01:
        case 0x02:
        case 0x03:
        case 0x04:
        case 0x05:
        case 0x06:
        case 0x07:
        //RRC
        case 0x08:
        case 0x09:
        case 0x0A:
        case 0x0B:
        case 0x0C:
        case 0x0D:
        case 0x0E:
        case 0x0F:
        // RL
        case 0x10:
        case 0x11:
        case 0x12:
        case 0x13:
        case 0x14:
        case 0x15:
        case 0x16:
        case 0x17:
        //RR
        case 0x18:
        case 0x19:
        case 0x1A:
        case 0x1B:
        case 0x1C:
        case 0x1D:
        case 0x1E:
        case 0x1F:
        {

            if (low >> 4 == 0x00) // 0x00
            {
                if (low & 0x0F < 0x08)
                {
                    // RLC src
                    f_register_set_c(bit8 & *src);
                    *src = *src << 1 | f_register_get_c();

                    f_register_set_z(*src == 0);
                    f_register_set_n(false);
                    f_register_set_h(false);
                }
                else
                {
                    // RRC
                    f_register_set_c(bit1 & *src);
                    *src = *src >> 1 | (f_register_get_c() << 7);

                    f_register_set_z(*src == 0);
                    f_register_set_n(false);
                    f_register_set_h(false);
                }
            }
            else // 0x10
            {
                if (low & 0x0F < 0x08)
                {
                    // RL src
                    f_register_set_c(bit8 & *src);
                    *src = *src << 1 | f_register_get_c();

                    f_register_set_z(*src == 0);
                    f_register_set_n(false);
                    f_register_set_h(false);
                }
                else
                {
                    // RR
                    f_register_set_c(bit1 & *src);
                    *src = *src >> 1 | (f_register_get_c() << 7);

                    f_register_set_z(*src == 0);
                    f_register_set_n(false);
                    f_register_set_h(false);
                }

            }
            break;
        }

        // Shifts
        // SLA
        case 0x20:
        case 0x21:
        case 0x22:
        case 0x23:
        case 0x24:
        case 0x25:
        case 0x26:
        case 0x27:
        // SRA
        case 0x28:
        case 0x29:
        case 0x2A:
        case 0x2B:
        case 0x2C:
        case 0x2D:
        case 0x2E:
        case 0x2F:
        {
            if (low & 0x0F < 0x08)
            {
                // SLA
                f_register_set_c(bit8 & *src);
                *src <<= 1;

                f_register_set_z(*src == 0);
                f_register_set_n(false);
                f_register_set_h(false);
            }
            else
            {
                // SRA
                u8 tmp = bit8 & *src;

                f_register_set_c(bit1 & *src);
                *src >>= 1;
                *src |= tmp;

                f_register_set_z(*src == 0);
                f_register_set_n(false);
                f_register_set_h(false);
            }

            break;
        }

        // SWAP
        case 0x30:
        case 0x31:
        case 0x32:
        case 0x33:
        case 0x34:
        case 0x35:
        case 0x36:
        case 0x37:
        // SRL
        case 0x38:
        case 0x39:
        case 0x3A:
        case 0x3B:
        case 0x3C:
        case 0x3D:
        case 0x3E:
        case 0x3F:
        {
            if (low & 0x0F < 0x08)
            {
                // SWAP
                u8 low_4 = *src & 0x07;
                u8 hi_4 = *src >> 4;
                *src = (low_4 << 4) | hi_4;

                f_register_set_z(*src == 0);
                f_register_set_n(false);
                f_register_set_h(false);
                f_register_set_c(false);
            }
            else
            {
                // SRL
                f_register_set_c(bit1 & *src);
                *src >>= 1;

                f_register_set_z(*src == 0);
                f_register_set_n(false);
                f_register_set_h(false);
            }

            break;
        }

        // BIT
        case 0x40:
        case 0x41:
        case 0x42:
        case 0x43:
        case 0x44:
        case 0x45:
        case 0x46:
        case 0x47:
        case 0x48:
        case 0x49:
        case 0x4A:
        case 0x4B:
        case 0x4C:
        case 0x4D:
        case 0x4E:
        case 0x4F:
        case 0x50:
        case 0x51:
        case 0x52:
        case 0x53:
        case 0x54:
        case 0x55:
        case 0x56:
        case 0x57:
        case 0x58:
        case 0x59:
        case 0x5A:
        case 0x5B:
        case 0x5C:
        case 0x5D:
        case 0x5E:
        case 0x5F:
        case 0x60:
        case 0x61:
        case 0x62:
        case 0x63:
        case 0x64:
        case 0x65:
        case 0x66:
        case 0x67:
        case 0x68:
        case 0x69:
        case 0x6A:
        case 0x6B:
        case 0x6C:
        case 0x6D:
        case 0x6E:
        case 0x6F:
        case 0x70:
        case 0x71:
        case 0x72:
        case 0x73:
        case 0x74:
        case 0x75:
        case 0x76:
        case 0x77:
        case 0x78:
        case 0x79:
        case 0x7A:
        case 0x7B:
        case 0x7C:
        case 0x7D:
        case 0x7E:
        case 0x7F:
        {
            if (low >> 4 == 0x04) // 0x40
            {
                if (low & 0x0F < 0x08)
                {
                    // BIT 0, src
                    f_register_set_z(~(bit1 & *src));
                    f_register_set_n(false);
                    f_register_set_h(true);
                }
                else
                {
                    // BIT 1, src
                    f_register_set_z(~(bit2 & *src));
                    f_register_set_n(false);
                    f_register_set_h(true);
                }
            }
            else if (low >> 4 == 0x05) // 0x50
            {
                if (low & 0x0F < 0x08)
                {
                    // BIT 2, src
                    f_register_set_z(~(bit3 & *src));
                    f_register_set_n(false);
                    f_register_set_h(true);

                }
                else
                {
                    // BIT 3, src
                    f_register_set_z(~(bit4 & *src));
                    f_register_set_n(false);
                    f_register_set_h(true);
                }

            }
            else if (low >> 4 == 0x05) // 0x60
            {
                if (low & 0x0F < 0x08)
                {
                    // BIT 4, src
                    f_register_set_z(~(bit5 & *src));
                    f_register_set_n(false);
                    f_register_set_h(true);

                }
                else
                {
                    // BIT 5, src
                    f_register_set_z(~(bit6 & *src));
                    f_register_set_n(false);
                    f_register_set_h(true);
                }

            }
            else // 0x70
            {
                if (low & 0x0F < 0x08)
                {
                    // BIT 6, src
                    f_register_set_z(~(bit7 & *src));
                    f_register_set_n(false);
                    f_register_set_h(true);

                }
                else
                {
                    // BIT 7, src
                    f_register_set_z(~(bit8 & *src));
                    f_register_set_n(false);
                    f_register_set_h(true);
                }

            }

            break;
        }
        // RES
        case 0x80:
        case 0x81:
        case 0x82:
        case 0x83:
        case 0x84:
        case 0x85:
        case 0x86:
        case 0x87:
        case 0x88:
        case 0x89:
        case 0x8A:
        case 0x8B:
        case 0x8C:
        case 0x8D:
        case 0x8E:
        case 0x8F:
        case 0x90:
        case 0x91:
        case 0x92:
        case 0x93:
        case 0x94:
        case 0x95:
        case 0x96:
        case 0x97:
        case 0x98:
        case 0x99:
        case 0x9A:
        case 0x9B:
        case 0x9C:
        case 0x9D:
        case 0x9E:
        case 0x9F:
        case 0xA0:
        case 0xA1:
        case 0xA2:
        case 0xA3:
        case 0xA4:
        case 0xA5:
        case 0xA6:
        case 0xA7:
        case 0xA8:
        case 0xA9:
        case 0xAA:
        case 0xAB:
        case 0xAC:
        case 0xAD:
        case 0xAE:
        case 0xAF:
        case 0xB0:
        case 0xB1:
        case 0xB2:
        case 0xB3:
        case 0xB4:
        case 0xB5:
        case 0xB6:
        case 0xB7:
        case 0xB8:
        case 0xB9:
        case 0xBA:
        case 0xBB:
        case 0xBC:
        case 0xBD:
        case 0xBE:
        case 0xBF:
        {
            if (low >> 4 == 0x08) // 0x80
            {
                if (low & 0x0F < 0x08)
                {
                    // RES 0, src
                    *src &= ~bit1;
                }
                else
                {
                    // RES 1, src
                    *src &= ~bit2;
                }
            }
            else if (low >> 4 == 0x09) // 0x90
            {
                if (low & 0x0F < 0x08)
                {
                    // RES 2, src
                    *src &= ~bit3;
                }
                else
                {
                    // RES 3, src
                    *src &= ~bit4;
                }
            }
            else if (low >> 4 == 0x0A) // 0xA0
            {
                if (low & 0x0F < 0x08)
                {
                    // RES 4, src
                    *src &= ~bit5;
                }
                else
                {
                    // RES 5, src
                    *src &= ~bit6;
                }
            }
            else // 0xB0
            {
                if (low & 0x0F < 0x08)
                {
                    // RES 6, src
                    *src &= ~bit7;
                }
                else
                {
                    // RES 7, src
                    *src &= ~bit8;
                }
            }

            break;
        }

        // SET
        case 0xC0:
        case 0xC1:
        case 0xC2:
        case 0xC3:
        case 0xC4:
        case 0xC5:
        case 0xC6:
        case 0xC7:
        case 0xC8:
        case 0xC9:
        case 0xCA:
        case 0xCB:
        case 0xCC:
        case 0xCD:
        case 0xCE:
        case 0xCF:
        case 0xD0:
        case 0xD1:
        case 0xD2:
        case 0xD3:
        case 0xD4:
        case 0xD5:
        case 0xD6:
        case 0xD7:
        case 0xD8:
        case 0xD9:
        case 0xDA:
        case 0xDB:
        case 0xDC:
        case 0xDD:
        case 0xDE:
        case 0xDF:
        case 0xE0:
        case 0xE1:
        case 0xE2:
        case 0xE3:
        case 0xE4:
        case 0xE5:
        case 0xE6:
        case 0xE7:
        case 0xE8:
        case 0xE9:
        case 0xEA:
        case 0xEB:
        case 0xEC:
        case 0xED:
        case 0xEE:
        case 0xEF:
        case 0xF0:
        case 0xF1:
        case 0xF2:
        case 0xF3:
        case 0xF4:
        case 0xF5:
        case 0xF6:
        case 0xF7:
        case 0xF8:
        case 0xF9:
        case 0xFA:
        case 0xFB:
        case 0xFC:
        case 0xFD:
        case 0xFE:
        case 0xFF:
        {
            if (low >> 4 == 0x0C) // 0xC0
            {
                if (low & 0x0F < 0x08)
                {
                    // SET 0, src
                    *src |= bit1;
                }
                else
                {
                    // SET 1, src
                    *src |= bit2;
                }
            }
            else if (low >> 4 == 0x0D) // 0xD0
            {
                if (low & 0x0F < 0x08)
                {
                    // SET 2, src
                    *src |= bit3;
                }
                else
                {
                    // SET 3, src
                    *src |= bit4;
                }
            }
            else if (low >> 4 == 0x0E) // 0xE0
            {
                if (low & 0x0F < 0x08)
                {
                    // SET 4, src
                    *src |= bit5;
                }
                else
                {
                    // SET 5, src
                    *src |= bit6;
                }
            }
            else // 0xF0
            {
                if (low & 0x0F < 0x08)
                {
                    // SET 6, src
                    *src |= bit7;
                }
                else
                {
                    // SET 7, src
                    *src |= bit8;
                }
            }

            break;
        }
    }
}