#include "common.h"

v256 xvmod_d(v256 a, v256 b) {
  v256 dst;
#include "xvmod_d.h"
  return dst;
}

void test() { XFUZZ2(xvmod_d); }
