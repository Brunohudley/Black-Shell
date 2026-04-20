CC = gcc

SRC = commands.c main.c parser.c shell.c utils.c interpreter.c cfgfile.c

OUT = shell.exe

CFLAGS = -Wall -Wextra

all:
	$(CC) $(CFLAGS) $(SRC) -o $(OUT)
