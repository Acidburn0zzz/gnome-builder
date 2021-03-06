/* ide-terminal-workspace.h
 *
 * Copyright 2018-2019 Christian Hergert <chergert@redhat.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#pragma once

#if !defined (IDE_TERMINAL_INSIDE) && !defined (IDE_TERMINAL_COMPILATION)
# error "Only <libide-terminal.h> can be included directly."
#endif

#include <libide-core.h>
#include <libide-gui.h>

G_BEGIN_DECLS

#define IDE_TYPE_TERMINAL_WORKSPACE (ide_terminal_workspace_get_type())

IDE_AVAILABLE_IN_3_32
G_DECLARE_FINAL_TYPE (IdeTerminalWorkspace, ide_terminal_workspace, IDE, TERMINAL_WORKSPACE, IdeWorkspace)

IDE_AVAILABLE_IN_3_34
IdeTerminalWorkspace *ide_terminal_workspace_new (IdeApplication *application);

G_END_DECLS
