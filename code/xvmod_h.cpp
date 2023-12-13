#include "common.h"

v256 xvmod_h(v256 a, v256 b) {
  v256 dst;
#include "xvmod_h.h"
  return dst;
}

void test() { XFUZZ2(xvmod_h); }
