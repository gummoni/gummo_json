#include "dictionary.h"

static char _dictionary[VARIABLE_MEMORY_SIZE][2][VARIABLE_NAME_LENGTH];    //1kB[RAM]

void dic_clear(void) {
  int i;
  for (i = 0; i < VARIABLE_MEMORY_SIZE; i++) {
    _dictionary[i][0][0] = _dictionary[i][1][0] = '\0';
  }
}

char* dic_get(char* key) {
  int idx = 0;
  for (idx = 0; idx < VARIABLE_MEMORY_SIZE; idx++) {
    if (strlen(_dictionary[idx][0]) == 0) break;
    if (strcmp(key, _dictionary[idx][0]) == 0) return _dictionary[idx][1];
  }
  return 0;
}

void dic_set(char* key, char* value) {
  int idx = 0;
  for (idx = 0; idx < VARIABLE_MEMORY_SIZE; idx++) {
    if (strlen(_dictionary[idx][0]) == 0) break;
    if (strcmp(key, _dictionary[idx][0]) == 0) break;
  }
  strcpy(_dictionary[idx][0], key);
  strcpy(_dictionary[idx][1], value);
}
