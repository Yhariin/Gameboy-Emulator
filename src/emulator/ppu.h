#pragma once

#define RES_X 160
#define RES_Y 144
#define TICKS_PER_LINE 456
#define LINES_PER_FRAME 154

struct OAM
{
    u8 y;
    u8 x;
    u8 tile_index;
    u8 flags;
};

static u8 ppu_oam_read(u16 address);
static void ppu_oam_write(u16 address, u8 data);

static u8 ppu_vram_read(u16 address);
static void ppu_vram_write(u16 address, u8 data);

enum LCD_Mode : u8
{
    LCD_Mode_0_HBLANK,
    LCD_Mode_1_VBLANK,
    LCD_Mode_2_OAM_SCAN,
    LCD_Mode_3_TRANSFER
};

enum LCDC_Flags : u8
{
    LCDC_Flags_BG_ENABLE = bit0,
    LCDC_Flags_SPRITE_ENABLE = bit1,
    LCDC_Flags_SPRITE_SIZE = bit2,
    LCDC_Flags_BG_TILEMAP = bit3,
    LCDC_Flags_TILEDATA = bit4,
    LCDC_Flags_WINDOW_ENABLE = bit5,
    LCDC_Flags_WINDOW_TILEMAP = bit6,
    LCDC_Flags_LCD_ENABLE = bit7
};

enum Stat_Select : u8
{
    Stat_Select_HBLANK = bit3,
    Stat_Select_VBLANK = bit4,
    Stat_Select_OAM = bit5,
    Stat_Select_LYC = bit6
};

struct Tile
{
    u8 pixels[8 * 8];
};

enum TileColor : u8
{
    TileColor_0_Black = 0,
    TileColor_1_DarkGray,
    TileColor_2_LightGray,
    TileColor_3_Black
};

struct Sprite
{
    i32 oam_index;
    u8 sy;
    u8 sx;
    u8 tile;
    u8 flags;
};

struct PPU_State
{
    u8* vram;
    OAM *oam_ram;
    Tile tile_set[384];
    LCD_Mode lcd_mode;
    u8* lcdc; // lcd compare
    u8* lcds; // lcd stat
    u8* ly;
    u8* lyc; // ly compare
    u8* scx; // viewport x position
    u8* scy; // viewport y position
    u8* bgp;
    u8* obp0;
    u8* obp1;
    u8* wy;
    u8* wx;

    u32 current_frame;
    u32 window_line;
    u32 cycles;
    u32 frame_buffer[RES_X * RES_Y];
};

static PPU_State *ppu_state = nullptr;

static void ppu_init();
static void ppu_step(u32 cycles);
static void ppu_render_scanline();
static void ppu_render_background();
static void ppu_render_window();
static void ppu_render_sprites();

static void increment_ly();

static u8 appy_palette(u8 palette, u8 color);
