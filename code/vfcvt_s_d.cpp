#include "common.h"

v128 vfcvt_s_d(v128 a, v128 b) {
  v128 dst;
#include "vfcvt_s_d.h"
  return dst;
}

void test() { FUZZ2(vfcvt_s_d); }
