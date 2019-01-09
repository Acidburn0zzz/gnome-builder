/* ide-gtk.h
 *
 * Copyright 2015-2019 Christian Hergert <chergert@redhat.com>
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

#include "ide-context.h"

#include "workbench/ide-workbench.h"

G_BEGIN_DECLS

typedef void (*IdeWidgetContextHandler) (GtkWidget  *widget,
                                         IdeContext *context);

IDE_AVAILABLE_IN_3_32
void          ide_widget_set_context_handler (gpointer                 widget,
                                              IdeWidgetContextHandler  handler);
IDE_AVAILABLE_IN_3_32
IdeContext   *ide_widget_get_context         (GtkWidget               *widget);
IDE_AVAILABLE_IN_3_32
IdeWorkbench *ide_widget_get_workbench       (GtkWidget               *widget);
IDE_AVAILABLE_IN_3_32
void          ide_widget_message             (gpointer                 instance,
                                              const gchar             *format,
                                              ...) G_GNUC_PRINTF (2, 3);
IDE_AVAILABLE_IN_3_32
void          ide_widget_warning             (gpointer                 instance,
                                              const gchar             *format,
                                              ...) G_GNUC_PRINTF (2, 3);
IDE_AVAILABLE_IN_3_32
gboolean      ide_gtk_show_uri_on_window     (GtkWindow               *window,
                                              const gchar             *uri,
                                              guint32                  timestamp,
                                              GError                 **error);

G_END_DECLS
