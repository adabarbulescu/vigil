CC     = gcc
CFLAGS = -Wall -Wextra

SRC    = src/main.c src/proc.c src/display.c
TARGET = vigil

all: release

release: CFLAGS += -O2
release: $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

debug: CFLAGS += -g -O0
debug: $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

asan: CFLAGS += -g -O0 -fsanitize=address,undefined
asan: $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

clean:
	rm -f $(TARGET)

.PHONY: all release debug clean asan