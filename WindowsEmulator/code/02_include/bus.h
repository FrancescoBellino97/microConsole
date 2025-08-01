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

#include <common.h>

u8 bus_read(u16 address);
void bus_write(u16 address, u8 value);

#endif