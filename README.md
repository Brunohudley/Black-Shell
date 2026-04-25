# BlackShell 🐚

> A custom shell written in C with its own scripting language, built-in\
> commands, and interpreter-based execution.

## Features 💫 

> Lexer.\
> Parser.\
> Executor.

> Variables handling.\
> Loops.\
> Logic statements.\
> Builtins.\
> Process execution. 

## builtins 🛠️
> These ate the builtins 

``cd`` `` Change dir``\
``pwd`` ``Show working dir``\
``ls`` ``List files and directories in a directory``\
``chown`` ``Chance group | user permission``\
``chmod`` ``Chance permissions``\
``print`` ``Show input``\
``clear`` ``Clear screen``\
``nm`` ``Name file``\
``rm`` ``Remove file``\
``rmdir`` ``Remove directory``\
``nf`` ``Create file``\
``mkdir`` ``Create directory``\
``df`` ``Show disk properties``\
``whoami`` ``Show user name``\
``uname`` ``Show hardware and OS info``\
``head`` ``Show file content``\
``source`` ``execute script files (no need for extension)``\
``exit`` ``Exit shell``

> These are scripting builtins

``let`` ``Create variable``\
``add`` ``Add the value of var2 into var1``\
``sub`` ``Sub the value of var2 into var1``\
``div`` ``Div the value of var2 into var1``\
``mul`` ``Mul the value of var2 into var1``\
``cmp`` ``Compare var1 with var2``\
``do`` ``Do command based on expression and result of cmp``\
``while`` ``While loop``

> The Error Type

``This shell can show errors in the multi prompt``\
``{0}`` ``OK``\
``{1}`` ``ERROR``

## flags 🚩
> Variables Related

``-i`` ``INTEGER``\
``-s`` ``STRING``\
``-f`` ``FLOAT``

``Example: let -i x 100``

> Condition Related

``-e`` ``EQUAL``\
``-n`` ``NOT EQUAL``\
``-l`` ``LESS``\
``-g`` ``GREATER``\
``-le`` ``LESS || EQUAL``\
``-ge`` ``GREATER || EQUAL``

# How to run 👟

> Windows :
> in Windows you can create an batch file or Makefile

``run.bat``
```batch
set CC=gcc
set SRC=cfgfile.c \
    commands.c \
	  globals.c \
	  interpreter.c \
	  main.c \
	  parser.c \
	  shell.c \
	  utils.c
set FLAGS=-Wall \
	        -Wextra \
	        -Werror
set NAME=Shell.exe

%CC% %SRC% %FLAGS% -o %NAME%
```

``Makefile``
```Makefile
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
```
