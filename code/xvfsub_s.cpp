#include "common.h"

v256 xvfsub_s(v256 a, v256 b) {
  v256 dst;
#include "xvfsub_s.h"
  return dst;
}

void test() { XFUZZ2(xvfsub_s); }
