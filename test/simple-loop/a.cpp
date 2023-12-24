extern int do_something(int);
int foo(int a) {
  int ret = 0;
  for (int i = 0; i < a; i++) {
    ret += do_something(i);
  }
  return ret;
}
