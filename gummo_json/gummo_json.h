/*
The MIT License (MIT)
Copyright (c) 2018 Koichi Nishino
Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/
#pragma once


//type index
typedef enum
{
	TYPE_ID_STRING = -8,
	TYPE_ID_DOUBLE = -7,
	TYPE_ID_FLOAT = -6,
	TYPE_ID_LONG = -5,
	TYPE_ID_INT = -4,
	TYPE_ID_SHORT = -3,
	TYPE_ID_BYTE = -2,
	TYPE_ID_BOOL = -1,
	//>>>>>>>>>> user type define <<<<<<<<<<
	TYPE_ID_TEST1_DATA = 0,
	TYPE_ID_TEST2_DATA = 1,
	//======================================
	TYPE_ID_MAX,
} TYPE_ID;

//field information
#define JSON_KEY_LENGTH		16
typedef struct
{
	char name[JSON_KEY_LENGTH];
	TYPE_ID type;
	uint32_t offset;
	uint8_t size;
	uint8_t length;
} struct_field;

//JSON parse context
typedef struct
{
	char* obj;
	const struct_field* fields;
	char* msg;
	char tmp[JSON_KEY_LENGTH];
	const struct_field* pfield;
	char* pval;
} json_parse_context;

//macro
#define TYPE_GET_FIELDS(type_id)				(0 > type_id) ? NULL : struct_types[type_id]
#define TYPE_GET_FIELD_POINTER(obj, offset)		&obj[offset]
#define FIELD_TYPE_SIZE(field)					(field->size / field->length)
#define FIELD_OFFSET(obj, field)				((uint32_t)&obj.field - (uint32_t)&obj)
#define FIELD_INFO(obj, name, type, length)		{ #name, type, FIELD_OFFSET(obj, name), sizeof(obj.name), length }
#define FIELD_END()								{ "\0", TYPE_ID_BYTE, 0, 0, 0 }

//JSON API
extern char* json_deserialize(void* obj, const struct_field* fields, char* json_str);
extern int json_serialize(void* obj, const struct_field* fields, char* json_str, int length);

#include "refrection.h"
