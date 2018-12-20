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

//example: user define
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

//grobal variable
extern const char* struct_type_names[];
extern const struct_field* struct_types[];
extern const struct_field test1_data_fields[];
extern const struct_field test2_data_fields[];

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
