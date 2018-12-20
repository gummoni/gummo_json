#include "stack.h"

static stStack _stack;


void stack_push(char* data) {
  _stack.heap_memory[_stack.index++] = data;
}

char* stack_pop(void) {
  return (0 < _stack.index) ? _stack.heap_memory[--_stack.index] : 0;
}
