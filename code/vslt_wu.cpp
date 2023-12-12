#include "common.h"

v128 vslt_wu(v128 a, v128 b) {
  v128 dst;
#include "vslt_wu.h"
  return dst;
}

void test() { FUZZ2(vslt_wu); }
