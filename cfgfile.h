#pragma once

#include "includes.h"
#include "interpreter.h"

typedef struct cfgEntry 
{
    void *ptr;
    char *key;
    DataType data_type;
}cfgEntry;

typedef struct
{
    int maxloop_reach;
    char *username;
}Program_config;

extern Program_config cfgProgram;

int read_cfg(char *cfg_path);
