#pragma once

struct Rom
{
    u8* data;
    u64 size;
};

static Rom load_rom(Arena *arena, String rom_path);