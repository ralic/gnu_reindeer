/*
    This file is part of Reindeer.

    Copyright (C) 2010 - Patrik Olsson

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef REN_DATABLOCK_H
#define REN_DATABLOCK_H

#include <ren/types.h>

/* Public */

extern RenDataBlock*
ren_data_block_new (const void *data, ren_size size, RenUsage usage);

extern RenDataBlock*
ren_data_block_ref (RenDataBlock *data_block);

extern void
ren_data_block_unref (RenDataBlock *data_block);

extern void
ren_data_block_callback (
    RenDataBlock *datablock,
    RenDataBlockCallback reload_func,
    RenDataBlockCallback unload_func,
    void *user_data);

extern void
ren_data_block_relocated (RenDataBlock *datablock, const void *data);

extern void
ren_data_block_resized (RenDataBlock *datablock, ren_size size);

extern void
ren_data_block_changed (RenDataBlock *datablock, ren_size from, ren_size to);

/* Backend */

extern void
ren_data_block_data (RenDataBlock *data_block, const void **data_p,
    ren_size *size_p, RenUsage *usage_p);

#endif /* REN_DATABLOCK_H */
