#include "common.h"

v128 vsrlrn_w_d(v128 a, v128 b) {
  v128 dst;
#include "vsrlrn_w_d.h"
  return dst;
}

void test() { FUZZ2(vsrlrn_w_d); }
