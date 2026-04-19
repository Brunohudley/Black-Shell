#include "includes.h"
#include "parser.h"
#include "interpreter.h"
#include "commands.h"
#include "utils.h"

#define MAX_BUFFSIZE 1024
#define TOK_BUFFSIZE 64
#define TOK_DELIM " \t\r\n\a"

char *builtin_str[];
int (*builtin_func[])(char **args);
int numofbuiltin(void);

char *sh_getLine(void)
{
	int buffsize = MAX_BUFFSIZE;
	int pos = 0;
	char *buffer = malloc(sizeof(char) * buffsize);

	int c;

	if (!buffer)
	{
		printf("Cannot Allocate Memory!\n");
		exit(1);
	}

	for (;; pos++)
	{
		c = getchar();

		if (c == EOF || c == '\n')
		{
			buffer[pos] = '\0';
			return buffer;
		}
		else
		{
			buffer[pos] = c;
		}

		if (pos >= buffsize)
		{
			buffsize += MAX_BUFFSIZE;
			buffer = realloc(buffer, buffsize);
			if (!buffer)
			{
				printf("Cannot Reallocate Memory!\n");
				exit(1);
			}
		}
	}
}

char **sh_splitLine(char *line)
{
	int buffsize = TOK_BUFFSIZE;
	int pos = 0;

	char **tokens = malloc(buffsize * sizeof(char *));
	char *token;

	if (!tokens)
	{
		printf("Cannot Allocate Memory!\n");
		exit(1);
	}

	token = strtok(line, TOK_DELIM);

	for (; token != NULL; pos++)
	{
		tokens[pos] = token;

		if (pos >= buffsize)
		{
			buffsize += TOK_BUFFSIZE;
			tokens = realloc(tokens, buffsize * sizeof(char *));
			if (!tokens)
			{
				printf("Cannot Reallocate Memory!\n");
				exit(1);
			}
		}
		token = strtok(NULL, TOK_DELIM);
	}
	tokens[pos] = NULL;
	return tokens;
}

int sh_execute(char **args)
{
	if (args[0] == NULL)
	{
		return 1;
	}

	for (int i = 0; i < numofbuiltin(); i++)
	{
		if (strcmp(args[0], builtin_str[i]) == 0)
		{
			return builtin_func[i](args);
		}
	}
	if (exec_fun(args[0]))
	{
		return 1;
	}

	if (strcmp(args[0], "func") == 0)
	{
		return sh_fundef(args);
	}

#ifdef _WIN32
	if (win_proc(args) != 1)
	{
		return error_set("Cannot Execute Command!\n");
	}
#else
	if (unix_proc(args) != 1)
	{
		return error_set("Cannot Execute Command!\n");
	}
#endif
	return 1;
}

char *builtin_str[] =
	{
		"head",
		"cd",
		"whoami",
		"rm",
		"rmdir",
		"name",
		"chown",
		"clear",
		"pwd",
		"mk",
		"mkdir",
		"copy",
		"ls",
		"chmod",
		"uname",
		"print",
		"let",
		"add",
		"sub",
		"div",
		"mul",
		"do",
		"while",
		"help",
		"source",
		"cmp",
		"exit"};

int (*builtin_func[])(char **args) =
	{
		&sh_head,
		&sh_cd,
		&sh_whoami,
		&sh_rm,
		&sh_dirrm,
		&sh_name,
		&sh_chown,
		&sh_cls,
		&sh_pwd,
		&nw_file,
		&nw_dir,
		&sh_copy,
		&sh_ls,
		&sh_chmod,
		&sh_uname,
		&sh_print,
		&let_var,
		&add_var,
		&sub_var,
		&div_var,
		&mul_var,
		&cmd_do,
		&sh_while,
		&sh_help,
		&sh_source,
		&cmp_var,
		&sh_exit};

int numofbuiltin(void)
{
	return sizeof(builtin_str) / sizeof(char *);
}