/**
 ******************************************************************************
 * @file    cpu_fetch.c
 * @author  Bellino Francesco
 * @brief   CPU fetch source code.
 *
 *
 ******************************************************************************
 */

#include <cpu_fetch.h>
#include <common.h>
#include <cpu.h>
#include <cpu_util.h>
#include <bus.h>
#include <emu.h>


extern cpu_context cpu_ctx;


/**
  * @brief  Fetch data according to the instruction loaded
  * @param  None
  * @retval None
  */
void fetch_data()
{
	u16 addr, lo, hi;

	cpu_ctx.mem_dest = 0;
	cpu_ctx.dest_is_mem = false;

    if (cpu_ctx.cur_inst == NULL)
    {
        return;
    }

    /* Based on the Addressing Mode read from Bus more data */
    switch(cpu_ctx.cur_inst->mode)
    {
        case AM_IMP: return;

        case AM_R:
        	cpu_ctx.fetched_data = cpu_read_reg(cpu_ctx.cur_inst->reg_1);
            return;

        case AM_R_D8:
        	cpu_ctx.fetched_data = bus_read(cpu_ctx.regs.pc);
            emu_cycles(1);
            cpu_ctx.regs.pc++;
            return;

		case AM_R_D16:
        case AM_D16:
            lo = bus_read(cpu_ctx.regs.pc);
            emu_cycles(1);
            hi = bus_read(cpu_ctx.regs.pc + 1);
            emu_cycles(1);
            cpu_ctx.fetched_data = lo | (hi << 8);
            cpu_ctx.regs.pc += 2;
            return;
        
        case AM_MR_R:
        	cpu_ctx.fetched_data = cpu_read_reg(cpu_ctx.cur_inst->reg_2);
        	cpu_ctx.mem_dest = cpu_read_reg(cpu_ctx.cur_inst->reg_1);
        	cpu_ctx.dest_is_mem = true;
            if (cpu_ctx.cur_inst->reg_1 == RT_C)
            {
            	cpu_ctx.mem_dest |= 0xFF00;
            }
            return;

        case AM_R_MR:
            addr = cpu_read_reg(cpu_ctx.cur_inst->reg_2);
            if (cpu_ctx.cur_inst->reg_2 == RT_C)
            {
                addr |= 0xFF00;
            }
            cpu_ctx.fetched_data = bus_read(addr);
            emu_cycles(1);
            return;

        case AM_R_HLI:
        	cpu_ctx.fetched_data = bus_read(cpu_read_reg(cpu_ctx.cur_inst->reg_2));
            emu_cycles(1);
            cpu_set_reg(RT_HL, cpu_read_reg(RT_HL) + 1);
            return;

        case AM_R_HLD:
        	cpu_ctx.fetched_data = bus_read(cpu_read_reg(cpu_ctx.cur_inst->reg_2));
            emu_cycles(1);
            cpu_set_reg(RT_HL, cpu_read_reg(RT_HL) - 1);
            return;

        case AM_HLI_R:
        	cpu_ctx.fetched_data = cpu_read_reg(cpu_ctx.cur_inst->reg_2);
        	cpu_ctx.mem_dest = cpu_read_reg(cpu_ctx.cur_inst->reg_1);
        	cpu_ctx.dest_is_mem = true;
            cpu_set_reg(RT_HL, cpu_read_reg(RT_HL) + 1);
            return;

        case AM_HLD_R:
        	cpu_ctx.fetched_data = cpu_read_reg(cpu_ctx.cur_inst->reg_2);
        	cpu_ctx.mem_dest = cpu_read_reg(cpu_ctx.cur_inst->reg_1);
        	cpu_ctx.dest_is_mem = true;
            cpu_set_reg(RT_HL, cpu_read_reg(RT_HL) - 1);
            return;

        case AM_R_A8:
        	cpu_ctx.fetched_data = bus_read(cpu_ctx.regs.pc);
            emu_cycles(1);
            cpu_ctx.regs.pc++;
            return;

        case AM_A8_R:
        	cpu_ctx.mem_dest = bus_read(cpu_ctx.regs.pc) | 0xFF00;
        	cpu_ctx.dest_is_mem = true;
            emu_cycles(1);
            cpu_ctx.regs.pc++;
            return;

        case AM_HL_SPR:
        	cpu_ctx.fetched_data = bus_read(cpu_ctx.regs.pc);
            emu_cycles(1);
            cpu_ctx.regs.pc++;
            return;

        case AM_D8:
        	cpu_ctx.fetched_data = bus_read(cpu_ctx.regs.pc);
            emu_cycles(1);
            cpu_ctx.regs.pc++;
            return;

        case AM_A16_R:
        case AM_D16_R:
            lo = bus_read(cpu_ctx.regs.pc);
            emu_cycles(1);
            hi = bus_read(cpu_ctx.regs.pc + 1);
            emu_cycles(1);
            cpu_ctx.mem_dest = lo | (hi << 8);
            cpu_ctx.dest_is_mem = true;
            cpu_ctx.regs.pc += 2;
            cpu_ctx.fetched_data = cpu_read_reg(cpu_ctx.cur_inst->reg_2);
            return;

        case AM_MR_D8:
        	cpu_ctx.fetched_data = bus_read(cpu_ctx.regs.pc);
            emu_cycles(1);
            cpu_ctx.regs.pc++;
            cpu_ctx.mem_dest = cpu_read_reg(cpu_ctx.cur_inst->reg_1);
            cpu_ctx.dest_is_mem = true;
            return;

        case AM_MR:
        	cpu_ctx.mem_dest = cpu_read_reg(cpu_ctx.cur_inst->reg_1);
        	cpu_ctx.dest_is_mem = true;
        	cpu_ctx.fetched_data = bus_read(cpu_read_reg(cpu_ctx.cur_inst->reg_1));
            emu_cycles(1);
            return;

        case AM_R_A16:
            lo = bus_read(cpu_ctx.regs.pc);
            emu_cycles(1);
            u16 hi = bus_read(cpu_ctx.regs.pc + 1);
            emu_cycles(1);
            addr = lo | (hi << 8);
            cpu_ctx.regs.pc += 2;
            cpu_ctx.fetched_data = bus_read(addr);
            emu_cycles(1);
            return;

        default:
#if DEBUG==true
            printf("Unknown Addressing Mode: %d (%02X)\n", cpu_ctx.cur_inst->mode, cpu_ctx.cur_opcode);
#endif
            exit(ERR_UNKNOWM_ADDRESS_MODE);
            return;
    }
}
