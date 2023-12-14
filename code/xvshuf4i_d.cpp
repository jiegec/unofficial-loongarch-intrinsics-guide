#include "common.h"

v256 xvshuf4i_d(v256 a, v256 b, int imm) {
  v256 dst;
#include "xvshuf4i_d.h"
  return dst;
}

void test() {
  XFUZZ2(xvshuf4i_d, 0);
  XFUZZ2(xvshuf4i_d, 13);
  XFUZZ2(xvshuf4i_d, 100);
  XFUZZ2(xvshuf4i_d, 128);
  XFUZZ2(xvshuf4i_d, 255);
}
