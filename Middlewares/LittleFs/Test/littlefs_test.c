#include "lfs.h"
#include <fal.h>
#include <sfud.h>
#include <cm_backtrace.h>

/* LittleFs partition name on FAL partition table */
#define FAL_FS_PART_NAME               "littlefs"

#define HARDWARE_VERSION                "V1.0.0"
#define SOFTWARE_VERSION                "V0.1.0"

// variables used by the filesystem
lfs_t lfs;
lfs_file_t file;

struct lfs_config cfg;

extern void _lfs_load_config(struct lfs_config* lfs_cfg, const struct fal_partition *part);

/**
* search the dirctory tree.
* none.
* @param[in]   lfs: lfs struct, path;path.
* @param[out]  noen.
* @retval  none.
* @par TAG
*      reserved
* @par other
*      none
* @par modify
*      ken 2019-01-07 create
*/
int lfs_ls( lfs_t *lfs, const char *path )
{
    lfs_dir_t dir;
    int err = lfs_dir_open( lfs, &dir, path );
    if( err )
    {
        return err;
    }
    struct lfs_info info;
    while( true )
    {
        int res = lfs_dir_read( lfs, &dir, &info );
        if( res < 0 )
        {
            return res;
        }
        if( res == 0 )
        {
            break;
        }
        printf( "\t%s", info.name );
        switch( info.type )
        {
            case LFS_TYPE_REG:
                printf( "\t\t%u Byte \r\n", info.size );
                break;
            case LFS_TYPE_DIR:
                printf( "\t\t\tdir\r\n" );
                break;
            default:
                printf( "?\r\n" );
                break;
        }
    }
    err = lfs_dir_close( lfs, &dir );
    if( err )
    {
        return err;
    }
    return 0;
}

/**
* print all the file infomation of root directory.
* none.
* @param[in]   void.
* @param[out]  noen.
* @retval  none.
* @par TAG
*      reserved
* @par other
*      none
* @par modify
*      ken 2019-01-07 create
*/
void ls( void )
{
    printf( "\r\nfiles on [\\]\r\n" );
    lfs_ls( &lfs,  "/" );
    printf( "\r\n\r\n" );
}

// entry point
void lfs_test(void) {
    const struct fal_partition *fal_part;

    cm_backtrace_init("STM32F103ZET6", HARDWARE_VERSION, SOFTWARE_VERSION);

    if (sfud_init() == SFUD_SUCCESS && fal_init() > 0) {
        /* test Env demo */
        printf("Fal init success!\r\n");
    }

    fal_part = fal_partition_find(FAL_FS_PART_NAME);

    _lfs_load_config(&cfg, fal_part);

    // mount the filesystem
    int err = lfs_mount(&lfs, &cfg);

    // reformat if we can't mount the filesystem
    // this should only happen on the first boot
    if (err) {
        lfs_format(&lfs, &cfg);
        lfs_mount(&lfs, &cfg);
    }
    ls();
    // read current count
    uint32_t boot_count = 0;
    lfs_file_open(&lfs, &file, "boot_count", LFS_O_RDWR | LFS_O_CREAT);
    lfs_file_read(&lfs, &file, &boot_count, sizeof(boot_count));

    // update boot count
    boot_count += 1;
    lfs_file_rewind(&lfs, &file);
    lfs_file_write(&lfs, &file, &boot_count, sizeof(boot_count));

    // remember the storage is not updated until the file is closed successfully
    lfs_file_close(&lfs, &file);
    ls();
    // release any resources we were using
    lfs_unmount(&lfs);

    // print the boot count
    printf("boot_count: %d\n", boot_count);
}