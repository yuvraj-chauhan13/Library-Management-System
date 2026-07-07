CC=gcc
CFLAGS=-Wall -Wextra -g

SRC=main.c menu.c storage.c users.c books.c
OUT=library

all:
	$(CC) $(CFLAGS) -o $(OUT) $(SRC)

clean:
	rm -f $(OUT)

