#include "interpreter.h"
#include "includes.h"
#include "parser.h"
#include "cfgfile.h"
#include "utils.h"

#define LOOP_EXEC_ARG 64

Var vars[MAXVAR];
int var_count = 0;
int cmp_flag = 0;

/*
==========

get_var		arguments:
			NAME
			EXPR

			details:
			LET "-i"		| return a pointer of the integer value of vars.data.
			LET "-f"		| return a pointer of the float value of vars.data.
			LET "-s" 		| return a pointer of the string value of vars.data.

			description:

			* NONE

==========
*/

void *get_var(const char *name, const char *expr)
{
	for (int i = 0; i < var_count; i++)
	{
		if (strcmp(vars[i].name, name) == 0)
		{
			if (strcmp(expr, "-i") == 0)
			{
				return &vars[i].data.int_value;
			}
			if (strcmp(expr, "-f") == 0)
			{
				return &vars[i].data.flt_value;
			}
			if (strcmp(expr, "-s") == 0)
			{
				return &vars[i].data.str_value;
			}
		}
	}
	return NULL;
}

/*
==========

set_var		arguments:
			EXPR
			NAME
			VALUE

			details:
			EXPR "-i"		| set vars.data integer value into the type casted var of value.
			EXPR "-f"		| set vars.data float 	value into the type casted var of value.
			EXPR "-s" 		| set vars.data string 	value into the type casted var of value.

			description:

			* NONE

==========
*/

void set_var(const char *expr, const char *name, void *value)
{
	for (int i = 0; i < var_count; i++)
	{
		if (strcmp(vars[i].name, name) == 0)
		{
			if (strcmp(expr, "-i") == 0)
			{
				int ivalue = *(int *)value;
				vars[i].data.int_value = ivalue;
			}
			if (strcmp(expr, "-f") == 0)
			{
				float fvalue = *(float *)value;
				vars[i].data.flt_value = fvalue;
			}
			if (strcmp(expr, "-s") == 0)
			{
				char *svalue = (char *)value;
				strncpy(vars[i].data.str_value, svalue, sizeof(vars[i].data.str_value));
			}
			return;
		}
	}

	if (var_count >= MAXVAR)
	{
		printf("Max variables reached!\n");
		return;
	}

	strncpy(vars[var_count].name, name, sizeof(vars[var_count].name) - 1);
	vars[var_count].name[sizeof(vars[var_count].name) - 1] = '\0';

	if (strcmp(expr, "-i") == 0)
	{
		vars[var_count].data.int_value = *(int *)value;
	}
		
	else if (strcmp(expr, "-f") == 0)
	{
		vars[var_count].data.flt_value = *(float *)value;
	}
		
	else if (strcmp(expr, "-s") == 0)
	{
		strncpy(vars[var_count].data.str_value, (char *)value,sizeof(vars[var_count].data.str_value));
	}			

	var_count++;
}

int let_var(char **args)
{
	if (!args[1] || !args[2] || !args[3])
	{
		return error_set("USAGE: <LET> EXPR NAME VALUE\n");
	}

	if (strcmp(args[1], "-i") == 0)
	{
		int val = atoi(args[3]);
		set_var("-i", args[2], &val);
		return 1;
	}

	if (strcmp(args[1], "-f") == 0)
	{
		float val = atof(args[3]);
		set_var("-f", args[2], &val);
		return 1;
	}

	if (strcmp(args[1], "-s") == 0)
	{
		set_var("-s", args[2], args[3]);
		return 1;
	}

	return error_set("INVALID TYPE! USE -i -f -s\n");
}

/*
==========

sh_print	arguments:
			ARGS / EXPR
				 / VAR

			details:
			EXPR -i | result = INTEGER
			EXPR -f | result = FLOATING
			EXPR -s | result = STRING

			description:
			print variables value

==========
*/

int sh_print(char **args)
{
	if (!args[1])
	{
		return error_set("PRINT: missing args\n");
	}

	/* print literal */
	if (!args[2])
	{
		for (int i = 1; args[i]; i++)
			printf("%s ", args[i]);
		printf("\n");
		return 1;
	}

	void *res1 = get_var(args[2], args[1]);
	if (!res1)
	{
		return error_set("PRINT: var not found\n");
	}

	if (strcmp(args[1], "-i") == 0)
	{
		printf("%d\n", *(int *)res1);
	}

	else if (strcmp(args[1], "-f") == 0)
	{
		printf("%f\n", *(float *)res1);
	}

	else if (strcmp(args[1], "-s") == 0)
	{
		printf("%s\n", (char *)res1);
	}

	return 1;
}

/*
==========

add_var		arguments:
			ARGS | EXPR
				 | DEST
				 | SRC

			details:
			EXPR "-i"	| INTEGER
			EXPR "-f"	| FLOAT
			EXPR "-s"	| STRING

			VAR1 += VAR2

			description:

			* add var1 with var2 and set the result in var1

==========
*/

int add_var(char **args)
{
	if (!args[1] || !args[2] || !args[3])
		return error_set("USAGE: <ADD> EXPR DEST SRC\n");

	void *res1 = get_var(args[2], args[1]);
	void *res2 = get_var(args[3], args[1]);

	if (!res1 || !res2)
		return 1;

	if (strcmp(args[1], "-i") == 0)
	{
		int var1 = *(int *)res1;
		int var2 = *(int *)res2;

		var1 += var2;
		set_var(args[1], args[2], &var1);
	}
	else if (strcmp(args[1], "-f") == 0)
	{
		float var1 = *(float *)res1;
		float var2 = *(float *)res2;

		var1 += var2;
		set_var(args[1], args[2], &var1);
	}

	return 1;
}

/*
==========

sub_var		arguments:
			ARGS | EXPR
				 | DEST
				 | SRC

			details:
			EXPR "-i"	| INTEGER
			EXPR "-f"	| FLOAT
			EXPR "-s"	| STRING

			VAR1 -= VAR2

			description:

			* subtract var1 with var2 and set the result in var1

==========
*/

int sub_var(char **args)
{
	if (!args[1] || !args[2] || !args[3])
		return error_set("USAGE: <ADD> EXPR DEST SRC\n");

	void *res1 = get_var(args[2], args[1]);
	void *res2 = get_var(args[3], args[1]);

	if (!res1 || !res2)
		return 1;

	if (strcmp(args[1], "-i") == 0)
	{
		int var1 = *(int *)res1;
		int var2 = *(int *)res2;

		var1 -= var2;
		set_var(args[1], args[2], &var1);
	}
	else if (strcmp(args[1], "-f") == 0)
	{
		float var1 = *(float *)res1;
		float var2 = *(float *)res2;

		var1 -= var2;
		set_var(args[1], args[2], &var1);
	}

	return 1;
}

/*
==========

div_var		arguments:
			ARGS | EXPR
				 | DEST
				 | SRC

			details:
			EXPR "-i"	| INTEGER
			EXPR "-f"	| FLOAT
			EXPR "-s"	| STRING

			VAR1 /= VAR2

			description:

			* div var1 with var2 and set the result in var1

==========
*/

int div_var(char **args)
{
	if (!args[1] || !args[2] || !args[3])
		return error_set("USAGE: <ADD> EXPR DEST SRC\n");

	void *res1 = get_var(args[2], args[1]);
	void *res2 = get_var(args[3], args[1]);

	if (!res1 || !res2)
		return 1;

	if (strcmp(args[1], "-i") == 0)
	{
		int var1 = *(int *)res1;
		int var2 = *(int *)res2;

		if (var2 == 0)
			return error_set("DIV BY ZERO\n");

		var1 /= var2;
		set_var(args[1], args[2], &var1);
	}
	else if (strcmp(args[1], "-f") == 0)
	{
		float var1 = *(float *)res1;
		float var2 = *(float *)res2;

		var1 /= var2;
		set_var(args[1], args[2], &var1);
	}

	return 1;
}

/*
==========

mul_var		arguments:
			ARGS | EXPR
				 | DEST
				 | SRC

			details:
			EXPR "-i"	| INTEGER
			EXPR "-f"	| FLOAT
			EXPR "-s"	| STRING

			VAR1 *= VAR2

			description:

			* mul var1 with var2 and set the result in var1

==========
*/

int mul_var(char **args)
{
	if (!args[1] || !args[2] || !args[3])
		return error_set("USAGE: <ADD> EXPR DEST SRC\n");

	void *res1 = get_var(args[2], args[1]);
	void *res2 = get_var(args[3], args[1]);

	if (!res1 || !res2)
		return 1;

	if (strcmp(args[1], "-i") == 0)
	{
		int var1 = *(int *)res1;
		int var2 = *(int *)res2;

		var1 *= var2;
		set_var(args[1], args[2], &var1);
	}
	else if (strcmp(args[1], "-f") == 0)
	{
		float var1 = *(float *)res1;
		float var2 = *(float *)res2;

		var1 *= var2;
		set_var(args[1], args[2], &var1);
	}

	return 1;
}

/*
==========

get_cmp_type		
			arguments:
			EXPR


			details:
			EXPR "-e"	| CMP_EQUAL
			EXPR "-g"	| CMP_GREATER
			EXPR "-l"	| CMP_LESS
			EXPR "-ge"	| CMP_GEQUAL
			EXPR "-le"	| CMP_LEQUAL
			EXPR "-n"	| CMP_LEQUAL

			description:

			* return value based on expr

==========
*/

int get_cmp_type(char *expr)
{
	if (!expr)
		return 999;

	if (strcmp(expr, "-e") == 0)
	{
		return CMP_EQUAL;
	}
	if (strcmp(expr, "-g") == 0)
	{
		return CMP_GREATER;
	}
	if (strcmp(expr, "-l") == 0)
	{
		return CMP_LESS;
	}
	if (strcmp(expr, "-n") == 0)
	{
		return CMP_NEQUAL;
	}
	if (strcmp(expr, "-ge") == 0)
	{
		return CMP_GEQUAL;
	}
	if (strcmp(expr, "-le") == 0)
	{
		return CMP_LEQUAL;
	}

	return 999;
}

/*
==========

get_cmp_type		
			arguments:
			EXPR


			details:
			type 		| CMP_EQUAL
						| CMP_GREATER
						| CMP_LESS
						| CMP_GEQUAL
						| CMP_LEQUAL
						| CMP_LEQUAL

			description:

			* chance the cmp_flag value based on type value

==========
*/

int check_cmp_flag(int type)
{
	if (type == CMP_EQUAL)
		return cmp_flag == CMP_EQUAL;

	if (type == CMP_GREATER)
		return cmp_flag == CMP_GREATER;

	if (type == CMP_LESS)
		return cmp_flag == CMP_LESS;

	if (type == CMP_NEQUAL)
		return cmp_flag != CMP_EQUAL;

	if (type == CMP_GEQUAL)
		return (cmp_flag == CMP_GREATER || cmp_flag == CMP_EQUAL);

	if (type == CMP_LEQUAL)
		return (cmp_flag == CMP_LESS || cmp_flag == CMP_EQUAL);

	return 0;
}

/*
==========

cmp_var

			arguments:
			ARGS / EXPR
				 / VAR1
				 / VAR2

			details:
			CMP_LESS	| -1
			CMP_EQUAL	|  0
			CMP_GREATER |  1
			CMP_NEQUAL  |  2
			CMP_GEQUAL  |  3
			CMP_LEQUAL  |  4


			description:

			* change cmpflag based on result

==========
*/

int cmp_var(char **args)
{
	if (!args[1] || !args[2] || !args[3])
	{
		return error_set("USAGE: <CMP> EXPR ARG ARG!\n");
	}
	char *type = args[1];

	void *res1 = get_var(args[2], type);
	void *res2 = get_var(args[3], type);

	if (!res1 || !res2)
	{
		return error_set("CMP: INVALID USE!\n");
	}

	if (strcmp(type, "-i") == 0)
	{
		int a = *(int *)res1;
		int b = *(int *)res2;

		if (a == b)
		{
			cmp_flag = CMP_EQUAL;
		}
		else if (a > b)
		{
			cmp_flag = CMP_GREATER;
		}
		else
		{
			cmp_flag = CMP_LESS;
		}
	}

	else if (strcmp(type, "-f") == 0)
	{
		float a = *(float *)res1;
		float b = *(float *)res2;

		if (a == b)
		{
			cmp_flag = CMP_EQUAL;
		}
		else if (a > b)
		{
			cmp_flag = CMP_GREATER;
		}
		else
		{
			cmp_flag = CMP_LESS;
		}
	}

	else if (strcmp(type, "-s") == 0)
	{
		char *a = (char *)res1;
		char *b = (char *)res2;

		if (strcmp(a, b) == 0)
		{
			cmp_flag = CMP_EQUAL;
		}
		else
		{
			cmp_flag = CMP_NEQUAL;
		}
	}
	else
	{
		return error_set("CMP: unknown type\n");
	}

	return 1;
}

/*
==========

cmp_do

			arguments:
			ARGS / EXPR
				 / DO

			details:
			CMP_LESS	| -1
			CMP_EQUAL	|  0
			CMP_GREATER |  1
			CMP_NEQUAL  |  2
			CMP_GEQUAL  |  3
			CMP_LEQUAL  |  4

			INVALID		| 999


			description:
			
			* do if condition is met.

==========
*/

int cmd_do(char **args)
{
	if (!args[0] || !args[1])
	{
		return error_set("USAGE: <DO> EXPR CMD!\n");
	}

	int type = get_cmp_type(args[1]);

	if (type == 999)
	{
		return error_set("INVALID EXPR!\n");
	}

	if (!check_cmp_flag(type))
	{
		return 1;
	}

	return sh_execute(&args[2]);
}

int evaluate_expr(char **expr)
{
	if (!expr[0])
		return 0;

	if (strcmp(expr[0], "-i") == 0 ||
		strcmp(expr[0], "-f") == 0 ||
		strcmp(expr[0], "-s") == 0)
	{
		cmp_var(expr);
		return check_cmp_flag(CMP_LESS) || 
		       check_cmp_flag(CMP_EQUAL) || 
		       check_cmp_flag(CMP_GREATER);
	}

	return 0;
}

/*
==========

cmp_do

			arguments:
			ARGS  

			details:
			NONE

			description:

			* execute command based on args, the "," delimits the end of one command
			  and allow you to use another command.

==========
*/

int loop_exec(char **args)
{
	
	
	for (int i = 0;args[i];)
	{
		int buffsize = LOOP_EXEC_ARG;
		int pos = 0;

		char **cmd = malloc(sizeof(char *) * buffsize);
		if (!cmd)
		{
			printf("ALLOC ERROR\n");
			exit(1);
		}

		for (;args[i] && strcmp(args[i], ",") != 0;)
		{
			if (pos >= buffsize - 1)
			{
				buffsize *= 2;
				char **tmp = realloc(cmd, sizeof(char *) * buffsize);
				if (!tmp)
				{
					free(cmd);
					printf("REALLOC ERROR\n");
					exit(1);
				}
				cmd = tmp;
			}

			cmd[pos++] = args[i++];
		}

		cmd[pos] = NULL;

		if (pos > 0)
		{
			sh_execute(cmd);
		}

		free(cmd);

		if (args[i] && strcmp(args[i], ",") == 0)
		{
			i++;
		}
	}

	return 1;
}

int sh_while(char **args)
{
	if (!args[1] || !args[2] || !args[3] || !args[4])
		return error_set("USAGE: while -t a b -cond , cmds\n");

	int i;

	for (i = 1;args[i] && strcmp(args[i], ",") != 0;i++)

	if (!args[i])
	{
		return error_set("WHILE: missing ','\n");
	}

	args[i] = NULL;

	char **expr = &args[1];    
	char **cmds = &args[i + 1]; 

	if (!cmds[0])
	{
		return error_set("WHILE: missing commands\n");
	}
	size_t loopcx = 0;

	while (1)
	{
		if (cfgProgram.maxloop_reach != 0 &&
			loopcx++ >= cfgProgram.maxloop_reach)
		{
			printf("SAFETY LIMIT REACHED!\n");
			break;
		}

		int cond = get_cmp_type(expr[3]);
		if (cond == 999)
		{
			return error_set("WHILE: invalid condition\n");
		}

		char *cmp_args[5];
		cmp_args[0] = "cmp";
		cmp_args[1] = expr[0];
		cmp_args[2] = expr[1];
		cmp_args[3] = expr[2];
		cmp_args[4] = NULL;

		cmp_var(cmp_args);

		if (!check_cmp_flag(cond))
		{
			break;			
		}


		loop_exec(cmds);
	}

	return 1;
}