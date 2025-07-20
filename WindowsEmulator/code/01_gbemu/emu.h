/**
 ******************************************************************************
 * @file    emu.h
 * @author  Bellino Francesco
 * @brief   Emulation header file.
 *
 *
 ******************************************************************************
 */

#ifndef EMU_H
#define EMU_H


/** INCLUDES **/
#include "common.h"

/** PUBLIC FUNCTIONS **/
uint8_t emu_init();
uint8_t emu_run(int argc, char **argv);


#endif
