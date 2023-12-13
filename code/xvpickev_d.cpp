#include "common.h"

v256 xvpickev_d(v256 a, v256 b) {
  v256 dst;
#include "xvpickev_d.h"
  return dst;
}

void test() { XFUZZ2(xvpickev_d); }
