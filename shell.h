#pragma once

#include "shelldef.h"

typedef struct s_shell
{
  t_func *func;
  t_var var[MAXVAR];
  t_fun_props fun_props;
  int var_count;
  int error;
  int cmp_flag;
} t_shell;

extern t_shell shell;

void sh_loop (void);
void initShell (void);