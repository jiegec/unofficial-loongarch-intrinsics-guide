#include "common.h"

v256 xvfsub_d(v256 a, v256 b) {
  v256 dst;
#include "xvfsub_d.h"
  return dst;
}

void test() { XFUZZ2(xvfsub_d); }
