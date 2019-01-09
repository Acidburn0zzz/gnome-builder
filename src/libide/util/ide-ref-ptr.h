/* ide-ref-ptr.h
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

#include <glib-object.h>

#include "ide-version-macros.h"

G_BEGIN_DECLS

#define IDE_TYPE_REF_PTR (ide_ref_ptr_get_type())

typedef struct _IdeRefPtr IdeRefPtr;

IDE_AVAILABLE_IN_3_32
GType      ide_ref_ptr_get_type (void);
IDE_AVAILABLE_IN_3_32
IdeRefPtr *ide_ref_ptr_new      (gpointer        data,
                                 GDestroyNotify  free_func);
IDE_AVAILABLE_IN_3_32
IdeRefPtr *ide_ref_ptr_ref      (IdeRefPtr      *self);
IDE_AVAILABLE_IN_3_32
void       ide_ref_ptr_unref    (IdeRefPtr      *self);
IDE_AVAILABLE_IN_3_32
gpointer   ide_ref_ptr_get      (IdeRefPtr      *self);

G_DEFINE_AUTOPTR_CLEANUP_FUNC (IdeRefPtr, ide_ref_ptr_unref)

G_END_DECLS
