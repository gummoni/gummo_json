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
#ifndef __GUMMO_JSON_H__
#define __GUMMO_JSON_H__

//macro
#define TYPE_GET_INFO(type_id)					(0 > type_id) ? 0 : &struct_types[type_id]
#define TYPE_INFO(name, var)					{ name, (unsigned int)&var }
#define TYPE_END()								{ 0, (unsigned int)0 }

#define FIELD_TYPE_SIZE(field)					(field->size / field->length)
#define FIELD_INFO(obj, field, type, length)	{ #field, type, (unsigned int)&obj.field, sizeof(obj.field), length }
#define FIELD_END()								{ "\0", TYPE_ID_BYTE, 0, 0, 0 }

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
	unsigned int index;
	unsigned char size;
	unsigned char length;
} struct_field;

//struct type information
typedef struct
{
	const struct_field* fields;
	unsigned int offset;
} struct_type;

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

//JSON API
extern const struct_type struct_types[];
extern char* json_deserialize(void* obj, const struct_type* fields, char* json_str);
extern int json_serialize(void* obj, const struct_type* fields, char* json_str, int length);


//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
#define TEST2_DATA_B_LENGTH			2
#define TEST2_DATA_MSG_LENGTH		16
typedef struct
{
	unsigned short b[TEST2_DATA_B_LENGTH];
	char msg[TEST2_DATA_MSG_LENGTH];
} test2_data;

#define TEST1_DATA_A_LENGTH			1
#define TEST1_DATA_DATA_LENGTH		2
#define TEST1_DATA_MSG_LENGTH		16
typedef struct
{
	int a;
	test2_data data[TEST1_DATA_DATA_LENGTH];
	char msg[TEST1_DATA_MSG_LENGTH];
} test1_data;
//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<



#endif//__GUMMO_JSON_H__