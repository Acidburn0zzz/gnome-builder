/* ide-workbench-header-bar.c
 *
 * Copyright 2015 Christian Hergert <chergert@redhat.com>
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

#define G_LOG_DOMAIN "ide-workbench-header-bar"

#include "config.h"

#include <dazzle.h>
#include <glib/gi18n.h>

#include "application/ide-application.h"
#include "runner/ide-run-button.h"
#include "search/ide-search-entry.h"
#include "util/ide-gtk.h"
#include "workbench/ide-perspective.h"
#include "workbench/ide-workbench.h"
#include "workbench/ide-workbench-private.h"
#include "workbench/ide-workbench-header-bar.h"

typedef struct
{
  GtkToggleButton    *fullscreen_button;
  GtkImage           *fullscreen_image;
  DzlShortcutTooltip *fullscreen_tooltip;
  GtkMenuButton      *menu_button;
  DzlShortcutTooltip *menu_tooltip;
  DzlPriorityBox     *right_box;
  DzlPriorityBox     *left_box;
  IdeOmniBar         *omni_bar;
  IdeSearchEntry     *search_entry;
  GtkBox             *primary;
} IdeWorkbenchHeaderBarPrivate;

static void buildable_iface_init (GtkBuildableIface *iface);

G_DEFINE_TYPE_EXTENDED (IdeWorkbenchHeaderBar, ide_workbench_header_bar, GTK_TYPE_HEADER_BAR, 0,
                        G_ADD_PRIVATE (IdeWorkbenchHeaderBar)
                        G_IMPLEMENT_INTERFACE (GTK_TYPE_BUILDABLE, buildable_iface_init))

GtkWidget *
ide_workbench_header_bar_new (void)
{
  return g_object_new (IDE_TYPE_WORKBENCH_HEADER_BAR, NULL);
}

static void
apply_quirks (IdeWorkbenchHeaderBar *self)
{
  IdeWorkbenchHeaderBarPrivate *priv = ide_workbench_header_bar_get_instance_private (self);
  const gchar *session;

  g_assert (IDE_IS_WORKBENCH_HEADER_BAR (self));

  /* Hide fullscreen on Pantheon, which adds it's own fullscreen button
   * without any app negotiation.
   */
  session = g_getenv ("DESKTOP_SESSION");
  if (dzl_str_equal0 (session, "pantheon"))
    gtk_widget_hide (GTK_WIDGET (priv->fullscreen_button));
}

static void
search_popover_position_func (DzlSuggestionEntry *entry,
                              GdkRectangle       *area,
                              gboolean           *is_absolute,
                              gpointer            user_data)
{
  gint new_width;

  g_assert (DZL_IS_SUGGESTION_ENTRY (entry));
  g_assert (area != NULL);
  g_assert (is_absolute != NULL);
  g_assert (user_data == NULL);

#define RIGHT_MARGIN 6

  /* We want the search area to be the right 2/5ths of the window, with a bit
   * of margin on the popover.
   */

  dzl_suggestion_entry_window_position_func (entry, area, is_absolute, NULL);

  new_width = (area->width * 2 / 5);
  area->x += area->width - new_width;
  area->width = new_width - RIGHT_MARGIN;
  area->y -= 3;

#undef RIGHT_MARGIN
}

static gboolean
query_fullscreen_tooltip_cb (IdeWorkbenchHeaderBar *self,
                             gint                   x,
			     gint                   y,
			     gboolean               keyboard_mode,
			     GtkTooltip            *tooltip,
			     GtkWidget             *widget)
{
  IdeWorkbenchHeaderBarPrivate *priv = ide_workbench_header_bar_get_instance_private (self);
  g_autoptr(GVariant) state = NULL;
  GtkWidget *window;

  g_assert (IDE_IS_WORKBENCH_HEADER_BAR (self));
  g_assert (GTK_IS_TOOLTIP (tooltip));
  g_assert (GTK_IS_WIDGET (widget));

  window = gtk_widget_get_toplevel (widget);

  state = g_action_group_get_action_state (G_ACTION_GROUP (window), "fullscreen");

  if (!g_variant_get_boolean (state))
    dzl_shortcut_tooltip_set_title (priv->fullscreen_tooltip, _("Display the window in full screen"));
  else
    dzl_shortcut_tooltip_set_title (priv->fullscreen_tooltip, _("Exit full screen"));

  return FALSE;
}

static void
ide_workbench_header_bar_class_init (IdeWorkbenchHeaderBarClass *klass)
{
  GtkWidgetClass *widget_class = GTK_WIDGET_CLASS (klass);

  gtk_widget_class_set_template_from_resource (widget_class, "/org/gnome/builder/ui/ide-workbench-header-bar.ui");
  gtk_widget_class_bind_template_child_private (widget_class, IdeWorkbenchHeaderBar, fullscreen_button);
  gtk_widget_class_bind_template_child_private (widget_class, IdeWorkbenchHeaderBar, fullscreen_image);
  gtk_widget_class_bind_template_child_private (widget_class, IdeWorkbenchHeaderBar, fullscreen_tooltip);
  gtk_widget_class_bind_template_child_private (widget_class, IdeWorkbenchHeaderBar, left_box);
  gtk_widget_class_bind_template_child_private (widget_class, IdeWorkbenchHeaderBar, menu_button);
  gtk_widget_class_bind_template_child_private (widget_class, IdeWorkbenchHeaderBar, menu_tooltip);
  gtk_widget_class_bind_template_child_private (widget_class, IdeWorkbenchHeaderBar, omni_bar);
  gtk_widget_class_bind_template_child_private (widget_class, IdeWorkbenchHeaderBar, primary);
  gtk_widget_class_bind_template_child_private (widget_class, IdeWorkbenchHeaderBar, right_box);
  gtk_widget_class_bind_template_child_private (widget_class, IdeWorkbenchHeaderBar, search_entry);

  g_type_ensure (IDE_TYPE_RUN_BUTTON);
  g_type_ensure (IDE_TYPE_SEARCH_ENTRY);
}

static void
ide_workbench_header_bar_init (IdeWorkbenchHeaderBar *self)
{
  IdeWorkbenchHeaderBarPrivate *priv = ide_workbench_header_bar_get_instance_private (self);

  gtk_widget_init_template (GTK_WIDGET (self));

  dzl_suggestion_entry_set_position_func (DZL_SUGGESTION_ENTRY (priv->search_entry),
                                          search_popover_position_func, NULL, NULL);

  g_signal_connect_object (priv->fullscreen_button,
                           "query-tooltip",
                           G_CALLBACK (query_fullscreen_tooltip_cb),
                           self,
			   G_CONNECT_SWAPPED);

  apply_quirks (self);
}

void
ide_workbench_header_bar_focus_search (IdeWorkbenchHeaderBar *self)
{
  IdeWorkbenchHeaderBarPrivate *priv = ide_workbench_header_bar_get_instance_private (self);

  g_return_if_fail (IDE_IS_WORKBENCH_HEADER_BAR (self));

  gtk_widget_grab_focus (GTK_WIDGET (priv->search_entry));
}

void
ide_workbench_header_bar_insert_left (IdeWorkbenchHeaderBar *self,
                                      GtkWidget             *widget,
                                      GtkPackType            pack_type,
                                      gint                   priority)
{
  IdeWorkbenchHeaderBarPrivate *priv = ide_workbench_header_bar_get_instance_private (self);

  g_return_if_fail (IDE_IS_WORKBENCH_HEADER_BAR (self));
  g_return_if_fail (GTK_IS_WIDGET (widget));
  g_return_if_fail (pack_type == GTK_PACK_START || pack_type == GTK_PACK_END);

  gtk_container_add_with_properties (GTK_CONTAINER (priv->left_box), widget,
                                     "pack-type", pack_type,
                                     "priority", priority,
                                     NULL);
}

/**
 * ide_workbench_header_bar_add_primary:
 * @self: a #IdeWorkbenchHeaderBar
 *
 * This will add @widget to the special box at the top left of the window next
 * to the perspective selector. This is a special location in that the spacing
 * is treated differently than other locations on the header bar.
 *
 * Since: 3.26
 */
void
ide_workbench_header_bar_add_primary (IdeWorkbenchHeaderBar *self,
                                      GtkWidget             *widget)
{
  IdeWorkbenchHeaderBarPrivate *priv = ide_workbench_header_bar_get_instance_private (self);

  g_return_if_fail (IDE_IS_WORKBENCH_HEADER_BAR (self));
  g_return_if_fail (GTK_IS_WIDGET (widget));

  gtk_container_add (GTK_CONTAINER (priv->primary), widget);
}

void
ide_workbench_header_bar_insert_right (IdeWorkbenchHeaderBar *self,
                                       GtkWidget             *widget,
                                       GtkPackType            pack_type,
                                       gint                   priority)
{
  IdeWorkbenchHeaderBarPrivate *priv = ide_workbench_header_bar_get_instance_private (self);

  g_return_if_fail (IDE_IS_WORKBENCH_HEADER_BAR (self));
  g_return_if_fail (GTK_IS_WIDGET (widget));
  g_return_if_fail (pack_type == GTK_PACK_START || pack_type == GTK_PACK_END);

  gtk_container_add_with_properties (GTK_CONTAINER (priv->right_box), widget,
                                     "pack-type", pack_type,
                                     "priority", priority,
                                     NULL);
}

static GObject *
ide_workbench_header_bar_get_internal_child (GtkBuildable *buildable,
                                             GtkBuilder   *builder,
                                             const gchar  *childname)
{
  IdeWorkbenchHeaderBar *self = (IdeWorkbenchHeaderBar *)buildable;
  IdeWorkbenchHeaderBarPrivate *priv = ide_workbench_header_bar_get_instance_private (self);

  g_assert (GTK_IS_BUILDABLE (buildable));
  g_assert (GTK_IS_BUILDER (builder));
  g_assert (childname != NULL);

  if (g_str_equal (childname, "left"))
    return G_OBJECT (priv->left_box);
  else if (g_str_equal (childname, "right"))
    return G_OBJECT (priv->right_box);
  else
    return NULL;
}

static void
ide_workbench_header_bar_add_child (GtkBuildable *buildable,
                                    GtkBuilder   *builder,
                                    GObject      *object,
                                    const gchar  *type)
{
  IdeWorkbenchHeaderBar *self = (IdeWorkbenchHeaderBar *)buildable;
  GtkBuildableIface *parent;

  g_return_if_fail (IDE_IS_WORKBENCH_HEADER_BAR (self));
  g_return_if_fail (GTK_IS_BUILDER (builder));
  g_return_if_fail (GTK_IS_WIDGET (object));

  if (dzl_str_equal0 (type, "primary"))
    {
      ide_workbench_header_bar_add_primary (self, GTK_WIDGET (object));
      return;
    }

  parent = g_type_interface_peek_parent (GTK_BUILDABLE_GET_IFACE (self));
  parent->add_child (buildable, builder, object, type);
}

static void
buildable_iface_init (GtkBuildableIface *iface)
{
  iface->get_internal_child = ide_workbench_header_bar_get_internal_child;
  iface->add_child = ide_workbench_header_bar_add_child;
}

/**
 * ide_workbench_header_bar_get_omni_bar:
 *
 * Returns: (transfer none): An #IdeOmniBar.
 */
IdeOmniBar *
ide_workbench_header_bar_get_omni_bar (IdeWorkbenchHeaderBar *self)
{
  IdeWorkbenchHeaderBarPrivate *priv = ide_workbench_header_bar_get_instance_private (self);

  g_return_val_if_fail (IDE_IS_WORKBENCH_HEADER_BAR (self), NULL);

  return priv->omni_bar;
}

void
_ide_workbench_header_bar_set_fullscreen (IdeWorkbenchHeaderBar *self,
                                          gboolean               fullscreen)
{
  IdeWorkbenchHeaderBarPrivate *priv = ide_workbench_header_bar_get_instance_private (self);
  const gchar *icon_names[] = {
    "view-fullscreen-symbolic",
    "view-restore-symbolic",
  };

  g_return_if_fail (IDE_IS_WORKBENCH_HEADER_BAR (self));

  g_object_set (priv->fullscreen_image,
                "icon-name", icon_names[!!fullscreen],
                NULL);
  gtk_header_bar_set_show_close_button (GTK_HEADER_BAR (self), fullscreen == FALSE);
}

void
ide_workbench_header_bar_show_menu (IdeWorkbenchHeaderBar *self)
{
  IdeWorkbenchHeaderBarPrivate *priv = ide_workbench_header_bar_get_instance_private (self);

  g_return_if_fail (IDE_IS_WORKBENCH_HEADER_BAR (self));

  gtk_widget_activate (GTK_WIDGET (priv->menu_button));
}
