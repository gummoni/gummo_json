#include "pch.h"
#include "gummo_json.h"

int main()
{
	char json_data[512];
	char msg[] = "{\"a\" : 123 , \"data\" :[{ \"b\": [ 64 , 12 ], \"msg\" : \"foo bar\" },{ \"b\": [ 22 , 99 ], \"msg\" : \"unko\" }],  \"msg\" : \"hello world\"}";

	json_deserialize(&tmp1, test1_data_fields, msg);

	json_serialize(&tmp1, test1_data_fields, json_data);

	printf("---deserialize---\n");
	printf("test1_data.a = %d\n", tmp1.a);
	printf("test1_data.msg = %s\n", tmp1.msg);
	printf("test1_data.data[0].b[0] = %d\n", tmp1.data[0].b[0]);
	printf("test1_data.data[0].b[1] = %d\n", tmp1.data[0].b[1]);
	printf("test1_data.data[0].msg = %s\n",  tmp1.data[0].msg);
	printf("test1_data.data[1].b[0] = %d\n", tmp1.data[1].b[0]);
	printf("test1_data.data[1].b[1] = %d\n", tmp1.data[1].b[1]);
	printf("test1_data.data[1].msg = %s\n",  tmp1.data[1].msg);
	printf("---serialize---\n");
	printf("%s\n\n\n\n\n\n\n\n\n", json_data);
}
