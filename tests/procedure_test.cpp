#include <iostream>
#include <cassert>
#include <cstring>

using namespace std;

void copy(char *ptr) {
  strcpy(ptr, "test buffer");
}

void runTest() {
  int a;
  int b;
  char buf[20];
  char buf2[20];
  char buf3[20];

  copy(buf2);

  assert((void *)&buf2 > (void *)&a);
  assert((void *)&buf2 > (void *)&b);
  assert((void *)&buf2 > (void *)&buf);
  assert((void *)&buf2 > (void *)&buf3);

  assert((void *)&buf < (void *)&a);
  assert((void *)&buf < (void *)&b);
  assert((void *)&buf3 < (void *)&a);
  assert((void *)&buf3 < (void *)&b);
}

int main() {
  runTest();
  return 0;
}