extern int do_something(int);

int for_continue(int a) {
  int ret = 0;
  for (int i = 0; i < a; i++) {
    if (i % 2)
      continue;
    ret += do_something(i);
  }
  return ret;
}

int while_continue(int a) {
  int idx = 0, ret =0;
  while (idx != a) {
    idx ++;
    if (idx == 42) continue;
    ret += do_something(idx);
  }
  return ret;
}
