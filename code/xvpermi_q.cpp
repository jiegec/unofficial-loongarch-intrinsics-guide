#include "common.h"

v256 xvpermi_q(v256 a, v256 b, int imm) {
  v256 dst;
#include "xvpermi_q.h"
  return dst;
}

void test() {
  XFUZZ2(xvpermi_q, 0);
  XFUZZ2(xvpermi_q, 1);
  XFUZZ2(xvpermi_q, 2);
  XFUZZ2(xvpermi_q, 0x10);
  XFUZZ2(xvpermi_q, 0x80);
  XFUZZ2(xvpermi_q, 0xC0);
  XFUZZ2(xvpermi_q, 0xF0);
  XFUZZ2(xvpermi_q, 0xFF);
}
