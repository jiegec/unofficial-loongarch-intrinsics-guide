#include "common.h"

v256 xvpickev_b(v256 a, v256 b) {
  v256 dst;
#include "xvpickev_b.h"
  return dst;
}

void test() { XFUZZ2(xvpickev_b); }
