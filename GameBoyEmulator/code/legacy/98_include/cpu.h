/**
 ******************************************************************************
 * @file    cpu.h
 * @author  Bellino Francesco
 * @brief   CPU header file.
 *
 *
 ******************************************************************************
 */

#ifndef CPU_H_
#define CPU_H_

#include "../98_include/common.h"
#include "../98_include/instructions.h"

#define	BIT_Z	7
#define	BIT_N	6
#define	BIT_H	5
#define BIT_C	4

typedef struct {
    u8 a;	/* Register A */
    u8 f;	/* Register F */
    u8 b;	/* Register B */
    u8 c;	/* Register C */
    u8 d;	/* Register D */
    u8 e;	/* Register E */
    u8 h;	/* Register H */
    u8 l;	/* Register L */
    u16 pc;	/* Program Counter */
    u16 sp;	/* Stack Pointer */
} cpu_registers;

typedef struct {
    cpu_registers regs;			/* Registers of CPU */

    /* Fetch data */
    u16 fetched_data;			/* Data fetched based on the Instruction */
    u16 mem_dest;				/* Memory Destination ? */
    bool dest_is_mem;			/* Flag for Memory Destination ? */
    u8 cur_opcode;				/* Operation Code read from Cartridge */
    instruction *cur_inst;		/* Instruction decoded by Op Code */

    /* CPU state */
    bool halted;				/* Flag for Halted CPU */
    bool stepping;				/* Flag for Running CPU */

    bool int_master_enabled;	/* Flag for Interrupts enabled? */
    u8 ie_register;				/* Interrupt register */
    
} cpu_context;


void cpu_init();
bool cpu_step();


#endif
