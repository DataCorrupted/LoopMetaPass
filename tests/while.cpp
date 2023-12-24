int foo(int a) {
  int ret = 0;
  while (a) {
    ret++;
    a = a & (a - 1);
  }
  return ret;
}

int bar(int a) {
  int ret = 1;
  while (a) {
    ret *= a;
    a--;
  }
  return ret;
}