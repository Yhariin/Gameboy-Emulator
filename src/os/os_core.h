#pragma once

enum OS_WindowFlags
{
    OS_WindowFlag_None = 0,
    OS_WindowFlag_DefaultPosition = (1 << 0),
};


enum OS_Key
{
    OS_Key_Null,
    OS_Key_LeftMouseButton,
    OS_Key_RightMouseButton,
    OS_Key_MiddleMouseButton,
    OS_Key_BackMouseButton,
    OS_Key_ForwardMouseButton,
    OS_Key_0,
    OS_Key_1,
    OS_Key_2,
    OS_Key_3,
    OS_Key_4,
    OS_Key_5,
    OS_Key_6,
    OS_Key_7,
    OS_Key_8,
    OS_Key_9,
    OS_Key_A,
    OS_Key_B,
    OS_Key_C,
    OS_Key_D,
    OS_Key_E,
    OS_Key_F,
    OS_Key_G,
    OS_Key_H,
    OS_Key_I,
    OS_Key_J,
    OS_Key_K,
    OS_Key_L,
    OS_Key_M,
    OS_Key_N,
    OS_Key_O,
    OS_Key_P,
    OS_Key_Q,
    OS_Key_R,
    OS_Key_S,
    OS_Key_T,
    OS_Key_U,
    OS_Key_V,
    OS_Key_W,
    OS_Key_X,
    OS_Key_Y,
    OS_Key_Z,
    OS_Key_Semicolon,
    OS_Key_Quote,
    OS_Key_Comma,
    OS_Key_Period,
    OS_Key_Slash,
    OS_Key_LeftBracket,
    OS_Key_RightBracket,
    OS_Key_BackSlash,
    OS_Key_Backspace,
    OS_Key_BackTick,
    OS_Key_Minus,
    OS_Key_Equal,
    OS_Key_Tab,
    OS_Key_Return,
    OS_Key_Shift,
    OS_Key_Ctrl,
    OS_Key_Alt,
    OS_Key_Pause,
    OS_Key_CapsLock,
    OS_Key_Esc,
    OS_Key_Space,
    OS_Key_PageUp,
    OS_Key_PageDown,
    OS_Key_End,
    OS_Key_Home,
    OS_Key_Select,
    OS_Key_Insert,
    OS_Key_Delete,
    OS_Key_ScrollLock,
    OS_Key_Left,
    OS_Key_Right,
    OS_Key_Up,
    OS_Key_Down,
    OS_Key_Num0,
    OS_Key_Num1,
    OS_Key_Num2,
    OS_Key_Num3,
    OS_Key_Num4,
    OS_Key_Num5,
    OS_Key_Num6,
    OS_Key_Num7,
    OS_Key_Num8,
    OS_Key_Num9,
    OS_Key_NumLock,
    OS_Key_NumSlash,
    OS_Key_NumStar,
    OS_Key_NumMinus,
    OS_Key_NumPlus,
    OS_Key_NumPeriod,
    OS_Key_F1,
    OS_Key_F2,
    OS_Key_F3,
    OS_Key_F4,
    OS_Key_F5,
    OS_Key_F6,
    OS_Key_F7,
    OS_Key_F8,
    OS_Key_F9,
    OS_Key_F10,
    OS_Key_F11,
    OS_Key_F12,
    OS_Key_F13,
    OS_Key_F14,
    OS_Key_F15,
    OS_Key_F16,
    OS_Key_F17,
    OS_Key_F18,
    OS_Key_F19,
    OS_Key_F20,
    OS_Key_F21,
    OS_Key_F22,
    OS_Key_F23,
    OS_Key_F24,
    OS_Key_Misc0,
    OS_Key_Misc1,
    OS_Key_Misc2,
    OS_Key_Misc3,
    OS_Key_Misc4,
    OS_Key_Misc5,
    OS_Key_Misc6,
    OS_Key_Misc7,
    OS_Key_Misc8,
    OS_Key_Misc9,
    OS_Key_Misc10,
    OS_Key_Misc11,
    OS_Key_Misc12,
    OS_Key_Misc13,
    OS_Key_Misc14,
    OS_Key_Misc15,
    OS_Key_Misc16,
    OS_Key_Misc17,
    OS_Key_Misc18,
    OS_Key_Misc19,
    OS_Key_Misc20,
    OS_Key_Misc21,
    OS_Key_Misc22,
    OS_Key_Misc23,
    OS_Key_Misc24,
    OS_Key_Misc25,
    OS_Key_Misc26,
    OS_Key_Misc27,
    OS_Key_Misc28,
    OS_Key_Misc29,
    OS_Key_COUNT
};

enum OS_EventType
{
    OS_EventType_Null = 0,
    OS_EventType_WindowClose,
    OS_EventType_WindowResize,
    OS_EventType_WindowFocus,
    OS_EventType_WindowLostFocus,
    OS_EventType_WindowMoved,
    OS_EventType_KeyPressed,
    OS_EventType_KeyReleased,
    OS_EventType_KeyTyped,
    OS_EventType_MouseButtonPressed,
    OS_EventType_MouseButtonReleased,
    OS_EventType_MouseMoved,
    OS_EventType_MouseScrolled,
    OS_EventType_COUNT
};

typedef u8 OS_Modifiers;
enum : u8
{
    OS_Modifiers_None = 0,
    OS_Modifier_Ctrl  = 1 << 0,
    OS_Modifier_Shift = 1 << 1,
    OS_Modifier_Alt   = 1 << 2,
};

/*
                            |                    When the file...
This argument:              |             Exists            Does not exist
----------------------------+------------------------------------------------------
OS_FileAccessFlag_Read      |              Opens                Fails
OS_FileAccessFlag_Write     |            Truncates             Creates
OS_FileAccessFlag_Append    |              Opens               Creates
OS_FileAccessFlag_Execute   |              Opens                Fails
*/
enum OS_FileAccessFlag
{
    OS_FileAccessFlag_Read          = 1 << 0,
    OS_FileAccessFlag_Write         = 1 << 1,
    OS_FileAccessFlag_Append        = 1 << 2,
    OS_FileAccessFlag_Execute       = 1 << 3,
    OS_FileAccessFlag_ShareRead     = 1 << 4,
    OS_FileAccessFlag_ShareWrite    = 1 << 5,
};

// Threading
// TODO: Thread spawning
struct OS_Mutex; // define in OS implementation

void os_mutex_init(OS_Mutex *mutex);
void os_mutex_lock(OS_Mutex *mutex);
void os_mutex_unlock(OS_Mutex *mutex);
void os_mutex_release(OS_Mutex *mutex);

typedef u64 OS_WindowID;

struct OS_Event
{
    OS_Event *next;
    OS_Event *prev;
    OS_EventType type;
    OS_Key key;
    c32 character;
    OS_Modifiers modifiers;
    Vec2 pos;
    Vec2 delta;
    OS_WindowID window_id;
};

struct OS_EventQueue
{
    OS_Event *first;
    OS_Event *last;
    u64 count;
};

struct OS_Window
{
    OS_Window *next;
    OS_Window *prev;
    OS_EventQueue event_queue[2];
    u32 write_index;
    u32 read_index;
    OS_Mutex *mutex;
    OS_WindowID id;
};

struct OS_State
{
    b8 running;
    b8 active;

    OS_Window *first_window;
    OS_Window *last_window;
};


// File I/O
struct DataHeader
{
    u64 size;
    void* data;
};

typedef void* OS_FileHandle; // Redefine in OS implementation


CORE_EXPORT(u64 os_file_size(OS_FileHandle file));
CORE_EXPORT(OS_FileHandle os_open_file(OS_FileAccessFlag flags, String path));
CORE_EXPORT(void os_close_file(OS_FileHandle file));

// To read entire file, start and end parameters should be set to 0. This is the default behavior.
CORE_EXPORT(DataHeader os_read_file(Arena *arena, OS_FileHandle file, u64 start = 0, u64 end = 0));
CORE_EXPORT(b8 os_write_file(OS_FileHandle file, void *data, u64 size));

typedef u64 OS_FileTime;

b8 os_copy_file(String destination, String source);
OS_FileTime os_get_file_last_write_time(String filename);
i8 os_compare_file_time(OS_FileTime a, OS_FileTime b);

// DLL/Libraries
typedef void* OS_DLL;

OS_DLL os_load_dll(String dll_name);
void *os_load_dll_function(OS_DLL dll, String function_name);
b8 os_unload_dll(OS_DLL dll);
void os_export_core_library(OS_DLL library);

String os_key_to_string(const OS_Key key);
String os_event_type_to_string(const OS_EventType event_type);
CORE_EXPORT(String os_event_to_string(Arena *arena, const OS_Event *const event));

void _os_set_debug_log_color(enum LogColor : u32);
#define SET_DEBUG_LOG_COLOR(color) _os_set_debug_log_color(color)

#define NUM_KEY_STATES 256
static b8 os_key_states[NUM_KEY_STATES];

static OS_EventQueue os_event_queue = {};

OS_Modifiers os_get_modifiers();

void os_exit(i32 exit_code);
void os_message_box(b8 error, String title, String message);

OS_Event *os_push_event(Arena **arena, OS_State *os_state, OS_EventType eventType, OS_WindowID window_id);
OS_Event *os_pop_event();
void os_clear_event_queue();
void os_process_events();

// Memory Alloc
u64 os_page_size();
void *os_reserve(u64 size);
void os_commit(void *ptr, u64 size);
void os_decommit(void *ptr, u64 size);
void os_release(void *ptr, u64 size);

// Windowing
OS_WindowID os_generate_window_id();
OS_Window *os_get_window_from_id(OS_WindowID window_id);
OS_WindowID os_window_open(Rect rect, OS_WindowFlags flags, String title);
CORE_EXPORT(void os_window_close(OS_WindowID window_id));
Rect os_get_window_client_rect(OS_WindowID window_id);
Rect os_get_window_full_rect(OS_WindowID window_id);

// Timing
f64 os_time_now_micro();
f64 os_time_now_milli();
f64 os_time_now_sec();
// TODO: Local Date & Time

// Globals
// extern OS_State global_os_state;

OS_State &global_os_state();