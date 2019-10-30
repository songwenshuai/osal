/*
 * lfs util functions
 *
 * Copyright (c) 2017, Arm Limited. All rights reserved.
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include "lfs_util.h"
#include "lfs.h"
#include "fal.h"

// Software CRC implementation with small lookup table
uint32_t lfs_crc(uint32_t crc, const void *buffer, size_t size) {
    static const uint32_t rtable[16] = {
        0x00000000, 0x1db71064, 0x3b6e20c8, 0x26d930ac,
        0x76dc4190, 0x6b6b51f4, 0x4db26158, 0x5005713c,
        0xedb88320, 0xf00f9344, 0xd6d6a3e8, 0xcb61b38c,
        0x9b64c2b0, 0x86d3d2d4, 0xa00ae278, 0xbdbdf21c,
    };

    const uint8_t *data = buffer;

    for (size_t i = 0; i < size; i++) {
        crc = (crc >> 4) ^ rtable[(crc ^ (data[i] >> 0)) & 0xf];
        crc = (crc >> 4) ^ rtable[(crc ^ (data[i] >> 4)) & 0xf];
    }

    return crc;
}

//
//  LittleFs Port
//

#ifndef RT_DEF_LFS_DRIVERS
    #define RT_DEF_LFS_DRIVERS 1
#endif

#if (RT_DEF_LFS_DRIVERS < 1)
    #error "#define RT_DEF_LFS_DRIVERS must > 0"
#endif

#ifndef LFS_READ_SIZE
    #define LFS_READ_SIZE 256
#endif

#ifndef LFS_PROG_SIZE
    #define LFS_PROG_SIZE 256
#endif

#ifndef LFS_BLOCK_SIZE
    #define LFS_BLOCK_SIZE 4096
#endif

#ifndef LFS_CACHE_SIZE
    #define LFS_CACHE_SIZE LFS_PROG_SIZE
#endif

#ifndef LFS_BLOCK_CYCLES
    #define LFS_BLOCK_CYCLES (-1)
#endif

#ifndef LFS_LOOKAHEAD_MAX
    #define LFS_LOOKAHEAD_MAX 128
#endif

/** 
* liffleFS call the sfud read port.
* none. 
* @param[in]   c:lfs_conifg struct , block:block address, off: address 
               offset, buffer:read buffer, size:read size.
* @param[out]  noen.
* @retval  none.
* @par TAG 
*      reserved 
* @par other 
*      none 
* @par modify
*      ken 2019-01-07 create
*/
static int32_t lfs_read( const struct lfs_config *c, lfs_block_t block,
                 lfs_off_t off, void *buffer, lfs_size_t size )
{
    // Check if read is valid
    LFS_ASSERT(off  % c->read_size == 0);
    LFS_ASSERT(size % c->read_size == 0);
    LFS_ASSERT(block < c->block_count);
    LFS_ASSERT(c != NULL);
    LFS_ASSERT(c->context != NULL);

    LFS_TRACE("lfs_read(%p, 0x%"PRIx32", %"PRIu32", %p, %"PRIu32")",
            (void*)c, block, off, buffer, size);

    struct fal_partition *part = NULL;

    uint8_t *data = buffer;
    uint32_t addr = c->block_size * block + off;

    part = (struct fal_partition *)c->context;

    if(fal_partition_read( part, addr, data, size ) < 0)
    {
        return LFS_ERR_IO;
    }

    return LFS_ERR_OK;
}

/** 
* liffleFS call the sfud write port.
* none. 
* @param[in]   c:lfs_conifg struct , block:block address, off: address offset, buffer:write buffer, size:write size.
* @param[out]  noen.
* @retval  none.
* @par TAG 
*      reserved 
* @par other 
*      none 
* @par modify
*      ken 2019-01-07 create
*/
static int32_t lfs_write( const struct lfs_config *c, lfs_block_t block,
                 lfs_off_t off, const void *buffer, lfs_size_t size )
{
    // Check if write is valid
    LFS_ASSERT(off  % c->prog_size == 0);
    LFS_ASSERT(size % c->prog_size == 0);
    LFS_ASSERT(block < c->block_count);
    LFS_ASSERT(c != NULL);
    LFS_ASSERT(c->context != NULL);

    LFS_TRACE("lfs_prog(%p, 0x%"PRIx32", %"PRIu32", %p, %"PRIu32")",
            (void*)c, block, off, buffer, size);

    struct fal_partition *part = NULL;

    const uint8_t *data = buffer;
    uint32_t addr = c->block_size * block + off;

    part = (struct fal_partition *)c->context;

    if(fal_partition_write( part, addr, data, size ) < 0)
    {
        return LFS_ERR_IO;
    }

    return LFS_ERR_OK;
}

/** 
* liffleFS call the sfud erase port.
* none. 
* @param[in]   c:lfs_conifg struct , block:block address .
* @param[out]  noen.
* @retval  none.
* @par TAG 
*      reserved 
* @par other 
*      none 
* @par modify
*      ken 2019-01-07 create
*/
static int32_t lfs_erase( const struct lfs_config *c, lfs_block_t block )
{
    // Check if erase is valid
    LFS_ASSERT(block < c->block_count);
    LFS_ASSERT(c != NULL);
    LFS_ASSERT(c->context != NULL);

    LFS_TRACE("lfs_erase(%p, 0x%"PRIx32")", (void*)c, block);

    struct fal_partition *part = NULL;

    uint32_t addr = c->block_size * block;
    size_t size = c->block_size;

    part = (struct fal_partition *)c->context;

    if(fal_partition_erase( part, addr, size) < 0)
    {
        return LFS_ERR_IO;
    }

    return LFS_ERR_OK;
}

/** 
* liffleFS call the sfud sync port.
* none. 
* @param[in]   c:lfs_conifg struct.
* @param[out]  noen.
* @retval  none.
* @par TAG 
*      reserved 
* @par other 
*      none 
* @par modify
*      ken 2019-01-07 create
*/
static int32_t lfs_sync( const struct lfs_config *c )
{
    LFS_TRACE("lfs_sync(%p)", (void*)c);

    return LFS_ERR_OK;
}

void _lfs_load_config(struct lfs_config* lfs_cfg, const struct fal_partition *part)
{
    const struct fal_flash_dev *fal_flash = NULL;
    if ((fal_flash = fal_flash_device_find(part->flash_name)) == NULL)
    {
        printf("Error: the flash device name (%s) is not found.", part->flash_name);
        return;
    }
    lfs_cfg->context = (void*)part;

    lfs_cfg->read_size = LFS_READ_SIZE;
    lfs_cfg->prog_size = LFS_PROG_SIZE;

    lfs_cfg->block_size = fal_flash->blk_size;
    if (lfs_cfg->block_size < LFS_BLOCK_SIZE)
    {
        lfs_cfg->block_size = LFS_BLOCK_SIZE;
    }

    lfs_cfg->cache_size = LFS_CACHE_SIZE;
    lfs_cfg->block_cycles = LFS_BLOCK_CYCLES;

    lfs_cfg->block_count = part->len / fal_flash->blk_size;

    lfs_cfg->lookahead_size = 32 * ((lfs_cfg->block_count + 31) / 32);
    if (lfs_cfg->lookahead_size > LFS_LOOKAHEAD_MAX)
    {
        lfs_cfg->lookahead_size = LFS_LOOKAHEAD_MAX;
    }

    lfs_cfg->read = &lfs_read;
    lfs_cfg->prog = &lfs_write;
    lfs_cfg->erase = &lfs_erase;
    lfs_cfg->sync = &lfs_sync;
}

