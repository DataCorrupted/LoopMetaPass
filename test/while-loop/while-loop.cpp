int foo(int a) {
  int ret = 0;
  while (a) {
    ret++;
    a = a & (a - 1);
  }
  return ret;
}
