# Compilador y banderas
CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -I../include

# Directorios
SRC_DIR = ../src
INCLUDE_DIR = ../include

# Archivos fuente comunes
SRC_COMMON = $(SRC_DIR)/main.c $(SRC_DIR)/so_detect.c $(SRC_DIR)/utils.c

# Archivos fuente específicos para cada sistema operativo
SRC_OS = $(SRC_DIR)/imp_bsd.c $(SRC_DIR)/imp_deb.c

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
