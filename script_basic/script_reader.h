#ifndef __SCRIPT_READER_H__
#define __SCRIPT_READER_H__
#include "config.h"

typedef enum {
  CUR_NEWLINE,    // 0  : enter
  VAR_NUM,        // 1  : numeric[0-9]
  VAR_STR,        // 2  : "****"
  SYN_CMD,        // 3  : command
  SYN_STR,        // 4  : variable:string
  SYN_NUM,        // 5  : variable:numeric
  CALC_EQUAL,     // 6  : =
  CALC_PLUS,      // 7  : +
  CALC_MINUS,     // 8  : -
  CALC_MUL,       // 9  : *
  CALC_DIV,       // 10 : /
  CALC_MOD,       // 11 : \
  CALC_AND,       // 12 : &
  CALC_OR,        // 13 : |
  OPE_EQ,         // 14 : ==
  OPE_NE,         // 15 : <>
  OPE_GT,         // 16 : <
  OPE_GE,         // 17 : <=
  OPE_LT,         // 18 : >
  OPE_LE,         // 19 : >=
  CUR_LABEL,      // 20 : @
} script_token;


typedef struct {
  char* rp;                           // read pointer
  char* text;                         // text pointer
  script_token token;                 // token
  char context[VARIABLE_NAME_LENGTH]; // command / variable:number / variable:string
} script_reader;

extern char tmp_key[VARIABLE_NAME_LENGTH];
extern char tmp_value[VARIABLE_NAME_LENGTH];
extern char tmp_eval_left[VARIABLE_NAME_LENGTH];
extern char tmp_eval_right[VARIABLE_NAME_LENGTH];
extern script_token tmp_eval_op;
extern script_reader reader;
extern int reader_next(void);
extern void reader_init(char*);
extern int reader_seek_to(char* label);
extern int reader_seek_to_newline(void);


#endif//__SCRIPT_READER_H__
