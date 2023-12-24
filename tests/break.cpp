extern int do_something(int);

int for_break(int a, int b) {
  int ret = 0;
  for (int i = 0; i < a; i++) {
    if (i == b)
      break;
    ret += do_something(i);
  }
  return ret;
}

int while_break(int a, int b) {
  int ret = 0, idx = 0;
  while (idx != a) {
    if (idx == b)
      break;
    idx++;
    ret += do_something(idx);
  }
  return ret;
}
