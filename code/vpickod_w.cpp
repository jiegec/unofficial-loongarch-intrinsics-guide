#include "common.h"

v128 vpickod_w(v128 a, v128 b) {
  v128 dst;
#include "vpickod_w.h"
  return dst;
}

void test() { FUZZ2(vpickod_w); }
