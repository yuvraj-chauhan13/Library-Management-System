CC = gcc
CFLAGS = -Wall -Wextra -g

SRC = main.c \
      menu.c \
      storage.c \
      users.c \
      books.c \
      loans.c \
      statistics.c \
      logs.c \
      security.c

OUT = library

all:
	$(CC) $(CFLAGS) -o $(OUT) $(SRC)

clean:
	rm -f $(OUT)