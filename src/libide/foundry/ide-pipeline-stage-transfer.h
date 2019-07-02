/* ide-pipeline-stage-transfer.h
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
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#pragma once

#if !defined (IDE_FOUNDRY_INSIDE) && !defined (IDE_FOUNDRY_COMPILATION)
# error "Only <libide-foundry.h> can be included directly."
#endif

#include <libide-core.h>

#include "ide-pipeline-stage.h"

G_BEGIN_DECLS

#define IDE_TYPE_PIPELINE_STAGE_TRANSFER (ide_pipeline_stage_transfer_get_type())

IDE_AVAILABLE_IN_3_32
G_DECLARE_FINAL_TYPE (IdePipelineStageTransfer, ide_pipeline_stage_transfer, IDE, PIPELINE_STAGE_TRANSFER, IdePipelineStage)

IDE_AVAILABLE_IN_3_32
IdePipelineStageTransfer *ide_pipeline_stage_transfer_new (IdeContext  *context,
                                                           IdeTransfer *transfer);

G_END_DECLS