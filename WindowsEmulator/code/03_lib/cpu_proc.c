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
static void proc_cb(cpu_context *ctx);
static void proc_and(cpu_context *ctx);
static void proc_xor(cpu_context *ctx);
static void proc_or(cpu_context *ctx);
static void proc_cp(cpu_context *ctx);
static void proc_nop(cpu_context *ctx) {};
static void proc_di(cpu_context *ctx);
static void proc_ld(cpu_context *ctx);
static void proc_ldh(cpu_context *ctx);
static void proc_jp(cpu_context *ctx);
static void proc_jr(cpu_context *ctx);
static void proc_call(cpu_context *ctx);
static void proc_rst(cpu_context *ctx);
static void proc_ret(cpu_context *ctx);
static void proc_reti(cpu_context *ctx);
static void proc_pop(cpu_context *ctx);
static void proc_push(cpu_context *ctx);
static void proc_inc(cpu_context *ctx);
static void proc_dec(cpu_context *ctx);
static void proc_sub(cpu_context *ctx);
static void proc_sbc(cpu_context *ctx);
static void proc_adc(cpu_context *ctx);
static void proc_add(cpu_context *ctx);
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
    [IN_DEC] = proc_dec,
    [IN_INC] = proc_inc,
    [IN_ADD] = proc_add,
	[IN_ADC] = proc_adc,
	[IN_SUB] = proc_sub,
	[IN_SBC] = proc_sbc,
	[IN_AND] = proc_and,
    [IN_XOR] = proc_xor,
    [IN_OR] = proc_or,
    [IN_CP] = proc_cp,
    [IN_CB] = proc_cb,
    [IN_RETI] = proc_reti,
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
  * @brief  Emulation of CB operators
  * @param  ctx:		context of the CPU
  * @retval None
  */
static void proc_cb(cpu_context *ctx)
{
    u8 op = ctx->fetched_data;
    reg_type reg = decode_reg(op & 0b111);
    u8 bit = (op >> 3) & 0b111;
    u8 bit_op = (op >> 6) & 0b11;
    u8 reg_val = cpu_read_reg8(reg);

    emu_cycles(1);

    if (reg == RT_HL)
    {
        emu_cycles(2);
    }

    /* Switch between BIT, RST, SET (Bitwise operators are manage below) */
    switch(bit_op)
    {
        case 1:		/* BIT (set Z flag if bit selected is 0) */
            cpu_set_flags(ctx, !(reg_val & (1 << bit)), 0, 1, -1);
            return;

        case 2:		/* RST (reset bit selected) */
            reg_val &= ~(1 << bit);
            cpu_set_reg8(reg, reg_val);
            return;

        case 3:		/* SET (reset bit selected) */
            reg_val |= (1 << bit);
            cpu_set_reg8(reg, reg_val);
            return;
    }

    bool flagC = BIT(ctx->regs.f, BIT_C);
    u8 old = reg_val;
    
    /* Switch for 0b00XXXXXX CB OpCode */
    switch(bit)
    {
        case 0:		/* RLC (Rotate Left with Carry)*/
        	reg_val <<= 1;
        	reg_val |= (old >> 7);

        	/* Update register */
        	cpu_set_reg8(reg, reg_val);
        	/* Update flags */
        	cpu_set_flags(ctx, reg_val == 0, false, false, BIT(old, 7));
            return;

        case 1:		/* RRC (Rotate Right with Carry)*/
            reg_val >>= 1;
            reg_val |= (old << 7);

            /* Update register */
            cpu_set_reg8(reg, reg_val);
            /* Update flags */
            cpu_set_flags(ctx, !reg_val, false, false, BIT(old, 0));
            return;

        case 2: 	/* RL (Rotate Left) */
            reg_val <<= 1;
            reg_val |= flagC;

            /* Update register */
            cpu_set_reg8(reg, reg_val);
            /* Update flags */
            cpu_set_flags(ctx, !reg_val, false, false, BIT(old, 7));
            return;

        case 3:		/* RR (Rotate Right) */
            reg_val >>= 1;
            reg_val |= (flagC << 7);

            /* Update register */
            cpu_set_reg8(reg, reg_val);
            /* Update flags */
            cpu_set_flags(ctx, !reg_val, false, false, BIT(old, 0));
            return;

        case 4:		/* SLA (Shift Left Arithmetic)*/
            reg_val <<= 1;

            /* Update register */
            cpu_set_reg8(reg, reg_val);
            /* Update flags */
            cpu_set_flags(ctx, !reg_val, false, false, BIT(old, 7));
            return;

        case 5:		/* SRA (Shift Right Arithmetic) */
        	reg_val = (int8_t)old >> 1;
            
        	/* Update register */
            cpu_set_reg8(reg, reg_val);
            /* Update flags */
            cpu_set_flags(ctx, !reg_val, 0, 0, BIT(old, 0));
            return;

        case 6:		/* SWAP (Swap Nibbles) */
            reg_val = ((reg_val & 0xF0) >> 4) | ((reg_val & 0xF) << 4);
            
            /* Update register */
            cpu_set_reg8(reg, reg_val);
            /* Update flags */
            cpu_set_flags(ctx, reg_val == 0, false, false, false);
            return;

        case 7:		/* SRL (Shift Right Logic) */
        	reg_val = old >> 1;
        	
        	/* Update register */
            cpu_set_reg8(reg, reg_val);
            /* Update flags */
            cpu_set_flags(ctx, !reg_val, 0, 0, BIT(old, 0));
            return;
    }

    fprintf(stderr, "ERROR: INVALID CB: %02X", op);
    NO_IMPL
}

/**
  * @brief  Emulation of AND (and operator between register A and a value/register/memory address)
  * @param  ctx:		context of the CPU
  * @retval None
  */
static void proc_and(cpu_context *ctx)
{
    ctx->regs.a &= ctx->fetched_data;
    cpu_set_flags(ctx, ctx->regs.a == 0, 0, 1, 0);
}

/**
  * @brief  Emulation of XOR (xor operator between register A and a value/register/memory address)
  * @param  ctx:		context of the CPU
  * @retval None
  */
static void proc_xor(cpu_context *ctx)
{
    ctx->regs.a ^= ctx->fetched_data & 0xFF;
    cpu_set_flags(ctx, ctx->regs.a == 0, 0, 0, 0);
}

/**
  * @brief  Emulation of OR (or operator between register A and a value/register/memory address)
  * @param  ctx:		context of the CPU
  * @retval None
  */
static void proc_or(cpu_context *ctx)
{
    ctx->regs.a |= ctx->fetched_data & 0xFF;
    cpu_set_flags(ctx, ctx->regs.a == 0, 0, 0, 0);
}

/**
  * @brief  Emulation of CP (compare between register A and a value/register/memory address)
  * @param  ctx:		context of the CPU
  * @retval None
  */
static void proc_cp(cpu_context *ctx)
{
    int n = (int)ctx->regs.a - (int)ctx->fetched_data;

    cpu_set_flags(ctx, n == 0,
    		1,
			((int)ctx->regs.a & 0x0F) - ((int)ctx->fetched_data & 0x0F) < 0,
			n < 0
			);
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

        if (is_16_bit(ctx->cur_inst->reg_2))
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
  * @brief  Emulation of INC (increment a register/memory address)
  * @param  ctx:		context of the CPU
  * @retval None
  */
static void proc_inc(cpu_context *ctx)
{
	/* General increment */
    u16 val = cpu_read_reg(ctx->cur_inst->reg_1) + 1;

    /* For cycle accuracy wait a cycle if the register is 16 bit */
    if (is_16_bit(ctx->cur_inst->reg_1))
    {
        emu_cycles(1);
    }

    /* Memory address increment */
    if (ctx->cur_inst->reg_1 == RT_HL && ctx->cur_inst->mode == AM_MR)
    {
        val = bus_read(cpu_read_reg(RT_HL)) + 1;
        val &= 0xFF;
        bus_write(cpu_read_reg(RT_HL), val);
    }
    else /* Write the general increment to register */
    {
        cpu_set_reg(ctx->cur_inst->reg_1, val);
        val = cpu_read_reg(ctx->cur_inst->reg_1);
    }

    /* For 0xX3 OpCodes the flags are not updated */
    if ((ctx->cur_opcode & 0x03) == 0x03)
    {
        return;
    }

    /* Update flags (not C) */
    cpu_set_flags(ctx, val == 0, 0, (val & 0x0F) == 0, -1);
}

/**
  * @brief  Emulation of DEC (decrement a register/memory address)
  * @param  ctx:		context of the CPU
  * @retval None
  */
static void proc_dec(cpu_context *ctx)
{
	/* General decrement */
    u16 val = cpu_read_reg(ctx->cur_inst->reg_1) - 1;

    /* For cycle accuracy wait a cycle if the register is 16 bit */
    if (is_16_bit(ctx->cur_inst->reg_1))
    {
        emu_cycles(1);
    }

    /* Memory address decrement */
    if (ctx->cur_inst->reg_1 == RT_HL && ctx->cur_inst->mode == AM_MR)
    {
        val = bus_read(cpu_read_reg(RT_HL)) - 1;
        val &= 0xFF;
        bus_write(cpu_read_reg(RT_HL), val);
    }
    else /* Write the general increment to register */
    {
        cpu_set_reg(ctx->cur_inst->reg_1, val);
        val = cpu_read_reg(ctx->cur_inst->reg_1);
    }

    /* For 0xX3 OpCodes the flags are not updated */
    if ((ctx->cur_opcode & 0x0B) == 0x0B)
    {
        return;
    }

    /* Update flags (not C) */
    cpu_set_flags(ctx, val == 0, 1, (val & 0x0F) == 0x0F, -1);
}

/**
  * @brief  Emulation of SUB (subtract a register with a value/register/memory address)
  * @param  ctx:		context of the CPU
  * @retval None
  */
static void proc_sub(cpu_context *ctx)
{
    u16 val = cpu_read_reg(ctx->cur_inst->reg_1) - ctx->fetched_data;

    int z = val == 0;
    int h = ((int)cpu_read_reg(ctx->cur_inst->reg_1) & 0xF) - ((int)ctx->fetched_data & 0xF) < 0;
    int c = ((int)cpu_read_reg(ctx->cur_inst->reg_1)) - ((int)ctx->fetched_data) < 0;

    cpu_set_reg(ctx->cur_inst->reg_1, val);
    cpu_set_flags(ctx, z, 1, h, c);
}

/**
  * @brief  Emulation of SBC (subtract a register with a value/register/memory address and add carry)
  * @param  ctx:		context of the CPU
  * @retval None
  */
static void proc_sbc(cpu_context *ctx)
{
	u8 carry = BIT(ctx->regs.f, BIT_C);
    u8 val = ctx->fetched_data + carry;

    int z = cpu_read_reg(ctx->cur_inst->reg_1) - val == 0;

    int h = ((int)cpu_read_reg(ctx->cur_inst->reg_1) & 0xF)
        - ((int)ctx->fetched_data & 0xF) - ((int)carry) < 0;
    int c = ((int)cpu_read_reg(ctx->cur_inst->reg_1))
        - ((int)ctx->fetched_data) - ((int)carry) < 0;

    cpu_set_reg(ctx->cur_inst->reg_1, cpu_read_reg(ctx->cur_inst->reg_1) - val);
    cpu_set_flags(ctx, z, 1, h, c);
}

/**
  * @brief  Emulation of ADC (sum a register with a value/register/memory address and carry)
  * @param  ctx:		context of the CPU
  * @retval None
  */
static void proc_adc(cpu_context *ctx)
{
    u16 u = ctx->fetched_data;
    u16 a = ctx->regs.a;
    u16 c = BIT(ctx->regs.f, BIT_C);

    /* ADC works only with reigster A */
    ctx->regs.a = (a + u + c) & 0xFF;

    cpu_set_flags(ctx, ctx->regs.a == 0, 0,
        (a & 0xF) + (u & 0xF) + c > 0xF,
        a + u + c > 0xFF);
}

/**
  * @brief  Emulation of ADD (sum a register with a value/register/memory address)
  * @param  ctx:		context of the CPU
  * @retval None
  */
static void proc_add(cpu_context *ctx)
{
	/* Generic add (32 bit to check overflow) */
    u32 val = cpu_read_reg(ctx->cur_inst->reg_1) + ctx->fetched_data;

    bool is_16bit = is_16_bit(ctx->cur_inst->reg_1);

    /* For cycle accuracy wait a cycle if the register is 16 bit */
    if (is_16bit)
    {
        emu_cycles(1);
    }

    /* For SP use signed sum */
    if (ctx->cur_inst->reg_1 == RT_SP)
    {
        val = cpu_read_reg(ctx->cur_inst->reg_1) + (i16)ctx->fetched_data;
    }

    /* 8 bit sum flags update */
    int z = (val & 0xFF) == 0;
    int h = (cpu_read_reg(ctx->cur_inst->reg_1) & 0xF) + (ctx->fetched_data & 0xF) >= 0x10;
    int c = (int)(cpu_read_reg(ctx->cur_inst->reg_1) & 0xFF) + (int)(ctx->fetched_data & 0xFF) >= 0x100;

    /* 16 bit sum flags update (for SP use 8 bit sum) */
    if (is_16bit && ctx->cur_inst->reg_1 != RT_SP)
    {
        z = -1; /* For 16b add the Z flags is don't care */
        h = (cpu_read_reg(ctx->cur_inst->reg_1) & 0xFFF) + (ctx->fetched_data & 0xFFF) >= 0x1000;
        u32 n = ((u32)cpu_read_reg(ctx->cur_inst->reg_1)) + ((u32)ctx->fetched_data);
        c = n >= 0x10000;
    }

    /* Write result in register */
    cpu_set_reg(ctx->cur_inst->reg_1, val & 0xFFFF);
    /* Update flags */
    cpu_set_flags(ctx, z, 0, h, c);
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
