#include "common.h"

v128 vmaddwod_d_wu_w(v128 a, v128 b, v128 c) {
  v128 dst;
#include "vmaddwod_d_wu_w.h"
  return dst;
}

void test() { FUZZ3(vmaddwod_d_wu_w); }
