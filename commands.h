#pragma once

#ifdef _WIN32
    extern char *(*pcwd)(char *, int);
#else
    extern char *(*pcwd)(char *, size_t);
#endif

// structs

typedef struct  
{
    const char *name;
    const char *desc;
} cmd_help;

typedef struct
{
    char *name;
    char **lines;
    int line_count;
    int capacity;
}s_func;
// Process Execution

extern s_func *func_s;
extern int func_cx;

int unix_proc(char **args);
int win_proc(char **args);

// Builtins

int sh_head(char **args);
int sh_exit(char **args);
int sh_cd(char **args);
int sh_whoami(char **args);
int sh_rm(char **args);
int sh_dirrm(char **args);
int sh_name(char **args);
int sh_chown(char **args);

int sh_cls(char **args);
int sh_pwd(char **args);

int nw_file(char **args);
int nw_dir(char **args);

int sh_copy(char **args);
int sh_ls(char **args);
int sh_chmod(char **args);

int sh_uname(char **args);

int sh_help(char **args);
int sh_source(char **args);

int exec_fun(char *name);
int sh_fundef(char **args);