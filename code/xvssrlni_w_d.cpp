#include "common.h"

v256 xvssrlni_w_d(v256 a, v256 b, int imm) {
  v256 dst;
#include "xvssrlni_w_d.h"
  return dst;
}

void test() {
  XFUZZ2(xvssrlni_w_d, 0);
  XFUZZ2(xvssrlni_w_d, 7);
  XFUZZ2(xvssrlni_w_d, 15);
}
