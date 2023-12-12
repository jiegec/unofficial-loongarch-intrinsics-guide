#include "common.h"

v128 vmsub_b(v128 a, v128 b, v128 c) {
  v128 dst;
#include "vmsub_b.h"
  return dst;
}

void test() { FUZZ3(vmsub_b); }
