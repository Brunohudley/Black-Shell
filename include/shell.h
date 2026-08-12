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
