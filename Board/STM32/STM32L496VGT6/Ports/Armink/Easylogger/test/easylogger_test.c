#define LOG_TAG    "APP"
#include "easyflash.h"
#include "elog_flash.h"
#include <fal.h>
#include <printf.h>

static void test_elog(void);
static void elog_user_assert_hook(const char* ex, const char* func, size_t line);

void easylogger_test( void ) {
    /* initialize EasyFlash and EasyLogger */
    if ((fal_init() > 0)&&(easyflash_init() == EF_NO_ERR)&&(elog_init() == ELOG_NO_ERR)) {
        /* set EasyLogger log format */
        elog_set_fmt(ELOG_LVL_ASSERT, ELOG_FMT_ALL & ~ELOG_FMT_P_INFO);
        elog_set_fmt(ELOG_LVL_ERROR, ELOG_FMT_LVL | ELOG_FMT_TAG | ELOG_FMT_TIME);
        elog_set_fmt(ELOG_LVL_WARN, ELOG_FMT_LVL | ELOG_FMT_TAG | ELOG_FMT_TIME);
        elog_set_fmt(ELOG_LVL_INFO, ELOG_FMT_LVL | ELOG_FMT_TAG | ELOG_FMT_TIME);
        elog_set_fmt(ELOG_LVL_DEBUG, ELOG_FMT_ALL & ~(ELOG_FMT_FUNC | ELOG_FMT_P_INFO));
        elog_set_fmt(ELOG_LVL_VERBOSE, ELOG_FMT_ALL & ~(ELOG_FMT_FUNC | ELOG_FMT_P_INFO));
        /* set EasyLogger assert hook */
        elog_assert_set_hook(elog_user_assert_hook);
        /* initialize EasyLogger Flash plugin */
        elog_flash_init();
        /* start EasyLogger */
        elog_start();
        /* set EasyLogger assert hook */
        elog_assert_set_hook(elog_user_assert_hook);
        /* test logger output */
        test_elog();
    }
}


/**
 * Elog demo
 */
static void test_elog(void) {
    /* output all saved log from flash */
    elog_flash_output_all();
    /* test log output for all level */
    log_a("Hello EasyLogger!");
    log_e("Hello EasyLogger!");
    log_w("Hello EasyLogger!");
    log_i("Hello EasyLogger!");
    log_d("Hello EasyLogger!");
    log_v("Hello EasyLogger!");
    elog_raw("Hello EasyLogger!");
    /* trigger assert. Now will run elog_user_assert_hook. All log information will save to flash. */
//    ELOG_ASSERT(0);
}

static void elog_user_assert_hook(const char* ex, const char* func, size_t line) {
//    rt_enter_critical();
    /* disable logger output lock */
    elog_output_lock_enabled(false);
    /* disable flash plugin lock */
    elog_flash_lock_enabled(false);
    /* output assert information */
    elog_a("elog", "(%s) has assert failed at %s:%ld.\n", ex, func, line);
    /* write all buffered log to flash */
    elog_flash_flush();
    while(1);
}