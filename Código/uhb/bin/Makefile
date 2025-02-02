# Compiler
CC = /usr/local/bin/gcc

# Compiler flags
CFLAGS = -Wall -Wextra -O2

# Source files
SRCDIR = ../src
SOURCES = $(wildcard $(SRCDIR)/*.c)

# Executable
BINDIR = ../bin
EXECUTABLE = $(BINDIR)/uhb

# Default target
all: $(EXECUTABLE)

# Debug information
print-%:
	@echo $* = $($*)

# Link and compile
$(EXECUTABLE): $(SOURCES)
	mkdir -p $(BINDIR)
	$(CC) $(CFLAGS) -o $@ $^

# Clean
clean:
	rm -f $(EXECUTABLE)

.PHONY: all clean print-%