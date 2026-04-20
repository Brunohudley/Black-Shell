#include "cfgfile.h"

#define READ_CFG_MAX 1024

cfgEntry configtable[] =
    {
        {&cfgProgram.maxloop_reach, "MAXLOOP_REACH:", INTEGER},
    };

#define CFGTABLE_SIZE (sizeof(configtable) / sizeof(configtable[0]))


int read_cfg(char *cfg_path)
{
    FILE *fp = fopen(cfg_path, "r");

    if (!fp)
    {
        fprintf(stderr, "Cannot Open File %s!\n", cfg_path);
        return -1;
    }

    char *read_str;
    char *cmd_name;

    while (fscanf(fp, "%ms %ms", cmd_name, read_str) == 2)
    {
        for (int i = 0; i < CFGTABLE_SIZE; i++)
        {
            if (strcmp(cmd_name, configtable[i].key) == 0)
            {
                switch (configtable[i].data_type)
                {
                case INTEGER:
                    *((int *)configtable[i].ptr) = atoi(read_str);
                    break;
                case FLOATING:
                    *((float *)configtable[i].ptr) = atof(read_str);
                    break;
                case STRING:
                    strcpy((char *)configtable[i].ptr, read_str);
                    break;
                default:
                    break;
                }
                break;
            }
        }
        free(cmd_name);
        free(read_str);
        cmd_name = NULL;
        read_str = NULL;
    }
    if(fp)
    {
        fclose(fp);
    }
    return 1;
}