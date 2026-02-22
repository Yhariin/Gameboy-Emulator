Arena *get_scratch_arena()
{
    static Arena *scratch;
    if (!scratch)
    {
        scratch = arena_init(MEGABYTES(16), KILOBYTES(64));
    }
    return scratch;
}

TempArena temp_arena_begin(Arena *arena)
{
    TempArena temp;

    // Store the state of the arena
    temp.arena = arena;
    temp.starting_position = arena->position;

    return temp;
}

void temp_arena_end(TempArena temp)
{
    // Restore the state of the arena
    temp.arena->position = temp.starting_position;
}

Arena *arena_init(u64 reserve_size, u64 commit_size)
{
    // Allocate one chunk: struct + buffer
    reserve_size = align_up_pow2(reserve_size, os_page_size());
    commit_size = align_up_pow2(commit_size, os_page_size());

    // Reserve/commit initial block
    void *base  = os_reserve(reserve_size);
    os_commit(base, commit_size);

    Arena *arena = (Arena *)base;
    arena->buf = (u8 *)(arena); // pointer arithmetic so arena->buf starts at the first byte after the Arena
    arena->position = sizeof(Arena);
    arena->reserved_size = reserve_size;
    arena->committed_size = commit_size;

    return arena;
}

void *arena_alloc(Arena *arena, u64 size)
{
    return arena_alloc_align(arena, size, size);
}

void *arena_alloc_align(Arena *arena, u64 size, u64 alignment)
{
    // Align current offset forward to specified alignment
    u64 aligned_pos = align_up_pow2(arena->position, alignment); // Absolute offset
    u64 position_postop = aligned_pos + size;

    // Not enough reserved memory, fail
    ASSERT(arena->reserved_size >= position_postop, "Not enough reserved memory for commit");
    if (arena->reserved_size < position_postop)
    {
        LOG_ERROR("ERROR: Not enough reserved memory for commit\n");
        return nullptr;
    }

    // Commit new pages
    if (arena->committed_size < position_postop)
    {
        LOG_DEBUG("Committing new pages\n");

        // Align to nearest commit size
        u64 commit_postop_aligned = align_up_pow2(position_postop, arena->committed_size);

        // Calculate additional commit_size by Total size required - Total committed memory size
        u64 total_committed_size = align_up_pow2(arena->position, os_page_size());
        u64 commit_size = commit_postop_aligned - total_committed_size;

        u8 *commit_ptr = (u8 *)arena->buf + arena->position;
        os_commit(commit_ptr, commit_size);
        arena->committed_size += commit_size;
    }

    // Push onto current block
    if (position_postop <= arena->committed_size)
    {
        void *ptr = &arena->buf[aligned_pos];
        arena->position = position_postop;

        return ptr;
    }

    return nullptr;
}

void arena_free(Arena *arena)
{
    os_release(arena, arena->reserved_size);
}

void arena_reset(Arena *arena)
{
    arena->position = sizeof(Arena);
}

void arena_reset_zero(Arena *arena)
{
    arena->position = sizeof(Arena);
    memset(arena->buf + sizeof(Arena), 0, arena->committed_size - sizeof(Arena));
}