/**
 * @file lv_conf_starterkit.h
 *
 */ /*Remove this to enable the content (Delete the last #endif too!)*/

#ifndef  LV_CONF_H
#define  LV_CONF_H

/*----------------
 * Dynamic memory
 *----------------*/
/*
 * Memory size which will be used by the library
 * to store the graphical objects and other data
 */
#define LV_MEM_CUSTOM      0                /*1: use custom malloc/free, 0: use the built-in lv_mem_alloc/lv_mem_free*/
#if LV_MEM_CUSTOM == 0
#define LV_MEM_SIZE    (10U * 1024U)        /*Size memory used by `lv_mem_alloc` in bytes (>= 2kB)*/
#define LV_MEM_ATTR                         /*Complier prefix for big array declaration*/
#define LV_MEM_AUTO_DEFRAG  1               /*Automatically defrag on free*/
#else       /*LV_MEM_CUSTOM*/
#define LV_MEM_CUSTOM_INCLUDE <stdlib.h>   /*Header for the dynamic memory function*/
#define LV_MEM_CUSTOM_ALLOC   malloc       /*Wrapper to malloc*/
#define LV_MEM_CUSTOM_FREE    free         /*Wrapper to free*/
#endif     /*LV_MEM_CUSTOM*/

#endif /*Remove this to enable the content*/
