#include "common.h"

v256 xvsrlrni_b_h(v256 a, v256 b, int imm) {
  v256 dst;
#include "xvsrlrni_b_h.h"
  return dst;
}

void test() {
  XFUZZ2(xvsrlrni_b_h, 0);
  XFUZZ2(xvsrlrni_b_h, 7);
  XFUZZ2(xvsrlrni_b_h, 15);
}
