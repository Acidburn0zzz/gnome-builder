/* main.c
 *
 * Copyright © 2014-2019 Christian Hergert <christian@hergert.me>
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

#define G_LOG_DOMAIN "builder"

#include <ide.h>
#include <glib/gi18n.h>
#include <gtksourceview/gtksource.h>
#include <stdlib.h>

#include "plugins/gnome-builder-plugins.h"

#include "bug-buddy.h"

static IdeApplicationMode early_mode;

static gboolean
verbose_cb (const gchar  *option_name,
            const gchar  *value,
            gpointer      data,
            GError      **error)
{
  ide_log_increase_verbosity ();
  return TRUE;
}

static void
early_params_check (gint    *argc,
                    gchar ***argv)
{
  g_autofree gchar *type = NULL;
  g_autoptr(GOptionContext) context = NULL;
  GOptionEntry entries[] = {
    { "verbose", 'v', G_OPTION_FLAG_NO_ARG, G_OPTION_ARG_CALLBACK, verbose_cb },
    { "type", 0, 0, G_OPTION_ARG_STRING, &type },
    { NULL }
  };

  context = g_option_context_new (NULL);
  g_option_context_set_ignore_unknown_options (context, TRUE);
  g_option_context_set_help_enabled (context, FALSE);
  g_option_context_add_main_entries (context, entries, NULL);
  g_option_context_parse (context, argc, argv, NULL);

  if (g_strcmp0 (type, "worker") == 0)
    early_mode = IDE_APPLICATION_MODE_WORKER;
  else if (g_strcmp0 (type, "cli") == 0)
    early_mode = IDE_APPLICATION_MODE_TOOL;
}

int
main (int   argc,
      char *argv[])
{
  IdeApplication *app;
  const gchar *desktop;
  int ret;

  /* Setup our gdb fork()/exec() helper */
  bug_buddy_init ();

  /* Always ignore SIGPIPE */
  signal (SIGPIPE, SIG_IGN);

  /*
   * We require a desktop session that provides a properly working
   * DBus environment. Bail if for some reason that is not the case.
   */
  if (g_getenv ("DBUS_SESSION_BUS_ADDRESS") == NULL)
    {
      g_printerr (_("GNOME Builder requires a desktop session with D-Bus. Please set DBUS_SESSION_BUS_ADDRESS."));
      return EXIT_FAILURE;
    }

  /* Early init of logging so that we get messages in a consistent
   * format. If we deferred this to GApplication, we'd get them in
   * multiple formats.
   */
  ide_log_init (TRUE, NULL);

  /* Extract options like -vvvv and --type=worker only */
  early_params_check (&argc, &argv);

  /* Log what desktop is being used to simplify tracking down
   * quirks in the future.
   */
  desktop = g_getenv ("XDG_CURRENT_DESKTOP");
  if (desktop == NULL)
    desktop = "unknown";

  g_message ("Initializing with %s desktop and GTK+ %d.%d.%d.",
             desktop,
             gtk_get_major_version (),
             gtk_get_minor_version (),
             gtk_get_micro_version ());

  /* Setup the application instance */
  app = ide_application_new (early_mode);

  /* Ensure that our static plugins init routine is called.
   * This is necessary to ensure that -Wl,--as-needed does not
   * drop our link to this shared library.
   */
  gnome_builder_plugins_init ();

  /* Block until the application exits */
  ret = g_application_run (G_APPLICATION (app), argc, argv);

  /* Force disposal of the application (to help catch cleanup
   * issues at shutdown) and then (hopefully) finalize the app.
   */
  g_object_run_dispose (G_OBJECT (app));
  g_clear_object (&app);

  /* Cleanup logging and flush anything that still needs it */
  ide_log_shutdown ();

  return ret;
}
