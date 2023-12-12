#include "common.h"

v128 vdiv_wu(v128 a, v128 b) {
  v128 dst;
#include "vdiv_wu.h"
  return dst;
}

void test() { FUZZ2(vdiv_wu); }
