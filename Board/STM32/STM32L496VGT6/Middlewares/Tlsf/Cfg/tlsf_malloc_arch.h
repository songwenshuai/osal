/*
 * Copyright (C) 2016 Alexandre Bustico, Gautier Hattenberger
 *
 * This file is part of paparazzi.
 *
 * paparazzi is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 * paparazzi is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with paparazzi; see the file COPYING.  If not, see
 * <http://www.gnu.org/licenses/>.
 */

/**
 * @file arch/chibios/modules/tlsf/tlsf_malloc_arch.h
 *
 * Dynamic memory allocation based on TLSF library.
 *
 */

#ifndef TLSF_MALLOC_ARCH_H
#define TLSF_MALLOC_ARCH_H


#define HEAP_SRAM          ccmHeap

#define HEAP_SRAM_SIZE     (300 * 1024)

#define HEAP_SRAM_SECTION  FAST_SECTION

#define HEAP_DEFAULT      HEAP_SRAM

#endif

