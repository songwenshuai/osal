#include <easyflash.h>
#include <fal.h>
#include <printf.h>
#include <OSAL.h>

static void test_env(void);

void easyflash_test( void )
{
    if (fal_init() > 0 && easyflash_init() == EF_NO_ERR) {
        /* test Env demo */
        test_env();
    }
}

/**
 * Env demo.
 */
static void test_env(void) {
    uint32_t i_boot_times = NULL;
    char *c_old_boot_times, c_new_boot_times[11] = {0};

    /* get the boot count number from Env */
    c_old_boot_times = ef_get_env("boot_times");
    assert_param(c_old_boot_times);
    i_boot_times = osal_atol(c_old_boot_times);
    /* boot count +1 */
    i_boot_times ++;
    printf("The system now boot %d times\n\r", i_boot_times);
    /* interger to string */
    sprintf(c_new_boot_times,"%ld", i_boot_times);
    /* set and store the boot count number to Env */
    ef_set_env("boot_times", c_new_boot_times);
    ef_save_env();
}
