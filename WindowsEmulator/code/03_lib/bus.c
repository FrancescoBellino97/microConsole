/**
 ******************************************************************************
 * @file    bus.c
 * @author  Bellino Francesco
 * @brief   Bus source code.
 *
 ******************************************************************************
 */
 
#include <bus.h>
#include <cart.h>
#include <ram.h>
#include <cpu_util.h>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                     CARTRIDGE MEMORY MAP                      *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * 0x0000 - 0x3FFF : ROM Bank 0									 *
 * 0x4000 - 0x7FFF : ROM Bank 1 - Switchable					 *
 * 0x8000 - 0x97FF : CHR RAM									 *
 * 0x9800 - 0x9BFF : BG Map 1									 *
 * 0x9C00 - 0x9FFF : BG Map 2									 *
 * 0xA000 - 0xBFFF : Cartridge RAM								 *
 * 0xC000 - 0xCFFF : RAM Bank 0									 *
 * 0xD000 - 0xDFFF : RAM Bank 1-7 - switchable - Color only		 *
 * 0xE000 - 0xFDFF : Reserved - Echo RAM						 *
 * 0xFE00 - 0xFE9F : Object Attribute Memory					 *
 * 0xFEA0 - 0xFEFF : Reserved - Unusable						 *
 * 0xFF00 - 0xFF7F : I/O Registers								 *
 * 0xFF80 - 0xFFFE : Zero Page									 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */


/**
  * @brief  Read 8b data from cart at a specific address
  * @param  address:	address to read
  * @retval uint8_t:	data read
  */
u8 bus_read(u16 address)
{
    if (address <= END_ROM_BANK1) {
    	/* ROM DATA */
        return cart_read(address);
    }
    else if (address <= END_VRAM)
    {
    	/* Char / Map Data */
    	//TODO
    	printf("UNSUPPORTED bus_read(%04X)\n", address);
    	NO_IMPL
    }
    else if (address <= END_EXT_RAM)
    {
    	/* EXTERNAL RAM */
    	return cart_read(address);
    }
    else if (address <= END_WRAM)
    {
    	/* WRAM */
    	return wram_read(address);
    }
    else if (address <= END_RESERVED1)
    {
    	/* RESERVED */
    	return 0;
    }
    else if (address <= END_OAM)
    {
    	/* OAM */
    	//TODO
    	printf("UNSUPPORTED bus_read(%04X)\n", address);
    	NO_IMPL
    }
    else if (address <= END_RESERVED2)
    {
    	/* RESERVED */
    	return 0;
    }
    else if (address <= END_IO_REG)
    {
    	/* IO Registers */
    	//TODO
    	printf("UNSUPPORTED bus_read(%04X)\n", address);
    	NO_IMPL
    }
    else if (address <= END_HRAM)
    {
    	 return hram_read(address);
    }

    /* CPU ENABLE REGISTER */
    //TODO
    return cpu_get_ie_register();
}

/**
  * @brief  Write 8b data to cart at a specific address
  * @param  address:	address to write
  * @retval None
  */
void bus_write(u16 address, u8 value)
{
    if (address <= END_ROM_BANK1)
    {
    	/* ROM DATA */
        cart_write(address, value);
    }
    else if (address <= END_VRAM)
    {
    	/* CHAR / MAP DATA */
        //TODO
        printf("UNSUPPORTED bus_write(%04X)\n", address);
        NO_IMPL
    }
    else if (address <= END_EXT_RAM)
    {
    	/* EXTERNAL RAM */
        cart_write(address, value);
    }
    else if (address <= END_WRAM)
    {
    	/* WRAM */
        wram_write(address, value);
    }
    else if (address <= END_RESERVED1)
    {
        /* RESERVED */
    }
    else if (address <= END_OAM)
    {
    	/* OAM */

        //TODO
        printf("UNSUPPORTED bus_write(%04X)\n", address);
        NO_IMPL
    }
    else if (address <= END_RESERVED2)
    {
    	/* RESERVED */
    }
    else if (address <= END_IO_REG)
    {
    	/* IO REGISTER */
        //TODO
        printf("UNSUPPORTED bus_write(%04X)\n", address);
        //NO_IMPL
    }
    else if (address <= END_IO_REG)
    {
    	/* HRAM */
    	hram_write(address, value);
    }
    else
    {
        //CPU SET ENABLE REGISTER

        cpu_set_ie_register(value);
    }
}

/**
  * @brief  Read 16b data from cart at a specific address
  * @param  address:	address to read
  * @retval uint8_t:	data read
  */
u16 bus_read16(u16 address) {
    u16 lo = bus_read(address);
    u16 hi = bus_read(address + 1);

    return lo | (hi << 8);
}

/**
  * @brief  Write 16b data to cart at a specific address
  * @param  address:	address to write
  * @retval None
  */
void bus_write16(u16 address, u16 value) {
    bus_write(address + 1, (value >> 8) & 0xFF);
    bus_write(address, value & 0xFF);
}
