#include "cfgfile.h"
#include "commands.h"
#include "shell.h"

#ifdef _WIN32
char *(*pcwd) (char *, int) = _getcwd;
#else
char *(*pcwd) (char *, size_t) = getcwd;
#endif

t_shell shell;

void
initShell ()
{
  shell.error = 1;
  shell.func = NULL;
  shell.fun_props.func_cx = 0;
  shell.fun_props.func_capacity = 0;
  shell.var_count = 0;
  shell.cmp_flag = 0;
}

Program_config cfgProgram;