u64 align_up_pow2(u64 size, u64 align)
{
    return (size + align - 1) & (~(align - 1));
}

static u64 align_up(u64 size, u64 align)
{
    size += align - 1;
    size -= size % align;
    return size;
}

// Only valid if align is a power of 2
static u64 align_down_pow2(u64 size, u64 align)
{
    return size & (~(align - 1));
}

static u64 align_down(u64 size, u64 align)
{
    size -= size % align;
    return size;
}
