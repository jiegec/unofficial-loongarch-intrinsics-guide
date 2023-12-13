#include "common.h"

v256 xvssrlni_bu_h(v256 a, v256 b, int imm) {
  v256 dst;
#include "xvssrlni_bu_h.h"
  return dst;
}

void test() {
  XFUZZ2(xvssrlni_bu_h, 0);
  XFUZZ2(xvssrlni_bu_h, 7);
  XFUZZ2(xvssrlni_bu_h, 15);
}
