#include "includes.h"
#include "commands.h"
#include "utils.h"
#include "cfgfile.h"
#include "parser.h"

#ifdef _WIN32
char *(*pcwd)(char *, int) = _getcwd;
#else
char *(*pcwd)(char *, size_t) = getcwd;
#endif


s_func *func_s = NULL;
int func_cx = 0;
int func_capacity = 0;

static cmd_help cmds_help[] = {
    {"cd", "change dir"},
    {"pwd", "show working dir"},
    {"ls", "list things in a dir"},
    {"chown", "change group / user perms"},
    {"chmod", "change perms"},
    {"echo", "show input"},
    {"clear", "clear screen"},
    {"nm", "rename a file / folder or move them"},
    {"rm", "remove files"},
    {"rmdir", "remove folders"},
    {"nf", "create a file"},
    {"mkdir", "create a folder"},
    {"exit", "exits the shell"},
    {"df", "show disk properties"},
    {"whoami", "show user name"},
    {"uname", "show hardware & OS info"},
    {"head", "show file content"},
    {"let", "move value on variables"},
    {"add", "add the value of an variable"},
    {"sub", "subtract the value of an variable"},
    {"div", "divide the value to an variable"},
    {"mul", "multiply the value to an variable"},
    {"cmp", "compare a var or a number with another"},
    {"do", "execute commands based on result of cmp"},
    {"while", "creates an loop"},
    {"source", "executes script inside a file"},
    {"-g", "used on do or while, check if the cmp of var > var2"},
    {"-l", "used on do or while, check if the cmp of var < var2"},
    {"-e", "used on do or while, check if the cmp of var == var2"},
    {"-n", "used on do or while, check if the cmp of var != var2"},
    {"-ge", "used on do or while, check if the cmp of var >= var2"},
    {"-le", "used on do or while, check if the cmp of var <= var2"},
};

/*
==========

win_proc

			arguments:
            ARGS
            
            details:
            USES FORK FOR CREATING A PROCESS

			description:

			* it try to execute the content in args 
            using the Unix functions

==========
*/

#ifndef _WIN32
int unix_proc(char **args)
{
    pid_t pid = fork();

    if (pid == -1)
    {
        error_set("fork");
    }

    if (pid == 0)
    {
        // processo filho
        execvp(args[0], args);
        perror("exec");
        exit(1);
    }

    // processo pai
    wait(NULL);
    return 1;
}
#endif

/*
==========

win_proc

			arguments:
            ARGS
            
            details:
            MAX ARGS 1024
            USES CREATEPROCESSA TO CREATE A PROCESS
            
			description:

			* it try to execute the content in args 
            using the Win-Api

==========
*/

#ifdef _WIN32
int win_proc(char **args)
{
    char wincmdl[1024] = {0};

    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    wincmdl[0] = '\0';

    for (int i = 0; args[i]; i++)
    {
        int rcmdl = strlen(wincmdl);
        int tokl = strlen(args[i]) + 1;

        if (rcmdl + tokl >= 1024)
        {
            return error_set("Long Arguments!\n");
        }

        strcat(wincmdl, args[i]);
        strcat(wincmdl, " ");
    }

    if (!CreateProcessA(
            NULL,
            wincmdl,
            NULL, NULL,
            FALSE,
            0,
            NULL,
            NULL,
            &si,
            &pi))
    {
        error_set("Cannot Create Process!\n");
    }

    WaitForSingleObject(pi.hProcess, INFINITE);
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    return 1;
}
#endif

/*
==========

sh_head

			arguments:
            ARGS
            
            details:
            MAX ARGS 32

			description:

			* if theres no expresion , it just print 10 lines
            of an file.

            * if the expresion is -a, it print all the content
            in the file

            * if the expresion is -n, it convert the args[2]
            into an long, and read the delimited numbers of bytes.

==========
*/

int sh_head(char **args)
{
    FILE *file;
    char buffer[256];
    int maxarg = 32;

    if (args[1][0] != '-')
    {
        for (int count = 0; count <= maxarg; count++)
        {
            file = fopen(args[count], "r");
            if (file == NULL)
            {
                return error_set("Cannot Open File!\n");
            }
            for (int line = 1; fgets(buffer, sizeof(buffer), file) && line <= 10; line++)
            {
                printf("%s", buffer);
            }
            fclose(file);
            printf("\n");
        }
    }
    else if (strcmp(args[1], "-a") == 0)
    {
        for (int count = 2; count <= (maxarg += 2); count++)
        {
            file = fopen(args[count], "r");
            if (file == NULL)
            {
                return error_set("Cannot Open File!\n");
            }
            for (; fgets(buffer, sizeof(buffer), file);)
            {
                printf("%s", buffer);
            }
            printf("\n");
            fclose(file);
        }
    }
    else if (strcmp(args[1], "-n") == 0)
    {
        long num = strtol(args[2], NULL, 10);
        for (int count = 3; count <= (maxarg += 3); count++)
        {
            file = fopen(args[count], "r");
            if (file == NULL)
            {
                return error_set("Cannot Open File!\n");
            }
            for (; fgets(buffer, sizeof(buffer), file);)
            {
                printf("%s", buffer);
            }
            printf("\n");
            fclose(file);
        }
    }
    return 1;
}

int sh_cd(char **args)
{
    if (chdir(args[1]) != 0)
    {
        return error_set("Cannot Change Directory!\n");
    }
    return 1;
}

/*
==========

sh_whoami

			arguments:
            ARGS
            
            details:
            NONE

			description:

			* it get the size of username and
            set it on the DWORD bufsize,
            then it uses the OS function
            for get the username and then print it.

==========
*/

int sh_whoami(char **args)
{
    (void)args;
    char username[256];
#ifdef _WIN32
    DWORD bufsize = sizeof(username);
    GetUserNameA(username, &bufsize);
#else
    size_t bufsize = sizeof(username);
    setlogin_r(username, bufsize);
#endif
    printf("%s\n", username);
    return 1;
}

int sh_rm(char **args)
{
    if (remove(args[1]) != 0)
    {
        return error_set("Cannot Remove File!\n");
    }
    return 1;
}

int sh_dirrm(char **args)
{
#ifdef _WIN32
    if (!RemoveDirectoryA(args[1]))
    {
        return error_set("Cannot Remove Folder!\n");
    }
#else
    if (rmdir(args[1]) != 0)
    {
        return error_set("Cannot Remove Folder!\n");
    }
#endif
    return 1;
}

int sh_name(char **args)
{
    if (rename(args[1], args[2]) != 0)
    {
        return error_set("Cannot Rename File!\n");
    }
    return 1;
}

/*
==========

sh_chown

			arguments:
            ARGS
            
            details:
            NONE

			description:

			* if its on windows it create
            expr, which is the value of args[2]
            and filepath beeing args[1].

            then it perform a bitwise operation on
            an DWORD winperm and return the funcion
            ~[func_win_perm].

            *else it uses the function ~[fdown].

==========
*/

int sh_chown(char **args)
{
#ifdef _WIN32
    const char *expr = args[2];
    const char *filepath = args[1];

    DWORD winperm = 0;

    for (; *expr; expr++)
    {
        if (*expr == 'r')
            winperm |= GENERIC_READ;
        if (*expr == 'w')
            winperm |= GENERIC_WRITE;
        if (*expr == 'x')
            winperm |= GENERIC_EXECUTE;
        if (*expr == 'a')
            winperm |= GENERIC_ALL;
    }
    return func_win_perm(filepath, winperm);
#else
    fdown(args);
#endif

    return -1;
}

int sh_cls(char **args)
{
    (void)args;
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
    return 1;
}

/*
==========

sh_pwd

			arguments:
            ARGS
            
            details:
            NONE

			description:

			* it prints the working
            directory.

==========
*/

int sh_pwd(char **args)
{
    (void)args;
    char cwd[1024];
    if (pcwd(cwd, sizeof(cwd)))
    {
        printf("%s\n", cwd);
    }
    else
    {
        return error_set("Pwd!\n");
    }
    return 1;
}

/*
==========

sh_copy

			arguments:
            ARGS
            
            details:
            NONE

			description:

			* it create a new file
            and check if any errrors
            occured.

==========
*/

int nw_file(char **args)
{
    FILE *nfile;
    nfile = fopen(args[1], "w");

    if (nfile == NULL)
    {
        return error_set("Error Creating File!\n");
    }

    fclose(nfile);
    return 1;
}

/*
==========

sh_copy

			arguments:
            ARGS
            
            details:
            NONE

			description:

			* it read from a src file
            and write into de dest file
            the bytes.

==========
*/

int sh_copy(char **args)
{

    FILE *sfile, *cfile;

    sfile = fopen(args[1], "rb");
    cfile = fopen(args[2], "wb");

    if (!sfile)
    {
        fclose(sfile);
        fclose(cfile);
        return error_set("Cannot Copy File!\n");
    }

    char copyf;

    while ((copyf = fgetc(sfile)) != EOF)
    {
        fputc(copyf, cfile);
    }

    fclose(sfile);
    fclose(cfile);

    return 1;
}

int sh_ls(char **args)
{
    (void)args;
#ifdef _WIN32
    winls();
#else
    lls();
#endif
    return 1;
}

/*
==========

lchmod

			arguments:
            FILEPATH
            EXPR
            
            details:
            NONE

			description:

			* it get the stats of the file,
            then create a parser, for parsing the 
            expressions and make bitwise operations 
            for setting usermask , groupmask and othermask
            then it uses chmod.

==========
*/

int lchmod(const char *filepath, const char *expr)
{
    // stat st and stat()
    struct stat st;
    if (stat(filepath, &st) != 0)
    {
        return error_set("Stat!\n");
    }

    // mode
    mode_t mode = st.st_mode;

    // perm
    int userp, groupp, otherp;
    userp = 0;
    groupp = 0;
    otherp = 0;

    // read u g o a
    const char *pexpr = expr;

    while (*pexpr && (*pexpr == 'u' || *pexpr == 'g' || *pexpr == 'o' || *pexpr == 'a'))
    {
        if (*pexpr == 'u')
            userp = 1;
        if (*pexpr == 'g')
            groupp = 1;
        if (*pexpr == 'o')
            otherp = 1;
        if (*pexpr == 'a')
            otherp = groupp = userp = 1;
        pexpr++;
    }

    // def a
    if (!otherp && !groupp && !userp)
    {
        otherp = groupp = userp = 1;
    }

    // masks
    mode_t usermask = S_IRUSR | S_IWUSR | S_IXUSR;
    mode_t groupmask = S_IRGRP | S_IWGRP | S_IXGRP;
    mode_t othermask = S_IROTH | S_IWOTH | S_IXOTH;

    char operator = *pexpr;
    if (operator != '+' && operator != '-' && operator != '=')
    {
        return error_set("Chmod invalid operator!\n");
    }
    pexpr++;

    // bits

    mode_t bits = 0;
    while (*pexpr && (*pexpr == 'w' || *pexpr == 'r' || *pexpr == 'x'))
    {
        if (*pexpr == 'w')
            bits |= S_IWUSR | S_IWGRP | S_IWOTH;
        if (*pexpr == 'x')
            bits |= S_IXUSR | S_IXGRP | S_IXOTH;
        if (*pexpr == 'r')
            bits |= S_IRUSR | S_IRGRP | S_IROTH;
        pexpr++;
    }

    if (userp)
    {
        if (operator == '+')
            mode |= (bits & usermask);
        if (operator == '-')
            mode &= ~(bits & usermask);
        if (operator == '=')
            mode = (mode & ~usermask) | (bits & usermask);
    }
    if (groupp)
    {
        if (operator == '+')
            mode |= (bits & groupmask);
        if (operator == '-')
            mode &= ~(bits & groupmask);
        if (operator == '=')
            mode = (mode & ~groupmask) | (bits & groupmask);
    }
    if (otherp)
    {
        if (operator == '+')
            mode |= (bits & othermask);
        if (operator == '-')
            mode &= ~(bits & othermask);
        if (operator == '=')
            mode = (mode & ~othermask) | (bits & othermask);
    }
    if (*pexpr != '\0')
    {
        return error_set("Invalid Characters!\n");
    }
    if (chmod(filepath, mode) != 0)
    {
        return error_set("chmod!\n");
    }
    return 1;
}

/*
==========

sh_chmod

			arguments:
            ARGS / FILEPATH
                 / EXPR
            
            details:
            NONE

			description:

			* it uses the funcion lchmod
            using tha value of **args as parameters
            and check if any error occur.

==========
*/

int sh_chmod(char **args)
{
    if (lchmod(args[2], args[1]) != 1)
    {
        return error_set("Lchmod Failed!\n");
    }
    return 1;
}

/*
==========

nw_dir

			arguments:
            ARGS / NAME
            
            details:
            NONE

			description:

			* it create a folder using the args[1], which is
            the name of the folder using mkdir, if it cannot
            create, it return the function ~[error_set].
==========
*/

int nw_dir(char **args)
{
    int status;
    status = mkdir(args[1]);

    if (status != 0)
    {
        return error_set("Error Creating A Folder!\n");
    }
    return 1;
}



int sh_uname(char **args)
{
    (void)args;
#ifndef _WIN32
    const char *ostype = "/proc/sys/kernel/ostype";
    const char *hostname = "/proc/sys/kernel/hostname";
    const char *cpuinfo = "/proc/cpuinfo";
    uget_uname(256, ostype, "Cannot Find", "Operational System Type", false);
    uget_uname(256, hostname, "Cannot Find", "Hostname", false);
    uget_uname(256, cpuinfo, "Cannot Find", "CPU Type: ", true);
#ifdef __x86_64__
    printf("CPU Architecture: x86_64\n");
#elif __i386__
    printf("CPU Architecture: x86\n");
#elif __arm__
    printf("CPU Architecture: ARM\n");
#elif __aarch64__
    printf("CPU Architecture: ARM64\n");
#endif // __x86_64__
#else
    char buffer[MAX_COMPUTERNAME_LENGTH + 1];
    DWORD size = MAX_COMPUTERNAME_LENGTH + 1;
    SYSTEM_INFO sysInfo;

    printf("Operational System Type: Windows\n");
    if (!GetComputerName(buffer, &size))
    {
        return error_set("GetComputerName!\n");
    }
    printf("Hostname: %s\n", buffer);
    GetSystemInfo(&sysInfo);
    HKEY key;
    char name[256];
    DWORD ssize = sizeof(name);
    char *hardware_path = "HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0";

    if (RegOpenKeyExA(
            HKEY_LOCAL_MACHINE,
            hardware_path,
            0,
            KEY_READ,
            &key) != ERROR_SUCCESS)
    {
        return error_set("Cannot Read CPU Name!\n");
    }
    if (RegQueryValueExA(
            key,
            "ProcessorNameString",
            NULL,
            NULL,
            (LPBYTE)name,
            &ssize) == ERROR_SUCCESS)
    {
        printf("CPU Name: %s\n", name);
    }
    else
    {
        printf("CPU Name: Unknown\n");
    }
    RegCloseKey(key);
    printf("CPU Architecture:");
    switch (sysInfo.wProcessorArchitecture)
    {
    case PROCESSOR_ARCHITECTURE_AMD64:
        printf("x86_64\n");
        break;
    case PROCESSOR_ARCHITECTURE_INTEL:
        printf("x86\n");
        break;
    case PROCESSOR_ARCHITECTURE_ARM:
        printf("ARM\n");
        break;
    case PROCESSOR_ARCHITECTURE_ARM64:
        printf("ARM64\n");
        break;
    default:
        printf("Unknown (%u)\n", sysInfo.wProcessorArchitecture);
        break;
    }
#endif
    return 1;
}

/*
==========

sh_help

			arguments:
            ARGS
            (NONE USED)
            
            details:
            NONE

			description:

			* it percur the struct cmds_help and print
            the name value and de desc value.
==========
*/

int sh_help(char **args)
{
    (void)args;
    int count = sizeof(cmds_help) / sizeof(cmds_help[0]);
    for (int i = 0; i < count; i++)
    {
        printf("\t%s %s\n", cmds_help[i].name, cmds_help[i].desc);
    }
    return 1;
}

/*
==========

read_line

			arguments:
            fp (FILE)
            
            details:
            NONE

			description:

			* it uses fgets to get the fist 256 line of fp (FILE)
            and put the result in the tmp.
            using the length of tmp it create an string newline and
            realloc using the: (size + length + 1).
            if newline is NULL it free line and end.
            else line point to newline.
            then it copy the 256st byte in tmp into the address
            of line + size, and set the last byte as '\0'.

            if there is an '\n' or the len of tmp is 0
            it break.

            it return line
==========
*/

char *read_line(FILE *fp)
{
    char tmp[256];
    char *line = NULL;
    size_t size = 0;

    while (fgets(tmp, sizeof(tmp), fp))
    {
        size_t len = strlen(tmp);
        char *newline = realloc(line, size + len + 1);
        if (!newline)
        {
            free(line);
            return NULL;
        }
        line = newline;

        memcpy(line + size, tmp, len);
        size += len;
        line[size] = '\0';

        if (len > 0 && tmp[len - 1] == '\n')
        {
            break;
        }
    }
    return line;
}

/*
==========

sh_soruce

			arguments:
			ARGS 

			details:
            * you can open a lot of files.
            * this functions read the file content, and using
            the commands used in ~[shell.c] for geting line,
            and execute.


			description:

			* for comment, use #, this function
            ignore '#' and '\0'.

==========
*/

int sh_source(char **args)
{
    if (!args[1])
    {
        return error_set("Missing File!\n");
    }
    static size_t loopcx = 0;
    for (int count = 1; args[count]; count++)
    {
        FILE *fp = fopen(args[count], "r");
        if (!fp)
        {
            return error_set("Cannot Open File!\n");
        }
        char *line;

        while ((line = read_line(fp)) != NULL)
        {
            if (loopcx++ >= 10)
            {
                printf("SAFETY! MAX LOOP LIMIT REACHED!\n");

                free(line);
                fclose(fp);

                return 1;
            }
            line[strcspn(line, "\n")] = 0;

            char *ptr = line;
            while (*ptr == ' ' || *ptr == '\t')
            {
                ptr++;
            }
            if (*ptr == '\0' || *ptr == '#')
            {
                free(line);
                continue;
            }

            char **cmd_args = sh_splitLine(ptr);
            int ret = sh_execute(cmd_args);
            free(cmd_args);
            free(line);

            if (ret != 1)
            {
                break;
            }
        }
        fclose(fp);
    }
    return 1;
}

/*
==========

funcNew

			arguments:
            NONE

            details:
            RETURN THE ADRESS OF FUNC_S (struct) INCREMENTED,
            BY FUNC_CX, EVERY TIME USED IT WILL INCREMENT MORE.



			description:

			* compare the number of functions (func_cx) and
            if the size is greater than the  (func_capacity),
            it will create ~[s_func *newfns] and realloc memory.

==========
*/

s_func *funcNew()
{
    if(func_cx >= func_capacity)
    {
        func_capacity = (func_capacity == 0) ? 4 : func_capacity * 2;

        s_func *newfns = realloc(func_s,sizeof(s_func) * func_capacity);
        if(!newfns)
        {
            printf("Cannot Realloc!\n");
            exit(1);
        }
        func_s = newfns;
    }
    return &func_s[func_cx++];
}

/*
==========

func_addLine

			arguments:
			FUNC (s_func)
            LINE (char)

			details:
            NONE


			description:

			* see if the function line count is greater than it own capacity,
            if it is, the capacity grows, if capacity is 0, capacity grows to
            4, if not, it is multiplied by two,
            it creates an array of strings (newlines) and realloc memory using
            the size of function capacity.
            then it chances the functions lines for the newlines.
            in the end, it percur the array, and set that part as the argument line

==========
*/

void func_addLine(s_func *func,char *line)
{
    if(func->line_count >= func->capacity)
    {
        func->capacity = (func->capacity == 0) ? 4 : func->capacity * 2;
        char **newlines = realloc(func->lines,sizeof(char*) * func->capacity);
        if(!newlines)
        {
            printf("Cannot Malloc!\n");
            exit(1);
        }
        func->lines = newlines;
    }
    func->lines[func->line_count++] = line;
}

/*
==========

sh_fundef

			arguments:
            ARGS

            details:
            END     | TERMINATE FUNCTION

            STRUCTURE:

            FUNC ~[NAME]
                CMD
            END

			description:

			* it create a new function, and reset it.
            * it create an string called line, and uses
            the function and uses the funcion ~[sh_getLine]
            in ~[parser.c]. 
            if line is null the loop break.
            if not, it checks if line is equal to "end",
            and if it is the line is free and the loop break.
            else it uses the ~[func_addLine]

==========
*/

int sh_fundef(char **args)
{
    if (!args[1])
    {
        return error_set("Missing Function Name!\n");
    }

    s_func *func = funcNew();
    func->name = strdup(args[1]);
    func->lines = NULL;
    func->line_count = 0;
    func->capacity  = 0;

    char *line;

    while (1)
    {
        line = sh_getLine();

        if (!line)
        {
            break;
        }
        if (strcmp(line, "end") == 0)
        {
            free(line);
            break;
        }
        func_addLine(func,line);
    }
    return 1;
}

/*
==========

funcNew

			arguments:
            name
            
            details:
            NONE



			description:

			* it goes into every function, searching by it name,
            if found it go trough every line and create an array
            of strings with the informations of the function line
            using the function ~[sh_splitLine].
            then it uses the function ~[sh_execute] and execute the
            strings.
            then, when it ends the execution the cmd is free,
            and return 1.
            else , if the function is not found, it return 0.

==========
*/

int exec_fun(char *name)
{
    for (int i = 0; i < func_cx; i++)
    {
        if(strcmp(func_s[i].name,name) == 0)
        {
            for(int j = 0;j < func_s[i].line_count;j++)
            {
                char **cmd = sh_splitLine(func_s[i].lines[j]);
                sh_execute(cmd);
                free(cmd);
            }
            return 1;
        }
    }
    return 0;
}

int sh_exit(char **args)
{
    (void)args;
    exit(0);
}
