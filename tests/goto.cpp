extern int do_something(int);

int foo_goto(int a, int b) {
  int i = 0, ret = 0;
head:
  i++;
  ret += do_something(i);
  if (i != a)
    goto head;
  else
    goto exit;
exit:
  return ret;
}
