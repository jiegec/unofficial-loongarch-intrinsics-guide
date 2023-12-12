#include "common.h"

v128 vmaddwev_d_wu_w(v128 a, v128 b, v128 c) {
  v128 dst;
#include "vmaddwev_d_wu_w.h"
  return dst;
}

void test() { FUZZ3(vmaddwev_d_wu_w); }
