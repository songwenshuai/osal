/**
 * \file            lwmem_init.c
 * \brief           lwmem init file
 */

/*
 * Copyright (c) 2019 Tilen MAJERLE
 *  
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without restriction,
 * including without limitation the rights to use, copy, modify, merge,
 * publish, distribute, sublicense, and/or sell copies of the Software, 
 * and to permit persons to whom the Software is furnished to do so, 
 * subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE
 * AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING 
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * Author:          Tilen MAJERLE <tilen@majerle.eu>
 * Version:         $_version_$
 */

#include <stdint.h>
#include "lwmem.h"
#include "printf.h"

/**
 * \brief           Define region data
 */
uint8_t region1_data[300 * 1024];

/**
 * \brief           Define final regions
 */
static lwmem_region_t
regions[] = {
    { region1_data, sizeof(region1_data) },
    /* Add more regions if needed */
};

/**
 * \brief           Program entry point
 */
void
lwmem_init(void) {

    /* Initialize LwMEM */
    printf("Initializing LwMEM...\r\n");
    if (!lwmem_assignmem(regions, sizeof(regions) / sizeof(regions[0]))) {
        printf("Cannot initialize LwMEM. Make sure your regions are not overlapping each other and are in ascending memory order\r\n");
        while (1) {}
    } else {
        printf("LwMEM initialized and ready to use\r\n");
    }
}

