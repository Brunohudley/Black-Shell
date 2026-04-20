# BlackShell 🐚

> A custom shell written in C with its own scripting language, built-in
> commands, and interpreter-based execution.

------------------------------------------------------------------------

## 🚀 Example

``` bash
let -i x 5
while -i x 0 -g , print -i x , sub x 1
```

------------------------------------------------------------------------

## ✨ Features

### 🔹 Command System

Execute built-in and system commands:

``` bash
print hello
ls
cd folder
```

### 🔹 Variables

Create and manipulate variables:

``` bash
let -i x 100
add -i x 5
print -i x
```

### 🔹 Loops

Basic loop support:

``` bash
while -i x 1000 -l , add x 1 , print -i x
```

### 🔹 Custom Functions

Define and execute functions:

``` bash
func teste
  print hello
end

teste
# output:
hello
```

### 🔹 Config File Support

Automatically loads configuration from:

``` bash
shell.cfg
```

### 🔹 Scripting

Run commands from files:

``` bash
source file
```

### 🔹 Styled Prompt

    ┌──[User]{0}
    └─❯

------------------------------------------------------------------------

## 🧪 Usage

### 🔹 Data Types

  Flag   Type      Example
  ------ --------- ---------------------
  `-i`   Integer   `let -i x 10`
  `-f`   Float     `let -f y 1.5`
  `-s`   String    `let -s name hello`

------------------------------------------------------------------------

### 🔹 Variables

``` bash
let -i x 10
add -i x 5
print -i x
```

------------------------------------------------------------------------

### 🔹 Comparison Operators

  Flag    Meaning
  ------- ------------------
  `-l`    less than
  `-le`   less or equal
  `-g`    greater than
  `-ge`   greater or equal
  `-e`    equal
  `-ne`   not equal

------------------------------------------------------------------------

### 🔹 Loops

``` bash
let -i x 0
while -i x 10 -l , add x 1 , print -i x
```

------------------------------------------------------------------------

### 🔹 Command Structure

``` bash
<command> <type> <arguments>
```

Example:

``` bash
add -i x 1
```

------------------------------------------------------------------------

### 🔹 Functions

``` bash
func greet
  print hello
end

greet
```

------------------------------------------------------------------------

### 🔹 Scripts

``` bash
source script.bs
```

------------------------------------------------------------------------

## 🛠️ Build

``` bash
gcc commands.c main.c parser.c shell.c utils.c interpreter.c cfgfile.c -o shell
```

------------------------------------------------------------------------

## ▶️ Run

``` bash
./shell
```

------------------------------------------------------------------------

## 🧠 Highlights

-   Built from scratch in C
-   Custom tokenizer and interpreter
-   Integrated scripting language
-   Cross-platform (Windows / Unix)

------------------------------------------------------------------------

## 👤 Author

Bruno
