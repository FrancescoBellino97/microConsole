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

#define BIT(a, n) ((a & (1 << n)) ? 1 : 0)
#define BIT_SET(a, n, on) (on ? ((a) |= (1 << n)) : ((a) &= ~(1 << n)))
#define BETWEEN(a, b, c) ((a >= b) && (a <= c))

#define NO_IMPL { fprintf(stderr, "NOT YET IMPLEMENTED\n"); exit(ERR_NOT_IMPLEMENTED); }


#endif
