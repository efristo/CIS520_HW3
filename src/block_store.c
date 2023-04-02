#include <stdio.h>
#include <stdint.h>
#include "bitmap.h"
#include "block_store.h"
// include more if you need

// You might find this handy.  I put it around unused parameters, but you should
// remove it before you submit. Just allows things to compile initially.
#define UNUSED(x) (void)(x)

// struct defining what a block actually is
// we went with uchar since each is 1 byte
typedef struct block {
    unsigned char block[BLOCK_SIZE_BYTES];
} block_t; 

// struct defining the block store
typedef struct block_store 
{
    bitmap_t *fbm;
    block_t blocks[BLOCK_STORE_NUM_BLOCKS];
    uint32_t bitmap_blocks;
} block_store_t;

block_store_t *block_store_create()
{
    return NULL;
}

void block_store_destroy(block_store_t *const bs)
{
    
    // checks that block store actually exists 
    if (bs != NULL) {
        bitmap_destroy(bs -> fbm);
        // free block store memory 
        free(bs);
    }

}

// Searches for free block, defines as in use, and returns the block number 
size_t block_store_allocate(block_store_t *const bs)
{
    // checking parameters 
    if (bs == NULL || bs -> fbm == NULL) {
        return SIZE_MAX;
    }

    // looks for first zero bit address in the bitmap 
    size_t block_number = bitmap_ffz(bs -> fbm);

    // checking boundaries to make sure it is not out of bounds 
    if (block_number >= (BLOCK_STORE_NUM_BLOCKS - bs -> bitmap_blocks) || block_number == SIZE_MAX) {
        return SIZE_MAX;
    }

    // sets the requested bit 
    bitmap_set (bs -> fbm, block_number);
    
    // returns the allocated block #
    return block_number;
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
    // checking parameters 
    if (bs == NULL || bs -> fbm == NULL) {
        return SIZE_MAX;
    }
	
	// return count of set bits in fbm
    return bitmap_total_set(bs -> fbm);
}

size_t block_store_get_free_blocks(const block_store_t *const bs)
{
    // checking parameters 
    if (bs == NULL || bs -> fbm == NULL) {
        return SIZE_MAX;
    }
	
	// return count of non-set bits in fbm
    return 256 - bitmap_total_set(bs -> fbm);
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