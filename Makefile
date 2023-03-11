CC=gcc
CFLAGS=-Wall -Wextra -std=c99

TARGETS=main

build: $(TARGETS)

main: main.c functions.c auxiliary.c
	$(CC) $(CFLAGS) main.c functions.c auxiliary.c -o main

clean:
	rm -f $(TARGETS)
