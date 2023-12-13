#include "common.h"

v256 xvmod_b(v256 a, v256 b) {
  v256 dst;
#include "xvmod_b.h"
  return dst;
}

void test() { XFUZZ2(xvmod_b); }
