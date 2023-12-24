extern int do_something(int);
extern int do_something_else(int, int);
int foo(int a, int b) {
  int ret_a = 0, ret_b = 0;
  for (int i = 0; i < a; i++) {
    ret_a += do_something(i);
    for (int j = 0; j < b; j++) {
      ret_b += do_something_else(i, j);
    }
  }
  return ret_a * ret_b;
}
