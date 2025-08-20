TARGET = paint
CC = gcc
CFLAGS = -Wall -Wextra -Iinclude
LIBS = -lopengl32 -lglu32 -lfreeglut -lm

SRC := $(shell find src -name "*.c")
OBJ = $(SRC:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $(TARGET) $(LIBS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)

run: all
	./$(TARGET)
