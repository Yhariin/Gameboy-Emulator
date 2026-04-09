
static u8 io_read(u16 address)
{
    if (address == Hardware_Registers_P1_JOYP)
    {
        // TODO: handle joypad output
        return 0;
    }

    // if (address >= Hardware_Registers_LCDC && address <= Hardware_Registers_WX)
    // {
    //     // TODO: handle lcd control
    //     return 0;
    // }

    return cpu_state->memory[address];

}

static void io_write(u16 address, u8 data)
{
    if (address == Hardware_Registers_P1_JOYP)
    {
        // TODO: handle joypad output
        return;
    }

    // if (address >= Hardware_Registers_LCDC && address <= Hardware_Registers_WX)
    // {
    //     // TODO: handle lcd control
    //     return;
    // }

    if (address == Hardware_Registers_DMA)
    {
        dma_start(data);
        LOG_DEBUG("DMA START\n");
    }

    cpu_state->memory[address] = data;
}
