/* gbp-symbol-layout-stack-addin.c
 *
 * Copyright (C) 2017 Christian Hergert <chergert@redhat.com>
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

#define G_LOG_DOMAIN "gbp-symbol-layout-stack-addin"

#include "gbp-symbol-layout-stack-addin.h"
#include "gbp-symbol-menu-button.h"

struct _GbpSymbolLayoutStackAddin {
  GObject              parent_instance;
  GbpSymbolMenuButton *button;
  GCancellable        *cancellable;
};

static void
gbp_symbol_layout_stack_addin_load (IdeLayoutStackAddin *addin,
                                    IdeLayoutStack      *stack)
{
  GbpSymbolLayoutStackAddin *self = (GbpSymbolLayoutStackAddin *)addin;
  GtkWidget *header;

  g_assert (GBP_IS_SYMBOL_LAYOUT_STACK_ADDIN (self));
  g_assert (IDE_IS_LAYOUT_STACK (stack));

  header = ide_layout_stack_get_titlebar (stack);
  self->button = g_object_new (GBP_TYPE_SYMBOL_MENU_BUTTON, NULL);
  g_signal_connect (self->button,
                    "destroy",
                    G_CALLBACK (gtk_widget_destroyed),
                    &self->button);
  ide_layout_stack_header_add_custom_title (IDE_LAYOUT_STACK_HEADER (header),
                                            GTK_WIDGET (self->button),
                                            100);
}

static void
gbp_symbol_layout_stack_addin_unload (IdeLayoutStackAddin *addin,
                                      IdeLayoutStack      *stack)
{
  GbpSymbolLayoutStackAddin *self = (GbpSymbolLayoutStackAddin *)addin;

  g_assert (GBP_IS_SYMBOL_LAYOUT_STACK_ADDIN (self));
  g_assert (IDE_IS_LAYOUT_STACK (stack));

  g_cancellable_cancel (self->cancellable);
  g_clear_object (&self->cancellable);

  if (self->button != NULL)
    gtk_widget_destroy (GTK_WIDGET (self->button));
}

static void
gbp_symbol_layout_stack_addin_get_symbol_tree_cb (GObject      *object,
                                                  GAsyncResult *result,
                                                  gpointer      user_data)
{
  IdeSymbolResolver *symbol_resolver = (IdeSymbolResolver *)object;
  g_autoptr(GbpSymbolLayoutStackAddin) self = user_data;
  g_autoptr(IdeSymbolTree) tree = NULL;
  g_autoptr(GError) error = NULL;

  g_assert (GBP_IS_SYMBOL_LAYOUT_STACK_ADDIN (self));
  g_assert (G_IS_ASYNC_RESULT (result));
  g_assert (G_IS_ASYNC_RESULT (result));

  tree = ide_symbol_resolver_get_symbol_tree_finish (symbol_resolver, result, &error);

  if (error != NULL &&
      !g_error_matches (error, G_IO_ERROR, G_IO_ERROR_CANCELLED) &&
      !g_error_matches (error, G_IO_ERROR, G_IO_ERROR_NOT_SUPPORTED))
    g_warning ("%s", error->message);

  /* If we were destroyed, short-circuit */
  if (self->button == NULL)
    return;

  if (tree == NULL)
    {
      gtk_widget_hide (GTK_WIDGET (self->button));
      return;
    }

  gbp_symbol_menu_button_set_symbol_tree (self->button, tree);
}

static void
gbp_symbol_layout_stack_addin_set_view (IdeLayoutStackAddin *addin,
                                        IdeLayoutView       *view)
{
  GbpSymbolLayoutStackAddin *self = (GbpSymbolLayoutStackAddin *)addin;
  IdeSymbolResolver *symbol_resolver;
  IdeBuffer *buffer;
  IdeFile *file;

  g_assert (GBP_IS_SYMBOL_LAYOUT_STACK_ADDIN (self));
  g_assert (!view || IDE_IS_LAYOUT_VIEW (view));

  g_cancellable_cancel (self->cancellable);
  g_clear_object (&self->cancellable);

  gbp_symbol_menu_button_set_symbol_tree (self->button, NULL);

  if (!IDE_IS_EDITOR_VIEW (view))
    {
      gtk_widget_hide (GTK_WIDGET (self->button));
      return;
    }

  buffer = ide_editor_view_get_buffer (IDE_EDITOR_VIEW (view));
  g_assert (IDE_IS_BUFFER (buffer));

  symbol_resolver = ide_buffer_get_symbol_resolver (buffer);
  g_assert (!symbol_resolver || IDE_IS_SYMBOL_RESOLVER (symbol_resolver));

  if (symbol_resolver == NULL)
    {
      gtk_widget_hide (GTK_WIDGET (self->button));
      return;
    }

  file = ide_buffer_get_file (buffer);
  g_assert (IDE_IS_FILE (file));

  self->cancellable = g_cancellable_new ();

  ide_symbol_resolver_get_symbol_tree_async (symbol_resolver,
                                             ide_file_get_file (file),
                                             buffer,
                                             self->cancellable,
                                             gbp_symbol_layout_stack_addin_get_symbol_tree_cb,
                                             g_object_ref (self));

  gtk_widget_show (GTK_WIDGET (self->button));
}

static void
layout_stack_addin_iface_init (IdeLayoutStackAddinInterface *iface)
{
  iface->load = gbp_symbol_layout_stack_addin_load;
  iface->unload = gbp_symbol_layout_stack_addin_unload;
  iface->set_view = gbp_symbol_layout_stack_addin_set_view;
}

G_DEFINE_TYPE_WITH_CODE (GbpSymbolLayoutStackAddin,
                         gbp_symbol_layout_stack_addin,
                         G_TYPE_OBJECT,
                         G_IMPLEMENT_INTERFACE (IDE_TYPE_LAYOUT_STACK_ADDIN,
                                                layout_stack_addin_iface_init))

static void
gbp_symbol_layout_stack_addin_class_init (GbpSymbolLayoutStackAddinClass *klass)
{
}

static void
gbp_symbol_layout_stack_addin_init (GbpSymbolLayoutStackAddin *self)
{
}
