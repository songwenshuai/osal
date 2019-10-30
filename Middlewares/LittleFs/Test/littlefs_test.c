#include "lfs.h"
#include <fal.h>
#include <sfud.h>
#include <cm_backtrace.h>


// variables used by the filesystem
lfs_t lfs;
lfs_file_t file;
#define HARDWARE_VERSION                "V1.0.0"
#define SOFTWARE_VERSION                "V0.1.0"

extern void littlefs_port_init( void );
extern const struct lfs_config lfs_cfg;

// entry point
void lfs_test(void) {
    cm_backtrace_init("STM32F103ZET6", HARDWARE_VERSION, SOFTWARE_VERSION);

    if (sfud_init() == SFUD_SUCCESS && fal_init() > 0) {
        /* test Env demo */
        printf("Fal init success!\r\n");
    }

    littlefs_port_init();
    
    // mount the filesystem
    int err = lfs_mount(&lfs, &lfs_cfg);

    // reformat if we can't mount the filesystem
    // this should only happen on the first boot
    if (err) {
        lfs_format(&lfs, &lfs_cfg);
        lfs_mount(&lfs, &lfs_cfg);
    }

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

    // release any resources we were using
    lfs_unmount(&lfs);

    // print the boot count
    printf("boot_count: %d\n", boot_count);
}