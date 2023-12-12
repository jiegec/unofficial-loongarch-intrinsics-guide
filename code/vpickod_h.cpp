#include "common.h"

v128 vpickod_h(v128 a, v128 b) {
  v128 dst;
#include "vpickod_h.h"
  return dst;
}

void test() { FUZZ2(vpickod_h); }
