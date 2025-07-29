/**
 ******************************************************************************
 * @file    bus.h
 * @author  Bellino Francesco
 * @brief   Bus header file.
 *
 *
 ******************************************************************************
 */

#ifndef BUS_H_
#define BUS_H_

#include "../98_include/common.h"

u8 bus_read(u16 address);
void bus_write(u16 address, u8 value);
u16 bus_read16(u16 address);
void bus_write16(u16 address, u16 value);

#endif