#include "common.h"

v128 vmaddwod_w_hu(v128 a, v128 b, v128 c) {
  v128 dst;
#include "vmaddwod_w_hu.h"
  return dst;
}

void test() { FUZZ3(vmaddwod_w_hu); }
