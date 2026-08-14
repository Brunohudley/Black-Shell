CC=gcc
SRC=$(wildcard src/*.c)

FLAGS=-Wall \
	  -Wextra \
	  -Werror

NAME=shell.exe

all: main

main:
	$(CC) $(FLAGS) $(SRC) -o $(NAME)
