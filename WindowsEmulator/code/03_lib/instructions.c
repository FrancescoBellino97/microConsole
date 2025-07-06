/**
 ******************************************************************************
 * @file    instructions.c
 * @author  Bellino Francesco
 * @brief   Instructions source code.
 *
 ******************************************************************************
 */

#include <instructions.h>
#include <cpu.h>

/* Look-Up table from OpCode to Instruction structure */
instruction instructions[0x100] =
{
    [0x00] = {IN_NOP, AM_IMP},

    [0x05] = {IN_DEC, AM_R, RT_B},

    [0x0E] = {IN_LD, AM_R_D8, RT_C},

    [0xAF] = {IN_XOR, AM_R, RT_A},

    [0xC3] = {IN_JP, AM_D16},

    [0xF3] = {IN_DI}
};

/**
  * @brief  Convert the OpCode to an instruction
  * @param  opcode:			operation code read
  * @retval instruction:	instruction decoded
  */
instruction *instruction_by_opcode(u8 opcode)
{
    return &instructions[opcode];
}

#if DEBUG==true
/* Look-Up table to convert the type to string (for debug)  */
char *inst_lookup[] =
{
    "<NONE>",
    "NOP",
    "LD",
    "INC",
    "DEC",
    "RLCA",
    "ADD",
    "RRCA",
    "STOP",
    "RLA",
    "JR",
    "RRA",
    "DAA",
    "CPL",
    "SCF",
    "CCF",
    "HALT",
    "ADC",
    "SUB",
    "SBC",
    "AND",
    "XOR",
    "OR",
    "CP",
    "POP",
    "JP",
    "PUSH",
    "RET",
    "CB",
    "CALL",
    "RETI",
    "LDH",
    "JPHL",
    "DI",
    "EI",
    "RST",
    "IN_ERR",
    "IN_RLC", 
    "IN_RRC",
    "IN_RL", 
    "IN_RR",
    "IN_SLA", 
    "IN_SRA",
    "IN_SWAP", 
    "IN_SRL",
    "IN_BIT", 
    "IN_RES", 
    "IN_SET"
};
#endif

#if DEBUG==true
/**
  * @brief  Convert the instruction type to a string (for debug)
  * @param  t:		instruction type
  * @retval char*:	instruction string name
  */
char *inst_name(in_type t)
{
    return inst_lookup[t];
}
#endif
