/* ide-layout-pane.h
 *
 * Copyright 2015-2019 Christian Hergert <christian@hergert.me>
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

#include <dazzle.h>

#include "ide-version-macros.h"

G_BEGIN_DECLS

#define IDE_TYPE_LAYOUT_PANE (ide_layout_pane_get_type())

IDE_AVAILABLE_IN_3_32
G_DECLARE_DERIVABLE_TYPE (IdeLayoutPane, ide_layout_pane, IDE, LAYOUT_PANE, DzlDockBinEdge)

struct _IdeLayoutPaneClass
{
  DzlDockBinEdgeClass parent_class;

  /*< private >*/
  gpointer _reserved[8];
};

G_END_DECLS
