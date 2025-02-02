# Compilador y banderas
CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -I../include

# Ubicación de los fuentes: en ../src (respecto al directorio del Makefile)
SRC = $(wildcard ../src/*.c)
# Definimos los objetos reemplazando la ruta y extensión
OBJ = $(patsubst ../src/%.c, %.o, $(SRC))

# Nombre del binario a generar (en el directorio actual, /root/uhb/bin)
BIN = uhb

# Regla por defecto: compila el binario
all: $(BIN)

# Enlaza los objetos para crear el binario
$(BIN): $(OBJ)
	$(CC) $(CFLAGS) -o $(BIN) $(OBJ)

# Regla genérica para compilar archivos .c a .o
# Los archivos objeto se generarán en el directorio actual (/root/uhb/bin)
%.o: ../src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Regla para limpiar los archivos generados (objetos y binario)
clean:
	rm -f $(OBJ) $(BIN)

# Regla para compilar y ejecutar el binario (requiere permisos de root)
run: all
	./$(BIN)