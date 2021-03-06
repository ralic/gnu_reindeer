/*
	This file is part of Reindeer.

	Copyright (C) 2008, 2009, 2010 - Patrik Olsson

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

#include <ren/matrix.h>
#include <ren/base.h>
#include <glib.h>

#include "reindeer.h"
#include "backdata.h"

typedef struct _RenMatrixBackDataItem _RenMatrixBackDataItem;

struct _RenMatrix
{
	ren_uint32 ref_count;

	_RenMatrixBackDataItem *bd_list;
	ren_uint32 change;

	RenMatrixInfo info;
};

struct _RenMatrixBackDataKey
{
	ren_uint32 ref_count;
	RenMatrixBackDataKeyDestroyNotifyFunc destroy_notify;

	_RenMatrixBackDataItem *bd_list;

	ren_size data_size;
	void *user_data;
	RenMatrixBackDataInitFunc init;
	RenMatrixBackDataFiniFunc fini;
	RenMatrixBackDataUpdateFunc update;
};

struct _RenMatrixBackDataItem
{
	_RenBackDataItem base;
	ren_uint32 change;
};

RenMatrix*
ren_matrix_new (ren_size width, ren_size height,
	RenType type, ren_bool transposed)
{
	if (type != REN_TYPE_SFLOAT && type != REN_TYPE_DFLOAT)
		return NULL;

	RenMatrix *matrix = g_new (RenMatrix, 1);

	matrix->ref_count = 1;

	matrix->bd_list = NULL;
	matrix->change = 1;

	matrix->info.width = width;
	matrix->info.height = height;
	matrix->info.type = type;
	matrix->info.transposed = transposed;
	matrix->info.data = g_malloc (ren_type_sizeof (type) * width * height);

	return matrix;
}

RenMatrix*
ren_matrix_ref (RenMatrix *matrix)
{
	++(matrix->ref_count);
	return matrix;
}

void
ren_matrix_unref (RenMatrix *matrix)
{
	if (--(matrix->ref_count) > 0)
		return;

	_REN_RES_BACK_DATA_LIST_CLEAR (Matrix, matrix,
		matrix, _REN_BACK_DATA_SIMPLE_FINI_FUNC);

	g_free ((gpointer) matrix->info.data);
	g_free (matrix);
}

void*
ren_matrix_begin_edit (RenMatrix *matrix)
{
	return (void *) matrix->info.data;
}

void
ren_matrix_end_edit (RenMatrix *matrix)
{
	_REN_BACK_DATA_SIMPLE_CHANGED (Matrix, matrix, matrix);
}

const RenMatrixInfo*
ren_matrix_info (RenMatrix *matrix)
{
	return &(matrix->info);
}

RenMatrixBackDataKey*
ren_matrix_back_data_key_new (ren_size data_size,
	RenMatrixBackDataInitFunc init,
	RenMatrixBackDataFiniFunc fini,
	RenMatrixBackDataUpdateFunc update)
{
	RenMatrixBackDataKey *key = g_new (RenMatrixBackDataKey, 1);

	key->ref_count = 1;
	key->destroy_notify = NULL;

	key->bd_list = NULL;

	key->data_size = data_size;
	key->user_data = NULL;
	key->init = init;
	key->fini = fini;
	key->update = update;

	return key;
}

void
ren_matrix_back_data_key_user_data (RenMatrixBackDataKey *key,
	void *user_data)
{
	key->user_data = user_data;
}

void
ren_matrix_back_data_key_destroy_notify (RenMatrixBackDataKey *key,
	RenMatrixBackDataKeyDestroyNotifyFunc destroy_notify)
{
	key->destroy_notify = destroy_notify;
}

RenMatrixBackDataKey*
ren_matrix_back_data_key_ref (RenMatrixBackDataKey *key)
{
	++(key->ref_count);
	return key;
}

void
ren_matrix_back_data_key_unref (RenMatrixBackDataKey *key)
{
	if (--(key->ref_count) > 0)
		return;

	_REN_KEY_BACK_DATA_LIST_CLEAR (Matrix, matrix,
		key, _REN_BACK_DATA_SIMPLE_FINI_FUNC);
	if (key->destroy_notify != NULL)
		key->destroy_notify (key, key->user_data);

	g_free (key);
}

RenMatrixBackData*
ren_matrix_back_data (RenMatrix *matrix, RenMatrixBackDataKey *key)
{
	_REN_BACK_DATA_RETURN (Matrix, matrix,
		matrix, key,
		_REN_BACK_DATA_SIMPLE_INIT_FUNC,
		_REN_BACK_DATA_SIMPLE_UPDATE_FUNC);
}
