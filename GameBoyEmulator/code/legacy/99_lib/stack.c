/**
 ******************************************************************************
 * @file    stack.c
 * @author  Bellino Francesco
 * @brief   STACK source code.
 *
 ******************************************************************************
 */

#include "../98_include/stack.h"

#include "../98_include/bus.h"
#include "../98_include/cpu_util.h"


/**
  * @brief  Push 8b data in stack
  * @param  data:		data to push
  * @retval None
  */
 void stack_push(u8 data) {
    cpu_get_regs()->sp--;
    bus_write(cpu_get_regs()->sp, data);
}

 /**
   * @brief  Push 16b data in stack
   * @param  data:		data to push
   * @retval None
   */
void stack_push16(u16 data) {
    stack_push((data >> 8) & 0xFF);
    stack_push(data & 0xFF);
}

/**
  * @brief  Pop 8b data from stack
  * @param  None
  * @retval u8:		data get from stack
  */
u8 stack_pop() {
    return bus_read(cpu_get_regs()->sp++);
}

/**
  * @brief  Pop 16b data from stack
  * @param  None
  * @retval u16:		data get from stack
  */
u16 stack_pop16() {
    u16 lo = stack_pop();
    u16 hi = stack_pop();

    return (hi << 8) | lo;
}
