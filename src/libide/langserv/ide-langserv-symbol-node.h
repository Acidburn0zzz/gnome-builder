/* ide-langserv-symbol-node.h
 *
 * Copyright 2016-2019 Christian Hergert <chergert@redhat.com>
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
 */

#pragma once

#include "ide-version-macros.h"

#include "symbols/ide-symbol-node.h"

G_BEGIN_DECLS

#define IDE_TYPE_LANGSERV_SYMBOL_NODE (ide_langserv_symbol_node_get_type())

IDE_AVAILABLE_IN_3_32
G_DECLARE_FINAL_TYPE (IdeLangservSymbolNode, ide_langserv_symbol_node, IDE, LANGSERV_SYMBOL_NODE, IdeSymbolNode)

IDE_AVAILABLE_IN_3_32
const gchar *ide_langserv_symbol_node_get_parent_name (IdeLangservSymbolNode *self);
IDE_AVAILABLE_IN_3_32
gboolean     ide_langserv_symbol_node_is_parent_of    (IdeLangservSymbolNode *self,
                                                       IdeLangservSymbolNode *other);

G_END_DECLS
