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

#include "../include/shell.h"
#include "../include/cfgfile.h"
#include "../include/includes.h"
#include "../include/parser.h"
#include "../include/pcolors.h"

void
setup_prompt ()
{
  char *user;

#ifdef _WIN32
  system ("chcp 65001 > nul");
  user = getenv ("USERNAME");
#else
  user = getenv ("USER");
#endif

  printf (BOLD_BLUE "┌──" RESET "[" BOLD_GREEN "%s" RESET "]" BOLD_CYAN
                    "{%d}" RESET "\n",
          user, shell.error);
  printf (BOLD_BLUE "└─" BOLD_GREEN "❯ " RESET);
}

void
sh_loop (void)
{
  char *lines; // command
  char **args; // arguments in command
  int status;

  shell.error = 0;

  do
    {
      setup_prompt ();

      lines = sh_getLine ();
      args = sh_splitLine (lines);
      status = sh_execute (args);

      free (lines);
      free (args);
    }
  while (status);
}
