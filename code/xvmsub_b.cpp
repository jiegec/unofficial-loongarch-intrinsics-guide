#include "common.h"

v256 xvmsub_b(v256 a, v256 b, v256 c) {
  v256 dst;
#include "xvmsub_b.h"
  return dst;
}

void test() { XFUZZ3(xvmsub_b); }
