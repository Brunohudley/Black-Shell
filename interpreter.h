#pragma once

#define MAXVAR 128
#define MAXLOOPS 256

typedef enum CmpType
{
	CMP_LESS = -1,
	CMP_EQUAL,
	CMP_GREATER,
	CMP_NEQUAL,
	CMP_GEQUAL,
	CMP_LEQUAL
}CmpType;

/*
==========

DataType

			details:
			INTEGER		| 0
			FLOATING	| 1
			STRING		| 2

			description:

			* Useful when comparing variable data.

==========
*/

typedef enum DataType
{
	INTEGER,
	FLOATING,
	STRING
}DataType;

/*
==========

VarData

			details:
			INT_VALUE
			FLT_VALUE
			STR_VALUE

			description:

			* stores data inside the variable.

==========
*/


typedef union VarData
{
	int 	int_value;
	float 	flt_value;
	char	str_value[256];
}VarData;

/*
==========

Var

			details:
			VARDATA
			NAME

			description:

			* stores an variable.

==========
*/

typedef struct Var
{
	VarData data;
	char 	name[64];
}Var;


extern 	Var 	vars[MAXVAR];
extern 	int 	var_count;
extern 	int 	cmp_flag;

int 	let_var			(char **args);
int 	sh_print		(char **args);
int 	add_var			(char **args);
int 	sub_var			(char **args);
int 	div_var			(char **args);
int 	mul_var			(char **args);
int 	cmp_var			(char **args);
int 	cmd_do			(char **args);
int 	sh_while		(char **args);


