#include "common.h"

v256 xvsrlni_h_w(v256 a, v256 b, int imm) {
  v256 dst;
#include "xvsrlni_h_w.h"
  return dst;
}

void test() {
  XFUZZ2(xvsrlni_h_w, 0);
  XFUZZ2(xvsrlni_h_w, 7);
  XFUZZ2(xvsrlni_h_w, 15);
}
