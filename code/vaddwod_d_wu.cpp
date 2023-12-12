#include "common.h"

v128 vaddwod_d_wu(v128 a, v128 b) {
  v128 dst;
#include "vaddwod_d_wu.h"
  return dst;
}

void test() { FUZZ2(vaddwod_d_wu); }
