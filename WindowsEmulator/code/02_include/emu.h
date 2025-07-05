/**
 ******************************************************************************
 * @file    emu.h
 * @author  Bellino Francesco
 * @brief   Emu header file.
 *
 *
 ******************************************************************************
 */

#ifndef EMU_H_
#define EMU_H_

#include <common.h>

typedef struct {
    bool paused;
    bool running;
    u64 ticks;
} emu_context;

int emu_run(int argc, char **argv);
void emu_cycles(int cpu_cycles);
emu_context *emu_get_context();

#endif
