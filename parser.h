#pragma once

char *sh_getLine(void);
char **sh_splitLine(char *line);
int sh_execute(char **args);
    