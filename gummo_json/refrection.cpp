#include "pch.h"
#include "gummo_json.h"

const char test1_data_name[] = "test1_data";
const char test2_data_name[] = "test2_data";

const char* struct_type_names[] = {
	test1_data_name,
	test2_data_name,
};

test1_data tmp1;

const static struct_field test1_data_fields[] = {
	FIELD_INFO(tmp1			, a		, TYPE_ID_INT			, TEST1_DATA_A_LENGTH),
	FIELD_INFO(tmp1			, data	, TYPE_ID_TEST2_DATA	, TEST1_DATA_DATA_LENGTH),
	FIELD_INFO(tmp1			, msg	, TYPE_ID_STRING		, TEST1_DATA_MSG_LENGTH),
	FIELD_END()
};

const static struct_field test2_data_fields[] = {
	FIELD_INFO(tmp1.data[0]	, b		, TYPE_ID_SHORT			, TEST2_DATA_B_LENGTH),
	FIELD_INFO(tmp1.data[0]	, msg	, TYPE_ID_STRING		, TEST2_DATA_MSG_LENGTH),
	FIELD_END()
};

const struct_field* struct_types[] =
{
	test1_data_fields,
	test2_data_fields,
};
