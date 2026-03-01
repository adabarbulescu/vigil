CC = gcc
CFLAGS = -Wall -Wextra -g

SRC = src/main.c src/proc.c src/display.c
TARGET = vigil

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

clean:
	rm -f $(TARGET)