#include "common.h"

v256 xvssrlrni_hu_w(v256 a, v256 b, int imm) {
  v256 dst;
#include "xvssrlrni_hu_w.h"
  return dst;
}

void test() {
  XFUZZ2(xvssrlrni_hu_w, 0);
  XFUZZ2(xvssrlrni_hu_w, 7);
  XFUZZ2(xvssrlrni_hu_w, 15);
}
