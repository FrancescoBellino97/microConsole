/**
 ******************************************************************************
 * @file    emu.c
 * @author  Bellino Francesco
 * @brief   Emu source code.
 *
 ******************************************************************************
 */

#include <stdio.h>
#include <emu.h>
#include <cart.h>
#include <cpu.h>
#include <SDL.h>
#include <SDL_ttf.h>

/* 
  Emu components:

  |Cart|
  |CPU|
  |Address Bus|
  |PPU|
  |Timer|

*/

static void delay();

static emu_context emu_ctx;

/**
  * @brief  Run the emulation procedure
  * @param  argc:	number of arguments passed
  * 		**argv:	pointer to the arguments passed
  * @retval FALSE:	an error occurred
  * 		TRUE:	no errors
  */
int emu_run(int argc, char **argv) {
	/* Check if there is at least one argument */
    if (argc < 2)
    {
#if DEBUG==true
        printf("Usage: emu <rom_file>\n");
#endif
        return ERR_MISSING_ROM;
    }

    /* Load the cartridge */
    if (!cart_load(argv[1]))
    {
#if DEBUG==true
        printf("Failed to load ROM file: %s\n", argv[1]);
#endif
        return ERR_LOADING_ROM;
    }

#if DEBUG==true
    printf("Cart loaded..\n");
#endif

    /* SDL init */
    SDL_Init(SDL_INIT_VIDEO);
#if DEBUG==true
    printf("SDL INIT\n");
#endif

    /* SDL TTF init */
    TTF_Init();
#if DEBUG==true
    printf("TTF INIT\n");
#endif

    /* CPU init */
    cpu_init();
    
    /* Initialize EMU Context variable */
    emu_ctx.running = true;
    emu_ctx.paused = false;
    emu_ctx.ticks = 0;

    /* Infinite Loop */
    while(emu_ctx.running)
    {
        if (emu_ctx.paused)
        {
            delay(10);
            continue;
        }

        /* CPU Elaboration */
        if (!cpu_step())
        {
            printf("CPU Stopped\n");
            return ERR_CPU_STOPPED;
        }

        emu_ctx.ticks++;
    }

    return NO_ERROR;
}

/**
  * @brief  Get the emulation context
  * @param  None
  * @retval *emu_ctx:	emulation context variable
  */
emu_context *emu_get_context() {
    return &emu_ctx;
}

/**
  * @brief  Todo
  * @param  None
  * @retval None
  */
void emu_cycles(int cpu_cycles)
{
    //TODO...
}

/**
  * @brief  Wait a delay in ms
  * @param  ms:		value in ms to wait
  * @retval None
  */
static void delay(u32 ms)
{
    SDL_Delay(ms);
}

