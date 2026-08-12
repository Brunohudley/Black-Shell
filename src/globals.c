// ============================================================================
// Copyright (c) 2026 Brunohudley. All rights reserved.
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program. If not, see <https://gnu.org>.
// ============================================================================

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
