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

#include "includes.h"
#include "interpreter.h"

typedef struct cfgEntry
{
  void *ptr;
  char *key;
  t_data_type data_type;
} cfgEntry;

typedef struct
{
  size_t maxloop_reach;
} Program_config;

extern Program_config cfgProgram;

int read_cfg (char *cfg_path);
