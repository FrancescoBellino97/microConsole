/**
 ******************************************************************************
 * @file    ram.c
 * @author  Bellino Francesco
 * @brief   RAM source code.
 *
 ******************************************************************************
 */

#include <ram.h>
#include <common.h>

 typedef struct {
    u8 wram[WRAM_SIZE];		/* Working RAM */
    u8 hram[HRAM_SIZE];		/* High RAM */
} ram_context;

static ram_context ram_ctx;


/**
  * @brief  Load the Cartridge in memory and print some informations
  * @param  cart:	Cartridge filename with path
  * @retval bool:	true if no error, false otherwise
  */
u8 wram_read(u16 address)
{
    address -= START_WRAM;

    if (address >= WRAM_SIZE)
    {
        printf("INVALID WRAM ADDR %08X\n", address + START_WRAM);
        exit(ERR_INVALID_WRAM_ACCESS);
    }

    return ram_ctx.wram[address];
}

void wram_write(u16 address, u8 value)
{
    address -= START_WRAM;
    if (address >= WRAM_SIZE)
    {
    	printf("INVALID WRAM ADDR %08X\n", address + START_WRAM);
    	exit(ERR_INVALID_WRAM_ACCESS);
    }

    ram_ctx.wram[address] = value;
    return;
}

u8 hram_read(u16 address)
{
    address -= START_HRAM;

    if (address >= HRAM_SIZE)
    {
    	printf("INVALID HRAM ADDR %08X\n", address + START_HRAM);
    	exit(ERR_INVALID_HRAM_ACCESS);
    }

    return ram_ctx.hram[address];
}

void hram_write(u16 address, u8 value)
{
    address -= START_HRAM;

    if (address >= HRAM_SIZE)
    {
    	printf("INVALID HRAM ADDR %08X\n", address + START_HRAM);
    	exit(ERR_INVALID_HRAM_ACCESS);
    }

    ram_ctx.hram[address] = value;
    return;
}
