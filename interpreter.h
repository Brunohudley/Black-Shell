// ============================================================================
// Copyright (c) 2026 Brunohudley. All rights reserved.
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program. If not, see <https://gnu.org>.
// ============================================================================

#pragma once

#define MAXLOOPS 256

typedef enum CmpType
{
  CMP_LESS = -1,
  CMP_EQUAL,
  CMP_GREATER,
  CMP_NEQUAL,
  CMP_GEQUAL,
  CMP_LEQUAL
} CmpType;

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

typedef enum s_data_type
{
  INTEGER,
  FLOATING,
  LLUINT,
  STRING
} t_data_type;

int let_var (char **args);
int sh_print (char **args);
int add_var (char **args);
int sub_var (char **args);
int div_var (char **args);
int mul_var (char **args);
int cmp_var (char **args);
int cmd_do (char **args);
int sh_while (char **args);
