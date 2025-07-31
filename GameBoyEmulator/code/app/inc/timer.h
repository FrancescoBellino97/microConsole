#pragma once

#include <common.h>

typedef struct {
    u16 div;
    u8 tima;
    u8 tma;
    u8 tac;
} timer_context;

void timer_init();
void timer_tick();
