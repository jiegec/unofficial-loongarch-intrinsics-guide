#include "common.h"

v128 vssrlrni_bu_h(v128 a, v128 b, int imm) {
  v128 dst;
#include "vssrlrni_bu_h.h"
  return dst;
}

void test() {
  FUZZ2(vssrlrni_bu_h, 0);
  FUZZ2(vssrlrni_bu_h, 7);
  FUZZ2(vssrlrni_bu_h, 15);
}
