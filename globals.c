#include "shell.h"
#include "cfgfile.h"
#include "commands.h"


#ifdef _WIN32
char *(*pcwd)(char *, int) = _getcwd;
#else
char *(*pcwd)(char *, size_t) = getcwd;
#endif

ErrorSet errorset;
Program_config cfgProgram;

s_func *func_s = NULL;
int func_cx = 0;
int func_capacity = 0;

Var vars[MAXVAR];
int var_count = 0;
int cmp_flag = 0;