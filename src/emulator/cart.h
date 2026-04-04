#pragma once

enum Cart_Type : u8
{
    Cart_Type_ROM_ONLY                           = 0x00,
    Cart_Type_MBC1                               = 0x01,
    Cart_Type_MBC1_RAM                           = 0x02,
    Cart_Type_MBC1_RAM_BATTERY                   = 0x03,
    Cart_Type_MBC2                               = 0x05,
    Cart_Type_MBC2_BATTERY                       = 0x06,
    Cart_Type_ROM_RAM                            = 0x08,
    Cart_Type_ROM_RAM_BATTERY                    = 0x09,
    Cart_Type_MMM01                              = 0x0B,
    Cart_Type_MMM01_RAM                          = 0x0C,
    Cart_Type_MMM01_RAM_BATTERY                  = 0x0D,
    Cart_Type_MBC3_TIMER_BATTERY                 = 0x0F,
    Cart_Type_MBC3_TIMER_RAM_BATTERY             = 0x10,
    Cart_Type_MBC3                               = 0x11,
    Cart_Type_MBC3_RAM                           = 0x12,
    Cart_Type_MBC3_RAM_BATTERY                   = 0x13,
    Cart_Type_MBC5                               = 0x19,
    Cart_Type_MBC5_RAM                           = 0x1A,
    Cart_Type_MBC5_RAM_BATTERY                   = 0x1B,
    Cart_Type_MBC5_RUMBLE                        = 0x1C,
    Cart_Type_MBC5_RUMBLE_RAM                    = 0x1D,
    Cart_Type_MBC5_RUMBLE_RAM_BATTERY            = 0x1E,
    Cart_Type_MBC6                               = 0x20,
    Cart_Type_MBC7_SENSOR_RUMBLE_RAM_BATTERY     = 0x22,
    Cart_Type_POCKET_CAMERA                      = 0xFC,
    Cart_Type_BANDAI_TAMA5                       = 0xFD,
    Cart_Type_HUC3                               = 0xFE,
    Cart_Type_HUC1_RAM_BATTERY                   = 0xFF,
};

enum Cart_MBC_Type : u8
{
    Cart_MBC_Type_NO_MBC,
    Cart_MBC_Type_MBC1,
    Cart_MBC_Type_MBC2,
    Cart_MBC_Type_MBC3,
    Cart_MBC_Type_MBC5,
    Cart_MBC_Type_MBC6,
    Cart_MBC_Type_MBC7,
    Cart_MBC_Type_MMM01,
    Cart_MBC_Type_M161,
    Cart_MBC_Type_HUC1,
    Cart_MBC_Type_HUC3,
    Cart_MBC_Type_UNKNOWN
};

struct Cart_Header
{
    u8 entry_point[4];
    u8 logo[0x30];
    c8 title[16];
    u8 sgb_flag;
    Cart_Type cart_type;
    u8 rom_size;
    u8 ram_size;
    u8 dest_code;
    u8 license_code;
    u8 version;
    u8 checksum;
    u16 global_checksum;
};


struct Cart_State
{
    u8 *data;
    Cart_Header *header;
    u64 size;
    Cart_MBC_Type mbc_type;

    // MBC1, consider making a separate struct for these values
    b8 ram_enabled;
    b8 ram_banking_enabled;

    u8 *rom_bank_t2;
    u8 banking_mode;

    u8 rom_bank_index;
    u8 ram_bank_index;

    u8 *ram_bank;
    u8 *ram_banks[16];

    b8 has_battery;
    b8 need_save;
};

static Cart_State *cart_state = nullptr;
static void load_cart(Arena *arena, String rom_path);

static u8 cart_read(u16 address);
static void cart_write(u16 address, u8 data);

static void cart_battery_save();
static void cart_battery_load();