#ifndef __STACK_H__
#define __STACK_H__
#include "config.h"

typedef struct {
  int index;
  char* heap_memory[HEAP_SIZE];
} stStack;


extern void stack_push(char* data);
extern char* stack_pop(void);

#endif//__STACK_H__
