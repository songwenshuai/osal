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

#include <rtconfig.h>

#define ONCHIP_FLASH_DEV_NAME             "onchip_flash"

/* ===================== Flash device Configuration ========================= */
#define FAL_FLASH_PORT_DRIVER_STM32L4

extern const struct fal_flash_dev stm32_onchip_flash;

/* flash device table */
#define FAL_FLASH_DEV_TABLE                                          \
{                                                                    \
    &stm32_onchip_flash,                                             \
}
/* ====================== Partition Configuration ========================== */
#ifdef FAL_PART_HAS_TABLE_CFG
/* partition table */
#define FAL_PART_TABLE                                                                                                     \
{                                                                                                                          \
    {FAL_PART_MAGIC_WROD,        "app", ONCHIP_FLASH_DEV_NAME,                                    0,       384 * 1024, 0}, \
    {FAL_PART_MAGIC_WROD,      "param", ONCHIP_FLASH_DEV_NAME,                           384 * 1024,        64 * 1024, 0}, \
    {FAL_PART_MAGIC_WROD,   "download", ONCHIP_FLASH_DEV_NAME,                           448 * 1024,        64 * 1024, 0}, \
    {FAL_PART_MAGIC_WROD,  "easyflash", ONCHIP_FLASH_DEV_NAME,                           512 * 1024,       512 * 1024, 0}, \
}
#endif /* FAL_PART_HAS_TABLE_CFG */
#endif /* _FAL_CFG_H_ */
