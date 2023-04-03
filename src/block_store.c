#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <errno.h>
#include <unistd.h>
#include "bitmap.h"
#include "block_store.h"
// include more if you need

// You might find this handy.  I put it around unused parameters, but you should
// remove it before you submit. Just allows things to compile initially.
#define UNUSED(x) (void)(x)


typedef struct block_store 
{
    bitmap_t *fbm; 
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

    // initialize FBM
    bitmap_t *fbm = bitmap_create(BLOCK_STORE_NUM_BLOCKS);
    
    bs->fbm = fbm;

    return bs;
}


// Destroys the provided block storage device
void block_store_destroy(block_store_t *const bs)
{
   if (bs) 
   {
    bitmap_destroy(bs->fbm);
    free(bs);
   }
}

// Searches for free block, defines as in use, and returns the block number 
size_t block_store_allocate(block_store_t *const bs)
{
    // bad inputs
    if (!bs) return SIZE_MAX;

    // find first free block
    size_t id = bitmap_ffz(bs->fbm);
    if (id != SIZE_MAX && id != BLOCK_STORE_AVAIL_BLOCKS)
    {
        bitmap_set(bs->fbm, id);
		(bs->blocks)[id] = calloc(BLOCK_SIZE_BYTES, 1);
        return id; 
    }
    return SIZE_MAX;
}

// Attempts to allocate the requested block id
bool block_store_request(block_store_t *const bs, const size_t block_id)
{
    if (bs != NULL) {
		if (bs -> fbm != NULL) {
			if (block_id < 255) {
				if (bitmap_test(bs->fbm, block_id)) return false;
				bitmap_set(bs->fbm, block_id);
				(bs->blocks)[block_id] = calloc(BLOCK_SIZE_BYTES, 1);
				return true;
			}
		}
    }
    return false;
}

// Frees the specified block
void block_store_release(block_store_t *const bs, const size_t block_id)
{
    // checking parameters 
    if (bs != NULL) {
		if (bs -> fbm != NULL) {
			// reset bit in fbm
			if (block_id < 255) {
				free((bs->blocks)[block_id]);
				(bs->blocks)[block_id] = NULL;
				bitmap_reset(bs -> fbm, block_id);
			}
		}
    }
}

// Counts the number of blocks marked as in use
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
    return 256 - bitmap_total_set(bs -> fbm) - 1;
}

// Returns the total number of user-addressable blocks
size_t block_store_get_total_blocks()
{
    return BLOCK_STORE_AVAIL_BLOCKS;
}

size_t block_store_read(const block_store_t *const bs, const size_t block_id, void *buffer)
{

    if (bs == NULL || buffer == NULL || block_id >= BLOCK_STORE_NUM_BYTES || block_id == 0) {
        return 0;
    }

    //turns into a void pointer
    if ((bs->blocks)[block_id]){
		memcpy(buffer, ((bs->blocks)[block_id]), BLOCK_SIZE_BYTES);
	}

    //amount of bytes written
    return BLOCK_SIZE_BYTES;
}

size_t block_store_write(block_store_t *const bs, const size_t block_id, const void *buffer)
{
    if (bs == NULL || buffer == NULL || block_id > BLOCK_STORE_NUM_BYTES) {
        return 0;
    }

    //memcpys into a void pointer
	if ((bs->blocks)[block_id]){
		memcpy(((bs->blocks)[block_id]), buffer, BLOCK_SIZE_BYTES);
	}

    //amount of bytes written
    return BLOCK_SIZE_BYTES;
}

block_store_t *block_store_deserialize(const char *const filename)
{
    // bad inputs
    if (!filename || !strcmp(filename, "\n") || !strcmp(filename, "\0") || !strcmp(filename, ""))
    {
        return 0;
    } 
    
    // system call to open file
    int fd = open(filename, O_RDONLY); 
    if (fd == -1)
    {
        return NULL;
    }

    // deserialize data
    block_store_t *bs = block_store_create();
    char *buf = malloc(BLOCK_SIZE_BYTES);

    size_t i = 0; 
    for (i = 0; i < 256; i++)
    {
        if (read(fd, buf, BLOCK_SIZE_BYTES) == -1)
        {
            return NULL;
        }
        memcpy(bs->blocks[i], buf, BLOCK_SIZE_BYTES);
    }

    return bs; 

}

size_t block_store_serialize(const block_store_t *const bs, const char *const filename)
{
    // bad inputs
    if (!bs || !filename || !strcmp(filename, "\n") || !strcmp(filename, "\0") || !strcmp(filename, ""))
    {
        return 0;
    } 

    // system call to open file
    int fd = open(filename, O_CREAT | O_WRONLY | O_TRUNC, 0777);
    if (fd == -1)
    {
        return 0;
    }

    // serialize data
    size_t i = 0; 
    char *buf = malloc(BLOCK_SIZE_BYTES);
    for (i = 0; i < 256; i++) 
    {
        
        if (!bitmap_test(bs->fbm, i))
        {
            memset(buf, '0', BLOCK_SIZE_BYTES);
        }
        else 
        {
            memcpy(buf, (bs->blocks)[i], BLOCK_SIZE_BYTES);
        }
    
        // write bytes to output file
        if (write(fd, buf, BLOCK_SIZE_BYTES) == -1) 
        {
            close(fd); // close file descriptor
        }
    }
    return BLOCK_STORE_NUM_BYTES;
}