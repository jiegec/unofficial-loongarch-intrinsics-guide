#include "common.h"

v128 vmadd_h(v128 a, v128 b, v128 c) {
  v128 dst;
#include "vmadd_h.h"
  return dst;
}

void test() { FUZZ3(vmadd_h); }
