/**
 ******************************************************************************
 * @file    instructions.h
 * @author  Bellino Francesco
 * @brief   Instructions header file.
 *
 *
 ******************************************************************************
 */

#ifndef INSTRUCTIONS_H_
#define INSTRUCTIONS_H_

#include <common.h>

typedef enum {
    AM_IMP,		/* Addressing Mode Implied (not needed) */
    AM_R_D16,	/* Addressing Mode 16b Data to Register */
    AM_R_R,		/* Addressing Mode Register to Register */
    AM_MR_R,	/* Addressing Mode Register to Memory Register (address) */
    AM_R,		/* Addressing Mode Register */
    AM_R_D8,	/* Addressing Mode 8b Data to Register */
    AM_R_MR,	/* Addressing Mode Memory Register to Register */
    AM_R_HLI,	/* Addressing Mode Memory Address to Register and the incremented */
    AM_R_HLD,	/* Addressing Mode Memory Address to Register and the decremented */
    AM_HLI_R,	/* Addressing Mode Register to Memory Address and the incremented */
    AM_HLD_R,	/* Addressing Mode Register to Memory Address and the decremented */
    AM_R_A8,	/* Addressing Mode 8b Address to Register */
    AM_A8_R,	/* Addressing Mode Register to 8b Address */
    AM_HL_SPR,	/* Addressing Mode Stack Pointer Register to Memory Address */
    AM_D16,		/* Addressing Mode 16b Data */
    AM_D8,		/* Addressing Mode 8b Data */
    AM_D16_R,	/* Addressing Mode Register to 16b Data */
    AM_MR_D8,	/* Addressing Mode 8b Data to Memory Register (address) */
    AM_MR,		/* Addressing Mode Memory Register (address) */
    AM_A16_R,	/* Addressing Mode Register to 16b Address */
    AM_R_A16	/* Addressing Mode 16b Address to Register */
} addr_mode;

typedef enum {
    RT_NONE,	/* No Register needed (default value) */
    RT_A,		/* Use Accumulator */
    RT_F,		/* Use Flags */
    RT_B,		/* Use Register B */
    RT_C,		/* Use Register C */
    RT_D,		/* Use Register D */
    RT_E,		/* Use Register E */
    RT_H,		/* Use Register H */
    RT_L,		/* Use Register L */
    RT_AF,		/* Use 16b Register Accumulator + Flags */
    RT_BC,		/* Use 16b Register BC */
    RT_DE,		/* Use 16b Register DE */
    RT_HL,		/* Use 16b Register HL */
    RT_SP,		/* Use Stack Pointer */
    RT_PC		/* Use Program Counter */
} reg_type;

typedef enum {
    IN_NONE,	/* Not managed */
    IN_NOP,
    IN_LD,		/*  */
    IN_INC,		/* Increment A? */
    IN_DEC,		/* Decrement A? */
    IN_RLCA,	/*  */
    IN_ADD,
    IN_RRCA,	/*  */
    IN_STOP,	/*  */
    IN_RLA,		/*  */
    IN_JR,		/*  */
    IN_RRA,		/*  */
    IN_DAA,		/*  */
    IN_CPL,		/*  */
    IN_SCF,		/*  */
    IN_CCF,		/*  */
    IN_HALT,	/*  */
    IN_ADC,		/*  */
    IN_SUB,
    IN_SBC,		/*  */
    IN_AND,
    IN_XOR,
    IN_OR,
    IN_CP,		/*  */
    IN_POP,		/*  */
    IN_JP,
    IN_PUSH,	/*  */
    IN_RET,		/*  */
    IN_CB,		/*  */
    IN_CALL,	/*  */
    IN_RETI,	/*  */
    IN_LDH,		/*  */
    IN_JPHL,	/*  */
    IN_DI,		/* Disable Interrupts */
    IN_EI,		/* Enable Interrupts */
    IN_RST,		/*  */
    IN_ERR,		/*  */

/* CB instructions */
    IN_RLC,		/*  */
    IN_RRC,		/*  */
    IN_RL,		/*  */
    IN_RR,		/*  */
    IN_SLA,		/*  */
    IN_SRA,		/*  */
    IN_SWAP,	/*  */
    IN_SRL,		/*  */
    IN_BIT,		/*  */
    IN_RES,		/*  */
    IN_SET		/*  */
} in_type;

typedef enum
{
    CT_NONE,	/* No condition needed */
	CT_NZ, 		/* Condition on Not Zero */
	CT_Z, 		/* Condition on Zero */
	CT_NC, 		/* Condition on Not Carry */
	CT_C		/* Condition on Carry */
} cond_type;

typedef struct {
    in_type type;		/* Type */
    addr_mode mode;		/* Addressing mode */
    reg_type reg_1;		/* Register 1 */
    reg_type reg_2;		/* Register 2 */
    cond_type cond;		/* Condition type */
    u8 param;			/* Parameter?? */
} instruction;

instruction *instruction_by_opcode(u8 opcode);

char *inst_name(in_type t);

#endif
