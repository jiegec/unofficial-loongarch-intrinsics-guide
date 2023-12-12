#include "common.h"

v128 vpickod_d(v128 a, v128 b) {
  v128 dst;
#include "vpickod_d.h"
  return dst;
}

void test() { FUZZ2(vpickod_d); }
