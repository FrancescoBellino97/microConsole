/**
 ******************************************************************************
 * @file    cpu_util.h
 * @author  Bellino Francesco
 * @brief   CPU utilities header file.
 *
 *
 ******************************************************************************
 */

#ifndef CPU_UTIL_H_
#define CPU_UTIL_H_

#include "../98_include/cpu.h"

u16 reverse(u16 n);
cpu_registers *cpu_get_regs();
u16 cpu_read_reg(reg_type rt);
void cpu_set_reg(reg_type rt, u16 val);
u8 cpu_read_reg8(reg_type rt);
void cpu_set_reg8(reg_type rt, u8 val);
u8 cpu_get_ie_register();
void cpu_set_ie_register(u8 val);
void cpu_set_flags(cpu_context *ctx, char z, char n, char h, char c);
bool check_cond(cpu_context *ctx);
bool is_16_bit(reg_type rt);
reg_type decode_reg(u8 reg);


#endif
