#include "common.h"

v256 xvssub_d(v256 a, v256 b) {
  v256 dst;
#include "xvssub_d.h"
  return dst;
}

void test() { XFUZZ2(xvssub_d); }
