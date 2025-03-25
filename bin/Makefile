# Compilador y banderas
CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -I../include

# Directorios
SRC_DIR = ../src
INCLUDE_DIR = ../include

# Archivos fuente comunes
SRC_COMMON = $(SRC_DIR)/main.c $(SRC_DIR)/utils.c $(SRC_DIR)/config.c $(SRC_DIR)/input.c $(SRC_DIR)/menu.c

# Detect the operating system
UNAME_S := $(shell uname -s)

# Archivos fuente específicos para cada sistema operativo
ifeq ($(UNAME_S),FreeBSD)
	SRC_OS = $(SRC_DIR)/imp_bsd.c
	CFLAGS += -D__FreeBSD__
else ifeq ($(UNAME_S),Linux)
	SRC_OS = $(SRC_DIR)/imp_deb.c
	CFLAGS += -D__linux__
endif

# Todos los archivos fuente
SRC = $(SRC_COMMON) $(SRC_OS)

# Archivo ejecutable
TARGET = uhb

# Regla para compilar el programa
$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

# Regla para limpiar archivos compilados
clean:
	rm -f $(TARGET)
