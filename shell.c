#include "includes.h"
#include "shell.h"
#include "parser.h"
#include "cfgfile.h"
#include "pcolors.h"


void setup_prompt()
{
	char *user;

    #ifdef _WIN32
        system("chcp 65001 > nul");
		user = getenv("USERNAME");
	#else
		user = getenv("USER");
    #endif

	printf(BOLD_BLUE "┌──" RESET "[" BOLD_GREEN "@UserUndefined" RESET "]" RESET "\n");
	printf(BOLD_BLUE "└─" BOLD_GREEN "❯ " RESET);
}

void sh_loop(void)
{	
	char *lines; //command
	char **args; //arguments in command
	int status;

	do
	{
		setup_prompt();

		lines = sh_getLine();
		args = sh_splitLine(lines);
		status = sh_execute(args);

		free(lines);
		free(args);	
	}
	while(status);
}
