#include "common.h"

v256 xvssrlrni_wu_d(v256 a, v256 b, int imm) {
  v256 dst;
#include "xvssrlrni_wu_d.h"
  return dst;
}

void test() {
  XFUZZ2(xvssrlrni_wu_d, 0);
  XFUZZ2(xvssrlrni_wu_d, 7);
  XFUZZ2(xvssrlrni_wu_d, 15);
}
