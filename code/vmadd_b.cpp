#include "common.h"

v128 vmadd_b(v128 a, v128 b, v128 c) {
  v128 dst;
#include "vmadd_b.h"
  return dst;
}

void test() { FUZZ3(vmadd_b); }
