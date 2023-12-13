#include "common.h"

v128 vssrlni_wu_d(v128 a, v128 b, int imm) {
  v128 dst;
#include "vssrlni_wu_d.h"
  return dst;
}

void test() {
  FUZZ2(vssrlni_wu_d, 0);
  FUZZ2(vssrlni_wu_d, 7);
  FUZZ2(vssrlni_wu_d, 15);
}
