#include "common.h"

v128 vand_v(v128 a, v128 b) {
  v128 dst;
#include "vand_v.h"
  return dst;
}

void test() {
  FUZZ2(vand_v);
}
