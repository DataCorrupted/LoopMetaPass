extern int do_something(int);
int foo(int a) {
  int ret = 0;
  for (int i = 1; i < a; i += 2) {
    ret += do_something(i);
  }

  for (int i = a - 1; i >= 0; i -= 2) {
    ret += do_something(i);
  }
  return ret;
}
