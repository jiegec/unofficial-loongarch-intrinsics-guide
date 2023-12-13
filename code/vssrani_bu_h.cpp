#include "common.h"

v128 vssrani_bu_h(v128 a, v128 b, int imm) {
  v128 dst;
#include "vssrani_bu_h.h"
  return dst;
}

void test() {
  FUZZ2(vssrani_bu_h, 0);
  FUZZ2(vssrani_bu_h, 7);
  FUZZ2(vssrani_bu_h, 15);
}
