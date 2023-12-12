#include "common.h"

v128 vmaddwod_d_w(v128 a, v128 b, v128 c) {
  v128 dst;
#include "vmaddwod_d_w.h"
  return dst;
}

void test() { FUZZ3(vmaddwod_d_w); }
