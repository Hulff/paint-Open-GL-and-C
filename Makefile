# Nome do executável
TARGET = paint

# Compilador
CC = gcc

# Flags de compilação
CFLAGS = -Wall -Wextra -Iinclude

# Bibliotecas necessárias para OpenGL
LIBS = -lGL -lGLU -lglut

# Todos os arquivos .c dentro de src/
SRC = $(wildcard src/**/*.c) src/main.c

# Arquivos objeto (.o)
OBJ = $(SRC:.c=.o)

# Regra principal
all: $(TARGET)

# Como criar o executável
$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $(TARGET) $(LIBS)

# Como compilar cada arquivo .c em .o
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Limpar arquivos compilados
clean:
	rm -f $(OBJ) $(TARGET)

# Rodar o programa
run: all
	./$(TARGET)
