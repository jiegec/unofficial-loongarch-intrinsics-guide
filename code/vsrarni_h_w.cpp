#include "common.h"

v128 vsrarni_h_w(v128 a, v128 b, int imm) {
  v128 dst;
#include "vsrarni_h_w.h"
  return dst;
}

void test() {
  FUZZ2(vsrarni_h_w, 0);
  FUZZ2(vsrarni_h_w, 7);
  FUZZ2(vsrarni_h_w, 15);
}
