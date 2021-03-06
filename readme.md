# Very Simple JSON parser in C.
It is easy to serialize and deserialize by using reflection.
```
#include "stdio.h"
#include "gummo_json.h"

int main()
{
  char json_data[512];
  char msg[] = "{\"a\" : 123 , \"data\" :[{ \"b\": [ 64 , 12 ], \"msg\" : \"foo bar\" },{ \"b\": [ 22 , 99 ], \"msg\" : \"ok\" }],  \"msg\" : \"hello world\"}";	
  test1_data obj;
  struct_type* type = &struct_types[TYPE_ID_TEST1_DATA];

  json_deserialize(&obj, type, msg);

  json_serialize(&obj, type, json_data, 512);
}
```

## Step1. write refrection.h
 define struct test1_data and test2_data.
```
#define TEST2_DATA_B_LENGTH    2
#define TEST2_DATA_MSG_LENGTH  16
typedef struct
{
  unsigned short b[TEST2_DATA_B_LENGTH];
  char msg[TEST2_DATA_MSG_LENGTH];
} test2_data;

#define TEST1_DATA_A_LENGTH    1
#define TEST1_DATA_DATA_LENGTH 2
#define TEST1_DATA_MSG_LENGTH  16
typedef struct
{
  int a;
  test2_data data[TEST1_DATA_DATA_LENGTH];
  char msg[TEST1_DATA_MSG_LENGTH];
} test1_data;

typedef enum
{
  TYPE_ID_TEST1_DATA = 0,
  TYPE_ID_TEST2_DATA = 1,
} TYPE_ID_USR;
```


## Step2. write refrection.c
 define test1_data and test2_data struct refrection data.
```
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
  TYPE_INFO(test1_data_fields, tmp1         ),
  TYPE_INFO(test2_data_fields, tmp1.data[0] ),
  TYPE_END()
};
```


Done.

## API list
* extern char* json_deserialize(void* obj, const struct_type* fields, char* json_str);
* extern int json_serialize(void* obj, const struct_type* fields, char* json_str, int length);
