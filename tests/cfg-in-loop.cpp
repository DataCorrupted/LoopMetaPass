extern int do_something(int);
extern int do_something_else(int);

int foo(int a, int b) {
  int ret = 0;
  for (int i = 0; i < a; i++) {
    if (i % b)
      ret += do_something(i);
    else
      ret += do_something_else(i);

    if (i % (b - 1)) {
      for (int j = 0; j < b; j++) {
        ret += do_something(j);
      }
    }
  }
  return ret;
}
