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
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "stdbool.h"
#include "gummo_json.h"


static bool json_capture_value(json_parse_context* context);

static const struct_field* struct_get_field(const struct_field* fields, char* name)
{
	for (const struct_field* field = fields ; ; field++)
	{
		if (NULL == field) return NULL;
		if (!strcmp(field->name, name)) return field;
	}
}

static char CharToHex(char ch) { return ('0' <= ch && '9') ? (ch - '0') : ('a' <= ch && 'f' <= ch) ? (ch - 'a' + 10) : ('A' <= ch && ch <= 'F') ? (ch - 'A' + 10) : 0xff; }

static void struct_set_value(char* pval, TYPE_ID type, char* data)
{
	if (type == TYPE_ID_BYTE) *((unsigned char*)pval) = (unsigned char)strtol(data, NULL, 0);
	else if (type == TYPE_ID_SHORT) *((unsigned short*)pval) = (unsigned short)strtol(data, NULL, 0);
	else if (type == TYPE_ID_INT) *((unsigned int*)pval) = (unsigned int)strtol(data, NULL, 0);
	else if (type == TYPE_ID_LONG) *((unsigned long*)pval) = (unsigned long)strtol(data, NULL, 0);
	else if (type == TYPE_ID_FLOAT) *((float*)pval) = (float)strtof(data, NULL);
	else if (type == TYPE_ID_DOUBLE) *((double*)pval) = (double)strtod(data, NULL);
	else if (type == TYPE_ID_STRING) memcpy(pval, data, strlen(data) + 1);
}

static char json_skip_space(json_parse_context* context)
{
	for (char ch = *context->msg ; ; ch = *(++context->msg))
	{
		if (' ' == ch || '\t' == ch || '\r' == ch || '\n' == ch) continue;
		return ch;
	}
}

static char json_get_token(json_parse_context* context)
{
	char result;
	if (!(result = json_skip_space(context))) return '\0';
	context->msg++;
	json_skip_space(context);
	return result;
}

static bool json_capture_numeric(json_parse_context* context)
{
	char* dst = context->tmp;
	for (*dst = *context->msg; ; *(++dst) = *(++context->msg))
	{
		if (('0' <= *dst && *dst <= '9') || ('x' == *dst) || ('.' == *dst) || ('a' <= *dst && *dst <= 'f') || ('A' <= *dst && *dst <= 'F')) continue;
		*dst = '\0';
		struct_set_value(context->pval, context->pfield->type, context->tmp);
		return ('\0' != *context->msg);
	}
}

static bool json_capture_string(json_parse_context* context, char* dst)
{
	for (char ch = *(++context->msg); '\0' != ch; ch = *(++context->msg))
	{
		switch (ch)
		{
		case '\\':
			ch = *(++context->msg);
			if (ch == 'b') *dst = '\b';
			else if (ch == 'n') *dst = '\n';
			else if (ch == 'r') *dst = '\r';
			else if (ch == 't') *dst = '\t';
			else if (ch == 'x')
			{	//hex: 1byte
				char c0 = CharToHex(*(++context->msg));
				char c1 = CharToHex(*(++context->msg));				
				*(dst++) = (c0 << 4) | c1;
			}
			else if (ch == 'u')
			{	//multibyte: 2byte
				char c0 = CharToHex(*(++context->msg));
				char c1 = CharToHex(*(++context->msg));
				char c2 = CharToHex(*(++context->msg));
				char c3 = CharToHex(*(++context->msg));
				*(dst++) = (c0 << 4) | c1;
				*(dst++) = (c2 << 4) | c3;
			}
			else *(dst++) = ch;
			break;

		case '"':
			*(dst++) = '\0';
			context->msg++;
			return true;

		default:
			*(dst++) = ch;
			break;
		}
	}
	return false;
}

static bool json_capture_object(json_parse_context* context)
{
	const struct_type* type = TYPE_GET_INFO(context->pfield->type);
	if (0 == type) return false;
	context->msg = json_deserialize(context->pval, type, context->msg);
	return true;
}

static bool json_capture_array(json_parse_context* context)
{
	const struct_field* pfield = context->pfield;
	int length = pfield->length;
	int array_size = pfield->size / length;
	TYPE_ID type = pfield->type;

	int array_length = pfield->length;
	for (; ; context->pval += array_size)
	{	//TODO:buffer overrun proof
		if (0 > type)
		{	//generic type
			if (!json_capture_value(context)) break;
		}
		else
		{	//user type
			context->msg = json_deserialize(context->pval, TYPE_GET_INFO(type), context->msg);
		}
		if (0 <= array_length) array_length--;
		char ch = json_get_token(context);
		if (',' == ch) continue;
		if (']' == ch) break;
		return false;
	}
	return true;
}

static bool json_capture_value(json_parse_context* context)
{
	char ch = *context->msg;
	if ('"' == ch) { return json_capture_string(context, context->pval); }
	if ('{' == ch) { *context->msg++; json_skip_space(context); return json_capture_object(context); }
	if ('[' == ch) { *context->msg++; json_skip_space(context); return json_capture_array(context); }
	if ('t' == ch) { context->msg += 4; struct_set_value(context->pval, context->pfield->type, (char*)"1"); return true; }
	if ('f' == ch) { context->msg += 5; struct_set_value(context->pval, context->pfield->type, (char*)"0"); return true; }
	if ('n' == ch) { context->msg += 4; memset(context->pval, 0, FIELD_TYPE_SIZE(context->pfield)); return true; }
	return json_capture_numeric(context);
}

static bool json_parse_object(json_parse_context* context)
{
	for (;;)
	{
		// capture key
		if (!json_capture_string(context, context->tmp)) return false;
		context->pfield = struct_get_field(context->fields, context->tmp);
		context->pval = &context->obj[context->pfield->index];
		if (':' != json_get_token(context)) return false;
		// capture value
		if (!json_capture_value(context)) return false;
		//next
		char ch = json_get_token(context);
		if (',' == ch) continue;
		if ('}' == ch) return true;
		return false;
	}
}

char* json_deserialize(void* obj, const struct_type* type, char* json_str)
{
	const struct_field* fields = type->fields;
	json_parse_context context = { (char*)((unsigned int)obj - type->offset), fields, json_str };
	switch (json_get_token(&context))
	{
	case '{':	return (!json_parse_object(&context)) ? NULL : context.msg;
	case '[':	return (!json_capture_array(&context)) ? NULL : context.msg;
	default:	return NULL;
	}
}
//-------------------------Serialize-------------------------
static int json_to_string_value(char* pval, const struct_field* field, char* msg, int size)
{
	TYPE_ID type = field->type;
	if (type == TYPE_ID_BYTE) return sprintf_s(msg, size, "%d", *(unsigned char*)pval);
	if (type == TYPE_ID_SHORT) return sprintf_s(msg, size, "%d", *(unsigned short*)pval);
	if (type == TYPE_ID_INT) return sprintf_s(msg, size, "%d", *(unsigned int*)pval);
	if (type == TYPE_ID_LONG) return sprintf_s(msg, size, "%ld", *(unsigned long*)pval);
	if (type == TYPE_ID_FLOAT) return sprintf_s(msg, size, "%f", *(float*)pval);
	if (type == TYPE_ID_DOUBLE) return sprintf_s(msg, size, "%lf", *(double*)pval);
	if (type == TYPE_ID_STRING) return sprintf_s(msg, size, "\"%s\"", pval);
	return json_serialize(pval, TYPE_GET_INFO(type), msg, size);
}

int json_serialize(void* obj, const struct_type* type, char* json_str, int size)
{
	const struct_field* fields = type->fields;
	char* _obj = (char*)((unsigned int)obj - type->offset);
	int idx = sprintf_s(json_str, size, "{");
	for (;;)
	{
		int length = fields->length;
		TYPE_ID type = fields->type;
		char* pobj = &_obj[fields->index];
		//key
		idx += sprintf_s(&json_str[idx], size - idx, "\"%s\":", fields->name);
		//value
		if ((1 < length) && (TYPE_ID_STRING != type))
		{	//array
			int array_size = fields->size / length;
			idx += sprintf_s(&json_str[idx], size - idx, "[");
			int i = 0;
			for (;;)
			{
				idx += json_to_string_value(pobj, fields, &json_str[idx], size - idx);
				//next
				pobj += array_size;
				if (++i == length) break;
				idx += sprintf_s(&json_str[idx], size - idx, ",");
			}
			idx += sprintf_s(&json_str[idx], size - idx, "]");
		}
		else
		{	//object
			idx += json_to_string_value(pobj, fields, &json_str[idx], size - idx);
		}
		//next
		if (!*(++fields)->name) break;
		idx += sprintf_s(&json_str[idx], size - idx, ",");
	}
	idx += sprintf_s(&json_str[idx], size - idx, "}");
	json_str[idx] = '\0';
	return idx;
}
