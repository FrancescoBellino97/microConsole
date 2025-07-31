#include <timer.h>
#include <interrupts.h>

static timer_context ctx = {0};

void timer_init() {
    ctx.div = 0xAC00;
}

void timer_tick() {
	u16 prev_div = ctx.div;

    ctx.div++;

    bool timer_update = false;

    switch(ctx.tac & (0b11)) {
        case 0b00:
            timer_update = (prev_div & (1 << 9)) && (!(ctx.div & (1 << 9)));
            break;
        case 0b01:
            timer_update = (prev_div & (1 << 3)) && (!(ctx.div & (1 << 3)));
            break;
        case 0b10:
            timer_update = (prev_div & (1 << 5)) && (!(ctx.div & (1 << 5)));
            break;
        case 0b11:
            timer_update = (prev_div & (1 << 7)) && (!(ctx.div & (1 << 7)));
            break;
    }

    if (timer_update && ctx.tac & (1 << 2)) {
        ctx.tima++;

        if (ctx.tima == 0xFF) {
            ctx.tima = ctx.tma;

//            cpu_request_interrupt(IT_TIMER);
        }
    }
}
