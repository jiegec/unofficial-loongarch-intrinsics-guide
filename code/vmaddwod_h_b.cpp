#include "common.h"

v128 vmaddwod_h_b(v128 a, v128 b, v128 c) {
  v128 dst;
#include "vmaddwod_h_b.h"
  return dst;
}

void test() { FUZZ3(vmaddwod_h_b); }
