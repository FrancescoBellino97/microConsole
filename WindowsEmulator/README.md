# Windows Emulator

## Architecture (TBD)


### Build targets
Makefile can build 2 targets:
 - make all, build the project in 05_build/ directory and print the memory consumption estimation on console
 - make cleanall, delete the 05_build/ directory

NOTE: the code can be compiled for debug setting the relative define in "commons.h" to true or without debug (to save resources) setting the define to false.

### Launch
To launch the emulator run the following command:
"05_build\\microConsole.exe" "06_roms\\01-special.gb" 
where:
 - 05_build\\microConsole.exe is the emulator executable
 - 06_roms\\01-special.gb is the ROM to launch
