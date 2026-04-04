#pragma once

// UTF-8 compatible
struct String
{
    c8 *chars;
    u64 size; // Size in bytes
};

// UTF-16 compatible
struct String16
{
    c16 *chars;
    u64 size; // Size in bytes
};

#define STB_SPRINTF_IMPLEMENTATION
#define STB_SPRINTF_STATIC
#include "vendor/stb/stb_sprintf.h"

CORE_EXPORT(String string_cat(Arena *arena, String a, String b));
const char *c_string(Arena *arena, String str);
const c16 *c_string(Arena *arena, String16 str);
CORE_EXPORT(u64 c_strlen(const c8 *str));
CORE_EXPORT(String c_string_to_String(Arena *arena, const c8 *string, u64 size));
b8 operator==(String a, String b);
b8 operator==(String16 a, String16 b);

CORE_EXPORT(String string_format(Arena *arena, const char *fmt, ...));
String string_format_stub(Arena *arena, const char *format, va_list args);

String16 utf8_to_utf16(Arena *arena, String str8);
String utf16_to_utf8(Arena *arena, String16 str16);

u32 encode_utf16(Arena *arena, c32 codepoint, c16 *character);
u32 encode_utf8(Arena *arena, c32 codepoint, c8 *character);

#define string_lit(S) String((c8 *)(u8##S), sizeof(S) - 1)
#define string16_lit(S) String16((c16 *)(u##S), sizeof(S) - 2)