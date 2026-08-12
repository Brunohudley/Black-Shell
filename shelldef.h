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

#define MAXVAR 128

typedef struct s_cmd_help
{
  const char *name;
  const char *desc;
} t_cmd_help;

typedef struct s_func
{
  char *name;
  char **lines;
  int line_count;
  int capacity;
} t_func;

typedef struct t_fun_props
{
  int func_cx;
  int func_capacity;
} t_fun_props;

typedef union s_var_data
{
  int int_value;
  float flt_value;
  char str_value[256];
} t_var_data;

typedef struct s_var
{
  t_var_data data;
  char name[64];
} t_var;
