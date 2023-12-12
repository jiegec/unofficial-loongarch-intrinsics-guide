#include "common.h"

v128 vsadd_wu(v128 a, v128 b) {
  v128 dst;
#include "vsadd_wu.h"
  return dst;
}

void test() { FUZZ2(vsadd_wu); }
