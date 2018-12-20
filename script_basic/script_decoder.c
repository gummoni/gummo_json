#include "script_reader.h"
#include "script_decoder.h"


static inline int do_end(void) {
  int len = strlen(reader.text);
  reader.rp = &reader.text[len];
  return TRUE;
}

static inline int do_jump(script_token token, char* label) {
  switch (token) {
    case VAR_NUM:     return reader_seek_to(label);
    case SYN_NUM:     return reader_seek_to(dic_get(label));
    case CUR_LABEL:   return reader_seek_to(label);
    case CUR_NEWLINE: return TRUE;
    default:          return FALSE;
  }
}

static inline int do_goto() {
  if (FALSE == reader_next())  return FALSE;
  return do_jump(reader.token, reader.context);
}

static inline int do_gosub(void) {
  if (FALSE == reader_next()) return FALSE;
  script_token token = reader.token;
  strcpy(tmp_value, reader.context);

  reader_seek_to_newline();
  stack_push(reader.rp);

  return do_jump(token, tmp_value);
}

static inline int do_return(void) {
  reader.rp = stack_pop();
  if (0 == reader.rp) {
    do_end();
    return FALSE;
  }
  return TRUE;
}

static inline int eval(char* left, script_token op, char* right) {
  if (OPE_EQ == op) return strcmp(left, right);
  if (OPE_EQ == op) return (0 != strcmp(left, right)) ? 0 : -1;
  int valL = strtol(left, NULL, 0);
  int valR = strtol(right, NULL, 0);

  switch (op) {
    case OPE_GT: return (valL < valR) ? 0 : -1;
    case OPE_GE: return (valL <= valR) ? 0 : -1;
    case OPE_LT: return (valL > valR) ? 0 : -1;
    case OPE_LE: return (valL >= valR) ? 0 : -1;
  }
}

static inline int do_if(void) {
  //IF ABC$ == DEF$ THEN 10 ELSEIF ABC == "1" THEN @NNN

  while (1) {
    //step1-1.eval : left
    if (FALSE == reader_next())  return FALSE;
    switch (reader.token) {
      case VAR_NUM:
      case VAR_STR:
        strcpy(tmp_eval_left, reader.context);
        break;

      case SYN_NUM:
      case SYN_STR:
        strcpy(tmp_eval_left, dic_get(reader.context));
        break;

      default:
        return FALSE;
    }

    //step1-2.operation
    if (FALSE == reader_next())  return FALSE;
    tmp_eval_op = reader.token;

    //step1-3.eval : right
    if (FALSE == reader_next())  return FALSE;
    switch (reader.token) {
      case VAR_NUM:
      case VAR_STR:
        strcpy(tmp_eval_right, reader.context);
        break;
      case SYN_NUM:
      case SYN_STR:
        strcpy(tmp_eval_right, dic_get(reader.context));
        break;
    }

    //eval : then
    if (FALSE == reader_next()) return FALSE;
    if (0 != strcmp("THEN", reader.context)) return FALSE;

    if (0 != eval(tmp_eval_left, tmp_eval_op, tmp_eval_right)) {
      //eval : else
      if (FALSE == reader_next()) return FALSE;
      if (FALSE == reader_next()) return FALSE;
      if (0 == strcmp("ELSEIF", reader.context)) continue;
      if (0 != strcmp("ELSE", reader.context)) return reader_seek_to_newline();
    }

    return do_goto();
  }
}

static inline int do_request(void) {
  //通信（往復）
  return TRUE;
}

static inline int do_notify(void) {
  //通信（片道）
  return TRUE;
}

static inline int decode_cmd(void) {
  char* command = reader.context;
  if (0 == strcmp("IF", command)) return do_if();
  if (0 == strcmp("GOTO", command)) return do_goto();
  if (0 == strcmp("GOSUB", command)) return do_gosub();
  if (0 == strcmp("RETURN", command)) return do_return();
  if (0 == strcmp("END", command)) return do_end();
  if (0 == strcmp("REQUEST", command)) return do_request();
  if (0 == strcmp("NOTIFY", command)) return do_notify();
  if (0 == strcmp("REM", command)) return reader_seek_to_newline();
  return FALSE;
}

static inline int decode_num(void) {
  strcpy(tmp_key, reader.context);
  rpn_reset();

  reader_next();
  if (reader.token != CALC_EQUAL) return FALSE;

  while (1) {
    reader_next();
    switch (reader.token) {
      case VAR_NUM:
        rpn_decode(strtol(reader.context, NULL, 0));
        break;

      case SYN_STR:
      case SYN_NUM:
        rpn_decode(strtol(dic_get(reader.context), NULL, 0));
        break;

      case CALC_PLUS:
      case CALC_MINUS:
      case CALC_MUL:
      case CALC_DIV:
        rpn_decode((int)reader.context[0]);
        break;

      case CUR_NEWLINE:
        itoa(rpn_result(), tmp_value, 10);
        dic_set(tmp_key, tmp_value);
        return TRUE;

      default:
        return FALSE;
    }
  }
}

static inline int decode_str(void) {
  strcpy(tmp_key, reader.context);
  tmp_value[0] = '\0';

  reader_next();
  if (reader.token != CALC_EQUAL) return FALSE;

  while (1) {
    reader_next();
    switch (reader.token) {
      case VAR_STR:
      case VAR_NUM:
        strcat(tmp_value, reader.context);
        break;

      case SYN_STR:
      case SYN_NUM:
        strcat(tmp_value, dic_get(reader.context));
        break;

      case CALC_PLUS:
        break;

      case CUR_NEWLINE:
        dic_set(tmp_key, tmp_value);
        return TRUE;

      default:
        return FALSE;
    }
  }
}

int decoder_execute(void) {
  if (0 == reader_next()) return FALSE;
  if (CUR_NEWLINE == reader.token) return TRUE;
  if (VAR_NUM != reader.token) return FALSE;

  if (0 == reader_next()) return FALSE;
  switch (reader.token) {
    case SYN_CMD: return decode_cmd();
    case SYN_STR: return decode_str();
    case SYN_NUM: return decode_num();
    case CUR_LABEL: return reader_seek_to_newline();
    case CUR_NEWLINE: return TRUE;
  }
  return FALSE;
}
