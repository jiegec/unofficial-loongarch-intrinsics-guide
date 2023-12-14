#include "common.h"

v256 xvshuf_b(v256 a, v256 b, v256 c) {
  v256 dst;
#include "xvshuf_b.h"
  return dst;
}

void test() { XFUZZ3(xvshuf_b); }
