/**
 ******************************************************************************
 * @file    stack.h
 * @author  Bellino Francesco
 * @brief   STACK header file.
 *
 *
 ******************************************************************************
 */

#ifndef STACK_H_
#define STACK_H_

#include "../98_include/common.h"

void stack_push(u8 data);
void stack_push16(u16 data);

u8 stack_pop();
u16 stack_pop16();

#endif
