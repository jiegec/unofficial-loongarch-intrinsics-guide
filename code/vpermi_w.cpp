#include "common.h"

v128 vpermi_w(v128 a, v128 b, int imm) {
  v128 dst;
#include "vpermi_w.h"
  return dst;
}

void test() {
  FUZZ2(vpermi_w, 0x00);
  FUZZ2(vpermi_w, 0x04);
  FUZZ2(vpermi_w, 0x08);
  FUZZ2(vpermi_w, 0x10);
  FUZZ2(vpermi_w, 0xFF);
}
