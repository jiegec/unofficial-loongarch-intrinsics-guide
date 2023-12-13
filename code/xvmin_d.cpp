#include "common.h"

v256 xvmin_d(v256 a, v256 b) {
  v256 dst;
#include "xvmin_d.h"
  return dst;
}

void test() { XFUZZ2(xvmin_d); }
