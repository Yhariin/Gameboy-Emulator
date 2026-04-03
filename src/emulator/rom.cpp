static Rom load_rom(Arena *arena, String rom_path)
{
    Rom rom;

    OS_FileHandle rom_handle = os_open_file(OS_FileAccessFlag_Read, rom_path);
    DataHeader rom_data_header = os_read_file(arena, rom_handle, 0, 0);
    os_close_file(rom_handle);

    rom.data = (u8 *)rom_data_header.data;
    rom.size = rom_data_header.size;

    u8 checksum = 0;
    for (u16 address = 0x0134; address <= 0x014C; address++) {
        checksum = checksum - rom.data[address] - 1;
    }

    LOG_TRACE("Checksum %s\n", (checksum & 0xFF) ? "Passed" : "Failed");

    return rom;
}