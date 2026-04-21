#pragma once

#include "includes.h"

int func_win_perm (LPCSTR filepath, DWORD perms);
int fdown (char **args);
int unix_ls (void);
int winls (void);
int fun_df (char **args);

int error_set (char *msg);