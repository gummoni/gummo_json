static int left;
static int right;
static char old_op;
static char cur_op;
static int state;

void rpn_reset(void) {
  state = 0;
}

void rpn_decode(int value) {
  switch (state) {
    case 0:
      left = value;
      state = 1;
      break;

    case 1:
      old_op = (char)value;
      state = 2;
      break;

    case 2:
      if ('*' == old_op) {
        left = left * value;
        state = 1;
      } else if ('/' == old_op) {
        left = left / value;
        state = 1;
      } else {
        right = value;
        state = 3;
      }
      break;

    case 3:
      cur_op = (char)value;
      state = 4;
      break;

    case 4:
      if ('*' == cur_op) {
        right = right * value;
      } else if ('/' == old_op) {
        right = right / value;
      } else if ('+' == old_op) {
        left = left + right;
        right = value;
        old_op = cur_op;
      } else if ('-' == old_op) {
        left = left - right;
        right = value;
        old_op = cur_op;
      }
      state = 3;
      break;
  }
}

int rpn_result(void) {
  if (3 == state) {
    state = 0;
    left = ('+' == old_op) ? (left + right) : (left - right);
  }
  return left;
}
