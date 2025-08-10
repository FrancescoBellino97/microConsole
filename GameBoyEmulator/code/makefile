#===============================================================================
# @file    makefile
# @author  Bellino Francesco
# @brief   Makefile for microConsole emulator Windows
#
# BUILDING TARGET:
#	1) all: 			build the output file depending on the build configuration
#	2) clean:			remove the build directory depending on the build configuration
#	3) cleanall:		remove the BUILD_DIR if present
#
# WORKFLOW: TODO: Update description
#  1) Define the project name, application and driver directories,
#	  build directory, 
#
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
#===============================================================================


###		BUILD OPTIONS		###
# Project name
PROJECT_NAME	:=	microConsole

# Application and driver directories
APP_DIR			:=	app
DRV_DIR			:=	drivers/$(TARGET)

# Build directory (generic and specific for target)
BUILD_DIR		:=	build
BUILD_SUPP_DIR	:=	$(BUILD_DIR)/$(TARGET)

# SDL library path (only Windows)
ifeq ($(TARGET),windows)
SDL_PATH        :=    lib\sdl\sdl2\lib\x64
SDL_TTF_PATH    :=    lib\sdl\sdl2_ttf\lib\x64
endif

# Object files list
OBJS 			:= 	$(BUILD_SUPP_DIR)/main.o \
					$(BUILD_SUPP_DIR)/emu.o \
					$(BUILD_SUPP_DIR)/cart.o \
					$(BUILD_SUPP_DIR)/cpu.o \
					$(BUILD_SUPP_DIR)/cpu_fetch.o \
					$(BUILD_SUPP_DIR)/cpu_proc.o \
					$(BUILD_SUPP_DIR)/cpu_util.o \
					$(BUILD_SUPP_DIR)/instructions.o \
					$(BUILD_SUPP_DIR)/interrupts.o \
					$(BUILD_SUPP_DIR)/ram.o \
					$(BUILD_SUPP_DIR)/ppu.o \
					$(BUILD_SUPP_DIR)/ppu_sm.o \
					$(BUILD_SUPP_DIR)/stack.o \
					$(BUILD_SUPP_DIR)/timer.o \
					$(BUILD_SUPP_DIR)/ui.o \
					$(BUILD_SUPP_DIR)/bus.o \
					$(BUILD_SUPP_DIR)/io.o \
					$(BUILD_SUPP_DIR)/dma.o \
					$(BUILD_SUPP_DIR)/lcd.o \
					$(BUILD_SUPP_DIR)/dbg.o 

# Include directories list (depend on target)
ifeq ($(TARGET),stm32f401re)
INCS			:=	-I$(APP_DIR)/inc \
					-I$(DRV_DIR)/inc
else
INCS			:=	-I$(APP_DIR)/inc \
					-I$(DRV_DIR)/inc \
					-Ilib/sdl/sdl2/include/SDL2 \
					-Ilib/sdl/sdl2_ttf/include/SDL2
endif

# Path for DLL copy at end of build process (only windows)
ifeq ($(TARGET),windows)
DLLS			:=	$(BUILD_SUPP_DIR)\SDL2.dll \
					$(BUILD_SUPP_DIR)\SDL2_ttf.dll \
					$(BUILD_SUPP_DIR)\libfreetype-6.dll
endif

# Dependency files
DEPS			:=	$(OBJS:.o=.d)
-include $(DEPS)


###		COMPILER OPTIONS	###
ifeq ($(TARGET),windows)
CC				:=	gcc
DLLFLAGS		:=	-lSDL2 -lSDL2_ttf -lfreetype-6 -L$(BUILD_SUPP_DIR)
else
CC				:=	gcc
endif
CFLAGS			:=	-Wall -Wextra
CPPFLAGS		:=	$(INCS)
DEPFLAGS		:=	-MMD -MP


###		POST-BUILD OPTIONS	###
OBJDUMP			:=	C:\msys64\ucrt64\bin\objdump
MEM_USAGE		:=	tools\memoryUsage.exe


###		BUILD REQUIREMENTS	###
ifeq ($(TARGET),stm32f401re)
REQS			:=	$(BUILD_SUPP_DIR) \
					$(BUILD_SUPP_DIR)/$(PROJECT_NAME).elf \
					$(BUILD_SUPP_DIR)/$(PROJECT_NAME).lst \
					$(BUILD_SUPP_DIR)/$(PROJECT_NAME).hdr \
					$(BUILD_SUPP_DIR)/memory_usage.txt
else
REQS			:=	$(BUILD_SUPP_DIR) \
					$(DLLS) \
					$(BUILD_SUPP_DIR)/$(PROJECT_NAME).exe \
					$(BUILD_SUPP_DIR)/$(PROJECT_NAME).lst \
					$(BUILD_SUPP_DIR)/$(PROJECT_NAME).hdr \
					$(BUILD_SUPP_DIR)/memory_usage.txt
endif
					

###		TARGETS				###
.PHONY: all cleanall clean

all:	$(REQS)

cleanall:	#Delete directory recursively (/S) and without confirm (/Q)
	@if exist "$(BUILD_DIR)" rmdir /Q /S "$(BUILD_DIR)"
	@echo Deleting build directory
	

clean:		#Delete target directory recursively (/S) and without confirm (/Q)
	@if exist "$(BUILD_SUPP_DIR)" rmdir /Q /S "$(BUILD_SUPP_DIR)"
	@echo Deleting target build directory


###		BUILD PROCESS		###
# Windows exe file build
$(BUILD_SUPP_DIR)/$(PROJECT_NAME).exe: $(OBJS)
	@$(CC) $(CFLAGS) $(CPPFLAGS) $^ $(DLLFLAGS) -o $@ -Wl,-Map="$(BUILD_SUPP_DIR)/$(PROJECT_NAME).map"
	@echo Creating $@
	

# STM32F401RE .elf file build
$(BUILD_SUPP_DIR)/$(PROJECT_NAME).elf: $(OBJS)
	@$(CC) $(CFLAGS) $(CPPFLAGS) $^ -o $@ -Wl,-Map="$(BUILD_SUPP_DIR)/$(PROJECT_NAME).map"
	@echo Creating $@
	@move "$(BUILD_SUPP_DIR)\*.d" "$(BUILD_SUPP_DIR)\others" > NUL
	@echo Moving dependency files in others sub-directory
	@move "$(BUILD_SUPP_DIR)\*.o" "$(BUILD_SUPP_DIR)\others" > NUL
	@echo Moving object files in others sub-directory

# Assembly code file generation
$(BUILD_SUPP_DIR)/$(PROJECT_NAME).lst: $(BUILD_SUPP_DIR)/$(PROJECT_NAME).exe
	@$(OBJDUMP) -S $^ > $@

# Memory allocation file generation
$(BUILD_SUPP_DIR)/$(PROJECT_NAME).hdr: $(BUILD_SUPP_DIR)/$(PROJECT_NAME).exe
	@$(OBJDUMP) -h $^ > $@

# Memory usage file generation
$(BUILD_SUPP_DIR)/memory_usage.txt: $(BUILD_SUPP_DIR)/$(PROJECT_NAME).hdr
	@$(MEM_USAGE) $^ > $@
	@$(MEM_USAGE) $^

# Application files build
$(BUILD_SUPP_DIR)/%.o: $(APP_DIR)/src/%.c 
	@$(CC) $(CFLAGS) $(CPPFLAGS) $(DEPFLAGS) -c $< $(DLLFLAGS) -o $@
	@echo Compiling $<

# Driver files build
$(BUILD_SUPP_DIR)/%.o: $(DRV_DIR)/src/%.c 
	@$(CC) $(CFLAGS) $(CPPFLAGS) $(DEPFLAGS) -c $< $(DLLFLAGS) -o $@
	@echo Compiling $<

# Build directory creation
$(BUILD_SUPP_DIR):
	@mkdir "$(BUILD_SUPP_DIR)\others"

# DLL transfer to build directory (only windows)
$(BUILD_SUPP_DIR)\SDL2.dll:
	@copy $(SDL_PATH)\SDL2.dll $(BUILD_DIR)\$(TARGET) > NUL
	@echo Moving $@ to build directory

$(BUILD_SUPP_DIR)\SDL2_ttf.dll:
	@copy $(SDL_TTF_PATH)\SDL2_ttf.dll $(BUILD_DIR)\$(TARGET) > NUL
	@echo Moving $@ to build directory

$(BUILD_SUPP_DIR)\libfreetype-6.dll:
	@copy $(SDL_TTF_PATH)\libfreetype-6.dll $(BUILD_DIR)\$(TARGET) > NUL
	@echo Moving $@ to build directory