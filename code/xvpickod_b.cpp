#include "common.h"

v256 xvpickod_b(v256 a, v256 b) {
  v256 dst;
#include "xvpickod_b.h"
  return dst;
}

void test() { XFUZZ2(xvpickod_b); }
