#pragma once

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#pragma comment (lib, "user32")
#pragma comment (lib, "gdi32")
#pragma comment (lib, "Synchronization.lib")
#define SLEEP(x) Sleep(x)

struct OS_W32_Window
{
    OS_W32_Window *next;
    OS_W32_Window *prev;
    HWND hwnd;
    Rect window_placement;
    f32 dpi;
    b8 maximized;
    OS_WindowID id;
};

struct OS_W32_State
{
    Arena *arena;
    HINSTANCE hinstance;
    OS_W32_Window *first_window;
    OS_W32_Window *last_window;
    u64 microsecond_resolution;
};

struct W32_Window_Params
{
    DWORD     dwExStyle;
    LPCWSTR   lpClassName;
    LPCWSTR   lpWindowName;
    DWORD     dwStyle;
    int       X;
    int       Y;
    int       nWidth;
    int       nHeight;
    HWND      hWndParent;
    HMENU     hMenu;
    HINSTANCE hInstance;
    LPVOID    lpParam;
};

const DWORD WINDOWED_STYLE = WS_SYSMENU | WS_CAPTION | WS_SIZEBOX | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_VISIBLE | WS_THICKFRAME;

// Service Thread
#define CREATE_WINDOW (WM_USER + 0x0000)
#define DESTROY_WINDOW (WM_USER + 0x0001)

struct OS_W32_Service_State
{
    b8 service_startup_unfinished;
    b8 service_startup;
    DWORD service_thread_id;
    HWND service_window;
};

Rect os_w32_Rect_from_RECT(RECT rect);
static u8 os_w32_debug_log_color_table[LogColor_COUNT] = {7, 7, 6, 4};
void os_w32_fill_log_color_table();
void _os_set_debug_log_color(LogColor log_color);

static u64 os_w32_performance_frequency();

// Threading
struct OS_Mutex
{
    CRITICAL_SECTION lock;
};

// File I/O
typedef HANDLE OS_FileHandle;


OS_W32_State &global_os_w32_state();
Array<Arena*, 2> &os_w32_event_arena();
OS_W32_Service_State &service_state();


static void os_w32_fill_keytable();
static OS_Key os_w32_keytable[NUM_KEY_STATES];

struct ApplicationCode
{
    OS_DLL app_dll;
    OS_FileTime last_write_time;
    f_application_update_and_render *update_and_render;
};
//

static OS_Event *os_w32_push_event(OS_EventType eventType, OS_W32_Window *window);

OS_W32_Window *os_w32_get_window_from_id(OS_WindowID window_id);
OS_W32_Window *os_w32_get_window_from_hwnd(HWND hwnd);


// globals