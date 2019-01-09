/* ide-search-engine.h
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

#include "ide-version-macros.h"

#include "ide-object.h"

G_BEGIN_DECLS

#define IDE_TYPE_SEARCH_ENGINE (ide_search_engine_get_type())

IDE_AVAILABLE_IN_ALL
G_DECLARE_FINAL_TYPE (IdeSearchEngine, ide_search_engine, IDE, SEARCH_ENGINE, IdeObject)

IDE_AVAILABLE_IN_ALL
IdeSearchEngine *ide_search_engine_new           (void);
IDE_AVAILABLE_IN_ALL
gboolean         ide_search_engine_get_busy      (IdeSearchEngine      *self);
IDE_AVAILABLE_IN_ALL
void             ide_search_engine_search_async  (IdeSearchEngine      *self,
                                                  const gchar          *query,
                                                  guint                 max_results,
                                                  GCancellable         *cancellable,
                                                  GAsyncReadyCallback   callback,
                                                  gpointer              user_data);
IDE_AVAILABLE_IN_ALL
GListModel      *ide_search_engine_search_finish (IdeSearchEngine      *self,
                                                  GAsyncResult         *result,
                                                  GError              **error);

G_END_DECLS
