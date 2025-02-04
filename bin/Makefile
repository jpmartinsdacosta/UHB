# Compilador y banderas
CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -I../include

# Directorios
SRC_DIR = ../src
INCLUDE_DIR = ../include

# Obtener automáticamente todos los archivos .c dentro de /src/
SRC = $(wildcard $(SRC_DIR)/*.c)
OBJ = $(SRC:$(SRC_DIR)/%.c=%.o)

# Nombre del binario
BIN = uhb

# Regla por defecto
all: $(BIN)

# Compilar el binario
$(BIN): $(OBJ)
	$(CC) $(CFLAGS) -o $(BIN) $(OBJ)

# Compilar archivos .c a .o
%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Limpiar archivos compilados
clean:
	rm -f $(OBJ) $(BIN)

# Ejecutar el binario
run: all
	./$(BIN)
