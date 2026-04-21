CC=gcc
SRC=cfgfile.c \
    commands.c \
	globals.c \
	interpreter.c \
	main.c \
	parser.c \
	shell.c \
	utils.c

FLAGS=-Wall \
	  -Wextra \
	  -Werror

NAME=shell.exe

all: main

main:
	$(CC) $(FLAGS) $(SRC) -o $(NAME)