#include "common.h"

v128 vmsub_d(v128 a, v128 b, v128 c) {
  v128 dst;
#include "vmsub_d.h"
  return dst;
}

void test() { FUZZ3(vmsub_d); }
