/*
    This file is part of Reindeer.

    Copyright (C) 2008, 2009 - Patrik Olsson

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

#include <ren/datablock.h>
#include <glib.h>

struct _RenDataBlock
{
    ren_uint32 ref_count;

    RenDataBlockCallback reload_func;
    RenDataBlockCallback unload_func;
    void *user_data;

    const void *data;
    ren_size size;
    RenUsage usage;
};

RenDataBlock*
ren_data_block_new (const void *data, ren_size size, RenUsage usage)
{
    RenDataBlock *datablock = g_new0 (RenDataBlock, 1);

    datablock->ref_count = 1;
    datablock->data = data;
    datablock->size = size;
    datablock->usage = usage;

    return datablock;
}

RenDataBlock*
ren_data_block_ref (RenDataBlock *datablock)
{
    ++(datablock->ref_count);
    return datablock;
}

void
ren_data_block_unref (RenDataBlock *datablock)
{
    if (--(datablock->ref_count) > 0)
        return;

    if (datablock->unload_func != NULL)
        datablock->unload_func (NULL, datablock->user_data);
    g_free (datablock);
}

void
ren_data_block_callback (
    RenDataBlock *datablock,
    RenDataBlockCallback reload_func,
    RenDataBlockCallback unload_func,
    void *user_data)
{
    datablock->reload_func = reload_func;
    datablock->unload_func = unload_func;
    datablock->user_data = user_data;
}

void
ren_data_block_relocated (RenDataBlock *datablock, const void *data)
{
    datablock->data = data;
    /* TODO: This might need to be notified to binders, even though it does
    not change the actual data... */
}

void
ren_data_block_resized (RenDataBlock *datablock, ren_size size)
{
    datablock->size = size;
    /* TODO: This might need to be notified to binders, even though it does
    not change the actual data if new size > old size... */
}

void
ren_data_block_changed (RenDataBlock *datablock,
    ren_size from, ren_size to)
{

}

void
ren_data_block_data (RenDataBlock *datablock, const void **datap,
    ren_size *sizep, RenUsage *usagep)
{
    if (datap)
        (*datap) = datablock->data;
    if (sizep)
        (*sizep) = datablock->size;
    if (usagep)
        (*usagep) = datablock->usage;
}
