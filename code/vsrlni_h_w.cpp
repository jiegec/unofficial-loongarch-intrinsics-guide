#include "common.h"

v128 vsrlni_h_w(v128 a, v128 b, int imm) {
  v128 dst;
#include "vsrlni_h_w.h"
  return dst;
}

void test() {
  FUZZ2(vsrlni_h_w, 0);
  FUZZ2(vsrlni_h_w, 7);
}
