/**
 ******************************************************************************
 * @file    cpu_proc.c
 * @author  Bellino Francesco
 * @brief   CPU process source code.
 *
 ******************************************************************************
 */

#include <emu.h>
#include <cpu.h>
#include <cpu_util.h>
#include <cpu_proc.h>


/* Emulation of CPU Instructions */
static void proc_none(cpu_context *ctx);
static void proc_nop(cpu_context *ctx) {};
static void proc_di(cpu_context *ctx);
static void proc_ld(cpu_context *ctx);
static void proc_xor(cpu_context *ctx);
static void proc_jp(cpu_context *ctx);

/* Look-Up table to convert OpCode to Instruction Callback */
static IN_PROC processors[] =
{
    [IN_NONE] = proc_none,
    [IN_NOP] = proc_nop,
    [IN_LD] = proc_ld,
    [IN_JP] = proc_jp,
    [IN_DI] = proc_di,
    [IN_XOR] = proc_xor
};

/**
  * @brief  Get the emulated function from the type of instruction
  * @param  type:		type of the instruction (eg. LD, JP)
  * @retval IN_PROC:	emulated function pointer
  */
IN_PROC inst_get_processor(in_type type)
{
    return processors[type];
}


/* EMULATION OF CPU INSTRUCTIONS */

/**
  * @brief  Default function for not managed instructions
  * @param  ctx:		context of the CPU
  * @retval None
  */
static void proc_none(cpu_context *ctx)
{
#if DEBUG==true
    printf("INVALID INSTRUCTION!\n");
#endif
    exit(ERR_INVALID_INSTRUCTION);
}

/**
  * @brief  Emulation of DI (disable interrupts)
  * @param  ctx:		context of the CPU
  * @retval None
  */
static void proc_di(cpu_context *ctx)
{
    ctx->int_master_enabled = false;
}

/**
  * @brief  Emulation of LD (load a value/register)
  * @param  ctx:		context of the CPU
  * @retval None
  */
static void proc_ld(cpu_context *ctx)
{
    //TODO...
}

/**
  * @brief  Emulation of XOR
  * @param  ctx:		context of the CPU
  * @retval None
  */
static void proc_xor(cpu_context *ctx)
{
    ctx->regs.a ^= ctx->fetched_data & 0xFF;
    cpu_set_flags(ctx, ctx->regs.a == 0, 0, 0, 0);
}

/**
  * @brief  Emulation of JP (jump address)
  * @param  ctx:		context of the CPU
  * @retval None
  */
static void proc_jp(cpu_context *ctx)
{
    if (check_cond(ctx)) {
        ctx->regs.pc = ctx->fetched_data;
        emu_cycles(1);
    }
}
