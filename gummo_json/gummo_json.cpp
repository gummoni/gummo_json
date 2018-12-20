#include "pch.h"
#include "gummo_json.h"

static bool json_capture_value(json_parse_context* context);

static const struct_field* struct_get_field(const struct_field* fields, char* name)
{
	for (const struct_field* field = fields ; ; field++)
	{
		if (NULL == field) return NULL;
		if (NULL == strcmp(field->name, name)) return field;
	}
}

static void struct_set_value(char* pval, TYPE_ID type, char* data)
{
	if (type == TYPE_ID_BYTE) *((unsigned char*)pval) = (unsigned char)strtol(data, NULL, NULL);
	else if (type == TYPE_ID_SHORT) *((unsigned short*)pval) = (unsigned short)strtol(data, NULL, NULL);
	else if (type == TYPE_ID_INT) *((unsigned int*)pval) = (unsigned int)strtol(data, NULL, NULL);
	else if (type == TYPE_ID_LONG) *((unsigned long*)pval) = (unsigned long)strtol(data, NULL, NULL);
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
	if (!(result = json_skip_space(context))) return NULL;
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
	for (char ch = *(++context->msg); '\0' != ch; ch = *(++context->msg), dst++)
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
				char tmp0 = CharToHex(*(context->msg++));
				char tmp1 = CharToHex(*(context->msg++));				
				*dst = (tmp0 << 4) | tmp1;
			}
			else if (ch == 'u')
			{	//multibyte: 2byte
				char tmp0 = CharToHex(*(context->msg++));
				char tmp1 = CharToHex(*(context->msg++));
				char tmp2 = CharToHex(*(context->msg++));
				char tmp3 = CharToHex(*(context->msg++));
				*dst = (tmp0 << 4) | tmp1;
				dst++;
				*dst = (tmp2 << 4) | tmp3;
			}
			else *dst = ch;
			break;

		case '"':
			*dst = '\0';
			context->msg++;
			return true;

		default:
			*dst = ch;
			break;
		}
	}
	return false;
}

static bool json_capture_object(json_parse_context* context)
{
	const struct_field* pfields = TYPE_GET_FIELDS(context->pfield->type);
	if (0 > pfields->size) return false;
	context->msg = json_deserialize(context->pval, pfields, context->msg);
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
			const struct_field* pfields = TYPE_GET_FIELDS(type);
			context->msg = json_deserialize(context->pval, pfields, context->msg);
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
	if ('t' == ch) { context->msg += 4; return true; }
	if ('f' == ch) { context->msg += 5; return true; }
	if ('n' == ch) { context->msg += 4; return true; }
	return json_capture_numeric(context);
}

static bool json_parse_object(json_parse_context* context)
{
	for (;;)
	{
		// capture key
		if (!json_capture_string(context, context->tmp)) return false;
		context->pfield = struct_get_field(context->fields, context->tmp);
		context->pval = TYPE_GET_FIELD_POINTER(context->obj, context->pfield->offset);
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

char* json_deserialize(void* obj, const struct_field* fields, char* json_str)
{
	json_parse_context context{ (char*)obj, fields, json_str };
	switch (json_get_token(&context))
	{
	case '{':	return (!json_parse_object(&context)) ? NULL : context.msg;
	case '[':	return (!json_capture_array(&context)) ? NULL : context.msg;
	default:	return NULL;
	}
}
//-------------------------Deserialize-------------------------
static int json_to_string_value(char* pval, const struct_field* field, char* msg)
{
	TYPE_ID type = field->type;
	if (type == TYPE_ID_BYTE) return sprintf_s(msg, field->size, "%d", *(unsigned char*)pval);
	if (type == TYPE_ID_SHORT) return sprintf_s(msg, field->size, "%d", *(unsigned short*)pval);
	if (type == TYPE_ID_INT) return sprintf_s(msg, field->size, "%d", *(unsigned int*)pval);
	if (type == TYPE_ID_LONG) return sprintf_s(msg, field->size, "%ld", *(unsigned long*)pval);
	if (type == TYPE_ID_FLOAT) return sprintf_s(msg, field->size, "%f", *(float*)pval);
	if (type == TYPE_ID_DOUBLE) return sprintf_s(msg, field->size, "%lf", *(double*)pval);
	if (type == TYPE_ID_STRING) return sprintf_s(msg, field->size, "\"%s\"", pval);
	return json_serialize(pval, TYPE_GET_FIELDS(type), msg);
}

int json_serialize(void* obj, const struct_field* fields, char* json_str)
{
	int idx = sprintf_s(json_str, 256, "{");
	for (;;)
	{
		int length = fields->length;
		TYPE_ID type = fields->type;
		char* pobj = &((char*)obj)[fields->offset];
		//key
		idx += sprintf_s(&json_str[idx], 256, "\"%s\":", fields->name);
		//value
		if ((1 < length) && (TYPE_ID_STRING != type))
		{	//array
			int length = fields->length;
			int array_size = fields->size / length;
			idx += sprintf_s(&json_str[idx], 16, "[");
			int i = 0;
			for (;;)
			{
				idx += json_to_string_value(pobj, fields, &json_str[idx]);
				//next
				pobj += array_size;
				if (++i == length) break;
				idx += sprintf_s(&json_str[idx], 16, ",");
			}
			idx += sprintf_s(&json_str[idx], 16, "]");
		}
		else
		{	//object
			idx += json_to_string_value(pobj, fields, &json_str[idx]);
		}
		//next
		if (NULL == *(++fields)->name) break;
		idx += sprintf_s(&json_str[idx], 256, ",");
	}
	idx += sprintf_s(&json_str[idx], 256, "}");
	json_str[idx] = '\0';
	return idx;
}
