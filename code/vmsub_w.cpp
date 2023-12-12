#include "common.h"

v128 vmsub_w(v128 a, v128 b, v128 c) {
  v128 dst;
#include "vmsub_w.h"
  return dst;
}

void test() { FUZZ3(vmsub_w); }
