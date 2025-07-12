################################################################################
# @file    makefile
# @author  Bellino Francesco
# @brief   Makefile for microConsole emulator Windows
#
# Usage: launch the microConsole.exe passing a rom name as argument
# Ex. .\05_build\microConsole.exe .\06_roms\01-special.gb
#
# BUILDING TARGET:
#	1) all: 		build an the .exe
#	2) cleanall:	remove the BUILD_DIR if present
#
# WORKFLOW:
#  1) Define the project name, application directory, library directory,
#     headers directory and build output directory.
#  2) Define the object file list and the directory of header files.
#  3) Define the dependency variable (for each object file a .d dependecy file 
#	  is generated (they are considered makefiles).
#  4) Read all dependencies with the "include" command (the - is used to 
#	  suppress errors that are generated when the dependency files are
#	  missing, the command is invoked multiple times).
#  5) Define the compiler variable, the compiler options variable and the 
#     dependency variable.
#  6) Create the build directory if not exist.
#  7) Define the "all" and "cleanall" targets.
#
################################################################################


#		BUILD OPTIONS
PROJECT_NAME	:=	microConsole
APP_DIR			:=	01_gbemu
INC_DIR			:=	02_include
LIB_DIR			:=	03_lib
BUILD_DIR		:=	05_build
BUILD_SUPP_DIR	:=	$(BUILD_DIR)/code

SDL_PATH        :=    04_sdl\sdl2\lib\x64
SDL_TTF_PATH    :=    04_sdl\sdl2_ttf\lib\x64

#		FILES
OBJS			:=	$(BUILD_SUPP_DIR)/main.o \
					$(BUILD_SUPP_DIR)/bus.o \
					$(BUILD_SUPP_DIR)/cart.o \
					$(BUILD_SUPP_DIR)/cpu_proc.o \
					$(BUILD_SUPP_DIR)/cpu_util.o \
					$(BUILD_SUPP_DIR)/cpu_fetch.o \
					$(BUILD_SUPP_DIR)/cpu.o \
					$(BUILD_SUPP_DIR)/emu.o \
					$(BUILD_SUPP_DIR)/instructions.o \
					$(BUILD_SUPP_DIR)/ppu.o \
					$(BUILD_SUPP_DIR)/ram.o \
					$(BUILD_SUPP_DIR)/timer.o

DLLS			:=	$(BUILD_DIR)\SDL2.dll \
					$(BUILD_DIR)\SDL2_ttf.dll \
					$(BUILD_DIR)\libfreetype-6.dll
					
INCS			:=	-I$(INC_DIR) \
					-I04_sdl/sdl2/include/SDL2 \
					-I04_sdl/sdl2_ttf/include/SDL2
					
#		DEPENDENCIES
DEPS			:=	$(OBJS:.o=.d)
-include $(DEPS)
	
	
#		COMPILER OPTIONS
CC				:=	gcc
CFLAGS			:=	-Wall
CPPFLAGS		:=	$(INCS)
DEPFLAGS		:=	-MMD -MP
DLLFLAGS		:=	-lSDL2 -lSDL2_ttf -lfreetype-6 -L$(BUILD_DIR)
MEM_USAGE		:=	memoryUsage.exe

#		POST-BUILD OPTIONS
OBJDUMP			:=	C:\msys64\ucrt64\bin\objdump


#		TARGETS
.PHONY: all cleanall mem_usage

all: $(BUILD_DIR) $(DLLS) $(BUILD_DIR)/$(PROJECT_NAME).exe $(BUILD_DIR)/$(PROJECT_NAME).lst $(BUILD_DIR)/$(PROJECT_NAME).hdr $(BUILD_DIR)/memory_usage.txt
	

cleanall:	#Delete directory recursively (/S) and without confirm (/Q)
	@if exist "$(BUILD_DIR)" rmdir /Q /S "$(BUILD_DIR)"
	@echo Deleting build directory


mem_usage: $(BUILD_DIR)/$(PROJECT_NAME).exe $(BUILD_DIR)/$(PROJECT_NAME).hdr
	@$(MEM_USAGE) $(BUILD_DIR)/$(PROJECT_NAME).hdr

#		BUILD PROCESS
$(BUILD_DIR)/$(PROJECT_NAME).exe: $(OBJS)
	@$(CC) $(CFLAGS) $(CPPFLAGS) $^ $(DLLFLAGS) -o $@ -Wl,-Map="$(BUILD_DIR)/$(PROJECT_NAME).map"
	@echo Creating $@
	
$(BUILD_DIR)/$(PROJECT_NAME).lst: $(BUILD_DIR)/$(PROJECT_NAME).exe
	@$(OBJDUMP) -S $^ > $@

$(BUILD_DIR)/$(PROJECT_NAME).hdr: $(BUILD_DIR)/$(PROJECT_NAME).exe
	@$(OBJDUMP) -h $^ > $@


$(BUILD_DIR)/memory_usage.txt: $(BUILD_DIR)/$(PROJECT_NAME).hdr
	@$(MEM_USAGE) $^ > $@
	@$(MEM_USAGE) $^

#		MAIN
$(BUILD_SUPP_DIR)/%.o: $(APP_DIR)/%.c 
	@$(CC) $(CFLAGS) $(CPPFLAGS) $(DEPFLAGS) -c $< $(DLLFLAGS) -o $@
	@echo Compiling $<

#		LIB
$(BUILD_SUPP_DIR)/%.o: $(LIB_DIR)/%.c 
	@$(CC) $(CFLAGS) $(CPPFLAGS) $(DEPFLAGS) -c $< $(DLLFLAGS) -o $@
	@echo Compiling $<


#		BUILD DIRECTORY CREATION
$(BUILD_DIR):
	@mkdir "$(BUILD_SUPP_DIR)"


#		DLL TRASFER TO BUILD DIRECTORY
$(BUILD_DIR)\SDL2.dll:
	@copy $(SDL_PATH)\SDL2.dll $(BUILD_DIR) > NUL
	@echo Moving $@ to build directory
	
$(BUILD_DIR)\SDL2_ttf.dll:
	@copy $(SDL_TTF_PATH)\SDL2_ttf.dll $(BUILD_DIR) > NUL
	@echo Moving $@ to build directory
	
$(BUILD_DIR)\libfreetype-6.dll:
	@copy $(SDL_TTF_PATH)\libfreetype-6.dll $(BUILD_DIR) > NUL
	@echo Moving $@ to build directory