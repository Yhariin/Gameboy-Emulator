static void ppu_init(Arena *arena)
{
    ppu_state = (PPU_State *)arena_alloc_align(arena, sizeof(PPU_State), sizeof(PPU_State));
    ppu_state->vram = cpu_state->memory + VRAM_START;
    ppu_state->oam_ram = (OAM *)(cpu_state->memory + OAM_START);

    ppu_state->lcds = &cpu_state->memory[Hardware_Registers_STAT];
    ppu_state->lcdc = &cpu_state->memory[Hardware_Registers_LCDC];
    ppu_state->ly = &cpu_state->memory[Hardware_Registers_LY];
    ppu_state->lyc = &cpu_state->memory[Hardware_Registers_LYC];
    ppu_state->scx = &cpu_state->memory[Hardware_Registers_SCX];
    ppu_state->scy = &cpu_state->memory[Hardware_Registers_SCY];
    ppu_state->bgp = &cpu_state->memory[Hardware_Registers_BGP];
    ppu_state->obp0 = &cpu_state->memory[Hardware_Registers_OBP0];
    ppu_state->obp1 = &cpu_state->memory[Hardware_Registers_OBP1];
    ppu_state->wy = &cpu_state->memory[Hardware_Registers_WY];
    ppu_state->wx = &cpu_state->memory[Hardware_Registers_WX];

    ppu_state->current_frame = 0;
    ppu_state->cycles = 0;
    ppu_state->window_line = 0;
    ppu_state->lcd_mode = LCD_Mode_2_OAM_SCAN;

}

static u8 ppu_oam_read(u16 address)
{
    u8 *oam_8b = (u8 *)ppu_state->oam_ram;
    return oam_8b[address - OAM_START];
}

static void ppu_oam_write(u16 address, u8 data)
{
    u8 *oam_8b = (u8 *)ppu_state->oam_ram;
    oam_8b[address - OAM_START] = data;
}

static u8 ppu_vram_read(u16 address)
{
    return ppu_state->vram[address - VRAM_START];
}

static void ppu_vram_write(u16 address, u8 data)
{
    ppu_state->vram[address - VRAM_START] = data;
}

static void increment_ly()
{
    (*ppu_state->ly)++;

    if (*ppu_state->ly == *ppu_state->lyc)
    {
        *ppu_state->lcds |= bit2;

        if (*ppu_state->lcds & Stat_Select_LYC)
        {
            request_interrupt(InterruptFlags_LCD_STAT);
        }
    }
    else
    {
        *ppu_state->lcds &= ~bit2;
    }
}

static void ppu_step(u32 cycles)
{
    if (!(*ppu_state->lcdc & LCDC_Flags_LCD_ENABLE))
    {
        ppu_state->cycles = 0;
        *ppu_state->ly = 0;
        ppu_state->lcd_mode = LCD_Mode_0_HBLANK;
    }

    ppu_state->cycles += cycles;

    switch(ppu_state->lcd_mode)
    {
        case LCD_Mode_2_OAM_SCAN:
        {
            if (ppu_state->cycles >= 80)
            {
                ppu_state->cycles -= 80;
                ppu_state->lcd_mode = LCD_Mode_3_TRANSFER;
            }
            break;
        }
        case LCD_Mode_3_TRANSFER:
        {
            if (ppu_state->cycles >= 172)
            {
                ppu_state->cycles -= 172;
                ppu_render_scanline();
                ppu_state->lcd_mode = LCD_Mode_0_HBLANK;

                if (*ppu_state->lcds & (u16)Stat_Select_HBLANK)
                {
                    request_interrupt(InterruptFlags_LCD_STAT);
                }
            }
            break;
        }
        case LCD_Mode_0_HBLANK:
        {
            if (ppu_state->cycles >= 204)
            {
                ppu_state->cycles -= 204;
                increment_ly();

                if (*ppu_state->ly >= RES_Y)
                {
                    ppu_state->lcd_mode = LCD_Mode_1_VBLANK;

                    request_interrupt(InterruptFlags_VBlank);

                    if (*ppu_state->lcds & (u16)Stat_Select_VBLANK)
                    {
                        request_interrupt(InterruptFlags_LCD_STAT);
                    }

                    ppu_state->current_frame++;
                }
                else
                {
                    ppu_state->lcd_mode = LCD_Mode_2_OAM_SCAN;
                }

            }
            break;
        }
        case LCD_Mode_1_VBLANK:
        {
            if (ppu_state->cycles >= TICKS_PER_LINE)
            {
                ppu_state->cycles -= TICKS_PER_LINE;
                increment_ly();

                if (*ppu_state->ly >= LINES_PER_FRAME)
                {
                    ppu_state->lcd_mode = LCD_Mode_2_OAM_SCAN;
                    *ppu_state->ly = 0;
                    ppu_state->window_line = 0;
                }

            }
            break;
        }

    }
}

static u8 apply_palette(u8 palette, u8 color)
{
    return (palette >> (color * 2)) & 0x03;
}

static void ppu_render_scanline()
{
    if (*ppu_state->lcdc & LCDC_Flags_BG_ENABLE)
    {
        ppu_render_background();
    }

    if (*ppu_state->lcdc & LCDC_Flags_WINDOW_ENABLE)
    {
        ppu_render_window();
    }

    if (*ppu_state->lcdc & LCDC_Flags_SPRITE_ENABLE)
    {
        ppu_render_sprites();
    }

}

static void ppu_render_background()
{
    u16 tilemap = (*ppu_state->lcdc & LCDC_Flags_BG_TILEMAP) ? 0x9C00 : 0x9800;

    u16 tile_base_address = (*ppu_state->lcdc & LCDC_Flags_TILEDATA) ? 0x8000 : 0x9000;

    // Row of the background map
    u8 y = *ppu_state->ly + *ppu_state->scy;

    for (i32 x = 0; x < RES_X; x++)
    {
        // Column of the background map
        u8 bg_x = x + *ppu_state->scx;

        u16 tile_row = (y / 8) % 32;
        u16 tile_col = (bg_x / 8) % 32;
        u16 tile_addr = tilemap + (tile_row * 32) + tile_col;
        u8 tile_id = ppu_state->vram[tile_addr - VRAM_START];

        u16 tile_data_addr;
        if (tile_base_address == 0x8000)
        {
            // unsigned addressing
            tile_data_addr = tile_base_address + tile_id * 16;
        }
        else
        {
            // signed addressing
            tile_data_addr = tile_base_address + (i8)tile_id * 16;
        }

        u8 tile_line = y % 8;
        u8 byte1 = ppu_state->vram[tile_data_addr + (tile_line * 2) - VRAM_START];
        u8 byte2 = ppu_state->vram[tile_data_addr + (tile_line * 2) + 1 - VRAM_START];

        u8 bit = 7 - (bg_x % 8);
        u8 color = ((byte2 >> bit) & 1) << 1 | ((byte1 >> bit) & 1);

        ppu_state->frame_buffer[*ppu_state->ly * 160 + x] = apply_palette(*ppu_state->bgp, color);
    }
}

static void ppu_render_sprites()
{
    u8 sprite_size = *ppu_state->lcdc & LCDC_Flags_SPRITE_SIZE ? 16 : 8;

    Sprite visible[10];
    u8 count = 0;

    for (i32 i = 0; i < 40 && count < 10; i++)
    {
        u8 sy = ppu_state->oam_ram[i].y - 16;
        u8 sx = ppu_state->oam_ram[i].x - 8;

        if (*ppu_state->ly < sy || *ppu_state->ly >= sy + sprite_size)
        {
            continue;
        }

        visible[count].oam_index = i;
        visible[count].sy = sy;
        visible[count].sx = sx;
        visible[count].tile = ppu_state->oam_ram[i].tile_index;
        visible[count].flags = ppu_state->oam_ram[i].flags;
        count++;
    }

    for (i32 i = 0; i < count - 1; i++)
    {
        for (int j = i + 1; j < count; j++)
        {
            b8 swap = false;

            if (visible[j].sx < visible[i].sx)
            {
                swap = true;
            }
            else if (visible[i].sx == visible[i].sx && visible[j].oam_index < visible[i].oam_index)
            {
                swap = true;
            }

            if (swap)
            {
                Sprite tmp = visible[i];
                visible[i] = visible[j];
                visible[j] = tmp;
            }
        }
    }

    b8 sprite_drawn[RES_X];
    for (i32 i = 0; i < RES_X; i++)
    {
        sprite_drawn[i] = false;
    }

    for (i32 i = 0; i < count; i++)
    {

        u8 sy = visible[i].sy;
        u8 sx = visible[i].sx;
        u8 tile = visible[i].tile;
        u8 flags = visible[i].flags;

        b8 flip_x = flags & bit5;
        b8 flip_y = flags & bit6;
        b8 priority = flags & bit7;
        u8 palette = (flags & bit4) ? *ppu_state->obp1 : *ppu_state->obp0;

        if (sprite_size == 16)
        {
            tile &= 0xFE;
        }

        i32 tile_line = *ppu_state->ly - sy;
        if (flip_y)
        {
            tile_line = sprite_size - 1 - tile_line;
        }

        u16 tile_addr = VRAM_START + (tile * 16) + (tile_line * 2);
        u8 byte1 = ppu_state->vram[tile_addr - VRAM_START];
        u8 byte2 = ppu_state->vram[tile_addr + 1 - VRAM_START];

        for (i32 px = 0; px < 8; px++)
        {
            i32 screen_x = sx + px;
            if (screen_x < 0 || screen_x >= RES_X)
            {
                continue;
            }

            if (sprite_drawn[screen_x])
            {
                continue;
            }

            u8 bit = flip_x ? px : 7 - px;
            u8 color = ((byte2 >> bit) & 1) << 1 | ((byte1 >> bit) & 1);

            // color 0 is transparent for sprites
            if (color == 0)
            {
                continue;
            }

            u8 bg_color = ppu_state->frame_buffer[*ppu_state->ly * RES_X + screen_x];
            if (priority && bg_color != 0)
            {
                continue;
            }

            ppu_state->frame_buffer[*ppu_state->ly * RES_X + screen_x] = apply_palette(palette, color);
            sprite_drawn[screen_x] = true;
        }
    }

}

static void ppu_render_window()
{
    if (*ppu_state->ly < *ppu_state->wy)
    {
        return;
    }

    i32 wx = (i32)*ppu_state->wx - 7;

    if (wx >= RES_X)
    {
        return;
    }

    u16 tilemap = (*ppu_state->lcdc & LCDC_Flags_WINDOW_TILEMAP) ? 0x9C00 : 0x9800;

    b8 signed_addressing = !(*ppu_state->lcdc & LCDC_Flags_TILEDATA);

    b8 drew_pixels = false;

    for (i32 x = 0; x < RES_X; x++)
    {
        if (x < wx)
        {
            continue;
        }

        u8 win_x = x - wx;
        u8 win_y = ppu_state->window_line;

        u16 tile_row = (win_y / 8) % 32;
        u16 tile_col = (win_x / 8) % 32;
        u16 tile_addr = tilemap + (tile_row * 32) + tile_col;
        u8 tile_id = ppu_state->vram[tile_addr - VRAM_START];

        u16 tile_data_addr;
        if (signed_addressing)
        {
            tile_data_addr = (u16)(0x9000 + (i8)tile_id * 16);
        }
        else
        {
            tile_data_addr = 0x8000 + tile_id * 16;
        }

        u8 tile_line = win_y % 8;
        u8 byte1 = ppu_state->vram[tile_data_addr + tile_line * 2 - VRAM_START];
        u8 byte2 = ppu_state->vram[tile_data_addr + tile_line * 2 + 1 - VRAM_START];

        u8 bit = 7 - (win_x % 8);
        u8 color = ((byte2 >> bit) & 1) << 1 | ((byte1 >> bit) & 1);

        ppu_state->frame_buffer[*ppu_state->ly * RES_X + x] = apply_palette(*ppu_state->bgp, color);

        drew_pixels = true;
    }

    if (drew_pixels)
    {
        ppu_state->window_line++;
    }
}
