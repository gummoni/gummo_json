# Very Simple JSON parser in C.
It is easy to serialize and deserialize by using reflection.
```C
static test1_data tmp1;

void main(void) {}
	char json_data[512];
	char msg[] = "{\"a\" : 123 , \"data\" :[{ \"b\": [ 64 , 12 ], \"msg\" : \"foo bar\" },{ \"b\": [ 22 , 99 ], \"msg\" : \"unko\" }],  \"msg\" : \"hello world\"}";

	json_deserialize(&tmp1, test1_data_fields, msg);

	json_serialize(&tmp1, test1_data_fields, json_data);
}
```


## Step1.　write refrection.h
```
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
```

## Step2. write refrection.c
```
const char test1_data_name[] = "test1_data";
const char test2_data_name[] = "test2_data";

const char* struct_type_names[] = {
	test1_data_name,
	test2_data_name,
};

static const test1_data tmp1;

const struct_field test1_data_fields[] = {
	FIELD_INFO(tmp1			, a		, TYPE_ID_INT			, TEST1_DATA_A_LENGTH),
	FIELD_INFO(tmp1			, data	, TYPE_ID_TEST2_DATA	, TEST1_DATA_DATA_LENGTH),
	FIELD_INFO(tmp1			, msg	, TYPE_ID_STRING		, TEST1_DATA_MSG_LENGTH),
	FIELD_END()
};

const struct_field test2_data_fields[] = {
	FIELD_INFO(tmp1.data[0]	, b		, TYPE_ID_SHORT			, TEST2_DATA_B_LENGTH),
	FIELD_INFO(tmp1.data[0]	, msg	, TYPE_ID_STRING		, TEST2_DATA_MSG_LENGTH),
	FIELD_END()
};

const struct_field* struct_types[] =
{
	test1_data_fields,
	test2_data_fields,
};```

## Step3. write gummo_json.h
```
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
```

Done.

## API list
* json_deserialize(instance, structure info, json_string)
* json_serialize(instance, structure info, json_string)
