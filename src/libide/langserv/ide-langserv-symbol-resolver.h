/* ide-langserv-symbol-resolver.h
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
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#pragma once

#include "ide-version-macros.h"

#include "ide-object.h"

#include "langserv/ide-langserv-client.h"
#include "symbols/ide-symbol-resolver.h"

G_BEGIN_DECLS

#define IDE_TYPE_LANGSERV_SYMBOL_RESOLVER (ide_langserv_symbol_resolver_get_type())

IDE_AVAILABLE_IN_3_32
G_DECLARE_DERIVABLE_TYPE (IdeLangservSymbolResolver, ide_langserv_symbol_resolver, IDE, LANGSERV_SYMBOL_RESOLVER, IdeObject)

struct _IdeLangservSymbolResolverClass
{
  IdeObjectClass parent_class;

  /*< private >*/
  gpointer _reserved1;
  gpointer _reserved2;
  gpointer _reserved3;
  gpointer _reserved4;
  gpointer _reserved5;
  gpointer _reserved6;
  gpointer _reserved7;
  gpointer _reserved8;
};

IDE_AVAILABLE_IN_3_32
IdeLangservClient *ide_langserv_symbol_resolver_get_client (IdeLangservSymbolResolver *self);
IDE_AVAILABLE_IN_3_32
void               ide_langserv_symbol_resolver_set_client (IdeLangservSymbolResolver *self,
                                                            IdeLangservClient         *client);

G_END_DECLS
