#pragma once


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

struct GPU_State
{
    u8* vram;
    Tile tile_set[384];
};