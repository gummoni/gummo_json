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
#include "gummo_json.h"

static const test1_data tmp1;


const struct_field test1_data_fields[] = {
	FIELD_INFO(tmp1         , a    , TYPE_ID_INT        , TEST1_DATA_A_LENGTH    ),
	FIELD_INFO(tmp1         , data , TYPE_ID_TEST2_DATA , TEST1_DATA_DATA_LENGTH ),
	FIELD_INFO(tmp1         , msg  , TYPE_ID_STRING     , TEST1_DATA_MSG_LENGTH  ),
	FIELD_END()
};

const struct_field test2_data_fields[] = {
	FIELD_INFO(tmp1.data[0] , b    , TYPE_ID_SHORT      , TEST2_DATA_B_LENGTH    ),
	FIELD_INFO(tmp1.data[0] , msg  , TYPE_ID_STRING     , TEST2_DATA_MSG_LENGTH  ),
	FIELD_END()
};

const struct_type struct_types[] =
{
	TYPE_INFO(test1_data_fields, tmp1			),
	TYPE_INFO(test2_data_fields, tmp1.data[0]	),
	TYPE_END()
};
