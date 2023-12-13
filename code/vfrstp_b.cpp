#include "common.h"

v128 vfrstp_b(v128 a, v128 b, v128 c) {
  v128 dst;
#include "vfrstp_b.h"
  return dst;
}

void test() { FUZZ3(vfrstp_b); }
