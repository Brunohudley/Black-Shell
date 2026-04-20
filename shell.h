#pragma once

typedef struct ErrorSet
{
    int error;
}ErrorSet;

extern ErrorSet errorset;

void sh_loop(void);
