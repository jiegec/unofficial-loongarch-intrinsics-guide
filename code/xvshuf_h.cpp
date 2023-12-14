#include "common.h"

v256 xvshuf_h(v256 a, v256 b, v256 c) {
  v256 dst;
#include "xvshuf_h.h"
  return dst;
}

void test() { XFUZZ3(xvshuf_h); }
