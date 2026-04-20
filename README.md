# Black-Shell 🐚
It's a simple shell, made with builtin commands made for Unix and Windows. it have its own scripting language\
integrated,error controling, interpreter structure.

## Features 💡
The features are:

**Command System**\
execute builtin and system commands.
```python
print hello
ls
cd folder
```


**Variables**\
create and manipulate variables.
``` js
let -i x 100
add -i x 5
print -i x
```


**Loops**\
basic loop support

```c
while -i x 1000 -l , add x 1 , print -i x
```

**Custom Functions**\
create functions and use it.
```python
func teste
  print hello
end
 
teste
> hello
```


**Config File Support**\
load configuration file in ``shell.cfg``


**Scripting Files**\
load commands using
```python
source file
```

**Syled Prompt**
it has an styled prompt
```
┌──[User]{0}
└─❯
```
