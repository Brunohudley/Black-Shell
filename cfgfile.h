#pragma once

#include "includes.h"
#include "interpreter.h"

typedef struct cfgEntry
{
  void *ptr;
  char *key;
  t_data_type data_type;
} cfgEntry;

typedef struct
{
  size_t maxloop_reach;
} Program_config;

extern Program_config cfgProgram;

int read_cfg (char *cfg_path);
