#include "application.h"
#include "os/win32/os_main_win32.h"

#include "base/base_include.cpp"
#include "os/os_core.cpp"

static void os_w32_fill_keytable()
{
   os_w32_keytable['A'] = OS_Key_A;
   os_w32_keytable['B'] = OS_Key_B;
   os_w32_keytable['C'] = OS_Key_C;
   os_w32_keytable['D'] = OS_Key_D;
   os_w32_keytable['E'] = OS_Key_E;
   os_w32_keytable['F'] = OS_Key_F;
   os_w32_keytable['G'] = OS_Key_G;
   os_w32_keytable['H'] = OS_Key_H;
   os_w32_keytable['I'] = OS_Key_I;
   os_w32_keytable['J'] = OS_Key_J;
   os_w32_keytable['K'] = OS_Key_K;
   os_w32_keytable['L'] = OS_Key_L;
   os_w32_keytable['M'] = OS_Key_M;
   os_w32_keytable['N'] = OS_Key_N;
   os_w32_keytable['O'] = OS_Key_O;
   os_w32_keytable['P'] = OS_Key_P;
   os_w32_keytable['Q'] = OS_Key_Q;
   os_w32_keytable['R'] = OS_Key_R;
   os_w32_keytable['S'] = OS_Key_S;
   os_w32_keytable['T'] = OS_Key_T;
   os_w32_keytable['U'] = OS_Key_U;
   os_w32_keytable['V'] = OS_Key_V;
   os_w32_keytable['W'] = OS_Key_W;
   os_w32_keytable['X'] = OS_Key_X;
   os_w32_keytable['Y'] = OS_Key_Y;
   os_w32_keytable['Z'] = OS_Key_Z;
   os_w32_keytable['0'] = OS_Key_0;
   os_w32_keytable['1'] = OS_Key_1;
   os_w32_keytable['2'] = OS_Key_2;
   os_w32_keytable['3'] = OS_Key_3;
   os_w32_keytable['4'] = OS_Key_4;
   os_w32_keytable['5'] = OS_Key_5;
   os_w32_keytable['6'] = OS_Key_6;
   os_w32_keytable['7'] = OS_Key_7;
   os_w32_keytable['8'] = OS_Key_8;
   os_w32_keytable['9'] = OS_Key_9;
   os_w32_keytable[VK_NUMPAD0] = OS_Key_Num0;
   os_w32_keytable[VK_NUMPAD1] = OS_Key_Num1;
   os_w32_keytable[VK_NUMPAD2] = OS_Key_Num2;
   os_w32_keytable[VK_NUMPAD3] = OS_Key_Num3;
   os_w32_keytable[VK_NUMPAD4] = OS_Key_Num4;
   os_w32_keytable[VK_NUMPAD5] = OS_Key_Num5;
   os_w32_keytable[VK_NUMPAD6] = OS_Key_Num6;
   os_w32_keytable[VK_NUMPAD7] = OS_Key_Num7;
   os_w32_keytable[VK_NUMPAD8] = OS_Key_Num8;
   os_w32_keytable[VK_NUMPAD9] = OS_Key_Num9;
   os_w32_keytable[VK_F1] = OS_Key_F1;
   os_w32_keytable[VK_F2] = OS_Key_F2;
   os_w32_keytable[VK_F3] = OS_Key_F3;
   os_w32_keytable[VK_F4] = OS_Key_F4;
   os_w32_keytable[VK_F5] = OS_Key_F5;
   os_w32_keytable[VK_F6] = OS_Key_F6;
   os_w32_keytable[VK_F7] = OS_Key_F7;
   os_w32_keytable[VK_F8] = OS_Key_F8;
   os_w32_keytable[VK_F9] = OS_Key_F9;
   os_w32_keytable[VK_F10] = OS_Key_F10;
   os_w32_keytable[VK_F11] = OS_Key_F11;
   os_w32_keytable[VK_F12] = OS_Key_F12;
   os_w32_keytable[VK_F13] = OS_Key_F13;
   os_w32_keytable[VK_F14] = OS_Key_F14;
   os_w32_keytable[VK_F15] = OS_Key_F15;
   os_w32_keytable[VK_F16] = OS_Key_F16;
   os_w32_keytable[VK_F17] = OS_Key_F17;
   os_w32_keytable[VK_F18] = OS_Key_F18;
   os_w32_keytable[VK_F19] = OS_Key_F19;
   os_w32_keytable[VK_F20] = OS_Key_F20;
   os_w32_keytable[VK_F21] = OS_Key_F21;
   os_w32_keytable[VK_F22] = OS_Key_F22;
   os_w32_keytable[VK_F23] = OS_Key_F23;
   os_w32_keytable[VK_F24] = OS_Key_F24;
   os_w32_keytable[VK_SPACE] = OS_Key_Space;
   os_w32_keytable[VK_OEM_3] = OS_Key_BackTick;
   os_w32_keytable[VK_OEM_MINUS] = OS_Key_Minus;
   os_w32_keytable[VK_OEM_PLUS] = OS_Key_Equal;
   os_w32_keytable[VK_OEM_4] = OS_Key_LeftBracket;
   os_w32_keytable[VK_OEM_6] = OS_Key_RightBracket;
   os_w32_keytable[VK_OEM_1] = OS_Key_Semicolon;
   os_w32_keytable[VK_OEM_7] = OS_Key_Quote;
   os_w32_keytable[VK_OEM_COMMA] = OS_Key_Comma;
   os_w32_keytable[VK_OEM_PERIOD] = OS_Key_Period;
   os_w32_keytable[VK_OEM_2] = OS_Key_Slash;
   os_w32_keytable[VK_OEM_5] = OS_Key_BackSlash;
   os_w32_keytable[VK_TAB] = OS_Key_Tab;
   os_w32_keytable[VK_PAUSE] = OS_Key_Pause;
   os_w32_keytable[VK_ESCAPE] = OS_Key_Esc;
   os_w32_keytable[VK_SELECT] = OS_Key_Select;
   os_w32_keytable[VK_UP] = OS_Key_Up;
   os_w32_keytable[VK_DOWN] = OS_Key_Down;
   os_w32_keytable[VK_LEFT] = OS_Key_Left;
   os_w32_keytable[VK_RIGHT] = OS_Key_Right;
   os_w32_keytable[VK_BACK] = OS_Key_Backspace;
   os_w32_keytable[VK_RETURN] = OS_Key_Return;
   os_w32_keytable[VK_DELETE] = OS_Key_Delete;
   os_w32_keytable[VK_INSERT] = OS_Key_Insert;
   os_w32_keytable[VK_PRIOR] = OS_Key_PageUp;
   os_w32_keytable[VK_NEXT] = OS_Key_PageDown;
   os_w32_keytable[VK_HOME] = OS_Key_Home;
   os_w32_keytable[VK_END] = OS_Key_End;
   os_w32_keytable[VK_CAPITAL] = OS_Key_CapsLock;
   os_w32_keytable[VK_NUMLOCK] = OS_Key_NumLock;
   os_w32_keytable[VK_SCROLL] = OS_Key_ScrollLock;
   os_w32_keytable[VK_CONTROL] = OS_Key_Ctrl;
   os_w32_keytable[VK_LCONTROL] = OS_Key_Ctrl;
   os_w32_keytable[VK_RCONTROL] = OS_Key_Ctrl;
   os_w32_keytable[VK_SHIFT] = OS_Key_Shift;
   os_w32_keytable[VK_LSHIFT] = OS_Key_Shift;
   os_w32_keytable[VK_RSHIFT] = OS_Key_Shift;
   os_w32_keytable[VK_MENU] = OS_Key_Alt;
   os_w32_keytable[VK_LMENU] = OS_Key_Alt;
   os_w32_keytable[VK_RMENU] = OS_Key_Alt;
   os_w32_keytable[VK_DIVIDE] = OS_Key_NumSlash;
   os_w32_keytable[VK_MULTIPLY] = OS_Key_NumStar;
   os_w32_keytable[VK_SUBTRACT] = OS_Key_NumMinus;
   os_w32_keytable[VK_ADD] = OS_Key_NumPlus;
   os_w32_keytable[VK_DECIMAL] = OS_Key_NumPeriod;
   os_w32_keytable[VK_LBUTTON] = OS_Key_LeftMouseButton;
   os_w32_keytable[VK_RBUTTON] = OS_Key_RightMouseButton;
   os_w32_keytable[VK_MBUTTON] = OS_Key_MiddleMouseButton;
   os_w32_keytable[VK_XBUTTON1] = OS_Key_BackMouseButton;
   os_w32_keytable[VK_XBUTTON2] = OS_Key_ForwardMouseButton;
    for(i32 i = 0xDF, j = 0; i < 0xFF; i++, j++)
    {
        os_w32_keytable[i] = (OS_Key)(OS_Key_Misc0 + j);
    }

}

static OS_Event *os_w32_push_event(OS_EventType eventType, OS_W32_Window *window)
{
    OS_Event *result = os_push_event(os_w32_event_arena().elements, &global_os_state(), eventType, window->id);
    result->window_id = window->id;
    result->modifiers = os_get_modifiers();
    return result;
}

static void os_clear_event_queue()
{
    for(OS_Window *itr = global_os_state().first_window; itr != nullptr; itr = itr->next)
    {
        os_mutex_lock(itr->mutex);

        memset(&itr->event_queue[itr->read_index], 0, sizeof(OS_EventQueue));
        arena_reset_zero(os_w32_event_arena()[itr->read_index]);

        os_mutex_unlock(itr->mutex);
    }
}

OS_Modifiers os_get_modifiers()
{
    OS_Modifiers modifiers = OS_Modifiers_None;
    if (GetKeyState(VK_CONTROL) & 0x8000)
    {
        modifiers |= OS_Modifier_Ctrl;
    }
    if (GetKeyState(VK_SHIFT) & 0x8000)
    {
        modifiers |= OS_Modifier_Shift;
    }
    if (GetKeyState(VK_MENU) & 0x8000)
    {
        modifiers |= OS_Modifier_Alt;
    }
    return modifiers;
}

u64 os_page_size()
{
    SYSTEM_INFO info;
    GetSystemInfo(&info);
    return info.dwPageSize;
}

void *os_reserve(u64 size)
{
    // Round to nearest page size
    u64 gb_round_up = align_up_pow2(size, os_page_size());

    void *ptr = VirtualAlloc(nullptr, gb_round_up, MEM_RESERVE, PAGE_NOACCESS);
    return ptr;
}

void os_commit(void *ptr, u64 size)
{
    // Round to nearest page size
    u64 page_round_up = align_up_pow2(size, os_page_size());

    VirtualAlloc(ptr, page_round_up, MEM_COMMIT, PAGE_READWRITE);
}

void os_decommit(void *ptr, u64 size)
{
    VirtualFree(ptr, size, MEM_DECOMMIT);
}

void os_release(void *ptr, u64 size)
{
    VirtualFree(ptr, 0, MEM_RELEASE);
}

void os_exit(i32 exit_code)
{
    ExitProcess(exit_code);
}

void os_message_box(b8 error, String title, String message)
{
    TempArena temp_arena = temp_arena_begin(get_scratch_arena());
    String16 title16 = utf8_to_utf16(temp_arena.arena, title);
    String16 message16 = utf8_to_utf16(temp_arena.arena, message);

    const c16 *c_title16 = c_string(temp_arena.arena, title16);
    const c16 *c_message16 = c_string(temp_arena.arena, message16);

    u32 flags = 0;
    flags |= MB_OK;
    flags |= (error*MB_ICONERROR);
    flags |= (error*MB_SYSTEMMODAL);

    MessageBoxW(nullptr, (WCHAR*)c_message16, (WCHAR*)c_title16, flags);

    temp_arena_end(temp_arena);
}

void os_mutex_init(OS_Mutex *mutex)
{
    InitializeCriticalSection(&mutex->lock);
}

void os_mutex_lock(OS_Mutex *mutex)
{
    EnterCriticalSection(&mutex->lock);
}

void os_mutex_unlock(OS_Mutex *mutex)
{
    LeaveCriticalSection(&mutex->lock);
}

void os_mutex_release(OS_Mutex *mutex)
{
    DeleteCriticalSection(&mutex->lock);
}

void _os_set_debug_log_color(LogColor log_color)
{
    HANDLE console_handle = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(console_handle, os_w32_debug_log_color_table[log_color]);
}

OS_FileHandle os_open_file(OS_FileAccessFlag flags, String path)
{
    OS_FileHandle file = {};

    TempArena scratch = temp_arena_begin(get_scratch_arena());
    String16 path16 = utf8_to_utf16(scratch.arena, path);

    DWORD w32_access_flags = 0;
    DWORD w32_share_mode = 0;
    DWORD w32_creation_disposition = 0;

    if (flags & OS_FileAccessFlag_Read)         { w32_access_flags |= GENERIC_READ; w32_creation_disposition |= OPEN_EXISTING; }
    if (flags & OS_FileAccessFlag_Write)        { w32_access_flags |= GENERIC_WRITE; w32_creation_disposition |= CREATE_ALWAYS; }
    if (flags & OS_FileAccessFlag_Append)       { w32_access_flags |= FILE_APPEND_DATA; w32_creation_disposition |= OPEN_ALWAYS; }
    if (flags & OS_FileAccessFlag_Execute)      { w32_access_flags |= GENERIC_EXECUTE; w32_creation_disposition |= OPEN_EXISTING; }
    if (flags & OS_FileAccessFlag_ShareRead)    { w32_share_mode   |= FILE_SHARE_READ; }
    if (flags & OS_FileAccessFlag_ShareWrite)   { w32_share_mode   |= FILE_SHARE_WRITE; }

    file = CreateFileW(
        (WCHAR *)c_string(scratch.arena, path16),
        w32_access_flags,
        w32_share_mode,
        nullptr,
        w32_creation_disposition,
        FILE_ATTRIBUTE_NORMAL,
        nullptr
    );

    ASSERT(file != INVALID_HANDLE_VALUE, "Failed to open file");

    return file;
}

u64 os_file_size(OS_FileHandle file)
{
    LARGE_INTEGER size;
    b8 success = GetFileSizeEx(file, &size);
    ASSERT(success, "Failed to get file size");

    return (u64)size.QuadPart;
}

void os_close_file(OS_FileHandle file)
{
    BOOL result = CloseHandle(file);
    ASSERT(result != 0, "Failed to close file");
}

DataHeader os_read_file(Arena *arena, OS_FileHandle file, u64 start, u64 end)
{
    // Clamp by file size
    u64 file_size = os_file_size(file);
    u64 read_size = file_size;

    if (start != 0llu || end != 0llu)
    {
        ASSERT(start <= end);
        u64 start_clamped = clamp_bot(start, 0llu);
        u64 end_clamped = clamp_top(end, file_size);
        read_size = end_clamped - start_clamped;
    }


    void *data = arena_alloc_align(arena, read_size, sizeof(u8));

    SetFilePointerEx(file, {.QuadPart = (i64)start}, nullptr, FILE_BEGIN);

    DWORD bytes_read;
    b8 success = ReadFile(file, data, read_size, &bytes_read, nullptr);
    ASSERT(success && bytes_read == read_size, "Failed to read file");

    DataHeader result = {.size = bytes_read, .data = data};

    return result;
}

b8 os_write_file(OS_FileHandle file, void *data, u64 size)
{
    DWORD bytes_written;
    b8 success = WriteFile(file, data, size, &bytes_written, nullptr);
    ASSERT(success, "Failed to write to file");

    success = (bytes_written == size);
    return success;
}

OS_FileTime os_get_file_last_write_time(String filename)
{
    OS_FileTime last_write_time = {};
    WIN32_FIND_DATAW find_data;

    TempArena scratch = temp_arena_begin(get_scratch_arena());
    String16 filename16 = utf8_to_utf16(scratch.arena, filename);

    HANDLE find_handle = FindFirstFileW((WCHAR *)c_string(scratch.arena, filename16), &find_data);

    temp_arena_end(scratch);
    if (find_handle != INVALID_HANDLE_VALUE)
    {
        memcpy(&last_write_time, &find_data.ftLastWriteTime, sizeof(OS_FileTime));
        FindClose(find_handle);
    }

    return last_write_time;
}

b8 os_copy_file(String destination, String source)
{
    TempArena scratch = temp_arena_begin(get_scratch_arena());
    String16 dest16 = utf8_to_utf16(scratch.arena, destination);
    String16 src16 = utf8_to_utf16(scratch.arena, source);

    b8 result = CopyFileW((WCHAR *)c_string(scratch.arena, src16), (WCHAR *)c_string(scratch.arena, dest16), false);

    temp_arena_end(scratch);
    return result;
}

// Returns 1 if a is newer than b
// Returns 0 if a is same as b
// Returns -1 if a is older than b
i8 os_compare_file_time(OS_FileTime a, OS_FileTime b)
{
    if (a > b) return 1;
    if (a == b) return 0;
    return -1;
}

OS_DLL os_load_dll(String dll_name)
{
    TempArena scratch = temp_arena_begin(get_scratch_arena());
    String16 dll_name16 = utf8_to_utf16(scratch.arena, dll_name);
    HMODULE result = LoadLibraryW((WCHAR *)c_string(scratch.arena, dll_name16));
    ASSERT(result, "Failed to load dll: %S\n", dll_name);

    temp_arena_end(scratch);
    return (OS_DLL)result;
}

void *os_load_dll_function(OS_DLL dll, String function_name)
{
    TempArena scratch = temp_arena_begin(get_scratch_arena());
    FARPROC result = GetProcAddress((HMODULE)dll, c_string(scratch.arena, function_name));
    ASSERT(result, "Failed to load function: %s from dll\n", function_name);

    temp_arena_end(scratch);
    return (void *)result;
}

b8 os_unload_dll(OS_DLL dll)
{
    b8 result = FreeLibrary((HMODULE)dll);
    ASSERT(result, "Failed to unload dll\n");

    return result;
}


// typedef void load_core_library(LibraryFunctions *f);

void os_export_core_library(OS_DLL library)
{
    f_load_core_library *load_core_library_in_dll = (f_load_core_library*)GetProcAddress((HMODULE)library, "load_core_library");
    ASSERT(load_core_library_in_dll != nullptr);

    LibraryFunctions f;

    f._debug_log = _debug_log;
    f.temp_arena_begin = temp_arena_begin;
    f.get_scratch_arena = get_scratch_arena;
    f.temp_arena_end = temp_arena_end;
    f.arena_alloc_align = arena_alloc_align;
    f.os_window_close = os_window_close;
    f.os_event_to_string = os_event_to_string;

    f.string_format = string_format;
    f.string_cat = string_cat;
    f.c_strlen = c_strlen;
    f.c_string_to_String = c_string_to_String;

    f.os_file_size = os_file_size;
    f.os_open_file = os_open_file;
    f.os_close_file = os_close_file;
    f.os_read_file = os_read_file;
    f.os_write_file = os_write_file;

    f.os_time_now_micro = os_time_now_micro;
    f.os_time_now_milli = os_time_now_milli;
    f.os_time_now_sec = os_time_now_sec;
    f.os_sleep_milli = os_sleep_milli;

    load_core_library_in_dll(&f);
}

static u64 os_w32_performance_frequency()
{
    static u64 result = 0;
    if (result == 0)
    {
        LARGE_INTEGER frequency;
        QueryPerformanceFrequency(&frequency);
        result = frequency.QuadPart;
    }

    return result;
}

f64 os_time_now_micro()
{
    LARGE_INTEGER time;
    f64 result = 0;
    if(QueryPerformanceCounter(&time))
    {
        result = (double)(time.QuadPart * 1'000'000.0) / (double)os_w32_performance_frequency();
    }

    return result;
}

f64 os_time_now_milli()
{
    return os_time_now_micro() / 1'000.0;
}

f64 os_time_now_sec()
{
    return os_time_now_micro() / 1'000'000.0;
}

void os_sleep_milli(f64 milliseconds)
{
    Sleep((DWORD)milliseconds);
}

OS_W32_Window *os_w32_get_window_from_id(OS_WindowID window_id)
{
    for(OS_W32_Window *itr = global_os_w32_state().first_window; itr; itr = itr->next)
    {
        if (itr->id == window_id)
        {
            return itr;
        }
    }

    return nullptr;
}

OS_W32_Window *os_w32_get_window_from_hwnd(HWND hwnd)
{
    for(OS_W32_Window *itr = global_os_w32_state().first_window; itr; itr = itr->next)
    {
        if (itr->hwnd == hwnd)
        {
            return itr;
        }
    }

    return nullptr;

}

OS_WindowID os_window_open(const Rect rect, const OS_WindowFlags flags, String title)
{
    TempArena temp_arena = temp_arena_begin(get_scratch_arena());
    String16 title16 = utf8_to_utf16(temp_arena.arena, title);
    b8 use_default_pos = (flags & OS_WindowFlag_DefaultPosition);

    W32_Window_Params window_params = {
        WS_EX_APPWINDOW,
        L"graphical_window",
        (WCHAR *)c_string(temp_arena.arena, title16),
        WINDOWED_STYLE,
        use_default_pos ? CW_USEDEFAULT : (i32)rect.x,
        use_default_pos ? CW_USEDEFAULT : (i32)rect.y,
        (i32)rect.width,
        (i32)rect.height,
        nullptr,
        nullptr,
        global_os_w32_state().hinstance,
        nullptr
    };

    HWND hwnd = (HWND)SendMessageW(service_state().service_window, CREATE_WINDOW, (WPARAM)&window_params, 0);

    // DragAcceptFiles(hwnd, true);

    SetFocus(hwnd);
    temp_arena_end(temp_arena);

    OS_W32_Window *window = (OS_W32_Window *)arena_alloc(global_os_w32_state().arena, sizeof(OS_W32_Window));
    window->id = os_generate_window_id();
    window->hwnd = hwnd;

    RECT _rect;
    GetWindowRect(hwnd, &_rect);
    window->window_placement = os_w32_Rect_from_RECT(_rect);

    // TODO: GetDpiForWindow
    window->dpi = 96.f;

    linked_list_push_back(global_os_w32_state().first_window, global_os_w32_state().last_window, window);

    OS_Window *window_state = (OS_Window *)arena_alloc(global_os_w32_state().arena, sizeof(OS_Window));
    window_state->id = window->id;
    window_state->write_index = 1;
    window_state->read_index = 0;
    window_state->event_queue[0].count = window_state->event_queue[1].count = 0;
    window_state->mutex = (OS_Mutex *)arena_alloc(global_os_w32_state().arena, sizeof(OS_Mutex));
    os_mutex_init(window_state->mutex);
    linked_list_push_back(global_os_state().first_window, global_os_state().last_window, window_state);

    return window->id;
}

void os_window_close(OS_WindowID window_id)
{
    OS_W32_Window *w32_window = os_w32_get_window_from_id(window_id);
    ASSERT(w32_window != nullptr, "os_w32_get_window_from_id(%d) failed!", window_id);

    OS_Window *window = os_get_window_from_id(window_id);
    ASSERT(window != nullptr, "os_get_window_from_id(%d) failed!", window_id);

    linked_list_remove(global_os_w32_state().first_window, global_os_w32_state().last_window, w32_window);
    linked_list_remove(global_os_state().first_window, global_os_state().last_window, window);

    SendMessageW(service_state().service_window, DESTROY_WINDOW, (WPARAM)w32_window->hwnd, 0);
}

Rect os_get_window_client_rect(OS_WindowID window_id)
{
    OS_W32_Window *w32_window = os_w32_get_window_from_id(window_id);
    RECT _rect;
    GetClientRect(w32_window->hwnd, &_rect);
    Rect rect = os_w32_Rect_from_RECT(_rect);

    return rect;
}

Rect os_get_window_full_rect(OS_WindowID window_id)
{
    OS_W32_Window *w32_window = os_w32_get_window_from_id(window_id);
    RECT _rect;
    GetWindowRect(w32_window->hwnd, &_rect);
    Rect rect = os_w32_Rect_from_RECT(_rect);

    return rect;
}

Array<Arena*, 2> &os_w32_event_arena()
{
    static Array<Arena*, 2> os_w32_event_arena = {};

    return os_w32_event_arena;
}

OS_W32_State &global_os_w32_state()
{
    static OS_W32_State global_os_w32_state = {};

    return global_os_w32_state;
}

OS_W32_Service_State &service_state()
{
    static OS_W32_Service_State service_state = {};

    return service_state;
}

Rect os_w32_Rect_from_RECT(RECT rect)
{
   return { (f32)rect.left, (f32)rect.top, (f32)(rect.right - rect.left), (f32)(rect.bottom - rect.top)};
}

static LRESULT CALLBACK os_win32_message_callback(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam)
{
    LRESULT result = 0;
    b8 release = false;
    static i32 mwheel_delta_carry = 0;
    OS_W32_Window *window = os_w32_get_window_from_hwnd(hwnd);
    if (window == nullptr)
    {
        return DefWindowProcW(hwnd, message, wparam, lparam);
    }

    switch (message)
    {
        // Window Closing
        case WM_CLOSE:
        {
            if (window->id == 0)
            {
                global_os_state().running = false;
            }

            os_window_close(window->id);
            break;
        }
        case WM_DESTROY:
        {
            PostQuitMessage(0);
            return 0;
        }
        case WM_SIZE:
        {
            window->window_placement.dim = {(f32)LOWORD(lparam), (f32)HIWORD(lparam)};
            break;
        }
        case WM_MOVE:
        {
            window->window_placement.pos = {(f32)LOWORD(lparam), (f32)HIWORD(lparam)};
            break;
        }
        // Window Events
        case WM_ACTIVATE:
        {
            if (LOWORD(wparam) == WA_ACTIVE)
            {
                global_os_state().active = true;
            }
            else
            {
                global_os_state().active = false;
            }
            break;
        }
        case WM_KILLFOCUS:
        {
            os_w32_push_event(OS_EventType_WindowLostFocus, window);
            break;
        }
        case WM_SETFOCUS:
        {
            os_w32_push_event(OS_EventType_WindowFocus, window);
            break;
        }
        // Keyboard Messages
        case WM_SYSCOMMAND:
        {
            // Disable the window menu when pressing the alt key
            if ((wparam & 0xFFF0) == SC_KEYMENU)
            {
                return S_OK;
            }

            result = DefWindowProcW(hwnd, message, wparam, lparam);
            break;
        }

        case WM_KEYDOWN:
        case WM_SYSKEYDOWN:
        {
            // Extract win32 keycode
            u8 keycode = (u8)wparam;
            // Convert win32 keycode to OS_Key
            OS_Key key = os_w32_keytable[keycode];
            OS_Event *event = os_w32_push_event(OS_EventType_KeyPressed, window);
            event->key = key;
            // If key is a modifier, then remove it from the modifier mask to avoid duplicates
            if (event->key == OS_Key_Ctrl && event->modifiers & OS_Modifier_Ctrl) { event->modifiers &= ~OS_Modifier_Ctrl; }
            if (event->key == OS_Key_Shift && event->modifiers & OS_Modifier_Shift) { event->modifiers &= ~OS_Modifier_Shift; }
            if (event->key == OS_Key_Alt && event->modifiers & OS_Modifier_Alt) { event->modifiers &= ~OS_Modifier_Alt; }

            os_key_states[key] = true;

            break;
        }

        case WM_KEYUP:
        case WM_SYSKEYUP:
        {
            // Extract win32 keycode
            u8 keycode = (u8)wparam;
            // Convert win32 keycode to OS_Key
            OS_Key key = os_w32_keytable[keycode];
            OS_Event *event = os_w32_push_event(OS_EventType_KeyReleased, window);

            event->key = key;
            // If key is a modifier, then remove it from the modifier mask to avoid duplicates
            if (event->key == OS_Key_Ctrl && event->modifiers & OS_Modifier_Ctrl) { event->modifiers &= ~OS_Modifier_Ctrl; }
            if (event->key == OS_Key_Shift && event->modifiers & OS_Modifier_Shift) { event->modifiers &= ~OS_Modifier_Shift; }
            if (event->key == OS_Key_Alt && event->modifiers & OS_Modifier_Alt) { event->modifiers &= ~OS_Modifier_Alt; }

            os_key_states[key] = false;

            break;
        }
        case WM_CHAR:
        {
            c32 character = wparam;

            if (character >= 32 && character != 127)
            {
                OS_Event *event = os_w32_push_event(OS_EventType_KeyTyped, window);
                if (lparam & bit28)
                {
                    event->modifiers |= OS_Modifier_Alt;
                }
                event->character = character;

            }

            break;
        }
        case WM_MOUSEMOVE:
        {
            POINTS pt = MAKEPOINTS(lparam);

            OS_Event *event = os_w32_push_event(OS_EventType_MouseMoved, window);
            event->pos = {(f32)(i16)pt.x, (f32)(i16)pt.y};

            break;
        }

        case WM_LBUTTONUP:
        case WM_RBUTTONUP:
        case WM_MBUTTONUP:
        case WM_XBUTTONUP:
        {
            release = true;
        } // fallthrough
        case WM_LBUTTONDOWN:
        case WM_RBUTTONDOWN:
        case WM_MBUTTONDOWN:
        case WM_XBUTTONDOWN:
        {
            OS_Event *event = os_w32_push_event(release ? OS_EventType_MouseButtonReleased : OS_EventType_MouseButtonPressed, window);
            switch(message)
            {
                case WM_LBUTTONDOWN: case WM_LBUTTONUP:
                {
                    event->key = OS_Key_LeftMouseButton;
                    break;
                }
                case WM_RBUTTONDOWN: case WM_RBUTTONUP:
                {
                    event->key = OS_Key_RightMouseButton;
                    break;
                }
                case WM_MBUTTONDOWN: case WM_MBUTTONUP:
                {
                    event->key = OS_Key_MiddleMouseButton;
                    break;
                }
                case WM_XBUTTONDOWN: case WM_XBUTTONUP:
                {
                    WORD x_button = GET_XBUTTON_WPARAM(wparam);
                    if (x_button == 1)
                    {
                        event->key = OS_Key_BackMouseButton;
                    }
                    else if (x_button == 2)
                    {
                        event->key = OS_Key_ForwardMouseButton;
                    }
                    break;
                }
            }
            POINTS pt = MAKEPOINTS(lparam);
            event->pos = {(f32)(i16)pt.x, (f32)(i16)pt.y};

            os_key_states[event->key] = release ? false : true;

            break;
        }

        case WM_MOUSEWHEEL:
        {
            i16 wheel_delta = GET_WHEEL_DELTA_WPARAM(wparam);
            mwheel_delta_carry += wheel_delta;

            // Calculate when we should generate a mouse scroll event given delta
            while(mwheel_delta_carry >= WHEEL_DELTA) // Positive scrolling
            {
                mwheel_delta_carry -= WHEEL_DELTA;
                OS_Event *event = os_w32_push_event(OS_EventType_MouseScrolled, window);
                event->delta = {0, WHEEL_DELTA};

            }
            while(mwheel_delta_carry <= -WHEEL_DELTA) // Negative scrolling
            {
                mwheel_delta_carry += WHEEL_DELTA;
                OS_Event *event = os_w32_push_event(OS_EventType_MouseScrolled, window);
                event->delta = {0, -WHEEL_DELTA};
            }

            break;
        }

        default:
        {
            result = DefWindowProcW(hwnd, message, wparam, lparam);
            break;
        }
    }

    return result;
}
static LRESULT CALLBACK ServiceWndProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam)
{
    LRESULT result = 0;

    switch(message)
    {
        case CREATE_WINDOW:
        {
            W32_Window_Params *window_params = (W32_Window_Params *)wparam;
            result = (LRESULT)CreateWindowExW(
                window_params->dwExStyle,
                window_params->lpClassName,
                window_params->lpWindowName,
                window_params->dwStyle,
                window_params->X,
                window_params->Y,
                window_params->nWidth,
                window_params->nHeight,
                window_params->hWndParent,
                window_params->hMenu,
                window_params->hInstance,
                window_params->lpParam);

            break;
        }
        case DESTROY_WINDOW:
        {
            DestroyWindow((HWND)wparam);
            break;
        }

        default:
        {
            result = DefWindowProcW(hwnd, message, wparam, lparam);
        }
    }
    return result;
}

// Entry point
static DWORD WINAPI service_thread(LPVOID param)
{
    WNDCLASSEXW WindowClass = {};
    WindowClass.cbSize = sizeof(WindowClass);
    WindowClass.lpfnWndProc = &ServiceWndProc;
    WindowClass.hInstance = GetModuleHandleW(NULL);
    WindowClass.hIcon = LoadIconA(NULL, IDI_APPLICATION);
    WindowClass.hCursor = LoadCursorA(NULL, IDC_ARROW);
    WindowClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    WindowClass.lpszClassName = L"DTCClass";
    RegisterClassExW(&WindowClass);

    service_state().service_window = CreateWindowExW(
        0,
        WindowClass.lpszClassName,
        L"DTCService",
        0,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        0,
        0,
        WindowClass.hInstance,
        0
    );

    LOG_DEBUG("Service Window Created\n");
    service_state().service_startup = true;
    WakeByAddressSingle(&service_state().service_startup);

    MSG message;
    while(true)
    {
        GetMessageW(&message, 0, 0, 0);
        TranslateMessage(&message);
        DispatchMessageW(&message);
    }

}

static void os_w32_set_cwd()
{
    char buf[512];
    u32 size = GetModuleFileNameA(nullptr, buf, sizeof(buf));

    ASSERT(size != 0);

    u32 last_slash_pos = 0;
    for(u32 i = 0; i < size; i++)
    {
        if (buf[i] == '\\')
        {
            last_slash_pos = i;
        }
    }

    char directory[512];
    memcpy(directory, buf, last_slash_pos);
    directory[last_slash_pos] = '\0';

    b8 success = SetCurrentDirectoryA(directory);
    ASSERT(success, "Failed to set current working directory");
}

static ApplicationCode os_w32_load_app_code(String dll_name, String dll_load_name)
{
    ApplicationCode result = {};
    TempArena scratch = temp_arena_begin(get_scratch_arena());

    result.last_write_time = os_get_file_last_write_time(dll_name);

    b8 succeeded = os_copy_file(dll_load_name, dll_name);
    ASSERT(succeeded);

    result.app_dll = os_load_dll(dll_load_name);
    if (result.app_dll)
    {
        result.update_and_render = (f_application_update_and_render *)os_load_dll_function(result.app_dll, string_lit("application_update_and_render"));
        result.application_shutdown = (f_application_shutdown *)os_load_dll_function(result.app_dll, string_lit("application_shutdown"));
    }
    ASSERT(result.update_and_render != nullptr);

    temp_arena_end(scratch);
    return result;
}

static void os_w32_unload_app_code(ApplicationCode *code)
{
    if (code->app_dll)
    {
        os_unload_dll(code->app_dll);
        code->app_dll = nullptr;
    }

    code->update_and_render = nullptr;
}


int main(int argc, char **argv)
{
    os_w32_event_arena()[0] = arena_init(MEGABYTES(1), KILOBYTES(1));
    os_w32_event_arena()[1] = arena_init(MEGABYTES(1), KILOBYTES(1));
    global_os_w32_state().arena = arena_init(MEGABYTES(1), KILOBYTES(1));

    // Set minimum sleep granularity to 1 millisecond;
    timeBeginPeriod(1);

    // Setup service thread that will handle window creation to prevent blocking messages on win32 windows
    service_state().service_startup_unfinished = false;
    service_state().service_startup = false;

    os_w32_set_cwd();

    // TODO: Use OS_Threads instead of win32 specific
    CreateThread(0, 0, service_thread, 0, 0, &service_state().service_thread_id);
    WaitOnAddress(&service_state().service_startup, &service_state().service_startup_unfinished, sizeof(b8), INFINITE);

    SetProcessDPIAware();
    WNDCLASSEXW window_class = {};
    window_class.cbSize = sizeof(WNDCLASSEXW);
    window_class.style = 0;
    window_class.lpfnWndProc = os_win32_message_callback;
    window_class.hInstance = global_os_w32_state().hinstance;
    window_class.hIcon = LoadIcon(0, IDI_APPLICATION);
    window_class.hCursor = LoadCursor(0, IDC_ARROW);
    window_class.lpszClassName = L"graphical_window";
    RegisterClassExW(&window_class);
    global_os_w32_state().hinstance = GetModuleHandleA(nullptr);

    #if DEBUG
        SetConsoleOutputCP(CP_UTF8);
    #endif
    os_w32_fill_keytable();

    // Calculate center of screen to place window
    // TODO: Perform intial window sizing calculation baseed on user monitor resolution
    i32 const scalef = 4;
    i32 width = 160 * scalef;
    i32 height = 144 * scalef;
    i32 center_screen_x = GetSystemMetrics(SM_CXSCREEN) / 2 - width / 2;
    i32 center_screen_y = GetSystemMetrics(SM_CYSCREEN) / 2 - height / 2;

    Rect window_rect = {.x = (f32)center_screen_x, .y = (f32)center_screen_y, .width = (f32)width, .height = (f32)height};

    // Create main window
    OS_WindowID main_window = os_window_open(window_rect, OS_WindowFlag_None, string_lit("Game Boy Emulator"));


    ApplicationState application_state = {};
    application_state.arena = arena_init(MEGABYTES(128), KILOBYTES(128));

    // Render Buffer
    BITMAPINFO bmi = {};
    bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmi.bmiHeader.biWidth = 160;
    bmi.bmiHeader.biHeight = -144;
    bmi.bmiHeader.biPlanes = 1;
    bmi.bmiHeader.biBitCount = 32;
    bmi.bmiHeader.biCompression = BI_RGB;

    u32 frame_buffer[160 * 144];

    // TODO: Add this to window state
    // TODO: clean this up
    HDC hdc = GetDC(os_w32_get_window_from_id(main_window)->hwnd);
    u32 *ppv_bits = nullptr;

    HBITMAP bitmap = CreateDIBSection(hdc, &bmi, DIB_RGB_COLORS, (void **)&ppv_bits, nullptr, 0);
    for(int i = 0; i < ARRAY_COUNT(frame_buffer); i++)
    {
        // frame_buffer[i] = 0x00ff00ff;
        ppv_bits[i] = 0x00ff00ff;
        // if (i % 4 == 0)
        //     frame_buffer[i] = 0;
        // else
        //     frame_buffer[i] = 0xFf;
    }
    // memcpy(ppv_bits, frame_buffer, sizeof(frame_buffer));
    HDC hdc_mem = CreateCompatibleDC(hdc);
    SelectObject(hdc_mem, bitmap);


    // Main loop
    global_os_state().running = true;

    // Load application dll and export core functions
    String app_dll_name = string_lit("application.dll");
    String app_load_dll_name = string_lit("application_load.dll");
    ApplicationCode application_code = os_w32_load_app_code(app_dll_name, app_load_dll_name);
    os_export_core_library(application_code.app_dll);

    f64 last_frame_time = os_time_now_milli();
    while(global_os_state().running)
    {
        f64 frame_start = os_time_now_milli();
        f64 dt = frame_start - last_frame_time;
        last_frame_time = frame_start;

        application_state.dt = dt;
        application_state.frame_start = frame_start;
        // LOG_DEBUG("ms: %f, %f\n", dt, 1000.f/(dt));

        OS_FileTime new_dll_write_time = os_get_file_last_write_time(app_dll_name);

        if (os_compare_file_time(new_dll_write_time, application_code.last_write_time) != 0)
        {
            LOG_DEBUG("Reloading dll...\n");
            os_w32_unload_app_code(&application_code);
            application_code = os_w32_load_app_code(app_dll_name, app_load_dll_name);
            Sleep(50); // NOTE: we need to sleep for a bit to prevent errors
        }

        // Update events and key states
        application_state.first_window = global_os_state().first_window;
        application_state.last_window = global_os_state().last_window;
        application_state.key_states = os_key_states;

        // Swap queue buffers
        for(OS_Window *itr = global_os_state().first_window; itr != nullptr; itr = itr->next)
        {
            os_mutex_lock(itr->mutex);

            itr->write_index ^= 1;
            itr->read_index ^= 1;
            itr->event_queue[itr->write_index].count = 0;

            os_mutex_unlock(itr->mutex);
        }

        // TODO: initialize frame_buffer
        application_code.update_and_render(&application_state);

        StretchBlt(hdc, 0, 0, width, height, hdc_mem, 0, 0, 160, 144, SRCCOPY);
        // BitBlt(hdc, 0, 0, 160, 144, hdc_mem, 0, 0, SRCCOPY);

        os_clear_event_queue();
    }
    application_code.application_shutdown();

    timeEndPeriod(1);
    return 0;
}