#include "common.h"

v256 xvadd_q(v256 a, v256 b) {
  v256 dst;
#include "xvadd_q.h"
  return dst;
}

void test() { XFUZZ2(xvadd_q); }
