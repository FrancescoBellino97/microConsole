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
BUILD_SUPP_DIR	:=	$(BUILD_DIR)/

#		FILES
OBJS			:=	$(BUILD_SUPP_DIR)/main.o \
					$(BUILD_SUPP_DIR)/bus.o \
					$(BUILD_SUPP_DIR)/cart.o \
					$(BUILD_SUPP_DIR)/cpu.o \
					$(BUILD_SUPP_DIR)/emu.o \
					$(BUILD_SUPP_DIR)/ppu.o \
					$(BUILD_SUPP_DIR)/timer.o
INCS			:=	-I$(INC_DIR)
					
#		DEPENDENCIES
DEPS			:=	$(OBJS:.o=.d)
-include $(DEPS)
	
	
#		COMPILER OPTIONS
CC				:=	gcc
CFLAGS			:=
CPPFLAGS		:=	$(INCS)
DEPFLAGS		:=	-MMD -MP


#		BUILD PREPARATION
$(shell IF NOT EXIST "$(BUILD_SUPP_DIR)" MKDIR "$(BUILD_SUPP_DIR)")		#Create the build directory if not present


#		TARGETS
.PHONY: all cleanall
all: $(BUILD_DIR)/$(PROJECT_NAME)

cleanall:	#Delete directory recursively (/S) and without confirm (/Q)
	IF EXIST "$(BUILD_DIR)" RMDIR /Q /S "$(BUILD_DIR)"
	
#		BUILD PROCESS
$(BUILD_DIR)/$(PROJECT_NAME): $(OBJS)
	$(CC) $(CFLAGS) $(CPPFLAGS) $^ -o $@

#		MAIN
$(BUILD_SUPP_DIR)/%.o: $(APP_DIR)/%.c
	$(CC) $(CFLAGS) $(CPPFLAGS) $(DEPFLAGS) -c $< -o $@

#		LIB
$(BUILD_SUPP_DIR)/%.o: $(LIB_DIR)/%.c
	$(CC) $(CFLAGS) $(CPPFLAGS) $(DEPFLAGS) -c $< -o $@