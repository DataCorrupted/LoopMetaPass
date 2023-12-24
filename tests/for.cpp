extern int do_something(int);
extern int do_something_else(int, int);

int foo(int a) {
  int ret = 0;
  for (int i = 0; i < a; i++) {
    ret += do_something(i);
  }
  return ret;
}

int bar(int a) {
  int ret = 0;
  for (int i = a; i >= 0; i--) {
    ret += do_something(i);
  }
  return ret;
}

int multi_condition(int a, int b) {
  int ret = 0;
  for (int i = 0, j = 0; i < a && j < b; i += 1, j += 1) {
    ret += do_something_else(i, j);
  }
  return ret;
}