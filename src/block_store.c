#include <stdio.h>
#include <stdint.h>
#include "bitmap.h"
#include "block_store.h"
// include more if you need

// You might find this handy.  I put it around unused parameters, but you should
// remove it before you submit. Just allows things to compile initially.
#define UNUSED(x) (void)(x)


typedef struct block_store 
{
    void *blocks[BLOCK_STORE_NUM_BLOCKS]; // array of blocks
    size_t size; // size of bs in bytes
} block_store_t;


block_store_t *block_store_create()
{
    // create the block storage
    block_store_t *bs = malloc(sizeof(block_store_t));
    if (!bs) return NULL;
    
    // total # of bytes in block storage
    bs->size = BLOCK_STORE_NUM_BYTES;

    return bs;
}

void block_store_destroy(block_store_t *const bs)
{
    if (bs) 
    {
        free(bs);
    }
}

// Searches for free block, defines as in use, and returns the block number 
size_t block_store_allocate(block_store_t *const bs)
{
    UNUSED(bs);
    return 0;
}

bool block_store_request(block_store_t *const bs, const size_t block_id)
{
    UNUSED(bs);
    UNUSED(block_id);
    return false;
}

void block_store_release(block_store_t *const bs, const size_t block_id)
{
    UNUSED(bs);
    UNUSED(block_id);
}

size_t block_store_get_used_blocks(const block_store_t *const bs)
{
    UNUSED(bs);
    return 0;
}

size_t block_store_get_free_blocks(const block_store_t *const bs)
{
    UNUSED(bs);
    return 0;
}

size_t block_store_get_total_blocks()
{
    return BLOCK_STORE_AVAIL_BLOCKS;
}

size_t block_store_read(const block_store_t *const bs, const size_t block_id, void *buffer)
{
    UNUSED(bs);
    UNUSED(block_id);
    UNUSED(buffer);
    return 0;
}

size_t block_store_write(block_store_t *const bs, const size_t block_id, const void *buffer)
{
    UNUSED(bs);
    UNUSED(block_id);
    UNUSED(buffer);
    return 0;
}

block_store_t *block_store_deserialize(const char *const filename)
{
    UNUSED(filename);
    return NULL;
}

size_t block_store_serialize(const block_store_t *const bs, const char *const filename)
{
    UNUSED(bs);
    UNUSED(filename);
    return 0;
}