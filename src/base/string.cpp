String string_cat(Arena *arena, String a, String b)
{
    String result;

    result.size = a.size + b.size;
    result.chars = (c8 *)arena_alloc_align(arena, result.size, sizeof(c8));

    memcpy(&result.chars[0], a.chars, a.size);
    memcpy(&result.chars[a.size], b.chars, b.size);

    return result;
}

const char *c_string(Arena *arena, String str)
{
    char *string = (char *)arena_alloc_align(arena, str.size + 1, sizeof(char));
    memcpy(string, str.chars, str.size);
    string[str.size] = '\0';
    return string;
}

const c16 *c_string(Arena *arena, String16 str)
{
    c16 *string = (c16 *)arena_alloc_align(arena, str.size + 2, sizeof(c16));
    memcpy(string, str.chars, str.size);
    string[str.size / 2] = '\0';
    return string;
}

// NOTE: Does not include null terminator
u64 c_strlen(const c8 *str)
{
    u64 size = 0;

    while (str[size++] != '\0');

    return size - 1;
}

// NOTE: Size parameter should include null terminator
String c_string_to_String(Arena *arena, const c8 *string, u64 size)
{
    String result;
    result.size = size;
    result.chars = (c8 *)arena_alloc_align(arena, size, sizeof(c8));
    memcpy(result.chars, string, result.size);

    return result;
}

b8 operator==(String a, String b)
{
    if (a.size != b.size)
    {
        return false;
    }

    for(u64 i = 0; i < a.size; i++)
    {
        if (a.chars[i] != b.chars[i])
        {
            return false;
        }
    }

    return true;
}

b8 operator==(String16 a, String16 b)
{
    if (a.size != b.size)
    {
        return false;
    }

    for(u64 i = 0; i < a.size; i+=2)
    {
        if (a.chars[i] != b.chars[i])
        {
            return false;
        }
    }

    return true;

}

String string_format(Arena *arena, const char *format, ...)
{
    va_list args;
    va_start(args, format);

    String result = string_format_stub(arena, format, args);

    va_end(args);
    return result;
}

String string_format_stub(Arena *arena, const char *format, va_list args)
{
    String result = {};

    u32 needed_bytes = stbsp_vsnprintf(nullptr, 0, format, args) + 1;

    result.chars = (c8 *)arena_alloc_align(arena, needed_bytes, sizeof(c8));
    result.size = stbsp_vsnprintf((char *)result.chars, needed_bytes, format, args);

    return result;
}

String16 utf8_to_utf16(Arena *arena, String str8)
{
    String16 result = {};

    result.chars = (c16 *)arena_alloc_align(arena, str8.size * 4, sizeof(c16));
    c16 *ptr = result.chars;

    // i loops over each char, _8bit_itr loops over each 8 bit boundary
    for(i32 i = 0, _8bit_itr = 0; i < str8.size;)
    {
        // Extract codepoint

        c32 codepoint = 0;

        if (str8.chars[_8bit_itr] < 0b1000'0000) //
        {
            codepoint = str8.chars[_8bit_itr];
            i += 1;
            _8bit_itr += 1;
        }
        else if ((str8.chars[_8bit_itr] & 0b1110'0000) == 0b1100'0000)
        {
            codepoint = ((str8.chars[_8bit_itr] & 0b0001'1111) << 6) |
                (str8.chars[_8bit_itr + 1] & 0b0011'1111);

            i += 2;
            _8bit_itr += 2;
        }
        else if ((str8.chars[_8bit_itr] & 0b1111'0000) == 0b1110'0000)
        {
            codepoint = ((str8.chars[_8bit_itr] & 0b1111) << 12) |
                ((str8.chars[_8bit_itr + 1] & 0b0011'1111) << 6) |
                (str8.chars[_8bit_itr + 2] & 0b0011'1111);

            i += 3;
            _8bit_itr += 3;
        }
        else if ((str8.chars[_8bit_itr] & 0b1111'1000) == 0b1111'0000)
        {
            codepoint = ((str8.chars[_8bit_itr] & 0b0111) << 18) |
                ((str8.chars[_8bit_itr + 1] & 0b0011'1111) << 12) |
                ((str8.chars[_8bit_itr + 2] & 0b0011'1111) << 6) |
                (str8.chars[_8bit_itr + 3] & 0b0011'1111);

            i += 4;
            _8bit_itr += 4;
        }
        else // Invalid UTF-8
        {
            LOG_WARN("Unable to extract codepoint from utf-8 encoding\n");
        }

        u32 increment = encode_utf16(arena, codepoint, ptr);

        ptr += increment;
        result.size += (increment * 2);
    }

    return result;
}

String utf16_to_utf8(Arena *arena, String16 str16)
{
    String result = {};

    result.chars = (c8 *)arena_alloc_align(arena, str16.size * 4, sizeof(c8));
    c8 *ptr = result.chars;

    // i loops over each char, _16bit_itr loops over each 16 bit boundary
    for(i32 i = 0, _16bit_itr = 0; i < str16.size;)
    {
        // Extract codepoint

        c32 codepoint = 0;
        // if 16-bit is not in the range of 0xD800-0xDFFF
        if (str16.chars[_16bit_itr] < 0xD800 || str16.chars[_16bit_itr] > 0xDFFF)
        {
            codepoint = str16.chars[_16bit_itr];
            i += 2;
            _16bit_itr += 1;
        }
        // Surrogate pair
        else
        {
            u32 high = str16.chars[_16bit_itr] - 0xD800;
            u32 low = str16.chars[_16bit_itr+1] - 0xDC00;
            codepoint = (high << 10 | low) + 0x10000;
            i += 4;
            _16bit_itr += 2;
        }

        u32 increment = encode_utf8(arena, codepoint, ptr);

        ptr += increment;
        result.size += increment;

    }

    return result;
}

u32 encode_utf16(Arena *arena, c32 codepoint, c16 *character)
{
    u32 increment = 0;
    if (codepoint <= 0xFFFF) // 2 bytes
    {
        character[0] = codepoint;

        increment = 1;
    }
    else // 4 bytes
    {
        u32 u = codepoint - 0x10000;
        character[0] = (u16)(0xD800 + (u >> 10));
        character[1] = (u16)(0xDC00 + (u & bitmask10));

        increment = 2;
    }

    return increment;
}

u32 encode_utf8(Arena *arena, c32 codepoint, c8 *character)
{
    u32 increment = 0;
    if (codepoint <= 0x007F) // 1 byte
    {
        // 1 byte: 0xxx xxxx
        character[0] = (c8)codepoint;

        increment = 1;
    }
    else if (codepoint < 0x07FF) // 2 bytes
    {
        // 2 bytes: 110x xxxx 10xx xxxx
        character[0] = (0b11 << 6) | ((codepoint >> 6) & bitmask5);
        character[1] = bit7 | (codepoint & bitmask6);

        increment = 2;
    }
    else if (codepoint < 0xFFFF) // 3 bytes
    {
        // 3 bytes: 1110 xxxx 10xx xxxx 10xx xxxx
        character[0] = (0b111 << 5) | ((codepoint >> 12) & bitmask4);
        character[1] = bit7 | ((codepoint >> 6 ) & bitmask6);
        character[2] = bit7 | (codepoint & bitmask6);

        increment = 3;
    }
    else if (codepoint < 0x10FFFF) // 4 bytes
    {
        // 4 bytes: 1111 0xxx 10xx xxxx 10xx xxxx 10xx xxxx
        character[0] = (0b1111 << 4) | ((codepoint >> 18) & bitmask3);
        character[1] = bit7 | ((codepoint >> 12) & bitmask6);
        character[2] = bit7 | ((codepoint >> 6) & bitmask6);
        character[3] = bit7 | (codepoint & bitmask6);

        increment = 4;
    }
    else
    {
        character[0] = '?';

        increment = 1;
    }

    return increment;

}