#include "common.h"

v128 vmaddwev_w_hu_h(v128 a, v128 b, v128 c) {
  v128 dst;
#include "vmaddwev_w_hu_h.h"
  return dst;
}

void test() { FUZZ3(vmaddwev_w_hu_h); }
