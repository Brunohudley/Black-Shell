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

#ifdef _WIN32
extern char *(*pcwd) (char *, int);
#else
extern char *(*pcwd) (char *, size_t);
#endif

#include "shell.h"

/* ==== Process Execution ==== */

int unix_proc (char **args);
int win_proc (char **args);

/* ==== Bultins ==== */

int sh_head (char **args);
int sh_exit (char **args);
int sh_cd (char **args);
int sh_whoami (char **args);
int sh_rm (char **args);
int sh_dirrm (char **args);
int sh_name (char **args);
int sh_chown (char **args);
int sh_cls (char **args);
int sh_pwd (char **args);
int nw_file (char **args);
int nw_dir (char **args);
int sh_copy (char **args);
int sh_ls (char **args);
int sh_chmod (char **args);
int sh_uname (char **args);
int sh_help (char **args);
int sh_source (char **args);
int exec_fun (char *name);
int sh_fundef (char **args);
