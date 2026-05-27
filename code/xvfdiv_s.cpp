#include "common.h"

v256 xvfdiv_s(v256 a, v256 b) {
  v256 dst;
#include "xvfdiv_s.h"
  return dst;
}

void test() { XFUZZ2(xvfdiv_s); }
