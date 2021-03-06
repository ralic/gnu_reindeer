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

#include <ren/vector.h>
#include <ren/base.h>
#include <glib.h>

#include "reindeer.h"
#include "backdata.h"

typedef struct _RenVectorBackDataItem _RenVectorBackDataItem;

struct _RenVector
{
	ren_uint32 ref_count;

	_RenVectorBackDataItem *bd_list;
	ren_uint32 change;

	RenVectorInfo info;
};

struct _RenVectorBackDataKey
{
	ren_uint32 ref_count;
	RenVectorBackDataKeyDestroyNotifyFunc destroy_notify;

	_RenVectorBackDataItem *bd_list;

	ren_size data_size;
	void *user_data;
	RenVectorBackDataInitFunc init;
	RenVectorBackDataFiniFunc fini;
	RenVectorBackDataUpdateFunc update;
};

struct _RenVectorBackDataItem
{
	_RenBackDataItem base;
	ren_uint32 change;
};

RenVector*
ren_vector_new (ren_size length, RenType type)
{
	if (type != REN_TYPE_SFLOAT && type != REN_TYPE_DFLOAT)
		return NULL;

	RenVector *vector = g_new (RenVector, 1);

	vector->ref_count = 1;

	vector->bd_list = NULL;
	vector->change = 1;

	vector->info.length = length;
	vector->info.type = type;
	vector->info.data = g_malloc (length * ren_type_sizeof (type));

	return vector;
}

RenVector*
ren_vector_ref (RenVector *vector)
{
	++(vector->ref_count);
	return vector;
}

void
ren_vector_unref (RenVector *vector)
{
	if (--(vector->ref_count) > 0)
		return;

	_REN_RES_BACK_DATA_LIST_CLEAR (Vector, vector,
		vector, _REN_BACK_DATA_SIMPLE_FINI_FUNC);

	g_free ((gpointer) vector->info.data);
	g_free (vector);
}

void*
ren_vector_begin_edit (RenVector *vector)
{
	return (void *) vector->info.data;
}

void
ren_vector_end_edit (RenVector *vector)
{
	_REN_BACK_DATA_SIMPLE_CHANGED (Vector, vector, vector);
}

const RenVectorInfo*
ren_vector_info (RenVector *vector)
{
	return &(vector->info);
}

RenVectorBackDataKey*
ren_vector_back_data_key_new (ren_size data_size,
	RenVectorBackDataInitFunc init,
	RenVectorBackDataFiniFunc fini,
	RenVectorBackDataUpdateFunc update)
{
	RenVectorBackDataKey *key = g_new (RenVectorBackDataKey, 1);

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
ren_vector_back_data_key_user_data (RenVectorBackDataKey *key,
	void *user_data)
{
	key->user_data = user_data;
}

void
ren_vector_back_data_key_destroy_notify (RenVectorBackDataKey *key,
	RenVectorBackDataKeyDestroyNotifyFunc destroy_notify)
{
	key->destroy_notify = destroy_notify;
}

RenVectorBackDataKey*
ren_vector_back_data_key_ref (RenVectorBackDataKey *key)
{
	++(key->ref_count);
	return key;
}

void
ren_vector_back_data_key_unref (RenVectorBackDataKey *key)
{
	if (--(key->ref_count) > 0)
		return;

	_REN_KEY_BACK_DATA_LIST_CLEAR (Vector, vector,
		key, _REN_BACK_DATA_SIMPLE_FINI_FUNC);
	if (key->destroy_notify != NULL)
		key->destroy_notify (key, key->user_data);

	g_free (key);
}

RenVectorBackData*
ren_vector_back_data (RenVector *vector, RenVectorBackDataKey *key)
{
	_REN_BACK_DATA_RETURN (Vector, vector,
		vector, key,
		_REN_BACK_DATA_SIMPLE_INIT_FUNC,
		_REN_BACK_DATA_SIMPLE_UPDATE_FUNC);
}
