/**
 ******************************************************************************
 * @file    cpu_util.c
 * @author  Bellino Francesco
 * @brief   CPU utilities source code.
 *
 ******************************************************************************
 */

#include <cpu.h>
#include <cpu_util.h>

extern cpu_context cpu_ctx;

u16 reverse(u16 n)
{
    return ((n & 0xFF00) >> 8) | ((n & 0x00FF) << 8);
}

u16 cpu_read_reg(reg_type rt)
{
    switch(rt)
    {
        case RT_A: return cpu_ctx.regs.a;
        case RT_F: return cpu_ctx.regs.f;
        case RT_B: return cpu_ctx.regs.b;
        case RT_C: return cpu_ctx.regs.c;
        case RT_D: return cpu_ctx.regs.d;
        case RT_E: return cpu_ctx.regs.e;
        case RT_H: return cpu_ctx.regs.h;
        case RT_L: return cpu_ctx.regs.l;

        case RT_AF: return reverse(*((u16 *)&cpu_ctx.regs.a));
        case RT_BC: return reverse(*((u16 *)&cpu_ctx.regs.b));
        case RT_DE: return reverse(*((u16 *)&cpu_ctx.regs.d));
        case RT_HL: return reverse(*((u16 *)&cpu_ctx.regs.h));

        case RT_PC: return cpu_ctx.regs.pc;
        case RT_SP: return cpu_ctx.regs.sp;
        default: return 0;
    }
}

void cpu_set_flags(cpu_context *ctx, char z, char n, char h, char c)
{
    if (z != -1) {
        BIT_SET(ctx->regs.f, 7, z);
    }

    if (n != -1) {
        BIT_SET(ctx->regs.f, 6, n);
    }

    if (h != -1) {
        BIT_SET(ctx->regs.f, 5, h);
    }

    if (c != -1) {
        BIT_SET(ctx->regs.f, 4, c);
    }
}

bool check_cond(cpu_context *ctx)
{
    bool z = BIT(ctx->regs.f, BIT_Z);
    bool c = BIT(ctx->regs.f, BIT_C);

    switch(ctx->cur_inst->cond)
    {
        case CT_NONE: return true;
        case CT_C: return c;
        case CT_NC: return !c;
        case CT_Z: return z;
        case CT_NZ: return !z;
    }

    return false;
}
