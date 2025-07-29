/**
 ******************************************************************************
 * @file    common.h
 * @author  Bellino Francesco
 * @brief   Common header file.
 *
 *
 ******************************************************************************
 */

#ifndef COMMON_H_
#define COMMON_H_

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;


#define DEBUG			true

/* ERROR RETURN CODES */
#define NO_ERROR					0
#define ERR_MISSING_ROM				-1
#define ERR_LOADING_ROM				-2
#define ERR_CPU_STOPPED				-3
#define ERR_UNKNOWM_ADDRESS_MODE	-4
#define ERR_NOT_IMPLEMENTED			-5
#define ERR_UNKNOWN_INSTRUCTION		-6
#define ERR_INVALID_INSTRUCTION		-7
#define ERR_INVALID_WRAM_ACCESS		-8
#define ERR_INVALID_HRAM_ACCESS		-9


/* MEMORY MAPPING */
#define START_ROM_BANK0		0x0000
#define END_ROM_BANK0		0x3FFF
#define START_ROM_BANK1		0x4000
#define END_ROM_BANK1		0x7FFF
#define START_VRAM			0x8000
#define END_VRAM			0x9FFF
#define START_EXT_RAM		0xA000
#define END_EXT_RAM			0xBFFF
#define START_WRAM			0xC000
#define END_WRAM			0xDFFF
#define START_RESERVED1		0xE000
#define END_RESERVED1		0xFDFF
#define START_OAM			0xFE00
#define	END_OAM				0xFE9F
#define START_RESERVED2		0xFEA0
#define END_RESERVED2		0xFEFF
#define START_IO_REG		0xFF00
#define END_IO_REG			0xFF7F
#define START_HRAM			0xFF80
#define END_HRAM			0xFFFE
#define IE_REG				0xFFFF

#define ROM_BANK0_SIZE		END_ROM_BANK0 - START_ROM_BANK0 + 1
#define ROM_BANK1_SIZE		END_ROM_BANK1 - START_ROM_BANK1 + 1
#define VRAM_SIZE			END_VRAM - START_VRAM + 1
#define EXT_RAM_SIZE		END_EXT_RAM - START_EXT_RAM + 1
#define WRAM_SIZE			END_WRAM - START_WRAM + 1
#define RESERVED1_SIZE		END_RESERVED1 - START_RESERVED1 + 1
#define OAM_SIZE			END_OAM - START_OAM +1
#define RESERVED2_SIZE		END_RESERVED2 - START_RESERVED2 + 1
#define IO_REG_SIZE			END_IO_REG - START_IO_REG +1
#define HRAM_SIZE			END_HRAM - START_HRAM + 1


#define BIT(a, n) ((a & (1 << n)) ? 1 : 0)
#define BIT_SET(a, n, on) (on ? ((a) |= (1 << n)) : ((a) &= ~(1 << n)))
#define BETWEEN(a, b, c) ((a >= b) && (a <= c))

#define NO_IMPL { fprintf(stderr, "NOT YET IMPLEMENTED\n"); exit(ERR_NOT_IMPLEMENTED); }


#endif
