#include "common.h"

v128 vmsub_h(v128 a, v128 b, v128 c) {
  v128 dst;
#include "vmsub_h.h"
  return dst;
}

void test() { FUZZ3(vmsub_h); }
