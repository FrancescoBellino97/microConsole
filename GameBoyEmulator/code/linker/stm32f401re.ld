/* LINKER SCRIPT FOR STM32F401RE */

/* 
 * CHANGELOG:	Added (READONLY) for FLASH section in order to avoid write 
 * 				flag in the output elf file
*/

ENTRY(Reset_handler)

MEMORY
{
  FLASH (rx): ORIGIN = 0x08000000, LENGTH = 256K
  SRAM (rwx): ORIGIN = 0x20000000, LENGTH = 64K
}

SECTIONS
{
  .isr_vector (READONLY):
  {
    KEEP(*(.isr_vector))
  } >FLASH

  .text (READONLY):
  {
    . = ALIGN(4);
    
    _stext = .;
    *(.text)
    *(.rodata)
    . = ALIGN(4);
    _etext = .;
  } >FLASH

  .data :
  {
    . = ALIGN(4);
    _sdata = .;
    *(.data)
    . = ALIGN(4);
    _edata = .;
  } >SRAM AT> FLASH

  .bss :
  {
    . = ALIGN(4);
    _sbss = .;
    *(.bss)
    . = ALIGN(4);
    _ebss = .;
  } >SRAM
}