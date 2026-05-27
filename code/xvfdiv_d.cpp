#include "common.h"

v256 xvfdiv_d(v256 a, v256 b) {
  v256 dst;
#include "xvfdiv_d.h"
  return dst;
}

void test() { XFUZZ2(xvfdiv_d); }
