#include "common.h"

v128 vmax_wu(v128 a, v128 b) {
  v128 dst;
#include "vmax_wu.h"
  return dst;
}

void test() { FUZZ2(vmax_wu); }
