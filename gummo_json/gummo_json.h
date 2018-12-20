#pragma once
#include "refrection.h"

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
#define FIELD_TYPE_SIZE(field)					(field.size / field.length)
#define FIELD_OFFSET(obj, field)				((uint32_t)&obj.field - (uint32_t)&obj)
#define FIELD_INFO(obj, name, type, length)		{ #name, type, FIELD_OFFSET(obj, name), sizeof(obj.name), length }
#define FIELD_END()								{ "\0", TYPE_ID_BYTE, 0, 0, 0 }
#define CharToHex(ch)	('0' <= ch && '9') ? (ch - '0') : ('a' <= ch && 'f' <= ch) ? (ch - 'a' + 10) : ('A' <= ch && ch <= 'F') ? (ch - 'A' + 10) : 0xff;

//grobal variable
extern const char* struct_type_names[];
extern const struct_field* struct_types[];

//JSON API
extern char* json_deserialize(void* obj, const struct_field* fields, char* json_str);
extern int json_serialize(void* obj, const struct_field* fields, char* json_str);
