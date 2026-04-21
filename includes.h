#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

/* WIN AND UNIX FUNCTIONS */
#include <dirent.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
/* UNIX */
#include <sys/stat.h>
#include <sys/types.h>

#ifndef _WIN32
#include <sys/wait.h> // i dont have this lib in my OS, so this is for prevent errors
#endif

/* WINDOWS*/
#include <aclapi.h>
#include <direct.h>
#include <sddl.h>
#include <windows.h>
