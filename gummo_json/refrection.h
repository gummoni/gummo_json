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
	//user type define
	TYPE_ID_TEST1_DATA = 0,
	TYPE_ID_TEST2_DATA = 1,
	TYPE_ID_MAX,
} TYPE_ID;

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


extern test1_data tmp1;
