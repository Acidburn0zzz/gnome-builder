/* ide-transfer-manager.h
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

#include "ide-version-macros.h"

#include "ide-object.h"

#include "transfers/ide-transfer.h"

G_BEGIN_DECLS

#define IDE_TYPE_TRANSFER_MANAGER (ide_transfer_manager_get_type())

IDE_AVAILABLE_IN_ALL
G_DECLARE_FINAL_TYPE (IdeTransferManager, ide_transfer_manager, IDE, TRANSFER_MANAGER, GObject)

IDE_AVAILABLE_IN_ALL
gdouble  ide_transfer_manager_get_progress   (IdeTransferManager   *self);
IDE_AVAILABLE_IN_ALL
gboolean ide_transfer_manager_get_has_active (IdeTransferManager   *self);
IDE_AVAILABLE_IN_ALL
void     ide_transfer_manager_cancel_all     (IdeTransferManager   *self);
IDE_AVAILABLE_IN_ALL
void     ide_transfer_manager_clear          (IdeTransferManager   *self);
IDE_AVAILABLE_IN_ALL
void     ide_transfer_manager_execute_async  (IdeTransferManager   *self,
                                              IdeTransfer          *transfer,
                                              GCancellable         *cancellable,
                                              GAsyncReadyCallback   callback,
                                              gpointer              user_data);
IDE_AVAILABLE_IN_ALL
gboolean ide_transfer_manager_execute_finish (IdeTransferManager   *self,
                                              GAsyncResult         *result,
                                              GError              **error);

G_END_DECLS
