/*
 * File      : fal_cfg.h
 * COPYRIGHT (C) 2012-2018, Shanghai Real-Thread Technology Co., Ltd
 *
 * Change Logs:
 * Date           Author       Notes
 * 2018-08-21     MurphyZhao   the first version
 */

#ifndef _FAL_CFG_H_
#define _FAL_CFG_H_

#define FAL_PART_HAS_TABLE_CFG

#define FAL_DEBUG                           1

#define FAL_PRINTF                          printf

#define ONCHIP_FLASH_DEV_NAME               "onchip_flash"

#define NOR_FLASH_DEV_NAME                  "norflash0"

/* ===================== Flash device Configuration ========================= */
extern const struct fal_flash_dev stm32_onchip_flash;
extern struct fal_flash_dev nor_flash0;

/* flash device table */
#define FAL_FLASH_DEV_TABLE                                          \
{                                                                    \
    &stm32_onchip_flash,                                             \
    &nor_flash0,                                                     \
}
/* ====================== Partition Configuration ========================== */
#ifdef FAL_PART_HAS_TABLE_CFG
/* partition table */
#define FAL_PART_TABLE                                                                   \
{                                                                                        \
    {FAL_PART_MAGIC_WORD,        "bl", ONCHIP_FLASH_DEV_NAME,          0,  256*1024, 0}, \
    {FAL_PART_MAGIC_WORD,       "app", ONCHIP_FLASH_DEV_NAME,   256*1024,  256*1024, 0}, \
    {FAL_PART_MAGIC_WORD, "easyflash",    NOR_FLASH_DEV_NAME,          0, 1024*1024, 0}, \
    {FAL_PART_MAGIC_WORD,  "download",    NOR_FLASH_DEV_NAME,  1024*1024, 1024*1024, 0}, \
}
#endif /* FAL_PART_HAS_TABLE_CFG */
#endif /* _FAL_CFG_H_ */
