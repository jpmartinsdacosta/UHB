# Compilador y banderas
CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -I../include

# Directorios
SRC_DIR = ../src
INCLUDE_DIR = ../include

# Archivos fuente comunes
SRC_COMMON = $(SRC_DIR)/main.c $(SRC_DIR)/so_detect.c $(SRC_DIR)/utils.c

# Detectar el sistema operativo
OS := $(shell uname)

# Archivos fuente específicos para cada sistema operativo
ifeq ($(OS), FreeBSD)
    SRC_OS = $(SRC_DIR)/imp_bsd.c
else ifeq ($(OS), Linux)
    SRC_OS = $(SRC_DIR)/imp_deb.c
else
    $(warning No se definió un archivo específico para $(OS), compilando solo archivos comunes)
    SRC_OS = # No se incluye archivo específico
endif


# Combinar archivos comunes y específicos del SO
SRC = $(SRC_COMMON) $(SRC_OS)
OBJ = $(SRC:$(SRC_DIR)/%.c=$(SRC_DIR)/%.o)

# Nombre del binario
BIN = uhb

# Regla por defecto
all: $(BIN)

# Compilar el binario
$(BIN): $(OBJ)
	$(CC) $(CFLAGS) -o $(BIN) $(OBJ)

# Compilar archivos .c a .o
$(SRC_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Limpiar archivos compilados
clean:
	rm -f $(OBJ) $(BIN)

# Ejecutar el binario
run: all
	./$(BIN)

