#include "common.h"

v128 vmaddwev_q_du(v128 a, v128 b, v128 c) {
  v128 dst;
#include "vmaddwev_q_du.h"
  return dst;
}

void test() { FUZZ3(vmaddwev_q_du); }
