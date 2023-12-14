#include "common.h"

v256 xvshuf_d(v256 a, v256 b, v256 c) {
  v256 dst;
#include "xvshuf_d.h"
  return dst;
}

void test() { XFUZZ3(xvshuf_d); }
