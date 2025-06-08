# Compiler and flags
CC = gcc
CFLAGS = -Wall -g -Wextra -std=c11 -I../include/headers -I../include/interfaces

# Detect the operating system
UNAME_S := $(shell uname -s)

# OS-specific include paths and flags
ifeq ($(UNAME_S),FreeBSD)
	CFLAGS += -I../../additions/bsd/include -D__FreeBSD__
	SRC_OS = $(wildcard ../../additions/bsd/src/*.c)
else ifeq ($(UNAME_S),Linux)
	CFLAGS += -I../../additions/debian/include -D__linux__
	SRC_OS = $(wildcard ../../additions/debian/src/*.c)
endif

# Directories
UHB_SRC_DIR = ../src
UHB_HEA_DIR = ../include/headers
UHB_INT_DIR = ../include/interfaces
UHB_BSE_DIR = ../../base
UHB_ADD_DIR = ../../additions

# Common source files
SRC_COMMON = $(shell find $(UHB_SRC_DIR) -name '*.c')

# All source files
SRC = $(SRC_COMMON) $(SRC_OS)

# Object files
OBJ = $(SRC:.c=.o)

# Dependency files
DEP = $(OBJ:.o=.d)

# Executable filename
TARGET = uhb

# Default target
all: $(TARGET)

# Linking
$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ)

# Compiling object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@
	$(CC) -M $(CFLAGS) $< > $*.d

# Include dependency files
-include $(DEP)

# Cleanup rule
clean:
	rm -f $(TARGET) $(OBJ) $(DEP)

.PHONY: all clean
