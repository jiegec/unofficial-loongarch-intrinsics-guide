#include "common.h"

v128 vssrlni_hu_w(v128 a, v128 b, int imm) {
  v128 dst;
#include "vssrlni_hu_w.h"
  return dst;
}

void test() {
  FUZZ2(vssrlni_hu_w, 0);
  FUZZ2(vssrlni_hu_w, 7);
  FUZZ2(vssrlni_hu_w, 15);
}
