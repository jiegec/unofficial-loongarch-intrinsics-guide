#include "common.h"

v256 xvpickev_w(v256 a, v256 b) {
  v256 dst;
#include "xvpickev_w.h"
  return dst;
}

void test() { XFUZZ2(xvpickev_w); }
