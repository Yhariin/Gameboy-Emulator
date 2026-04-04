void _debug_log(LogColor log_color, const char *format, ...)
{
    SET_DEBUG_LOG_COLOR(log_color);
    // _os_set_debug_log_color(log_color);

    va_list args;
    va_start(args, format);

    const char *prefix;
    switch(log_color)
    {
        case LogColor_Debug:
        {
            prefix = "[DEBUG]: ";
            break;
        }
        case LogColor_Warn:
        {
            prefix = "[WARN]: ";
            break;
        }
        case LogColor_Error:
        {
            prefix = "[ERROR]: ";
            break;
        }
        case LogColor_None:
        default:
        {
            prefix = "";
        }
    }

    TempArena scratch = temp_arena_begin(get_scratch_arena());

    u64 prefix_size = c_strlen((c8 *)prefix);
    u64 format_size = c_strlen((c8 *)format);

    c8 *final_format;
    u64 final_format_length;
    final_format_length = prefix_size + format_size + 1;

    final_format = (c8 *)arena_alloc_align(scratch.arena, final_format_length, sizeof(c8));
    memcpy(final_format, prefix, prefix_size);
    memcpy(&final_format[prefix_size], format, format_size);
    final_format[final_format_length - 1] = 0;

    String str = string_format_stub(scratch.arena, (char *)final_format, args);
    printf("%s", c_string(scratch.arena, str));

    temp_arena_end(scratch);
    va_end(args);
}
