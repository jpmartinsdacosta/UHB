# Compilador y banderas
CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -I../include

# Detectar sistema operativo
OS = $(shell uname -s)

# Archivos fuente (comunes)
SRC_COMMON = ../src/main.c ../src/so_detect.c ../src/utils.c

# Seleccionar archivos específicos por sistema operativo
ifeq ($(OS), FreeBSD)
    SRC_OS = ../src/imp_bsd.c
else
    SRC_OS = ../src/imp_deb.c
endif

# Lista final de fuentes
SRC = $(SRC_COMMON) $(SRC_OS)
OBJ = $(patsubst ../src/%.c, %.o, $(SRC))

# Nombre del binario
BIN = uhb

# Regla por defecto: compilar el binario
all: $(BIN)

# Enlazar los objetos para crear el binario
$(BIN): $(OBJ)
	$(CC) $(CFLAGS) -o $(BIN) $(OBJ)

# Regla genérica para compilar archivos .c a .o
%.o: ../src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Limpiar archivos compilados
clean:
	rm -f $(OBJ) $(BIN)

# Ejecutar el binario
run: all
	sudo ./$(BIN)
