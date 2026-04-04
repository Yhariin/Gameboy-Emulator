static void load_cart(Arena *arena, String rom_path)
{
    // TODO: Proper battery and save support
    // TODO: Other MBC types support
    cart_state = (Cart_State *)arena_alloc_align(arena, sizeof(Cart_State), sizeof(Cart_State));

    OS_FileHandle rom_handle = os_open_file(OS_FileAccessFlag_Read, rom_path);
    DataHeader rom_data_header = os_read_file(arena, rom_handle, 0, 0);
    os_close_file(rom_handle);

    cart_state->data = (u8 *)rom_data_header.data;
    cart_state->size = rom_data_header.size;
    cart_state->header = (Cart_Header *)(cart_state->data + 0x100);
    cart_state->header->title[15] = 0;
    cart_state->has_battery =
        (cart_state->header->cart_type == Cart_Type_MBC1_RAM_BATTERY ||
        cart_state->header->cart_type == Cart_Type_MBC2_BATTERY ||
        cart_state->header->cart_type == Cart_Type_ROM_RAM_BATTERY ||
        cart_state->header->cart_type == Cart_Type_MMM01_RAM_BATTERY ||
        cart_state->header->cart_type == Cart_Type_MBC3_TIMER_BATTERY ||
        cart_state->header->cart_type == Cart_Type_MBC3_TIMER_RAM_BATTERY ||
        cart_state->header->cart_type == Cart_Type_MBC3_RAM_BATTERY ||
        cart_state->header->cart_type == Cart_Type_MBC5_RAM_BATTERY ||
        cart_state->header->cart_type == Cart_Type_MBC5_RUMBLE_RAM_BATTERY ||
        cart_state->header->cart_type == Cart_Type_MBC7_SENSOR_RUMBLE_RAM_BATTERY ||
        cart_state->header->cart_type == Cart_Type_HUC1_RAM_BATTERY);
    cart_state->need_save = false;

    switch(cart_state->header->cart_type)
    {
        case Cart_Type_ROM_ONLY:
        {
            cart_state->mbc_type = Cart_MBC_Type_NO_MBC;
            break;
        }
        case Cart_Type_MBC1:
        case Cart_Type_MBC1_RAM:
        case Cart_Type_MBC1_RAM_BATTERY:
        {
            cart_state->mbc_type = Cart_MBC_Type_MBC1;
            break;
        }
        case Cart_Type_MBC2:
        case Cart_Type_MBC2_BATTERY:
        {
            cart_state->mbc_type = Cart_MBC_Type_MBC2;
            break;
        }
        case Cart_Type_MMM01:
        case Cart_Type_MMM01_RAM:
        case Cart_Type_MMM01_RAM_BATTERY:
        {
            cart_state->mbc_type = Cart_MBC_Type_MMM01;
            break;
        }
        case Cart_Type_MBC3_TIMER_BATTERY:
        case Cart_Type_MBC3_TIMER_RAM_BATTERY:
        case Cart_Type_MBC3:
        case Cart_Type_MBC3_RAM:
        case Cart_Type_MBC3_RAM_BATTERY:
        {
            cart_state->mbc_type = Cart_MBC_Type_MBC3;
            break;
        }
        case Cart_Type_MBC5:
        case Cart_Type_MBC5_RAM:
        case Cart_Type_MBC5_RAM_BATTERY:
        case Cart_Type_MBC5_RUMBLE:
        case Cart_Type_MBC5_RUMBLE_RAM:
        case Cart_Type_MBC5_RUMBLE_RAM_BATTERY:
        {
            cart_state->mbc_type = Cart_MBC_Type_MBC5;
            break;
        }
        case Cart_Type_MBC6:
        {
            cart_state->mbc_type = Cart_MBC_Type_MBC6;
            break;
        }
        case Cart_Type_MBC7_SENSOR_RUMBLE_RAM_BATTERY:
        {
            cart_state->mbc_type = Cart_MBC_Type_MBC7;
            break;
        }
        case Cart_Type_HUC1_RAM_BATTERY:
        {
            cart_state->mbc_type = Cart_MBC_Type_HUC1;
            break;
        }
        case Cart_Type_HUC3:
        {
            cart_state->mbc_type = Cart_MBC_Type_HUC3;
            break;
        }
        case Cart_Type_BANDAI_TAMA5:
        case Cart_Type_POCKET_CAMERA:
        case Cart_Type_ROM_RAM:
        case Cart_Type_ROM_RAM_BATTERY:
        default:
        {
            cart_state->mbc_type = Cart_MBC_Type_UNKNOWN;
            break;
        }
    }

    // Setup banking
    u8 bank_num = 0;
    switch(cart_state->header->ram_size)
    {
        // 1 bank
        case 0x02:
        {
            bank_num = 1;
            break;
        }
        // 4 banks
        case 0x03:
        {
            bank_num = 4;
            break;
        }
        // 16 banks
        case 0x04:
        {
            bank_num = 16;
            break;
        }
        // 8 banks
        case 0x05:
        {
            bank_num = 8;
            break;
        }
        default:
        {
            bank_num = 0;
            break;
        }
    }

    for (i32 i = 0; i < bank_num; i++)
    {
        cart_state->ram_banks[i] = (u8 *)arena_alloc_align(arena, 0x2000, sizeof(u8));
    }
    cart_state->ram_bank = cart_state->ram_banks[0];
    cart_state->rom_bank_t2 = cart_state->data + 0x4000;


    u8 checksum = 0;
    for (u16 address = 0x0134; address <= 0x014C; address++) {
        checksum = checksum - cart_state->data[address] - 1;
    }

    LOG_TRACE("Checksum %s\n", (checksum & 0xFF) ? "Passed" : "Failed");
}

static u8 cart_read(u16 address)
{
    if (cart_state->mbc_type == Cart_MBC_Type_NO_MBC)
    {
        if (address < 0x8000)
        {
            return cart_state->data[address];
        }
        else
        {
            return 0xFF;
        }
    }
    else if (cart_state->mbc_type == Cart_MBC_Type_MBC1)
    {
        if (address < 0x4000)
        {
            // ROM Bank X0 (read only)
            return cart_state->data[address];
        }
        else if (address < 0x8000)
        {
            // ROM Bank second tier (read only)
            return cart_state->rom_bank_t2[address - 0x4000];
        }
        else if (address >= 0xA000 && address <= 0xBFFF)
        {
            // RAM bank
            if (!cart_state->ram_enabled)
            {
                return 0xFF;
            }

            if (!cart_state->ram_bank)
            {
                return 0xFF;
            }

            return cart_state->ram_bank[address - 0xA000];
        }
    }

    return 0xFF;
}

static void cart_write(u16 address, u8 data)
{
    if (cart_state->mbc_type == Cart_MBC_Type_NO_MBC)
    {
        return;
    }

    if (cart_state->mbc_type == Cart_MBC_Type_MBC1)
    {
        if (address < 0x2000)
        {
            // RAM Enable (write only)
            cart_state->ram_enabled = ((data & 0x0F) == 0x0A);
        }
        else if(address >= 0x2000 && address <= 0x3FFF)
        {
            // ROM Bank Number (write only)
            if (data == 0)
            {
                data = 1;
            }

            cart_state->rom_bank_index = data & 0b11111;
            cart_state->rom_bank_t2 = cart_state->data + (0x4000 * cart_state->rom_bank_index);
        }
        else if (address >= 0x4000 && address <= 0x5FFF)
        {
            // RAM bank number (write only)
            if (cart_state->need_save)
            {
                cart_battery_save();
            }

            cart_state->ram_bank_index = data & 0b11;
            if (cart_state->ram_banking_enabled)
            {
                cart_state->ram_bank = cart_state->ram_banks[cart_state->ram_bank_index];
            }
        }
        else if (address >= 0x6000 && address <= 0x7FFF)
        {
            // Banking mode select (write only)
            if (cart_state->need_save)
            {
                cart_battery_save();
            }

            cart_state->banking_mode = data & 0x1;

            cart_state->ram_banking_enabled = cart_state->banking_mode;

            if (cart_state->ram_banking_enabled)
            {
                cart_state->ram_bank = cart_state->ram_banks[cart_state->ram_bank_index];
            }
        }
        else if (address >= 0xA000 && address <= 0xBFFF)
        {
            // RAM Bank
            if (!cart_state->ram_enabled)
            {
                return;
            }

            if (!cart_state->ram_bank)
            {
                return;
            }

            if (cart_state->has_battery)
            {
                cart_state->need_save = true;
            }

            cart_state->ram_bank[address - 0xA000] = data;

        }
    }
}

static void cart_battery_save()
{
    TempArena scratch = temp_arena_begin(get_scratch_arena());

    String save_file_name = c_string_to_String(scratch.arena, cart_state->header->title, c_strlen(cart_state->header->title));
    save_file_name = string_cat(scratch.arena, save_file_name, string_lit(".save"));
    OS_FileHandle save_file_handle = os_open_file(OS_FileAccessFlag_Write, save_file_name);

    os_write_file(save_file_handle, cart_state->ram_bank, 0x2000);

    temp_arena_end(scratch);
}

static void cart_battery_load()
{
    TempArena scratch = temp_arena_begin(get_scratch_arena());

    String save_file_name = c_string_to_String(scratch.arena, cart_state->header->title, c_strlen(cart_state->header->title));
    save_file_name = string_cat(scratch.arena, save_file_name, string_lit(".save"));
    OS_FileHandle save_file_handle = os_open_file(OS_FileAccessFlag_Read, save_file_name);

    DataHeader save_file = os_read_file(scratch.arena, save_file_handle, 0, 0);
    memcpy(cart_state->ram_bank, save_file.data, 0x2000);

    temp_arena_end(scratch);
}