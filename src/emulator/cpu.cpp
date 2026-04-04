static void cpu_init(Arena *arena)
{
    cpu_state = (CPU_State *)arena_alloc_align(arena, sizeof(CPU_State), sizeof(CPU_State));
    cpu_state->memory = (u8 *)arena_alloc_align(arena, 0xFFFF, sizeof(u8));
    cpu_state->sp = 0xFFFE;
    cpu_state->ime = false;
    cpu_state->is_halted = false;
    cpu_state->timer.div_counter = 0;
    cpu_state->timer.tima_counter = 0;

    // ASSERT(rom->size <= 0x8000, "ERROR Cart size must be at most 32kb!");
    memcpy(cpu_state->memory, cart_state->data, cart_state->size);

}

static void cpu_post_boot_init()
{
    cpu_state->registers.a = 0x01;
    cpu_state->registers.f = 0xb0;
    cpu_state->registers.b = 0x00;
    cpu_state->registers.c = 0x13;
    cpu_state->registers.d = 0x00;
    cpu_state->registers.e = 0xD8;
    cpu_state->registers.h = 0x01;
    cpu_state->registers.l = 0x4D;

    cpu_state->sp = 0xFFFE;
    cpu_state->pc = 0x100;

    cpu_state->memory[Hardware_Registers_P1_JOYP]   = 0xCF;
    cpu_state->memory[Hardware_Registers_SB]        = 0x00;
    cpu_state->memory[Hardware_Registers_SC]        = 0x7E;
    cpu_state->memory[Hardware_Registers_DIV]       = 0xAB;
    cpu_state->memory[Hardware_Registers_TIMA]      = 0x00;
    cpu_state->memory[Hardware_Registers_TMA]       = 0x00;
    cpu_state->memory[Hardware_Registers_TAC]       = 0xF8;
    cpu_state->memory[Hardware_Registers_IF]        = 0xE1;
    cpu_state->memory[Hardware_Registers_NR10]      = 0x80;
    cpu_state->memory[Hardware_Registers_NR11]      = 0xBF;
    cpu_state->memory[Hardware_Registers_NR12]      = 0xF3;
    cpu_state->memory[Hardware_Registers_NR13]      = 0xFF;
    cpu_state->memory[Hardware_Registers_NR14]      = 0xBF;
    cpu_state->memory[Hardware_Registers_NR21]      = 0x3F;
    cpu_state->memory[Hardware_Registers_NR22]      = 0x00;
    cpu_state->memory[Hardware_Registers_NR23]      = 0xFF;
    cpu_state->memory[Hardware_Registers_NR24]      = 0xBF;
    cpu_state->memory[Hardware_Registers_NR30]      = 0x7F;
    cpu_state->memory[Hardware_Registers_NR31]      = 0xFF;
    cpu_state->memory[Hardware_Registers_NR32]      = 0x9F;
    cpu_state->memory[Hardware_Registers_NR33]      = 0xFF;
    cpu_state->memory[Hardware_Registers_NR34]      = 0xBF;
    cpu_state->memory[Hardware_Registers_NR41]      = 0xFF;
    cpu_state->memory[Hardware_Registers_NR42]      = 0x00;
    cpu_state->memory[Hardware_Registers_NR43]      = 0x00;
    cpu_state->memory[Hardware_Registers_NR44]      = 0xBF;
    cpu_state->memory[Hardware_Registers_NR50]      = 0x77;
    cpu_state->memory[Hardware_Registers_NR51]      = 0xF3;
    cpu_state->memory[Hardware_Registers_NR52]      = 0xF1;
    cpu_state->memory[Hardware_Registers_LCDC]      = 0x91;
    cpu_state->memory[Hardware_Registers_STAT]      = 0x85;
    cpu_state->memory[Hardware_Registers_SCY]       = 0x00;
    cpu_state->memory[Hardware_Registers_SCX]       = 0x00;
    cpu_state->memory[Hardware_Registers_LY]        = 0x00;
    cpu_state->memory[Hardware_Registers_LYC]       = 0x00;
    cpu_state->memory[Hardware_Registers_DMA]       = 0xFF;
    cpu_state->memory[Hardware_Registers_BGP]       = 0xFC;
    cpu_state->memory[Hardware_Registers_WY]        = 0x00;
    cpu_state->memory[Hardware_Registers_WX]        = 0x00;
    cpu_state->memory[Hardware_Registers_IE]        = 0x00;

#ifdef WRITE_LOG
    TempArena scratch = temp_arena_begin(get_scratch_arena());

    String current_state_log = string_format(scratch.arena, "A:%02X F:%02X B:%02X C:%02X D:%02X E:%02X H:%02X L:%02X SP:%04X PC:%04X PCMEM:%02X,%02X,%02X,%02X\n",
    cpu_state->registers.a, cpu_state->registers.f, cpu_state->registers.b, cpu_state->registers.c, cpu_state->registers.d,
    cpu_state->registers.e, cpu_state->registers.h, cpu_state->registers.l, cpu_state->sp, cpu_state->pc,
    cpu_state->memory[cpu_state->pc], cpu_state->memory[cpu_state->pc+1], cpu_state->memory[cpu_state->pc+2], cpu_state->memory[cpu_state->pc+3]);

    os_write_file(out_file, current_state_log.chars, current_state_log.size);

    temp_arena_end(scratch);
#endif
}

// Return number of cycles spent during the step
static u8 cpu_step()
{
    u8 cycles = 0;
    cycles += cpu_process_interrupts();
    if (cpu_state->is_halted)
    {
        return 4;
    }
    else
    {
        u8 instr = fetch_instr();
        cycles += execute_instr(instr);

        #ifdef WRITE_LOG
            TempArena scratch = temp_arena_begin(get_scratch_arena());

            String current_state_log = string_format(scratch.arena, "A:%02X F:%02X B:%02X C:%02X D:%02X E:%02X H:%02X L:%02X SP:%04X PC:%04X PCMEM:%02X,%02X,%02X,%02X\n",
            cpu_state->registers.a, cpu_state->registers.f, cpu_state->registers.b, cpu_state->registers.c, cpu_state->registers.d,
            cpu_state->registers.e, cpu_state->registers.h, cpu_state->registers.l, cpu_state->sp, cpu_state->pc,
            cpu_state->memory[cpu_state->pc], cpu_state->memory[cpu_state->pc+1], cpu_state->memory[cpu_state->pc+2], cpu_state->memory[cpu_state->pc+3]);

            os_write_file(out_file, current_state_log.chars, current_state_log.size);

            temp_arena_end(scratch);
        #endif
    }

    return cycles;
}

// TODO: Finish rest of memory reads
static u8 mem_read(u16 address)
{
    if (address <= ROM_END)
    {
        // ROM DATA
        return cart_read(address);
    }
    else if (address <= VRAM_END)
    {
        // VRAM
        // return ppu_vram_read();
        return cpu_state->memory[address];
    }
    else if (address <= SRAM_END)
    {
        // Cartridge RAM
        return cart_read(address);
    }
    else if (address <= WRAM_END)
    {
        // WRAM
        return cpu_state->memory[address];
    }
    else if (address <= ECHO_END)
    {
        // Unusuable echo RAM
        return 0;
    }
    else if (address <= OAM_END)
    {
        // OAM
        // return ppu_oam_read();
        return cpu_state->memory[address];
    }
    else if (address < IO_START)
    {
        // Unusuable RAM
        return 0;
    }
    else if (address <= IO_END)
    {
        // IO
        // return io_read();
        return cpu_state->memory[address];
    }
    else
    {
        return cpu_state->memory[address];
    }
}

static u8 *mem_read_ref(u16 address)
{
    return &(cpu_state->memory[address]);
}

// TODO: Finish rest of memory writes;
static void mem_write(u16 address, u8 data)
{
    if (address <= ROM_END)
    {
        // ROM DATA
        cart_write(address, data);
        // return cpu_state->memory[address];
    }
    else if (address <= VRAM_END)
    {
        // VRAM
        //  ppu_vram_read();
        cpu_state->memory[address] = data;
    }
    else if (address <= SRAM_END)
    {
        // Cartridge RAM
        cart_write(address, data);
    }
    else if (address <= WRAM_END)
    {
        // WRAM
        cpu_state->memory[address] = data;
    }
    else if (address <= ECHO_END)
    {
        // Unusuable echo RAM
    }
    else if (address <= OAM_END)
    {
        // OAM
        // return ppu_oam_read();
        cpu_state->memory[address] = data;
    }
    else if (address < IO_START)
    {
        // Unusuable RAM
    }
    else if (address <= IO_END)
    {
        // IO
        // return io_read();
        cpu_state->memory[address] = data;
    }
    else
    {
        cpu_state->memory[address] = data;
    }
}

static u8 fetch_instr()
{
    u8 instr = mem_read(cpu_state->pc);

    if (cpu_state->halt_bug)
    {
        cpu_state->halt_bug = false;
    }
    else
    {
        cpu_state->pc++;
    }

    return instr;
}

// Returns the number of clock cycles (T-cycles) taken during the insruction processing
static u8 execute_instr(u8 instr)
{
    switch(instr)
    {
        // NOP
        case 0x00:
        {
            return 4;
        }
        // LD BC, imm16
        case 0x01:
        {
            cpu_state->registers.c = mem_read(cpu_state->pc); // lo
            cpu_state->registers.b = mem_read(cpu_state->pc+1); // hi

            cpu_state->pc += 2;

            return 12;
        }
        // LD [BC], A
        case 0x02:
        {
            u16 address = get_BC();

            mem_write(address, cpu_state->registers.a);

            return 8;
        }
        // INC BC
        case 0x03:
        {
            set_BC(get_BC() + 1);

            return 8;
        }
        // INC B
        case 0x04:
        {
            cpu_state->registers.b += 1;

            f_register_set_z(cpu_state->registers.b == 0);
            f_register_set_n(false);
            f_register_set_h((((cpu_state->registers.b-1) & 0x0F) + 1) > 0x0F);


            return 4;
        }
        // DEC B
        case 0x05:
        {
            cpu_state->registers.b -= 1;

            f_register_set_z(cpu_state->registers.b == 0);
            f_register_set_n(true);
            f_register_set_h(((cpu_state->registers.b+1) & 0x0F) < 1);


            return 4;
        }
        // LD B, imm8
        case 0x06:
        {
            cpu_state->registers.b = mem_read(cpu_state->pc);

            cpu_state->pc += 1;

            return 8;
        }
        // RLCA
        case 0x07:
        {
            f_register_set_c(bit7 & cpu_state->registers.a);
            cpu_state->registers.a = cpu_state->registers.a << 1 | f_register_get_c();

            f_register_set_z(false);
            f_register_set_n(false);
            f_register_set_h(false);

            return 4;
        }
        // LD [imm16], sp
        case 0x08:
        {
            u16 lo = (u16)mem_read(cpu_state->pc);
            u16 hi = (u16)mem_read(cpu_state->pc+1) << 8;
            u16 address = hi | lo;

            u8 sp_lo = cpu_state->sp & 0xFF;
            u8 sp_hi = (cpu_state->sp & 0xFF00) >> 8;

            mem_write(address ,sp_lo);
            mem_write(address+1 ,sp_hi);

            cpu_state->pc += 2;

            return 20;
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

            return 8;
        }
        // LD A, [BC]
        case 0x0A:
        {
            cpu_state->registers.a = mem_read(get_BC());

            return 8;
        }
        // DEC BC
        case 0x0B:
        {
            set_BC(get_BC()-1);

            return 8;
        }
        // INC C
        case 0x0C:
        {
            cpu_state->registers.c += 1;

            f_register_set_z(cpu_state->registers.c == 0);
            f_register_set_n(false);
            f_register_set_h((((cpu_state->registers.c-1) & 0x0F) + 1) > 0x0F);

            return 4;
        }
        // DEC C
        case 0x0D:
        {
            cpu_state->registers.c -= 1;

            f_register_set_z(cpu_state->registers.c == 0);
            f_register_set_n(true);
            f_register_set_h(((cpu_state->registers.c+1) & 0x0F) < 1);

            return 4;
        }
        // LD C, imm8
        case 0x0E:
        {
            cpu_state->registers.c = mem_read(cpu_state->pc);
            cpu_state->pc += 1;

            return 8;
        }
        // RRCA
        case 0x0F:
        {
            f_register_set_c(bit0 & cpu_state->registers.a);
            cpu_state->registers.a = cpu_state->registers.a >> 1 | (f_register_get_c() << 7);

            f_register_set_z(false);
            f_register_set_n(false);
            f_register_set_h(false);

            return 4;
        }
        // TODO: STOP
        case 0x10:
        {
            LOG_WARN("STOP 0x10 instruction not yet implemented!\n");
            cpu_state->pc += 1;

            return 4;
        }
        // LD DE, imm16
        case 0x11:
        {
            cpu_state->registers.e = mem_read(cpu_state->pc); // lo
            cpu_state->registers.d = mem_read(cpu_state->pc+1); // hi

            cpu_state->pc += 2;

            return 12;
        }
        // LD [DE], A
        case 0x12:
        {
            u16 address = get_DE();

            mem_write(address, cpu_state->registers.a);

            return 8;
        }
        // INC DE
        case 0x13:
        {
            set_DE(get_DE() + 1);

            return 8;
        }
        // INC D
        case 0x14:
        {
            cpu_state->registers.d += 1;

            f_register_set_z(cpu_state->registers.d == 0);
            f_register_set_n(false);
            f_register_set_h((((cpu_state->registers.d-1) & 0x0F) + 1) > 0x0F);

            return 4;
        }
        // DEC D
        case 0x15:
        {
            cpu_state->registers.d -= 1;

            f_register_set_z(cpu_state->registers.d == 0);
            f_register_set_n(true);
            f_register_set_h(((cpu_state->registers.d+1) & 0x0F) < 1);

            return 4;
        }
        // LD D, imm8
        case 0x16:
        {
            cpu_state->registers.d = mem_read(cpu_state->pc);
            cpu_state->pc += 1;

            return 8;
        }
        // RLA
        case 0x17:
        {
            u8 old_value = cpu_state->registers.a;
            cpu_state->registers.a = cpu_state->registers.a << 1 | f_register_get_c();

            f_register_set_z(false);
            f_register_set_n(false);
            f_register_set_h(false);
            f_register_set_c(bit7 & old_value);

            return 4;
        }
        // JR imm8
        case 0x18:
        {
            cpu_state->pc += (i8)mem_read(cpu_state->pc);

            cpu_state->pc += 1;

            return 12;
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

            return 8;
        }
        // LD A, [DE]
        case 0x1A:
        {
            cpu_state->registers.a = mem_read(get_DE());

            return 8;
        }
        // DEC DE
        case 0x1B:
        {
            set_DE(get_DE()-1);

            return 8;
        }
        // INC E
        case 0x1C:
        {
            cpu_state->registers.e += 1;

            f_register_set_z(cpu_state->registers.e == 0);
            f_register_set_n(false);
            f_register_set_h((((cpu_state->registers.e-1) & 0x0F) + 1) > 0x0F);

            return 4;
        }
        // DEC E
        case 0x1D:
        {
            cpu_state->registers.e -= 1;

            f_register_set_z(cpu_state->registers.e == 0);
            f_register_set_n(true);
            f_register_set_h(((cpu_state->registers.e+1) & 0x0F) < 1);

            return 4;
        }
        // LD E, imm8
        case 0x1E:
        {
            cpu_state->registers.e = mem_read(cpu_state->pc);
            cpu_state->pc += 1;

            return 8;
        }
        // RRA
        case 0x1F:
        {
            u8 old_value = cpu_state->registers.a;
            cpu_state->registers.a = cpu_state->registers.a >> 1 | (f_register_get_c() << 7);


            f_register_set_z(false);
            f_register_set_n(false);
            f_register_set_h(false);
            f_register_set_c(bit0 & old_value);

            return 4;
        }
        // JR NZ, imm8
        case 0x20:
        {
            u8 cycles = 8;
            if (!f_register_get_z())
            {
                cpu_state->pc += (i8)mem_read(cpu_state->pc);
                cycles = 12;
            }
            cpu_state->pc += 1;

            return cycles;
        }
        // LD HL, imm16
        case 0x21:
        {
            cpu_state->registers.l = mem_read(cpu_state->pc); // lo
            cpu_state->registers.h = mem_read(cpu_state->pc+1); // hi

            cpu_state->pc += 2;

            return 12;
        }
        // LD[HL+], A
        case 0x22:
        {
            u16 address = get_HL();

            mem_write(address, cpu_state->registers.a);
            set_HL(address + 1);

            return 8;
        }
        // INC HL
        case 0x23:
        {
            set_HL(get_HL() + 1);

            return 8;
        }
        // INC H
        case 0x24:
        {
            cpu_state->registers.h += 1;

            f_register_set_z(cpu_state->registers.h == 0);
            f_register_set_n(false);
            f_register_set_h((((cpu_state->registers.h-1) & 0x0F) + 1) > 0x0F);

            return 4;
        }
        // DEC H
        case 0x25:
        {
            cpu_state->registers.h -= 1;

            f_register_set_z(cpu_state->registers.h == 0);
            f_register_set_n(true);
            f_register_set_h(((cpu_state->registers.h+1) & 0x0F) < 1);

            return 8;
        }
        // LD H, imm8
        case 0x26:
        {
            cpu_state->registers.h = mem_read(cpu_state->pc);
            cpu_state->pc += 1;

            return 8;
        }
        // DAA
        case 0x27:
        {
            u8 a_value = cpu_state->registers.a;

            b8 subtract = f_register_get_n();
            b8 half_carry = f_register_get_h();
            b8 carry = f_register_get_c();

            u8 offset = 0;

            if (!subtract)
            {
                if (((a_value & 0x0F) > 0x09) || half_carry)
                {
                    offset += 0x06;
                }

                if ((a_value > 0x99) || carry)
                {
                    offset += 0x60;
                    f_register_set_c(true);
                }

            }
            else
            {
                if (half_carry)
                {
                    offset += 0x06;
                }
                if (carry)
                {
                    offset += 0x60;
                }
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

            return 4;
        }
        // JR Z, imm8
        case 0x28:
        {
            u8 cycles = 8;
            if (f_register_get_z())
            {
                cpu_state->pc += (i8)mem_read(cpu_state->pc);
                cycles = 12;
            }

            cpu_state->pc += 1;
            return cycles;
        }
        // ADD HL, HL
        case 0x29:
        {
            u16 hl = get_HL();

            set_HL(hl + hl);

            f_register_set_n(false);
            f_register_set_h(((hl & 0xFFF) + (hl & 0xFFF)) > 0xFFF);
            f_register_set_c((hl+hl) > 0xFFFF);

            return 8;
        }
        // LD A, [HL+]
        case 0x2A:
        {
            u16 hl = get_HL();
            cpu_state->registers.a = mem_read(hl);

            set_HL(hl + 1);

            return 8;
        }
        // DEC HL
        case 0x2B:
        {
            set_HL(get_HL()-1);

            return 8;
        }
        // INC L
        case 0x2C:
        {
            cpu_state->registers.l += 1;

            f_register_set_z(cpu_state->registers.l == 0);
            f_register_set_n(false);
            f_register_set_h((((cpu_state->registers.l-1) & 0x0F) + 1) > 0x0F);

            return 4;
        }
        // DEC L
        case 0x2D:
        {
            cpu_state->registers.l -= 1;

            f_register_set_z(cpu_state->registers.l == 0);
            f_register_set_n(true);
            f_register_set_h(((cpu_state->registers.l+1) & 0x0F) < 1);

            return 4;
        }
        // LD L, imm8
        case 0x2E:
        {
            cpu_state->registers.l = mem_read(cpu_state->pc);
            cpu_state->pc += 1;

            return 8;
        }
        // CPL
        case 0x2F:
        {
            cpu_state->registers.a = ~(cpu_state->registers.a);

            f_register_set_n(true);
            f_register_set_h(true);

            return 4;
        }
        // JR NC, imm8
        case 0x30:
        {
            u8 cycles = 8;
            if (!f_register_get_c())
            {
                cpu_state->pc += (i8)mem_read(cpu_state->pc);
                cycles = 12;
            }
            cpu_state->pc += 1;

            return cycles;
        }
        // LD SP, imm16
        case 0x31:
        {
            u16 l = mem_read(cpu_state->pc); // lo
            u16 h = mem_read(cpu_state->pc+1); // hi

            cpu_state->sp = (l | h << 8);
            cpu_state->pc += 2;

            return 12;
        }
        // LD [HL-], A
        case 0x32:
        {
            u16 address = get_HL();

            mem_write(address, cpu_state->registers.a);
            set_HL(address - 1);

            return 8;
        }
        // INC SP
        case 0x33:
        {
            cpu_state->sp += 1;

            return 8;
        }
        // INC [HL]
        case 0x34:
        {

            u8 value = mem_read(get_HL()) + 1;
            mem_write(get_HL(), value);


            f_register_set_z(value == 0);
            f_register_set_n(false);
            f_register_set_h((((value-1) & 0x0F) + 1) > 0x0F);

            return 12;
        }
        // DEC [HL]
        case 0x35:
        {
            u8 value = mem_read(get_HL()) - 1;
            mem_write(get_HL(), value);

            f_register_set_z(value == 0);
            f_register_set_n(true);
            f_register_set_h(((value+1) & 0x0F) < 1);

            return 12;
        }
        // LD [HL], imm8
        case 0x36:
        {
            mem_write(get_HL(), mem_read(cpu_state->pc));
            cpu_state->pc += 1;

            return 12;
        }
        // SCF
        case 0x37:
        {
            f_register_set_n(false);
            f_register_set_h(false);
            f_register_set_c(true);

            return 4;
        }
        // JR C, imm8
        case 0x38:
        {
            u8 cycles = 8;
            if (f_register_get_c())
            {
                cpu_state->pc += (i8)mem_read(cpu_state->pc);
                cycles = 12;
            }
            cpu_state->pc += 1;

            return cycles;
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

            return 8;
        }
        // LD A, [HL-]
        case 0x3A:
        {
            u16 hl = get_HL();
            cpu_state->registers.a = mem_read(hl);

            set_HL(hl - 1);
            return 8;
        }
        // DEC SP
        case 0x3B:
        {
            cpu_state->sp -= 1;

            return 8;
        }
        // INC A
        case 0x3C:
        {
            cpu_state->registers.a += 1;

            f_register_set_z(cpu_state->registers.a == 0);
            f_register_set_n(false);
            f_register_set_h((((cpu_state->registers.a-1) & 0x0F) + 1) > 0x0F);

            return 4;
        }
        // DEC A
        case 0x3D:
        {
            cpu_state->registers.a -= 1;

            f_register_set_z(cpu_state->registers.a == 0);
            f_register_set_n(true);
            f_register_set_h(((cpu_state->registers.a+1) & 0x0F) < 1);

            return 4;
        }
        // LD A, imm8
        case 0x3E:
        {
            cpu_state->registers.a = mem_read(cpu_state->pc);
            cpu_state->pc += 1;

            return 8;
        }
        // CCF
        case 0x3F:
        {
            f_register_set_n(false);
            f_register_set_h(false);
            f_register_set_c(!f_register_get_c());

            return 4;
        }
        // HALT
        case 0x76:
        {
            cpu_state->is_halted = true;

            return 4;
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

            u8 *src, *dst;
            b8 hl_dst = false;
            u8 cycles = 4;

            if ((instr & 0xF0) == 0x40)
            {
                if ((instr & 0x0F) < 0x08)
                {
                    dst = &cpu_state->registers.b;
                }
                else
                {
                    dst = &cpu_state->registers.c;
                }
            }
            else if ((instr & 0xF0) == 0x50)
            {
                if ((instr & 0x0F) < 0x08)
                {
                    dst = &cpu_state->registers.d;
                }
                else
                {
                    dst = &cpu_state->registers.e;
                }
            }
            else if ((instr & 0xF0) == 0x60)
            {
                if ((instr & 0x0F) < 0x08)
                {
                    dst = &cpu_state->registers.h;
                }
                else
                {
                    dst = &cpu_state->registers.l;
                }
            }
            else if ((instr & 0xF0) == 0x70)
            {
                if ((instr & 0x0F) < 0x08)
                {
                    hl_dst = true;
                    cycles = 8;
                }
                else
                {
                    dst = &cpu_state->registers.a;
                }
            }

            switch((instr & 0x0F) % 8)
            {
                case 0:
                    src = &cpu_state->registers.b;
                    break;
                case 1:
                    src = &cpu_state->registers.c;
                    break;
                case 2:
                    src = &cpu_state->registers.d;
                    break;
                case 3:
                    src = &cpu_state->registers.e;
                    break;
                case 4:
                    src = &cpu_state->registers.h;
                    break;
                case 5:
                    src = &cpu_state->registers.l;
                    break;
                case 6:
                    src = mem_read_ref(get_HL());
                    cycles = 8;
                    break;
                case 7:
                    src = &cpu_state->registers.a;
                    break;
            }

            if (hl_dst)
            {
                mem_write(get_HL(), *src);
            }
            else
            {
                *dst = *src;
            }

            return cycles;
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
        // SBC A, reg
        case 0x98:
        case 0x99:
        case 0x9A:
        case 0x9B:
        case 0x9C:
        case 0x9D:
        case 0x9E:
        case 0x9F:
        {
            u8 src;
            u8 cycles = 4;
            switch ((instr & 0x0F) % 8)
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
                    src = mem_read(get_HL());
                    cycles = 8;
                    break;
                case 0x07:
                    src = cpu_state->registers.a;
                    break;
            }

            if ((instr & 0xF0) == 0x80) // Addition
            {
                if ((instr & 0x0F) < 0x08)
                {
                    // ADD
                    u16 full_result = cpu_state->registers.a + src;
                    u8 result = (u8)full_result;

                    f_register_set_z(result == 0);
                    f_register_set_n(false);
                    f_register_set_h((((cpu_state->registers.a) & 0x0F) + (src & 0x0F)) > 0x0F);
                    f_register_set_c(full_result > 0xFF);

                    cpu_state->registers.a = result;
                }
                else
                {
                    // ADC
                    u16 full_result = cpu_state->registers.a + src + (u8)f_register_get_c();
                    u8 result = (u8)full_result;

                    f_register_set_z(result == 0);
                    f_register_set_n(false);
                    f_register_set_h((((cpu_state->registers.a) & 0x0F) + (src & 0x0F) + (u8)f_register_get_c()) > 0x0F);
                    f_register_set_c(full_result > 0xFF);

                    cpu_state->registers.a = result;
                }
            }
            else
            {
                if ((instr & 0x0F) < 0x08)
                {
                    // Subtraction
                    u16 full_result = cpu_state->registers.a - src;
                    u8 result = (u8)full_result;

                    f_register_set_z(result == 0);
                    f_register_set_n(true);
                    f_register_set_h((((cpu_state->registers.a) & 0x0F) < (src & 0x0F)));
                    f_register_set_c(cpu_state->registers.a < src);

                    cpu_state->registers.a = result;
                }
                else
                {
                    // SBC
                    i32 full_result = cpu_state->registers.a - src - (u8)f_register_get_c();
                    u8 result = (u8)full_result;

                    f_register_set_z(result == 0);
                    f_register_set_n(true);
                    f_register_set_h(((cpu_state->registers.a & 0x0F) - (src & 0x0F) - (u8)f_register_get_c()) < 0);
                    f_register_set_c(full_result < 0);

                    cpu_state->registers.a = result;
                }
            }

            return cycles;
        }
        // AND
        case 0xA0:
        case 0xA1:
        case 0xA2:
        case 0xA3:
        case 0xA4:
        case 0xA5:
        case 0xA6:
        case 0xA7:
        // XOR
        case 0xA8:
        case 0xA9:
        case 0xAA:
        case 0xAB:
        case 0xAC:
        case 0xAD:
        case 0xAE:
        case 0xAF:
        // OR
        case 0xB0:
        case 0xB1:
        case 0xB2:
        case 0xB3:
        case 0xB4:
        case 0xB5:
        case 0xB6:
        case 0xB7:
        // CP
        case 0xB8:
        case 0xB9:
        case 0xBA:
        case 0xBB:
        case 0xBC:
        case 0xBD:
        case 0xBE:
        case 0xBF:
        {
            u8 src;
            u8 cycles = 4;

            switch ((instr & 0x0F) % 8)
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
                    src = mem_read(get_HL());
                    cycles = 8;
                    break;
                case 0x07:
                    src = cpu_state->registers.a;
                    break;
            }

            if ((instr & 0xF0) == 0xA0)
            {
                // AND reg
                if ((instr & 0x0F) < 0x08)
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
                if ((instr & 0x0F) < 0x08)
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
                    f_register_set_h(((cpu_state->registers.a) & 0x0F) < (src & 0x0F));
                    f_register_set_c(cpu_state->registers.a < src);
                }

            }

            return cycles;
        }
        // RET NZ
        case 0xC0:
        {
            u8 cycles = 8;
            if (!f_register_get_z())
            {
                u16 l = mem_read(cpu_state->sp++);
                u16 h = mem_read(cpu_state->sp++);

                cpu_state->pc = (l | h << 8);
                cycles = 20;
            }

            return cycles;
        }
        // POP BC
        case 0xC1:
        {
            cpu_state->registers.c = mem_read(cpu_state->sp++); // lo
            cpu_state->registers.b = mem_read(cpu_state->sp++); // hi

            return 12;
        }
        // JP NZ, imm16
        case 0xC2:
        {
            u8 cycles = 12;
            if (!f_register_get_z())
            {
                u16 l = mem_read(cpu_state->pc);
                u16 h = mem_read(cpu_state->pc+1);

                cpu_state->pc = (l | h << 8);
                cycles = 20;
            }
            else
            {
                cpu_state->pc += 2;
            }

            return cycles;
        }
        // JP imm16
        case 0xC3:
        {
            u16 l = mem_read(cpu_state->pc);
            u16 h = mem_read(cpu_state->pc+1);

            cpu_state->pc = (l | h << 8);

            return 20;
        }
        // CALL NZ, imm16
        case 0xC4:
        {
            u8 cycles = 12;
            if (!f_register_get_z())
            {
                u16 ret_addr = cpu_state->pc + 2;

                mem_write(--cpu_state->sp, (ret_addr & 0xFF00) >> 8);
                mem_write(--cpu_state->sp, (ret_addr & 0x00FF));

                u16 l = mem_read(cpu_state->pc);
                u16 h = mem_read(cpu_state->pc+1);

                cpu_state->pc = (l | h << 8);
                cycles = 24;
            }
            else
            {
                cpu_state->pc += 2;
            }

            return cycles;
        }
        // PUSH BC
        case 0xC5:
        {
            mem_write(--cpu_state->sp, cpu_state->registers.b); // hi
            mem_write(--cpu_state->sp, cpu_state->registers.c); // lo

            return 16;
        }
        // ADD A, imm8
        case 0xC6:
        {
            u8 src = mem_read(cpu_state->pc);
            u16 full_result = cpu_state->registers.a + src;
            u8 result = (u8)full_result;


            f_register_set_z(result == 0);
            f_register_set_n(false);
            f_register_set_h((((cpu_state->registers.a) & 0x0F) + (src & 0x0F)) > 0x0F);
            f_register_set_c(full_result > 0xFF);

            cpu_state->registers.a = result;

            cpu_state->pc += 1;
            return 8;
        }
        // RST 00H
        case 0xC7:
        {
            u16 ret_addr = cpu_state->pc;

            mem_write(--cpu_state->sp, (ret_addr & 0xFF00) >> 8);
            mem_write(--cpu_state->sp, (ret_addr & 0x00FF));

            cpu_state->pc = 0x0000;
            return 16;
        }
        // RET Z
        case 0xC8:
        {
            u8 cycles = 8;
            if (f_register_get_z())
            {
                u16 l = mem_read(cpu_state->sp++);
                u16 h = mem_read(cpu_state->sp++);

                cpu_state->pc = (l | h << 8);
                cycles = 20;
            }

            return cycles;
        }
        // RET
        case 0xC9:
        {
            u16 l = mem_read(cpu_state->sp++);
            u16 h = mem_read(cpu_state->sp++);

            cpu_state->pc = (l | h << 8);

            return 16;
        }
        // JP Z, imm16
        case 0xCA:
        {
            u8 cycles = 12;
            if (f_register_get_z())
            {
                u16 l = mem_read(cpu_state->pc);
                u16 h = mem_read(cpu_state->pc+1);

                cpu_state->pc = (l | h << 8);
                cycles = 16;
            }
            else
            {
                cpu_state->pc += 2;
            }

            return cycles;
        }
        // 16-bit opcodes with 0xCB prefix
        case 0xCB:
        {
            u8 cycles = process_16_bit_opcodes(mem_read(cpu_state->pc));

            cpu_state->pc += 1;
            return cycles;
        }
        // CALL Z, imm16
        case 0xCC:
        {
            u8 cycles = 12;
            if (f_register_get_z())
            {
                u16 ret_addr = cpu_state->pc + 2;

                mem_write(--cpu_state->sp, (ret_addr & 0xFF00) >> 8);
                mem_write(--cpu_state->sp, (ret_addr & 0x00FF));

                u16 l = mem_read(cpu_state->pc);
                u16 h = mem_read(cpu_state->pc+1);

                cpu_state->pc = (l | h << 8);
                cycles = 24;
            }
            else
            {
                cpu_state->pc += 2;
            }

            return cycles;
        }
        // CALL imm16
        case 0xCD:
        {
            u16 ret_addr = cpu_state->pc + 2;

            mem_write(--cpu_state->sp, (ret_addr & 0xFF00) >> 8);
            mem_write(--cpu_state->sp, (ret_addr & 0x00FF));

            u16 l = mem_read(cpu_state->pc);
            u16 h = mem_read(cpu_state->pc+1);

            cpu_state->pc = (l | h << 8);

            return 24;
        }
        // ADC A, imm8
        case 0xCE:
        {
            u8 src = mem_read(cpu_state->pc);
            u16 full_result = cpu_state->registers.a + src + (u8)f_register_get_c();
            u8 result = (u8)full_result;


            f_register_set_z(result == 0);
            f_register_set_n(false);
            f_register_set_h((((cpu_state->registers.a) & 0x0F) + (src & 0x0F) + (u8)f_register_get_c()) > 0x0F);
            f_register_set_c(full_result > 0xFF);

            cpu_state->registers.a = result;

            cpu_state->pc += 1;
            return 8;

        }
        // RST 08H
        case 0xCF:
        {
            u16 ret_addr = cpu_state->pc;
            mem_write(--cpu_state->sp, (ret_addr & 0xFF00) >> 8);
            mem_write(--cpu_state->sp, (ret_addr & 0x00FF));

            cpu_state->pc = 0x0008;

            return 16;
        }
        // RET NC
        case 0xD0:
        {
            u8 cycles = 8;
            if (!f_register_get_c())
            {
                u16 l = mem_read(cpu_state->sp++);
                u16 h = mem_read(cpu_state->sp++);

                cpu_state->pc = (l | h << 8);
                cycles = 20;
            }

            return cycles;
        }
        // POP DE
        case 0xD1:
        {
            cpu_state->registers.e = mem_read(cpu_state->sp++); // lo
            cpu_state->registers.d = mem_read(cpu_state->sp++); // hi

            return 12;
        }
        // JP NC, imm16
        case 0xD2:
        {
            u8 cycles = 12;
            if (!f_register_get_c())
            {
                u16 l = mem_read(cpu_state->pc);
                u16 h = mem_read(cpu_state->pc+1);

                cpu_state->pc = (l | h << 8);
                cycles = 16;
            }
            else
            {
                cpu_state->pc += 2;
            }

            return cycles;
        }
        // CALL NC, imm16
        case 0xD4:
        {
            u8 cycles = 12;
            if (!f_register_get_c())
            {
                u16 ret_addr = cpu_state->pc + 2;
                mem_write(--cpu_state->sp, (ret_addr & 0xFF00) >> 8);
                mem_write(--cpu_state->sp, (ret_addr & 0x00FF));

                u16 l = mem_read(cpu_state->pc);
                u16 h = mem_read(cpu_state->pc+1);

                cpu_state->pc = (l | h << 8);
                cycles = 24;
            }
            else
            {
                cpu_state->pc += 2;
            }

            return cycles;
        }
        // PUSH DE
        case 0xD5:
        {
            mem_write(--cpu_state->sp, cpu_state->registers.d); // hi
            mem_write(--cpu_state->sp, cpu_state->registers.e); // lo

            return 16;
        }
        // SUB imm8
        case 0xD6:
        {
            u8 src = mem_read(cpu_state->pc);
            u16 full_result = cpu_state->registers.a - src;
            u8 result = (u8)full_result;

            f_register_set_z(result == 0);
            f_register_set_n(true);
            f_register_set_h((((cpu_state->registers.a) & 0x0F) < (src & 0x0F)));
            f_register_set_c(cpu_state->registers.a < src);

            cpu_state->registers.a = result;

            cpu_state->pc += 1;
            return 8;
        }
        // RST 10H
        case 0xD7:
        {
            u16 ret_addr = cpu_state->pc;

            mem_write(--cpu_state->sp, (ret_addr & 0xFF00) >> 8);
            mem_write(--cpu_state->sp, (ret_addr & 0x00FF));

            cpu_state->pc = 0x0010;
            return 16;
        }
        // RET C
        case 0xD8:
        {
            u8 cycles = 8;
            if (f_register_get_c())
            {
                u16 l = mem_read(cpu_state->sp++);
                u16 h = mem_read(cpu_state->sp++);

                cpu_state->pc = (l | h << 8);
                cycles = 20;
            }

            return cycles;
        }
        // TODO: RETI
        case 0xD9:
        {
            cpu_state->ime = true;

            u16 l = mem_read(cpu_state->sp++);
            u16 h = mem_read(cpu_state->sp++);

            cpu_state->pc = (l | h << 8);

            return 16;
        }
        // JP C, imm16
        case 0xDA:
        {
            u8 cycles = 12;
            if (f_register_get_c())
            {
                u16 l = mem_read(cpu_state->pc);
                u16 h = mem_read(cpu_state->pc+1);

                cpu_state->pc = (l | h << 8);
                cycles = 16;
            }
            else
            {
                cpu_state->pc += 2;
            }

            return cycles;
        }
        // CALL C, imm16
        case 0xDC:
        {
            u8 cycles = 12;
            if (f_register_get_c())
            {
                u16 ret_addr = cpu_state->pc + 2;
                mem_write(--cpu_state->sp, (ret_addr & 0xFF00) >> 8);
                mem_write(--cpu_state->sp, (ret_addr & 0x00FF));

                u16 l = mem_read(cpu_state->pc);
                u16 h = mem_read(cpu_state->pc+1);

                cpu_state->pc = (l | h << 8);
                cycles = 24;
            }
            else
            {
                cpu_state->pc += 2;
            }

            return cycles;
        }
        // SBC A, imm8
        case 0xDE:
        {
            u8 imm8 = mem_read(cpu_state->pc);
            i32 full_result = cpu_state->registers.a - imm8 - (u8)f_register_get_c();
            u8 result = (u8)full_result;

            f_register_set_z(result == 0);
            f_register_set_n(true);
            f_register_set_h(((cpu_state->registers.a & 0x0F) - (imm8 & 0x0F) - (u8)f_register_get_c()) < 0);
            f_register_set_c(full_result < 0);

            cpu_state->registers.a = result;

            cpu_state->pc += 1;
            return 8;
        }
        // RST 18H
        case 0xDF:
        {
            u16 ret_addr = cpu_state->pc;
            mem_write(--cpu_state->sp, (ret_addr & 0xFF00) >> 8);
            mem_write(--cpu_state->sp, (ret_addr & 0x00FF));

            cpu_state->pc = 0x0018;
            return 16;
        }
        // LD [imm8], A
        case 0xE0:
        {
            u16 address = 0xFF00 | mem_read(cpu_state->pc);
            mem_write(address, cpu_state->registers.a);

            cpu_state->pc += 1;
            return 12;
        }
        // POP HL
        case 0xE1:
        {
            cpu_state->registers.l = mem_read(cpu_state->sp++); // lo
            cpu_state->registers.h = mem_read(cpu_state->sp++); // hi

            return 12;
        }
        // LD [C], A
        case 0xE2:
        {
            u16 address = 0xFF00 | cpu_state->registers.c;

            mem_write(address, cpu_state->registers.a);

            return 8;
        }
        // PUSH HL
        case 0xE5:
        {
            mem_write(--cpu_state->sp, cpu_state->registers.h); // hi
            mem_write(--cpu_state->sp, cpu_state->registers.l); // lo

            return 16;
        }
        // AND imm8
        case 0xE6:
        {
            cpu_state->registers.a &= mem_read(cpu_state->pc);

            f_register_set_z(cpu_state->registers.a == 0);
            f_register_set_n(false);
            f_register_set_h(true);
            f_register_set_c(false);

            cpu_state->pc += 1;
            return 8;
        }
        // RST 20H
        case 0xE7:
        {
            u16 ret_addr = cpu_state->pc;
            mem_write(--cpu_state->sp, (ret_addr & 0xFF00) >> 8);
            mem_write(--cpu_state->sp, (ret_addr & 0x00FF));

            cpu_state->pc = 0x0020;
            return 16;
        }
        // ADD SP, imm8
        case 0xE8:
        {
            u16 sp = cpu_state->sp;
            i8 imm8 = mem_read(cpu_state->pc);
            i32 full_result = sp + imm8;

            cpu_state->sp = full_result;

            f_register_set_z(false);
            f_register_set_n(false);
            f_register_set_h(((sp ^ imm8 ^ (full_result & 0xFFFF)) & 0x10) == 0x10);
            f_register_set_c(((sp ^ imm8 ^ (full_result & 0xFFFF)) & 0x100) == 0x100);

            cpu_state->pc += 1;
            return 16;
        }
        // JP HL
        case 0xE9:
        {
            cpu_state->pc = get_HL();
            return 4;
        }
        // LD [imm16], A
        case 0xEA:
        {
            u16 lo = (u16)mem_read(cpu_state->pc);
            u16 hi = (u16)mem_read(cpu_state->pc+1) << 8;
            u16 address = hi | lo;

            mem_write(address, cpu_state->registers.a);

            cpu_state->pc += 2;
            return 16;
        }
        // XOR imm8
        case 0xEE:
        {
            cpu_state->registers.a ^= mem_read(cpu_state->pc);

            f_register_set_z(cpu_state->registers.a == 0);
            f_register_set_n(false);
            f_register_set_h(false);
            f_register_set_c(false);

            cpu_state->pc += 1;
            return 8;
        }
        // RST 28H
        case 0xEF:
        {
            u16 ret_addr = cpu_state->pc;
            mem_write(--cpu_state->sp, (ret_addr & 0xFF00) >> 8);
            mem_write(--cpu_state->sp, (ret_addr & 0x00FF));

            cpu_state->pc = 0x0028;
            return 8;
        }
        // LD A, [imm8]
        case 0xF0:
        {
            u16 address = 0xFF00 | mem_read(cpu_state->pc);
            cpu_state->registers.a = mem_read(address);

            cpu_state->pc += 1;
            return 16;
        }
        // POP AF
        case 0xF1:
        {
            cpu_state->registers.f = mem_read(cpu_state->sp++) & 0xF0; // lo
            cpu_state->registers.a = mem_read(cpu_state->sp++); // hi

            return 12;
        }
        // LD A, [C]
        case 0xF2:
        {
            u16 address = 0xFF00 | cpu_state->registers.c;
            cpu_state->registers.a = mem_read(address);

            cpu_state->pc += 1;
            return 8;
        }
        // DI
        case 0xF3:
        {
            cpu_state->ime = false;

            return 4;
        }
        // PUSH AF
        case 0xF5:
        {
            mem_write(--cpu_state->sp, cpu_state->registers.a); // hi
            mem_write(--cpu_state->sp, cpu_state->registers.f); // lo

            return 16;
        }
        // OR imm8
        case 0xF6:
        {
            cpu_state->registers.a |= mem_read(cpu_state->pc);

            f_register_set_z(cpu_state->registers.a == 0);
            f_register_set_n(false);
            f_register_set_h(false);
            f_register_set_c(false);

            cpu_state->pc += 1;
            return 8;
        }
        // RST 30H
        case 0xF7:
        {
            u16 ret_addr = cpu_state->pc;
            mem_write(--cpu_state->sp, (ret_addr & 0xFF00) >> 8);
            mem_write(--cpu_state->sp, (ret_addr & 0x00FF));

            cpu_state->pc = 0x0030;
            return 16;
        }
        // LD HL, SP+imm8
        case 0xF8:
        {
            u16 sp = cpu_state->sp;
            i8 imm8 = mem_read(cpu_state->pc);
            i32 full_result = sp + imm8;

            set_HL((u16)full_result);

            f_register_set_z(false);
            f_register_set_n(false);
            f_register_set_h(((sp ^ imm8 ^ (full_result & 0xFFFF)) & 0x10) == 0x10);
            f_register_set_c(((sp ^ imm8 ^ (full_result & 0xFFFF)) & 0x100) == 0x100);

            cpu_state->pc += 1;
            return 12;
        }
        // LD SP, HL
        case 0xF9:
        {
            cpu_state->sp = get_HL();

            return 8;
        }
        // LD A, [imm16]
        case 0xFA:
        {
            u16 lo = (u16)mem_read(cpu_state->pc);
            u16 hi = (u16)mem_read(cpu_state->pc+1) << 8;
            u16 address = hi | lo;

            cpu_state->registers.a = mem_read(address);

            cpu_state->pc += 2;
            return 16;
        }
        // EI
        case 0xFB:
        {
            cpu_state->ime = true;

            return 4;
        }
        // CP imm8
        case 0xFE:
        {
            u8 imm8 = mem_read(cpu_state->pc);

            f_register_set_z((cpu_state->registers.a - imm8) == 0);
            f_register_set_n(true);
            f_register_set_h(((cpu_state->registers.a) & 0x0F) < (imm8 & 0x0F));
            f_register_set_c(cpu_state->registers.a < imm8);

            cpu_state->pc += 1;
            return 8;
        }
        // RST 38H
        case 0xFF:
        {
            u16 ret_addr = cpu_state->pc;
            mem_write(--cpu_state->sp, (ret_addr & 0xFF00) >> 8);
            mem_write(--cpu_state->sp, (ret_addr & 0x00FF));

            cpu_state->pc = 0x0038;
            return 16;
        }

        default:
            LOG_WARN("Instruction %x not found!", instr);

            return 4;
    }

}


static void request_interrupt(InterruptFlags interrupt)
{
    cpu_state->memory[Hardware_Registers_IF] |= interrupt;
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
        cpu_state->registers.f &= (~bit7);
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
        cpu_state->registers.f &= (~bit6);
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
        cpu_state->registers.f &= (~bit5);
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
        cpu_state->registers.f &= (~bit4);
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

static u8 process_16_bit_opcodes(u8 low)
{
    u8 *src;
    u8 cycles = 8;
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
            src = mem_read_ref(get_HL());
            cycles = 16;
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

            if ((low & 0xF0) == 0x00) // 0x00
            {
                if ((low & 0x0F) < 0x08)
                {
                    // RLC src
                    f_register_set_c(bit7 & *src);
                    *src = *src << 1 | f_register_get_c();

                    f_register_set_z(*src == 0);
                    f_register_set_n(false);
                    f_register_set_h(false);
                }
                else
                {
                    // RRC src
                    f_register_set_c(bit0 & *src);
                    *src = *src >> 1 | (f_register_get_c() << 7);

                    f_register_set_z(*src == 0);
                    f_register_set_n(false);
                    f_register_set_h(false);
                }
            }
            else // 0x10
            {
                if ((low & 0x0F) < 0x08)
                {
                    // RL src
                    u8 old_value = *src;
                    *src = *src << 1 | f_register_get_c();

                    f_register_set_z(*src == 0);
                    f_register_set_n(false);
                    f_register_set_h(false);
                    f_register_set_c(bit7 & old_value);
                }
                else
                {
                    // RR src
                    u8 old_value = *src;
                    *src = *src >> 1 | (f_register_get_c() << 7);


                    f_register_set_z(*src == 0);
                    f_register_set_n(false);
                    f_register_set_h(false);
                    f_register_set_c(bit0 & old_value);
                }

            }
            return cycles;
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
            if ((low & 0x0F) < 0x08)
            {
                // SLA
                f_register_set_c(bit7 & *src);
                *src <<= 1;

                f_register_set_z(*src == 0);
                f_register_set_n(false);
                f_register_set_h(false);
            }
            else
            {
                // SRA
                u8 tmp = bit7 & *src;

                f_register_set_c(bit0 & *src);
                *src >>= 1;
                *src |= tmp;

                f_register_set_z(*src == 0);
                f_register_set_n(false);
                f_register_set_h(false);
            }

            return cycles;
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
            if ((low & 0x0F) < 0x08)
            {
                // SWAP
                u8 low_4 = *src & 0x0F;
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
                f_register_set_c(bit0 & *src);
                *src >>= 1;

                f_register_set_z(*src == 0);
                f_register_set_n(false);
                f_register_set_h(false);
            }

            return cycles;
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
            if (((low & 0x0F) % 8) == 0x06)
            {
                cycles = 12;
            }
            if ((low & 0xF0) == 0x40) // 0x40
            {
                if ((low & 0x0F) < 0x08)
                {
                    // BIT 0, src
                    f_register_set_z((~(bit0 & *src)) & bit0);
                    f_register_set_n(false);
                    f_register_set_h(true);
                }
                else
                {
                    // BIT 1, src
                    f_register_set_z((~(bit1 & *src)) & bit1);
                    f_register_set_n(false);
                    f_register_set_h(true);
                }
            }
            else if ((low & 0xF0) == 0x50) // 0x50
            {
                if ((low & 0x0F) < 0x08)
                {
                    // BIT 2, src
                    f_register_set_z((~(bit2 & *src)) & bit2);
                    f_register_set_n(false);
                    f_register_set_h(true);

                }
                else
                {
                    // BIT 3, src
                    f_register_set_z((~(bit3 & *src)) & bit3);
                    f_register_set_n(false);
                    f_register_set_h(true);
                }

            }
            else if ((low & 0xF0) == 0x60) // 0x60
            {
                if ((low & 0x0F) < 0x08)
                {
                    // BIT 4, src
                    f_register_set_z((~(bit4 & *src)) & bit4);
                    f_register_set_n(false);
                    f_register_set_h(true);

                }
                else
                {
                    // BIT 5, src
                    f_register_set_z((~(bit5 & *src)) & bit5);
                    f_register_set_n(false);
                    f_register_set_h(true);
                }

            }
            else // 0x70
            {
                if ((low & 0x0F) < 0x08)
                {
                    // BIT 6, src
                    f_register_set_z((~(bit6 & *src)) & bit6);
                    f_register_set_n(false);
                    f_register_set_h(true);

                }
                else
                {
                    // BIT 7, src
                    f_register_set_z((~(bit7 & *src)) & bit7);
                    f_register_set_n(false);
                    f_register_set_h(true);
                }

            }

            return cycles;
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
            if ((low & 0xF0) == 0x80) // 0x80
            {
                if ((low & 0x0F) < 0x08)
                {
                    // RES 0, src
                    *src &= ~bit0;
                }
                else
                {
                    // RES 1, src
                    *src &= ~bit1;
                }
            }
            else if ((low & 0xF0) == 0x90) // 0x90
            {
                if ((low & 0x0F) < 0x08)
                {
                    // RES 2, src
                    *src &= ~bit2;
                }
                else
                {
                    // RES 3, src
                    *src &= ~bit3;
                }
            }
            else if ((low & 0xF0) == 0xA0) // 0xA0
            {
                if ((low & 0x0F) < 0x08)
                {
                    // RES 4, src
                    *src &= ~bit4;
                }
                else
                {
                    // RES 5, src
                    *src &= ~bit5;
                }
            }
            else // 0xB0
            {
                if ((low & 0x0F) < 0x08)
                {
                    // RES 6, src
                    *src &= ~bit6;
                }
                else
                {
                    // RES 7, src
                    *src &= ~bit7;
                }
            }

            return cycles;
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
            if ((low >> 4) == 0x0C) // 0xC0
            {
                if ((low & 0x0F) < 0x08)
                {
                    // SET 0, src
                    *src |= bit0;
                }
                else
                {
                    // SET 1, src
                    *src |= bit1;
                }
            }
            else if ((low >> 4) == 0x0D) // 0xD0
            {
                if ((low & 0x0F) < 0x08)
                {
                    // SET 2, src
                    *src |= bit2;
                }
                else
                {
                    // SET 3, src
                    *src |= bit3;
                }
            }
            else if ((low >> 4) == 0x0E) // 0xE0
            {
                if ((low & 0x0F) < 0x08)
                {
                    // SET 4, src
                    *src |= bit4;
                }
                else
                {
                    // SET 5, src
                    *src |= bit5;
                }
            }
            else // 0xF0
            {
                if ((low & 0x0F) < 0x08)
                {
                    // SET 6, src
                    *src |= bit6;
                }
                else
                {
                    // SET 7, src
                    *src |= bit7;
                }
            }

            return cycles;
        }

        default:
        {
            LOG_WARN("CB Instruction %x not found!", low);
            return cycles;
        }
    }
}

static void cpu_update_timer(u8 cycles)
{
    // Update the 0xFF04 divider register
    cpu_state->timer.div_counter += cycles;
    if (cpu_state->timer.div_counter >= DIV_UPDATE_RATE)
    {
        cpu_state->timer.div_counter -= DIV_UPDATE_RATE;
        cpu_state->memory[Hardware_Registers_DIV]++;
    }

    // Update the 0xFF05 TIMA timer counter
    u8 tac = cpu_state->memory[Hardware_Registers_TAC];

    if (tac & bit2)
    {
        cpu_state->timer.tima_counter += cycles;

        u16 tima_update_rate;
        if ((tac & 0x3) == 0x0)
        {
            tima_update_rate = 1024;
        }
        else if ((tac & 0x3) == 0x1)
        {
            tima_update_rate = 8;
        }
        else if ((tac & 0x3) == 0x2)
        {
            tima_update_rate = 64;
        }
        else
        {
            tima_update_rate = 256;
        }


        if (cpu_state->timer.tima_counter >= tima_update_rate)
        {
            cpu_state->timer.div_counter -= tima_update_rate;
            cpu_state->memory[Hardware_Registers_TIMA]++;

            // Check for overflow
            if (cpu_state->memory[Hardware_Registers_TIMA] == 0)
            {
                cpu_state->memory[Hardware_Registers_TIMA] = cpu_state->memory[Hardware_Registers_TMA];
                request_interrupt(InterruptFlags_Timer);
            }
        }
    }


}

static u8 cpu_process_interrupts()
{
    u8 interrupt_flag = cpu_state->memory[Hardware_Registers_IF];
    u8 interrupt_enabled = cpu_state->memory[Hardware_Registers_IE];

    if (cpu_state->ime)
    {

        // Interrupt pending
        if ((interrupt_flag & interrupt_enabled) > 0x00)
        {
            cpu_state->is_halted = false;
        }

        if ((interrupt_flag & interrupt_enabled & bit0) == bit0)
        {
            // VBlank
            u16 ret_addr = cpu_state->pc;
            cpu_state->memory[--cpu_state->sp] = (ret_addr & 0xFF00) >> 8;
            cpu_state->memory[--cpu_state->sp] = (ret_addr & 0x00FF);

            cpu_state->pc = 0x40;

            cpu_state->memory[Hardware_Registers_IF] &= ~bit0;
            cpu_state->ime = false;
            return 20;
        }
        if ((interrupt_flag & interrupt_enabled & bit1) == bit1)
        {
            // LCD
            u16 ret_addr = cpu_state->pc;
            cpu_state->memory[--cpu_state->sp] = (ret_addr & 0xFF00) >> 8;
            cpu_state->memory[--cpu_state->sp] = (ret_addr & 0x00FF);

            cpu_state->pc = 0x48;

            cpu_state->memory[Hardware_Registers_IF] &= ~bit1;
            cpu_state->ime = false;
            return 20;
        }
        if ((interrupt_flag & interrupt_enabled & bit2) == bit2)
        {
            // Timer
            u16 ret_addr = cpu_state->pc;
            cpu_state->memory[--cpu_state->sp] = (ret_addr & 0xFF00) >> 8;
            cpu_state->memory[--cpu_state->sp] = (ret_addr & 0x00FF);

            cpu_state->pc = 0x50;

            cpu_state->memory[Hardware_Registers_IF] &= ~bit2;
            cpu_state->ime = false;
            return 20;
        }
        if ((interrupt_flag & interrupt_enabled & bit3) == bit3)
        {
            // Serial
            u16 ret_addr = cpu_state->pc;
            cpu_state->memory[--cpu_state->sp] = (ret_addr & 0xFF00) >> 8;
            cpu_state->memory[--cpu_state->sp] = (ret_addr & 0x00FF);

            cpu_state->pc = 0x58;

            cpu_state->memory[Hardware_Registers_IF] &= ~bit3;
            cpu_state->ime = false;
            return 20;
        }
        if ((interrupt_flag & interrupt_enabled & bit4) == bit4)
        {
            // Joypad
            u16 ret_addr = cpu_state->pc;
            cpu_state->memory[--cpu_state->sp] = (ret_addr & 0xFF00) >> 8;
            cpu_state->memory[--cpu_state->sp] = (ret_addr & 0x00FF);

            cpu_state->pc = 0x60;

            cpu_state->memory[Hardware_Registers_IF] &= ~bit4;
            cpu_state->ime = false;
            return 20;
        }

    }
    else
    {
        // Interrupt pending (HALT bug scenario)
        if ((interrupt_flag & interrupt_enabled) > 0x00 && cpu_state->is_halted)
        {
            cpu_state->is_halted = false;
            cpu_state->halt_bug = true;
        }

        // IME = 0 and no interrupt pending
    }

    return 0;
}

// Helper function for blargg tests
static void print_tests()
{
    if (cpu_state->memory[Hardware_Registers_SC] == 0x81)
    {
        LOG_TRACE("%c", cpu_state->memory[Hardware_Registers_SB]);
        cpu_state->memory[Hardware_Registers_SC] = 0x0;
    }
}