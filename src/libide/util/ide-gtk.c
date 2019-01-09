/* ide-gtk.c
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
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#define G_LOG_DOMAIN "ide-gtk"

#include "config.h"

#include <gtk/gtk.h>

#include "application/ide-application.h"
#include "subprocess/ide-subprocess.h"
#include "subprocess/ide-subprocess-launcher.h"
#include "util/ide-flatpak.h"
#include "util/ide-gtk.h"

static GQuark quark_handler;
static GQuark quark_where_context_was;

static void
ide_widget_notify_context (GtkWidget  *toplevel,
                           GParamSpec *pspec,
                           GtkWidget  *widget)
{
  IdeWidgetContextHandler handler;
  IdeContext *old_context;
  IdeContext *context;

  g_assert (GTK_IS_WIDGET (toplevel));
  g_assert (GTK_IS_WIDGET (widget));

  handler = g_object_get_qdata (G_OBJECT (widget), quark_handler);
  old_context = g_object_get_qdata (G_OBJECT (widget), quark_where_context_was);

  if (handler == NULL)
    return;

  context = ide_widget_get_context (toplevel);

  if (context == old_context)
    return;

  g_object_set_qdata (G_OBJECT (widget), quark_where_context_was, context);

  handler (widget, context);
}

static gboolean
has_context_property (GtkWidget *widget)
{
  GParamSpec *pspec;

  g_assert (GTK_IS_WIDGET (widget));

  pspec = g_object_class_find_property (G_OBJECT_GET_CLASS (widget), "context");
  return pspec != NULL && g_type_is_a (pspec->value_type, IDE_TYPE_CONTEXT);
}

static void
ide_widget_hierarchy_changed (GtkWidget *widget,
                              GtkWidget *previous_toplevel,
                              gpointer   user_data)
{
  GtkWidget *toplevel;

  g_assert (GTK_IS_WIDGET (widget));

  if (GTK_IS_WINDOW (previous_toplevel))
    g_signal_handlers_disconnect_by_func (previous_toplevel,
                                          G_CALLBACK (ide_widget_notify_context),
                                          widget);

  toplevel = gtk_widget_get_toplevel (widget);

  if (GTK_IS_WINDOW (toplevel) && has_context_property (toplevel))
    {
      g_signal_connect_object (toplevel,
                               "notify::context",
                               G_CALLBACK (ide_widget_notify_context),
                               widget,
                               0);
      ide_widget_notify_context (toplevel, NULL, widget);
    }
}

/**
 * ide_widget_set_context_handler:
 * @widget: (type Gtk.Widget): a #GtkWidget
 * @handler: (scope async): A callback to handle the context
 *
 * Calls @handler when the #IdeContext has been set for @widget.
 *
 * Since: 3.32
 */
void
ide_widget_set_context_handler (gpointer                widget,
                                IdeWidgetContextHandler handler)
{
  GtkWidget *toplevel;

  g_return_if_fail (GTK_IS_WIDGET (widget));

  /* Ensure we have our quarks for quick key lookup */
  if G_UNLIKELY (quark_handler == 0)
    quark_handler = g_quark_from_static_string ("IDE_CONTEXT_HANDLER");
  if G_UNLIKELY (quark_where_context_was == 0)
    quark_where_context_was = g_quark_from_static_string ("IDE_CONTEXT");

  g_object_set_qdata (G_OBJECT (widget), quark_handler, handler);

  g_signal_connect (widget,
                    "hierarchy-changed",
                    G_CALLBACK (ide_widget_hierarchy_changed),
                    NULL);

  toplevel = gtk_widget_get_toplevel (widget);

  if (GTK_IS_WINDOW (toplevel))
    ide_widget_hierarchy_changed (widget, NULL, NULL);
}

/**
 * ide_widget_get_workbench:
 *
 * Gets the workbench @widget is associated with, if any.
 *
 * If no workbench is associated, NULL is returned.
 *
 * Returns: (transfer none) (nullable): An #IdeWorkbench
 *
 * Since: 3.32
 */
IdeWorkbench *
ide_widget_get_workbench (GtkWidget *widget)
{
  GtkWidget *ancestor;

  g_return_val_if_fail (GTK_IS_WIDGET (widget), NULL);

  if (IDE_IS_WORKBENCH (widget))
    return IDE_WORKBENCH (widget);

  ancestor = gtk_widget_get_ancestor (widget, IDE_TYPE_WORKBENCH);
  if (IDE_IS_WORKBENCH (ancestor))
    return IDE_WORKBENCH (ancestor);

  /*
   * TODO: Add "IDE_WORKBENCH" gdata for popout windows.
   */

  return NULL;
}

/**
 * ide_widget_get_context: (skip)
 *
 * Returns: (nullable) (transfer none): An #IdeContext or %NULL.
 *
 * Since: 3.32
 */
IdeContext *
ide_widget_get_context (GtkWidget *widget)
{
  IdeWorkbench *workbench;

  g_return_val_if_fail (GTK_IS_WIDGET (widget), NULL);

  workbench = ide_widget_get_workbench (widget);

  if (workbench == NULL)
    return NULL;

  return ide_workbench_get_context (workbench);
}

void
ide_widget_message (gpointer     instance,
                    const gchar *format,
                    ...)
{
  g_autofree gchar *str = NULL;
  IdeContext *context = NULL;
  va_list args;

  g_return_if_fail (IDE_IS_MAIN_THREAD ());
  g_return_if_fail (!instance || GTK_IS_WIDGET (instance));

  va_start (args, format);
  str = g_strdup_vprintf (format, args);
  va_end (args);

  if (instance != NULL)
    context = ide_widget_get_context (instance);

  if (context != NULL)
    ide_context_emit_log (context, G_LOG_LEVEL_MESSAGE, str, -1);
  else
    g_message ("%s", str);
}

void
ide_widget_warning (gpointer     instance,
                    const gchar *format,
                    ...)
{
  g_autofree gchar *str = NULL;
  IdeContext *context = NULL;
  va_list args;

  g_return_if_fail (IDE_IS_MAIN_THREAD ());
  g_return_if_fail (!instance || GTK_IS_WIDGET (instance));

  va_start (args, format);
  str = g_strdup_vprintf (format, args);
  va_end (args);

  if (instance != NULL)
    context = ide_widget_get_context (instance);

  if (context != NULL)
    ide_context_emit_log (context, G_LOG_LEVEL_WARNING, str, -1);
  else
    g_warning ("%s", str);
}

gboolean
ide_gtk_show_uri_on_window (GtkWindow    *window,
                            const gchar  *uri,
                            guint32       timestamp,
                            GError      **error)
{
  g_return_val_if_fail (!window || GTK_IS_WINDOW (window), FALSE);
  g_return_val_if_fail (uri != NULL, FALSE);

  if (ide_is_flatpak ())
    {
      g_autoptr(IdeSubprocessLauncher) launcher = NULL;
      g_autoptr(IdeSubprocess) subprocess = NULL;

      /* We can't currently trust gtk_show_uri_on_window() because it tries
       * to open our HTML page with Builder inside our current flatpak
       * environment! We need to ensure this is fixed upstream, but it's
       * currently unclear how to do so since we register handles for html.
       */

      launcher = ide_subprocess_launcher_new (0);
      ide_subprocess_launcher_set_run_on_host (launcher, TRUE);
      ide_subprocess_launcher_set_clear_env (launcher, FALSE);
      ide_subprocess_launcher_push_argv (launcher, "xdg-open");
      ide_subprocess_launcher_push_argv (launcher, uri);

      if (!(subprocess = ide_subprocess_launcher_spawn (launcher, NULL, error)))
        return FALSE;
    }
  else
    {
      if (!gtk_show_uri_on_window (window, uri, gtk_get_current_event_time (), error))
        return FALSE;
    }

  return TRUE;
}
