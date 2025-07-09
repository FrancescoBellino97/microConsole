# MicroConsole Project

## Windows Emulator (on going)
Emulator developed in C that runs on Windows

### Build targets
Makefile can build 2 targets:
 - make all, build the project in 05_build/ directory and print the memory consumption estimation on console
 - make cleanall, delete the 05_build/ directory
 - make mem_usage, print the memory usage (it builds the project if it is not present)

NOTE: the code can be compiled for debug setting the relative define in "commons.h" to true or without debug (to save resources) setting the define to false.

### Launch
To launch the emulator run the following command:
"05_build\\microConsole.exe" "06_roms\\01-special.gb" 
where:
 - 05_build\\microConsole.exe is the emulator executable
 - 06_roms\\01-special.gb is the ROM to launch

## STM Emulator
TBD