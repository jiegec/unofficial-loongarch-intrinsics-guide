#include "common.h"

v128 vbitsel_v(v128 a, v128 b, v128 c) {
  v128 dst;
#include "vbitsel_v.h"
  return dst;
}

void test() {
  FUZZ3(vbitsel_v);
}
