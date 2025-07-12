/**
 ******************************************************************************
 * @file    cart.h
 * @author  Bellino Francesco
 * @brief   Cart header file.
 *
 *
 ******************************************************************************
 */

#ifndef CART_H_
#define CART_H_

#include <common.h>

#define	KB					1024
#define ROM_SIZE			32*KB

#define START_BANK0_ROM		0x0000
#define END_BANK0_ROM		0x3FFF
#define START_BANK1_ROM		0x4000
#define END_BANK1_ROM		0x7FFF



typedef struct {
    u8 entry[4];
    u8 logo[0x30];

    char title[16];
    u16 new_lic_code;
    u8 sgb_flag;
    u8 type;
    u8 rom_size;
    u8 ram_size;
    u8 dest_code;
    u8 lic_code;
    u8 version;
    u8 checksum;
    u16 global_checksum;
} rom_header;

bool cart_load(char *cart);

u8 cart_read(u16 address);
void cart_write(u16 address, u8 value);

#endif
