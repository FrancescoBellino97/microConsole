/**
 ******************************************************************************
 * @file    ram.h
 * @author  Bellino Francesco
 * @brief   RAM header file.
 *
 *
 ******************************************************************************
 */

#ifndef RAM_H_
#define RAM_H_

#include <common.h>

u8 wram_read(u16 address);
void wram_write(u16 address, u8 value);

u8 hram_read(u16 address);
void hram_write(u16 address, u8 value);

#endif
