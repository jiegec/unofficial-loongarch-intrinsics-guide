#include "common.h"

v256 xvadd_d(v256 a, v256 b) {
  v256 dst;
#include "xvadd_d.h"
  return dst;
}

void test() { XFUZZ2(xvadd_d); }
