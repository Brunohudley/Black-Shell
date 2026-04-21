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
