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
#include "gummo_json.h"
int main()
{
	char json_data[512];
	char msg[] = "{\"a\" : 123 , \"data\" :[{ \"b\": [ 64 , 12 ], \"msg\" : \"foo bar\" },{ \"b\": [ 22 , 99 ], \"msg\" : \"ok\" }],  \"msg\" : \"hello world\"}";	
	test1_data tmp;

	json_deserialize(&tmp, &struct_types[TYPE_ID_TEST1_DATA], msg);
	printf("---deserialize---\n");
	printf("test1_data.a = %d\n", tmp.a);
	printf("test1_data.msg = %s\n", tmp.msg);
	printf("test1_data.data[0].b[0] = %d\n", tmp.data[0].b[0]);
	printf("test1_data.data[0].b[1] = %d\n", tmp.data[0].b[1]);
	printf("test1_data.data[0].msg = %s\n", tmp.data[0].msg);
	printf("test1_data.data[1].b[0] = %d\n", tmp.data[1].b[0]);
	printf("test1_data.data[1].b[1] = %d\n", tmp.data[1].b[1]);
	printf("test1_data.data[1].msg = %s\n", tmp.data[1].msg);

	json_serialize(&tmp, &struct_types[TYPE_ID_TEST1_DATA], json_data, 512);
	printf("---serialize---\n");
	printf("%s\n", json_data);
}
