# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -I../include

# Directories
UHB_SRC_DIR = ../src						# Directory where UHB base source files are stored.
UHB_HEA_DIR = ../include					# Directory where UHB base header files are stored.

COM_SRC_DIR = ../services/common/src		# Directory where UHB source files common to all OS are stored.
COM_HEA_DIR = ../services/common/include 	# Directory where UHB header files common to all OS are stored.

SPE_DIR	= ../services/specific				# Directory where OS-specific implementations are stored.

# Common source files
SRC_COMMON = $(shell find $(UHB_SRC_DIR) -name '*.c') $(shell find $(COM_SRC_DIR) -name '*.c')

# Detect the operating system
UNAME_S := $(shell uname -s)

# OS-specific files
ifeq ($(UNAME_S),FreeBSD)
	SRC_OS = ../services/specific/bsd/src/imp_bsd.c
	CFLAGS += -D__FreeBSD__
else ifeq ($(UNAME_S),Linux)
	SRC_OS = ../services/specific/linux/src/imp_deb.c
	CFLAGS += -D__linux__
endif

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
