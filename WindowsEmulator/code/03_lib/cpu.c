/**
 ******************************************************************************
 * @file    cpu.c
 * @author  Bellino Francesco
 * @brief   CPU source code.
 *
 ******************************************************************************
 */

#include <emu.h>
#include <cpu.h>
#include <cpu_util.h>
#include <cpu_proc.h>
#include <bus.h>


static void fetch_instruction();
static void fetch_data();
static void execute();

#if DEBUG==true
static void print_header();
static void print_instruction(u16 old_pc);
#endif

cpu_context cpu_ctx = {0};

/**
  * @brief  Initialize the CPU
  * @param  None
  * @retval None
  */
void cpu_init()
{
	/* Load PC at address 0x100 */
	cpu_ctx.regs.pc = 0x100;
	cpu_ctx.regs.a = 0x01;

#if DEBUG==true
	print_header();
#endif
}

/**
  * @brief  Run CPU operations
  * @param  None
  * @retval bool:	true if no errors
  */
bool cpu_step()
{
    if (!cpu_ctx.halted)
    {
#if DEBUG==true
        u16 pc = cpu_ctx.regs.pc;
#endif

        /* Fetch Instruction Code */
        fetch_instruction();

        /* Fetch Data (if required) */
        fetch_data();

#if DEBUG==true
        print_instruction(pc);
#endif

        /* Check if Instruction is managed */
        if (cpu_ctx.cur_inst == NULL)
        {
#if DEBUG==true
            printf("Unknown Instruction! %02X\n", cpu_ctx.cur_opcode);
#endif
            exit(ERR_UNKNOWN_INSTRUCTION);
        }

        /* Execute the Instruction */
        execute();
    }
    return true;
}

/**
  * @brief  Fetch instruction from BUS
  * @param  None
  * @retval None
  */
static void fetch_instruction()
{
	/* Fetch OpCode from Bus and increment PC */
	cpu_ctx.cur_opcode = bus_read(cpu_ctx.regs.pc++);

	/* Convert the OpCode to the Instruction */
	cpu_ctx.cur_inst = instruction_by_opcode(cpu_ctx.cur_opcode);
}

/**
  * @brief  Fetch data according to the instruction loaded
  * @param  None
  * @retval None
  */
static void fetch_data()
{
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

        case AM_D16: {
            u16 lo = bus_read(cpu_ctx.regs.pc);
            emu_cycles(1);

            u16 hi = bus_read(cpu_ctx.regs.pc + 1);
            emu_cycles(1);

            cpu_ctx.fetched_data = lo | (hi << 8);

            cpu_ctx.regs.pc += 2;

            return;
        }

        default:
#if DEBUG==true
            printf("Unknown Addressing Mode! %d (%02X)\n", cpu_ctx.cur_inst->mode, cpu_ctx.cur_opcode);
#endif
            exit(ERR_UNKNOWM_ADDRESS_MODE);
            return;
    }
}

/**
  * @brief  Execute the instruction fetched
  * @param  None
  * @retval None
  */
static void execute()
{
	/* From Instruction Type get the relative Callback */
    IN_PROC proc = inst_get_processor(cpu_ctx.cur_inst->type);

    if (!proc)
    {
        NO_IMPL
    }

    /* Execute the Callback */
    proc(&cpu_ctx);
}

#if DEBUG==true
/**
  * @brief  Print the header for instruction flow
  * @param  None
  * @retval None
  */
static void print_header()
{
	printf("\n");
	printf("PC\t\tINST\t(OP OP+1 OP+2)\tA\tB\tC\tD\tH\tL\tZC");
	printf("\n");
}
#endif

#if DEBUG==true
/**
  * @brief  Print the current instruction
  * @param  None
  * @retval None
  */
static void print_instruction(u16 old_pc)
{
	printf("%04X:\t%-7s\t(%02X   %02X   %02X)\t%02X\t%02X\t%02X\t%02X\t%02X\t%02X\t%01X%01X\n",
			old_pc, inst_name(cpu_ctx.cur_inst->type),
			cpu_ctx.cur_opcode, bus_read(old_pc + 1), bus_read(old_pc + 2),
			cpu_ctx.regs.a, cpu_ctx.regs.b, cpu_ctx.regs.c, cpu_ctx.regs.d, cpu_ctx.regs.h, cpu_ctx.regs.l,
			BIT(cpu_ctx.regs.f, BIT_Z), BIT(cpu_ctx.regs.f, BIT_C));
}
#endif
