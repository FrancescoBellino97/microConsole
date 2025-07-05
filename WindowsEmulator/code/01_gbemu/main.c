/**
 ******************************************************************************
 * @file    main.c
 * @author  Bellino Francesco
 * @brief   Main app source code.
 *
 ******************************************************************************
 */

#include <emu.h>

/**
  * @brief  Main app that call the emulation function
  * @param  argc:	number of arguments passed
  * 		**argv:	pointer to the arguments passed
  * @retval 0:		no errors
  * 		VAL:	error code
  */
int main(int argc, char **argv) {
    return emu_run(argc, argv);
}
