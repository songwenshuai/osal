/*
 * lfs util functions
 *
 * Copyright (c) 2017, Arm Limited. All rights reserved.
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include "lfs_util.h"
#include "fal.h"
#include "lfs.h"

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

/* LittleFs partition name on FAL partition table */
#define FAL_FS_PART_NAME               "littlefs"

static const struct fal_partition *part = NULL;

/**
 * Flash port for hardware initialize.
 *
 * @param default_env default ENV set for user
 * @param default_env_size default ENV size
 *
 * @return result
 */
void littlefs_port_init( void ) {

    part = fal_partition_find(FAL_FS_PART_NAME);
    LFS_ASSERT(part);
}

/** 
* liffleFS call the sfud read port.
* none. 
* @param[in]   c:lfs_conifg struct , block:block address, off: address offset, buffer:read buffer, size:read size.
* @param[out]  noen.
* @retval  none.
* @par TAG 
*      reserved 
* @par other 
*      none 
* @par modify
*      ken 2019-01-07 create
*/
static int32_t midfs_lfs_read( const struct lfs_config *c, lfs_block_t block,  lfs_off_t off, void *buffer, lfs_size_t size )
{
    uint32_t addr = c->block_size * block + off;
    fal_partition_read( part, addr, buffer, size );
    return 0;
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
static int32_t midfs_lfs_write( const struct lfs_config *c, lfs_block_t block, lfs_off_t off, const void *buffer,
                             lfs_size_t size )
{
    uint32_t addr = c->block_size * block + off;
    fal_partition_write( part, addr, buffer, size );
    return 0;
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
static int32_t midfs_lfs_erase( const struct lfs_config *c, lfs_block_t block )
{
    uint32_t addr = c->block_size * block;
    fal_partition_erase( part, addr, c->block_size );
    return 0;
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
static int32_t midfs_lfs_sync( const struct lfs_config *c )
{
    return 0;
}

// configuration of the filesystem is provided by this struct
const struct lfs_config lfs_cfg =
{
    // block device operations
    .read  = midfs_lfs_read,
    .prog  = midfs_lfs_write,
    .erase = midfs_lfs_erase,
    .sync  = midfs_lfs_sync,

    // block device configuration
    .read_size = 256,
    .prog_size = 256,
    .block_size = 4096,
    .block_count = 512,
    .block_cycles = 500,
    .cache_size = 512,
    .lookahead_size = 128
};
