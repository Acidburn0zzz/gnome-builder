/* ide-transfers-progress-icon.h
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

#include <gtk/gtk.h>

#include "ide-version-macros.h"

G_BEGIN_DECLS

#define IDE_TYPE_TRANSFERS_PROGRESS_ICON (ide_transfers_progress_icon_get_type())

IDE_AVAILABLE_IN_ALL
G_DECLARE_FINAL_TYPE (IdeTransfersProgressIcon, ide_transfers_progress_icon, IDE, TRANSFERS_PROGRESS_ICON, GtkDrawingArea)

IDE_AVAILABLE_IN_ALL
GtkWidget *ide_transfers_progress_icon_new          (void);
IDE_AVAILABLE_IN_ALL
gdouble    ide_transfers_progress_icon_get_progress (IdeTransfersProgressIcon *self);
IDE_AVAILABLE_IN_ALL
void       ide_transfers_progress_icon_set_progress (IdeTransfersProgressIcon *self,
                                                     gdouble                   progress);

G_END_DECLS
