#include "includes.h"
#include "shell.h"
#include "cfgfile.h"

int main(int argc,char **argv)
{
    // load Config files
    read_cfg("shell.cfg");
    
    //printf("MAXLOOP = %d\n", cfgProgram.maxloop_reach);

    // run command loop
	sh_loop();

    // peform shutdown / clean
	return 0;	
}
