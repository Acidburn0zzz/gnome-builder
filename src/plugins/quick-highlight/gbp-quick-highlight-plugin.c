/* gbp-quick-highlight-plugin.c
 *
 * Copyright 2016 Martin Blanchard <tchaik@gmx.com>
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

#include <libpeas/peas.h>
#include <ide.h>

#include "gbp-quick-highlight-editor-view-addin.h"
#include "gbp-quick-highlight-preferences.h"

void
gbp_quick_highlight_register_types (PeasObjectModule *module)
{
  peas_object_module_register_extension_type (module,
                                              IDE_TYPE_EDITOR_VIEW_ADDIN,
                                              GBP_TYPE_QUICK_HIGHLIGHT_EDITOR_VIEW_ADDIN);
  peas_object_module_register_extension_type (module,
                                              IDE_TYPE_PREFERENCES_ADDIN,
                                              GBP_TYPE_QUICK_HIGHLIGHT_PREFERENCES);
}
