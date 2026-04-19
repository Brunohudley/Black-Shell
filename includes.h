#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

/* WIN AND UNIX FUNCTIONS */
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <dirent.h>
#include <time.h>
/* UNIX */
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/types.h>

#ifndef _WIN32
    #include <sys/wait.h> // i dont have this lib in my OS, so this is for prevent errors
#endif

/* WINDOWS*/
#include <windows.h>
#include <direct.h>
#include <aclapi.h>
#include <sddl.h>
