#pragma once

static const u64 DEFAULT_RESERVE_SIZE = MEGABYTES(32);
static const u64 DEFAULT_COMMIT_SIZE = KILOBYTES(32);

struct Arena
{
    u8 *buf; // Data
    u64 position;
    u64 reserved_size;
    u64 committed_size;
};

struct TempArena
{
    Arena *arena;
    u64 starting_position;
};

extern Arena *global_scratch_arena;

void global_scratch_arena_init();

CORE_EXPORT(Arena *get_scratch_arena());
CORE_EXPORT(TempArena temp_arena_begin(Arena *arena));
CORE_EXPORT(void temp_arena_end(TempArena temp));
CORE_EXPORT(void *arena_alloc_align(Arena *arena, u64 size, u64 alignment));

Arena *arena_init(u64 reserve_size = DEFAULT_RESERVE_SIZE, u64 commit_size = DEFAULT_COMMIT_SIZE);
void *arena_alloc(Arena *arena, u64 size);
void arena_free(Arena *arena);
void arena_reset(Arena *arena);
void arena_reset_zero(Arena *arena);