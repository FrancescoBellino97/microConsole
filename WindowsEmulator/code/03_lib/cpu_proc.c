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
#include <bus.h>
#include <stack.h>


/* Emulation of CPU Instructions */
static void proc_none(cpu_context *ctx);
static void proc_nop(cpu_context *ctx) {};
static void proc_di(cpu_context *ctx);
static void proc_ld(cpu_context *ctx);
static void proc_ldh(cpu_context *ctx);
static void proc_xor(cpu_context *ctx);
static void proc_jp(cpu_context *ctx);
static void proc_jr(cpu_context *ctx);
static void proc_call(cpu_context *ctx);
static void proc_rst(cpu_context *ctx);
static void proc_ret(cpu_context *ctx);
static void proc_reti(cpu_context *ctx);
static void proc_pop(cpu_context *ctx);
static void proc_push(cpu_context *ctx);
static void goto_addr(cpu_context *ctx, u16 addr, bool pushpc);

/* Look-Up table to convert OpCode to Instruction Callback */
static IN_PROC processors[] =
{
    [IN_NONE] = proc_none,
    [IN_NOP] = proc_nop,
    [IN_LD] = proc_ld,
    [IN_LDH] = proc_ldh,
    [IN_JP] = proc_jp,
    [IN_DI] = proc_di,
    [IN_POP] = proc_pop,
    [IN_PUSH] = proc_push,
    [IN_JR] = proc_jr,
    [IN_CALL] = proc_call,
    [IN_RET] = proc_ret,
    [IN_RST] = proc_rst,
    [IN_RETI] = proc_reti,
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
static void proc_ld(cpu_context *ctx) {
    if (ctx->dest_is_mem)
    {
        //LD (BC), A for instance...

        if (ctx->cur_inst->reg_2 >= RT_AF) 
        {
            //if 16 bit register...
            emu_cycles(1);
            bus_write16(ctx->mem_dest, ctx->fetched_data);
        } 
        else 
        {
            bus_write(ctx->mem_dest, ctx->fetched_data);
        }

        emu_cycles(1);
        
        return;
    }

    if (ctx->cur_inst->mode == AM_HL_SPR) 
    {
        u8 hflag = (cpu_read_reg(ctx->cur_inst->reg_2) & 0xF) + 
            (ctx->fetched_data & 0xF) >= 0x10;

        u8 cflag = (cpu_read_reg(ctx->cur_inst->reg_2) & 0xFF) + 
            (ctx->fetched_data & 0xFF) >= 0x100;

        cpu_set_flags(ctx, 0, 0, hflag, cflag);
        cpu_set_reg(ctx->cur_inst->reg_1, 
            cpu_read_reg(ctx->cur_inst->reg_2) + (char)ctx->fetched_data);

        return;
    }

    cpu_set_reg(ctx->cur_inst->reg_1, ctx->fetched_data);
}

/**
  * @brief  Emulation of LDH (LOAD to/from address)
  * @param  ctx:		context of the CPU
  * @retval None
  */
static void proc_ldh(cpu_context *ctx)
{
    if (ctx->cur_inst->reg_1 == RT_A)
    {
        cpu_set_reg(ctx->cur_inst->reg_1, bus_read(0xFF00 | ctx->fetched_data));
    }
    else
    {
        bus_write(0xFF00 | ctx->fetched_data, ctx->regs.a);
    }

    emu_cycles(1);
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
	goto_addr(ctx, ctx->fetched_data, false);
}

/**
  * @brief  Emulation of JR (jump relative address)
  * @param  ctx:		context of the CPU
  * @retval None
  */
static void proc_jr(cpu_context *ctx)
{
    i16 rel = (char)(ctx->fetched_data & 0xFF);
    u16 addr = ctx->regs.pc + rel;
    goto_addr(ctx, addr, false);
}

/**
  * @brief  Emulation of CALL (jump address if condition is true and save current PC)
  * @param  ctx:		context of the CPU
  * @retval None
  */
static void proc_call(cpu_context *ctx)
{
    goto_addr(ctx, ctx->fetched_data, true);
}

/**
  * @brief  Emulation of RST (jump address and save current PC)
  * @param  ctx:		context of the CPU
  * @retval None
  */
static void proc_rst(cpu_context *ctx)
{
    goto_addr(ctx, ctx->cur_inst->param, true);
}

/**
  * @brief  Emulation of RET (return to last address saved)
  * @param  ctx:		context of the CPU
  * @retval None
  */
static void proc_ret(cpu_context *ctx)
{
    if (ctx->cur_inst->cond != CT_NONE)
    {
        emu_cycles(1);
    }

    if (check_cond(ctx))
    {
        u16 lo = stack_pop();
        emu_cycles(1);
        u16 hi = stack_pop();
        emu_cycles(1);

        u16 n = (hi << 8) | lo;
        ctx->regs.pc = n;

        emu_cycles(1);
    }
}

/**
  * @brief  Emulation of RETI (return from interrupt to last address saved)
  * @param  ctx:		context of the CPU
  * @retval None
  */
static void proc_reti(cpu_context *ctx)
{
    ctx->int_master_enabled = true;
    proc_ret(ctx);
}

/**
  * @brief  Emulation of POP (get value from STACK)
  * @param  ctx:		context of the CPU
  * @retval None
  */
static void proc_pop(cpu_context *ctx)
{
    u16 lo = stack_pop();
    emu_cycles(1);
    u16 hi = stack_pop();
    emu_cycles(1);

    u16 n = (hi << 8) | lo;

    cpu_set_reg(ctx->cur_inst->reg_1, n);

    if (ctx->cur_inst->reg_1 == RT_AF)
    {
        cpu_set_reg(ctx->cur_inst->reg_1, n & 0xFFF0);
    }
}

/**
  * @brief  Emulation of PUSH (put value to STACK)
  * @param  ctx:		context of the CPU
  * @retval None
  */
static void proc_push(cpu_context *ctx)
{
    u16 hi = (cpu_read_reg(ctx->cur_inst->reg_1) >> 8) & 0xFF;
    emu_cycles(1);
    stack_push(hi);

    u16 lo = cpu_read_reg(ctx->cur_inst->reg_1) & 0xFF;
    emu_cycles(1);
    stack_push(lo);

    emu_cycles(1);
}

/**
  * @brief  Generic jump to address
  * @param  ctx:		context of the CPU
  * 		addr:		address to jump
  * 		pushpc:		flag to save in stack current PC
  * @retval None
  */
static void goto_addr(cpu_context *ctx, u16 addr, bool pushpc)
{
    if (check_cond(ctx))
    {
        if (pushpc)
        {
            emu_cycles(2);
            stack_push16(ctx->regs.pc);
        }

        ctx->regs.pc = addr;
        emu_cycles(1);
    }
}
