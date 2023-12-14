#include "common.h"

v256 xvshuf_w(v256 a, v256 b, v256 c) {
  v256 dst;
#include "xvshuf_w.h"
  return dst;
}

void test() { XFUZZ3(xvshuf_w); }
