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

#include <cpu.h>

u16 reverse(u16 n);
u16 cpu_read_reg(reg_type rt);
void cpu_set_reg(reg_type rt, u16 val);
u8 cpu_get_ie_register();
void cpu_set_ie_register(u8 val);
void cpu_set_flags(cpu_context *ctx, char z, char n, char h, char c);
bool check_cond(cpu_context *ctx);


#endif
