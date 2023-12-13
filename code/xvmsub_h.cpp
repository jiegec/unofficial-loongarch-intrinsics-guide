#include "common.h"

v256 xvmsub_h(v256 a, v256 b, v256 c) {
  v256 dst;
#include "xvmsub_h.h"
  return dst;
}

void test() { XFUZZ3(xvmsub_h); }
