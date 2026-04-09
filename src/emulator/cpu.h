#pragma once

struct Registers
{
    u8 a;
    u8 b;
    u8 c;
    u8 d;
    u8 e;
    u8 f;
    u8 h;
    u8 l;
};

enum Hardware_Registers : u16
{
    Hardware_Registers_P1_JOYP          = 0xFF00,
    Hardware_Registers_SB               = 0xFF01,
    Hardware_Registers_SC               = 0xFF02,
    Hardware_Registers_DIV              = 0xFF04,
    Hardware_Registers_TIMA             = 0xFF05,
    Hardware_Registers_TMA              = 0xFF06,
    Hardware_Registers_TAC              = 0xFF07,
    Hardware_Registers_IF               = 0xFF0F,
    Hardware_Registers_NR10             = 0xFF10,
    Hardware_Registers_NR11             = 0xFF11,
    Hardware_Registers_NR12             = 0xFF12,
    Hardware_Registers_NR13             = 0xFF13,
    Hardware_Registers_NR14             = 0xFF14,
    Hardware_Registers_NR21             = 0xFF16,
    Hardware_Registers_NR22             = 0xFF17,
    Hardware_Registers_NR23             = 0xFF18,
    Hardware_Registers_NR24             = 0xFF19,
    Hardware_Registers_NR30             = 0xFF1A,
    Hardware_Registers_NR31             = 0xFF1B,
    Hardware_Registers_NR32             = 0xFF1C,
    Hardware_Registers_NR33             = 0xFF1D,
    Hardware_Registers_NR34             = 0xFF1E,
    Hardware_Registers_NR41             = 0xFF20,
    Hardware_Registers_NR42             = 0xFF21,
    Hardware_Registers_NR43             = 0xFF22,
    Hardware_Registers_NR44             = 0xFF23,
    Hardware_Registers_NR50             = 0xFF24,
    Hardware_Registers_NR51             = 0xFF25,
    Hardware_Registers_NR52             = 0xFF26,
    Hardware_Registers_WAVE_RAM_START   = 0xFF30,
    Hardware_Registers_WAVE_RAM_END     = 0xFF3F,
    Hardware_Registers_LCDC             = 0xFF40,
    Hardware_Registers_STAT             = 0xFF41,
    Hardware_Registers_SCY              = 0xFF42,
    Hardware_Registers_SCX              = 0xFF43,
    Hardware_Registers_LY               = 0xFF44,
    Hardware_Registers_LYC              = 0xFF45,
    Hardware_Registers_DMA              = 0xFF46,
    Hardware_Registers_BGP              = 0xFF47,
    Hardware_Registers_OBP0             = 0xFF48,
    Hardware_Registers_OBP1             = 0xFF49,
    Hardware_Registers_WY               = 0xFF4A,
    Hardware_Registers_WX               = 0xFF4B,
    Hardware_Registers_KEY0_SYS         = 0xFF4C,
    Hardware_Registers_KEY1_SPD         = 0xFF4D,
    Hardware_Registers_VBK              = 0xFF4F,
    Hardware_Registers_BANK             = 0xFF50,
    Hardware_Registers_HDMA1            = 0xFF51,
    Hardware_Registers_HDMA2            = 0xFF52,
    Hardware_Registers_HDMA3            = 0xFF53,
    Hardware_Registers_HDMA4            = 0xFF54,
    Hardware_Registers_HDMA5            = 0xFF55,
    Hardware_Registers_RP               = 0xFF56,
    Hardware_Registers_BCPS_BGPI        = 0xFF68,
    Hardware_Registers_BCPD_BGPD        = 0xFF69,
    Hardware_Registers_OCPS_OBPI        = 0xFF6A,
    Hardware_Registers_OCPD_OBPD        = 0xFF6B,
    Hardware_Registers_OPRI             = 0xFF6C,
    Hardware_Registers_SVBK_WBK         = 0xFF70,
    Hardware_Registers_PCM12            = 0xFF76,
    Hardware_Registers_PCM34            = 0xFF77,
    Hardware_Registers_IE               = 0xFFFF,
};

#define CLOCK_FREQ 4194304
#define DIV_FREQ 16384
#define DIV_UPDATE_RATE 256
#define REFRESH_RATE 59.7275
#define T_CYCLES_PER_FRAME 70224
#define FRAME_TIME_MS (1000.0 / REFRESH_RATE)

#define ROM_START   0x0000 // The game ROM supplied by the cartridge
#define ROM_END     0x7FFF
#define VRAM_START  0x8000 // Video RAM, where graphics are stored and arranged
#define VRAM_END    0x9FFF
#define VRAM_SIZE   (VRAM_END - VRAM_START + 1)
#define SRAM_START  0xA000 // Save RAM, optionally supplied by the cartridge to save data to
#define SRAM_END    0xBFFF
#define WRAM_START  0xC000 // Work RAM, general-purpose RAM for the game to store things in
#define WRAM_END    0xDFFF
#define ECHO_START  0xE000 // Echo RAM, use of this area is prohibited
#define ECHO_END    0xFDFF
#define OAM_START   0xFE00 // Object Attribute Memory, where "objects" are stored
#define OAM_END     0xFE9F
#define IO_START    0xFF00 // Where you control the console
#define IO_END      0xFF7F
#define HRAM_START  0xFF80 // High RAM, a tiny bit of general-purpose RAM with faster access
#define HRAM_END    0xFFFE
#define IE          0xFFFF // A lone I/O byte that's separated from the rest for some reason

// z - Zero flag
// n - Subtraction flag (BCD)
// h - Half Carry flag (BCD)
// c - Carry flag
static b8 f_register_get_z();
static b8 f_register_get_n();
static b8 f_register_get_h();
static b8 f_register_get_c();

static void f_register_set_z(b8 flag);
static void f_register_set_n(b8 flag);
static void f_register_set_h(b8 flag);
static void f_register_set_c(b8 flag);

enum InterruptFlags
{
    InterruptFlags_VBlank = bit0,
    InterruptFlags_LCD_STAT = bit1,
    InterruptFlags_Timer = bit2,
    InterruptFlags_Serial = bit3,
    InterruptFlags_Joypad = bit4,
};

static void request_interrupt(InterruptFlags interrupt);

struct Timer
{
    u32 div_counter;
    u32 tima_counter;
    b8 overflow_pending;
};

struct CPU_State
{
    Registers registers;
    u16 pc;
    u16 sp;
    b8 ime;
    b8 is_halted;
    b8 halt_bug;
    b8 ime_scheduled;
    b8 dma_transferring;
    u8 *memory;
    Cart_State *rom;
    Timer timer;
};

static String cpu_log = string_lit("");
static CPU_State *cpu_state = nullptr;
static OS_FileHandle out_file;

static u16 get_AF();
static void set_AF(u16 value);

static u16 get_BC();
static void set_BC(u16 value);

static u16 get_DE();
static void set_DE(u16 value);

static u16 get_HL();
static void set_HL(u16 value);

static void cpu_init(Arena *arena, u8 *rom_data, u64 rom_size);
static void cpu_post_boot_init();

static u8 mem_read(u16 address);
static u8 *mem_read_ref(u16 address);
static void mem_write(u16 address, u8 value);


static void read_boot_rom();

static u8 fetch_instr();
static u8 execute_instr(u8 instr);

static u8 cpu_step();

static void cpu_update_timer(u8 cycles);
static u8 cpu_process_interrupts();
static u8 process_16_bit_opcodes(u8 low);

static void print_tests();

struct DMA_State
{
    b8 active;
    u8 index;
    u8 source;
    i32 cycles_remaining;
};

DMA_State *dma_state = nullptr;

static void dma_start(u8 start);
static void dma_step(u32 cycles);